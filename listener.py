import time, csv, serial, logging, logging.handlers, datetime, sys, re, os, os.path
import MySQLdb as mdb
from subprocess import call
options = { "user": "graphs", "passwd": "graphs", "db": "graphs", "host": "127.0.0.1", "connect_timeout": 10 }

from BMP085 import BMP085
from decimal import Decimal
from apscheduler.scheduler import Scheduler

#vars
workingDir = "/var/www/graphs/data/"
logDir = "/var/www/graphs/logs/"
UpdateLogFile = "update.log"
SerialLogFile = "serial.log"
values = ['HUM','LIGH', 'TMPA', 'BATT', 'RELAYA', 'RELAYB']

loggerUpdate = logging.getLogger('UpdateLog')
loggerSerial = logging.getLogger('SerialLog')
loggerUpdate.setLevel(logging.INFO)
loggerSerial.setLevel(logging.INFO)
formatter = logging.Formatter(fmt="[%(asctime)s] %(message)s",datefmt='%Y-%m-%d %H:%M:%S')
handlerUpdate = logging.handlers.RotatingFileHandler(logDir + UpdateLogFile, maxBytes=1048576, backupCount=1)
handlerSerial = logging.handlers.RotatingFileHandler(logDir + SerialLogFile, maxBytes=1048576, backupCount=1)

handlerUpdate.setFormatter(formatter)
handlerSerial.setFormatter(formatter)
loggerUpdate.addHandler(handlerUpdate)
loggerSerial.addHandler(handlerSerial)

logging.basicConfig()
bmp = BMP085(0x77)
serial = serial.Serial('/dev/ttyAMA0', 9600)

def updateDB(value,type):
  connected = 0
  try:
    con = mdb.connect(**options);
    connected = 1
  except:
    t = time.localtime()
    currentTime = '%d-%02d-%02d %02d:%02d:%02d' % (t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec) 
    loggerUpdate.info("Offline: failed to connect to db ...")
    loggerUpdate.info("Offline: storing data [" + str(currentTime) + "," + str(value) + "," + str(type) +"] in flat file ...")
    fo = open(workingDir + "dbOffline", 'a')
    fo.write(currentTime + "," + str(value) + "," + str(type) + "\n")
    fo.close()
   
  if connected == 1: 
    cur = con.cursor()
    cur.execute("INSERT INTO home(value, type) VALUES(%s, %s)", (str(value),str(type),)) 
    if os.path.isfile(workingDir + "dbOffline"):
      loggerUpdate.info("Offline: db connection back, uploading backlog")
      fo = open(workingDir + "dbOffline", "rb")
      csvfile = csv.reader(fo)
      rowCount = 0
      for row in csvfile:
        cur.execute("INSERT INTO home(datetime_collected, value, type) VALUES(%s, %s, %s)", (str(row[0]),str(row[1]),str(row[2])))
        rowCount = rowCount +1;
      loggerUpdate.info("Update: dbOffline file updated [" + str(rowCount) + " records]")
      os.remove(workingDir + "dbOffline")
      fo.close()
    con.close()
    return 1;
  return 0;

def relayStatus():
  serial.write('aRARELAYA---')
  time.sleep(3)
  serial.write('aRARELAYB---')

