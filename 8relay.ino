//
// Simple sketch to interface to the 'Relais Interface Grundgerät'
// (http://www.ak-modul-bus.de/stat/relais_interface_grundgeraet,pd10!0,,RIST.html)
// The atmega328 is wired directly to the 20 pin DIP socket of the
// original microcontroller (AT90S1200).
// 
// This sketch uses an Ciseco XRF to transmit/receive data wirelessly, it relies
// on the Ciseco LLAPSerial library - https://github.com/CisecoPlc/LLAPSerial
//
// Functions supported:
//
// * aR8RELAY#ON- (turn relay # on)
// * aR8RELAY#OFF (turn relay # off)
// * aR8RELAY#TOG (toggle state of relay #)
// * aR8RELAYAON- (turn on all relays)
// * aR8RELAYAOFF (turn off all relays)
//
// Pinout of AT90S1200 socket
//
//        +-\/-+
//    NC 1|    |20  Vcc / Vin
//    NC 2|    |19 Arduino Pin 2
//    NC 3|    |18 Arduino Pin 3
//    NC 4|    |17 Arduino Pin 4
//    NC 5|    |16 Arduino Pin 5
//    NC 6|    |15 Arduino Pin 6
//    NC 7|    |14 Arduino Pin 7
//    NC 8|    |13 Arduino Pin 8
//    NC 9|    |12 Arduino Pin 9
//  GND 10|    |11 NC
//        +----+
//
//
// Ben Morrice June 2014


#include "LLAPSerial.h"

#define DEVICEID "R8"

#define Relay1 2
#define Relay2 3
#define Relay3 4
#define Relay4 5
#define Relay5 6
#define Relay6 7
#define Relay7 8
#define Relay8 9

int tmp;

void setup() {
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(Relay5, OUTPUT);
  pinMode(Relay6, OUTPUT);
  pinMode(Relay7, OUTPUT);
  pinMode(Relay8, OUTPUT);  
  
  Serial.begin(9600);
  LLAP.init(DEVICEID);
  LLAP.sendMessage(String("STARTED"));
}

