#include <Arduino.h>
#include "GpsHelper.h"
#include <TinyGPS++.h>    
#include "AxpHelper.h"   
#include "SyslogHelper.h"    
#include "ButtonSwitch.h"

TinyGPSPlus gps;                            
HardwareSerial SerialGPS(1);     

int GPS_RX = 12;
int GPS_TX = 34;

Coordinates coordinates;

void GpsHelper_setup() {
  if (isLisa() == false) {
    return;
  }

  if (isAxpEnabled()) {
    GPS_RX = 12;
    GPS_TX = 34;
  } else {
    GPS_RX = 15;
    GPS_TX = 12;
  }

  coordinates = {};

  SerialGPS.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);   //12, 15, //34-TX 12-RX
}

void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  String str="";
  do {
    while (SerialGPS.available()) {
      char ch = SerialGPS.read();
      gps.encode(ch);
      str = str+ch;
    }
  } while (millis() - start < ms);
  if (str != "") {
    //syslog(str);

    if (gps.location.isValid()) {
      coordinates.lat = gps.location.lat();
      coordinates.lng = gps.location.lng();
    } else {
      coordinates.lat = 0;
      coordinates.lng = 0;
    }

    if (gps.date.isValid()) {
      uint16_t year = gps.date.year();
      uint8_t month = gps.date.month();
      uint8_t day = gps.date.day();
      //coordinates.date = "";
      sprintf(coordinates.date,"%02d%02d%04d", day, month, year);
    } else {
      sprintf(coordinates.date,"00000000");
    }
    if (gps.time.isValid()) {
      uint8_t hour = gps.time.hour();
      uint8_t minute = gps.time.minute();
      uint8_t second = gps.time.second();
      
      sprintf(coordinates.time,"%02d%02d%02d", hour, minute, second);
    } else {
      sprintf(coordinates.time,"000000");
    }

    coordinates.isValid = gps.location.isValid() && gps.date.isValid() && gps.time.isValid();
    
    String isValidStr = (coordinates.isValid ? "true" : "false");
    syslog("");
    syslog("isValid: " + isValidStr);
    
    syslog("lat: "+String(coordinates.lat,8)+", lng: "+String(coordinates.lng,8));
    syslog("date: "+String(coordinates.date));
    syslog("time: "+String(coordinates.time));
    syslog("");
  }  
}

void GpsHelper_loop() {
  if (isLisa() == false) {
    return;
  }
  smartDelay(5000);
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    syslog("No GPS data received: check wiring");
    //syslog(F("No GPS data received: check wiring"));
  }
}

Coordinates getCoordinates() {
  return coordinates;
}