def checkRelay():
  if os.path.isfile(workingDir + "RARELAYA"):
    fo = open(workingDir + "RARELAYA", "r")
    dataValue = fo.read()
    fo.close()
    fo = open(workingDir + "relayAState", 'w')
    fo.write(dataValue)
    fo.close()
    os.remove(workingDir + "RARELAYA")

  if os.path.isfile(workingDir + "RARELAYB"):
    fo = open(workingDir + "RARELAYB", "r")
    dataValue = fo.read()
    fo.close()
    fo = open(workingDir + "relayBState", 'w')
    fo.write(dataValue)
    fo.close()
    os.remove(workingDir + "RARELAYB")
  dataValue = None
  if os.path.isfile(workingDir + "checkAppleTV"):
    fo = open(workingDir + "checkAppleTV", "r")
    dataValue = fo.read()  
    fo.close()
    os.remove(workingDir + "checkAppleTV")
    if dataValue == "OFF":
      serial.write('aRARELAYAOFF')
      loggerUpdate.info("Update: turned off AppleTV")
    elif dataValue == "ON":
      serial.write('aRARELAYAON-')
      loggerUpdate.info("Update: turned on AppleTV")

  if os.path.isfile(workingDir + "checkNAS"):
    fo = open(workingDir + "checkNAS", "r")
    dataValue = fo.read()
    fo.close()
    os.remove(workingDir + "checkNAS")
    if dataValue == "OFF":
      #cleanly shutdown devices
      call(["ssh", "-o", "UserKnownHostsFile=/dev/null", "-o", "StrictHostKeyChecking=no", "192.168.100.3", "halt"])
      time.sleep(60)
      serial.write('aRARELAYBOFF')
      loggerUpdate.info("Update: turned off NAS")
    elif dataValue == "ON":
      serial.write('aRARELAYBON-')
      loggerUpdate.info("Update: turned on NAS")

def bmp085():
  pressure = bmp.readPressure()
  status = updateDB(str(pressure/100),'Pressure')
  if status == 1:
    loggerUpdate.info("Update: Pressure " + str(pressure/100))
 
def checkfiles(): 
  for file in os.listdir(workingDir):
    fo = open(workingDir + file, "r")
    dataValue = fo.read()
    devID = file[1:3]
    if "TSTMPA" in file:
      status = updateDB(str(dataValue),'insidetemp')
      os.remove(workingDir + file)
      if status == 1:
        loggerUpdate.info("Update: Inside Temp " + str(dataValue))
    if "TOTMPA" in file:
      status = updateDB(str(dataValue),'outsidetemp')
      os.remove(workingDir + file)
      if status == 1:
        loggerUpdate.info("Update: Outside Temp " + str(dataValue))
    if "TSBATT" in file:
      status = updateDB(str(dataValue),'insidebatt')
      os.remove(workingDir + file)
      if status == 1:
        loggerUpdate.info("Update: Inside Battery " + str(dataValue))
    if "TOBATT" in file:
      status = updateDB(str(dataValue),'outsidebatt')
      os.remove(workingDir + file)
      if status == 1:
        loggerUpdate.info("Update: Outside Battery " + str(dataValue))
    if "THBATT" in file:
      status = updateDB(str(dataValue),'solarbatt')
      os.remove(workingDir + file)
      if status == 1:
        loggerUpdate.info("Update: Solar Battery " + str(dataValue))

scheduler = Scheduler()
scheduler.add_interval_job(bmp085, minutes=3)
scheduler.add_interval_job(checkfiles, minutes=1)
scheduler.add_interval_job(checkRelay, seconds=3)
scheduler.add_interval_job(relayStatus, minutes=3)
scheduler.start()

while True:
  c = serial.read()
  while c != 'a':
    c = serial.read()
  llapMsg = "a" + serial.read(11)
  loggerSerial.info("Received " + llapMsg)
  deviceID = llapMsg[1:3]
  if llapMsg == 'aRFxxxxxxx-':
    serial.write('aRFOPENLOCK-')
    loggerUpdate.info("Update: Unlocked door")
  for item in values:
    index = 12 - (9 - len(item))
    if item in llapMsg and re.search(r"^[\w\d'.-]+$",llapMsg) and deviceID in llapMsg:
      fo = open(workingDir + deviceID + item, 'w')
      fo.write(llapMsg[index:index+1]) #this is needed if there is a reading with a negetive value, such as a minus temperature value
      fo.write(llapMsg[index+1:12].translate(None, '-'))
      fo.close()
sched.shutdown()