void loop() {
  if (LLAP.bMsgReceived) {
    if (LLAP.sMessage.equals("RELAY1ON-")) { 
      digitalWrite(Relay1,HIGH);
      LLAP.sendMessage(String("RELAY1ON"));
    }
    if (LLAP.sMessage.equals("RELAY1OFF")) {
      digitalWrite(Relay1,LOW);
      LLAP.sendMessage(String("RELAY1OFF"));
    }
    if (LLAP.sMessage.equals("RELAY2ON-")) { 
      digitalWrite(Relay2,HIGH);
      LLAP.sendMessage(String("RELAY2ON"));
    }
    if (LLAP.sMessage.equals("RELAY2OFF")) {
      digitalWrite(Relay2,LOW);
      LLAP.sendMessage(String("RELAY2OFF"));
    }
    if (LLAP.sMessage.equals("RELAY3ON-")) {
      digitalWrite(Relay3,HIGH);
      LLAP.sendMessage(String("RELAY3ON"));
    }
    if (LLAP.sMessage.equals("RELAY3OFF")) {
      digitalWrite(Relay3,LOW);
      LLAP.sendMessage(String("RELAY3OFF"));
    }
    if (LLAP.sMessage.equals("RELAY4ON-")) {
      digitalWrite(Relay4,HIGH);
      LLAP.sendMessage(String("RELAY4ON"));
    }
    if (LLAP.sMessage.equals("RELAY4OFF")) {
      digitalWrite(Relay4,LOW);
      LLAP.sendMessage(String("RELAY4OFF"));
    }
    if (LLAP.sMessage.equals("RELAY5ON-")) {
      digitalWrite(Relay5,HIGH);
      LLAP.sendMessage(String("RELAY5ON"));
    }
    if (LLAP.sMessage.equals("RELAY5OFF")) {
      digitalWrite(Relay5,LOW);
      LLAP.sendMessage(String("RELAY5OFF"));
    }
    if (LLAP.sMessage.equals("RELAY6ON-")) {
      digitalWrite(Relay6,HIGH);
      LLAP.sendMessage(String("RELAY6ON"));
    }
    if (LLAP.sMessage.equals("RELAY6OFF")) {
      digitalWrite(Relay6,LOW);
      LLAP.sendMessage(String("RELAY6OFF"));
    }
    if (LLAP.sMessage.equals("RELAY7ON-")) {
      digitalWrite(Relay7,HIGH);
      LLAP.sendMessage(String("RELAY7ON"));
    }
    if (LLAP.sMessage.equals("RELAY7OFF")) {
      digitalWrite(Relay7,LOW);
      LLAP.sendMessage(String("RELAY7OFF"));
    }
    if (LLAP.sMessage.equals("RELAY8ON-")) {
      digitalWrite(Relay8,HIGH);
      LLAP.sendMessage(String("RELAY8ON"));
    }
    if (LLAP.sMessage.equals("RELAY8OFF")) {
      digitalWrite(Relay8,LOW);
      LLAP.sendMessage(String("RELAY8OFF"));
    }
    if (LLAP.sMessage.equals("RELAY1TOG")) { 
      tmp = digitalRead(Relay1);
      if (tmp == 1) {
        digitalWrite(Relay1,LOW);
        LLAP.sendMessage(String("RELAY1OFF"));
      } else {
        digitalWrite(Relay1,HIGH);
        LLAP.sendMessage(String("RELAY1ON"));
      }
    }
    if (LLAP.sMessage.equals("RELAY2TOG")) {
      tmp = digitalRead(Relay2);
      if (tmp == 1) {
        digitalWrite(Relay2,LOW);
        LLAP.sendMessage(String("RELAY2OFF"));
      } else {
        digitalWrite(Relay2,HIGH);
        LLAP.sendMessage(String("RELAY2ON"));
      }
    }
    if (LLAP.sMessage.equals("RELAY3TOG")) {
    tmp = digitalRead(Relay3);
      if (tmp == 1) {
        digitalWrite(Relay3,LOW);
        LLAP.sendMessage(String("RELAY3OFF"));
      } else {
        digitalWrite(Relay3,HIGH);
        LLAP.sendMessage(String("RELAY3ON"));
      }
    }
    if (LLAP.sMessage.equals("RELAY4TOG")) { 
      tmp = digitalRead(Relay4);
      if (tmp == 1) {
        digitalWrite(Relay4,LOW);
        LLAP.sendMessage(String("RELAY4OFF"));
      } else {
        digitalWrite(Relay4,HIGH);
        LLAP.sendMessage(String("RELAY4ON"));
      }
    }
    if (LLAP.sMessage.equals("RELAY5TOG")) { 
      tmp = digitalRead(Relay5);
      if (tmp == 1) {
        digitalWrite(Relay5,LOW);
        LLAP.sendMessage(String("RELAY5OFF"));
      } else {
        digitalWrite(Relay5,HIGH);
        LLAP.sendMessage(String("RELAY5ON"));
      }
    }
    if (LLAP.sMessage.equals("RELAY6TOG")) { 
    tmp = digitalRead(Relay6);
      if (tmp == 1) {
        digitalWrite(Relay6,LOW);
        LLAP.sendMessage(String("RELAY6OFF"));
      } else {
        digitalWrite(Relay6,HIGH);
        LLAP.sendMessage(String("RELAY6ON"));
      }
    }
    if (LLAP.sMessage.equals("RELAY7TOG")) { 
    tmp = digitalRead(Relay7);
      if (tmp == 1) {
        digitalWrite(Relay7,LOW);
        LLAP.sendMessage(String("RELAY7OFF"));
      } else {
        digitalWrite(Relay7,HIGH);
        LLAP.sendMessage(String("RELAY7ON"));
      }
    }
    if (LLAP.sMessage.equals("RELAY8TOG")) { 
    tmp = digitalRead(Relay8);
      if (tmp == 1) {
        digitalWrite(Relay8,LOW);
        LLAP.sendMessage(String("RELAY8OFF"));

      } else {
        digitalWrite(Relay8,HIGH);
        LLAP.sendMessage(String("RELAY8ON"));
      }
    }
    if (LLAP.sMessage.equals("RELAYAON0")) { 
      digitalWrite(Relay1,HIGH);
      digitalWrite(Relay2,HIGH);
      digitalWrite(Relay3,HIGH);
      digitalWrite(Relay4,HIGH);
      digitalWrite(Relay5,HIGH);
      digitalWrite(Relay6,HIGH);
      digitalWrite(Relay7,HIGH);
      digitalWrite(Relay8,HIGH);
      LLAP.sendMessage(String("RELAYAON"));
    }
    if (LLAP.sMessage.equals("RELAYAOFF")) { 
      digitalWrite(Relay1,LOW);
      digitalWrite(Relay2,LOW);
      digitalWrite(Relay3,LOW);
      digitalWrite(Relay4,LOW);
      digitalWrite(Relay5,LOW);
      digitalWrite(Relay6,LOW);
      digitalWrite(Relay7,LOW);
      digitalWrite(Relay8,LOW);
      LLAP.sendMessage(String("RELAYAOFF"));
    }
    LLAP.bMsgReceived = false;
  } // end message received 
} //end loop
