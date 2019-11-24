#include "LoraHelper.h"
#include <LoRa.h>
#include "DisplayHelper.h"
#include "GpsHelper.h"
#include "Tools.h"
#include "SyslogHelper.h"
#include "ButtonSwitch.h"

#include "QueueHelper.h"

#define BAND  433000E3
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)


float multiplier = 1.5152814573F;

PACKET struct_data;

String rssi = "RSSI --";
String packet;

unsigned long counter = 0;


Coordinates lastCoordinates = {};

void cbk(int packetSize);

void LoraHelper_setup(void) {

    LoRa.setPins(SS,RST,DI0);
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa ...");
        while (1);
    }
    LoRa.onReceive(cbk);
    LoRa.setSpreadingFactor(12);
    LoRa.crc();
    LoRa.receive();
    LoRa.setOCP(150);

    counter = 0;
}

void LoraHelper_send_loop() {
  Coordinates currentCoordinates = getCoordinates(); 
  /*if (
    (currentCoordinates.isValid) && (
      (currentCoordinates.lat != lastCoordinates.lat) ||
      (currentCoordinates.lng != lastCoordinates.lng) ||
      (currentCoordinates.date != lastCoordinates.date) ||
      (currentCoordinates.time != lastCoordinates.time) 
  )) {*/
  if (1 == 1) {
    // prepare send data
    String serialNumber = getSerialNumber();
    for (int i=0; i<6; i++) {
      struct_data.serialNumber[i] = serialNumber.charAt(i);
    }
    counter++;
    struct_data.counter = counter;

    String((currentCoordinates.lat * multiplier),8).toCharArray(struct_data.lat, 12);
    String((currentCoordinates.lng * multiplier),8).toCharArray(struct_data.lng, 12);

    for (int i=0; i<8; i++) {
      struct_data.date[i] = currentCoordinates.date[i];
    }

    for (int i=0; i<6; i++) {
      struct_data.time[i] = currentCoordinates.time[i];
    }
    //lat: 51.508129
    //lng: 10.128002

    //Sending Side
    char b[sizeof(struct_data)];
    memcpy(b, &struct_data, sizeof(struct_data));

    PACKET tmp; //Re-make the struct
    memcpy(&tmp, b, sizeof(tmp));

    syslog(" ");
    syslog("----");
    syslog("size: "+String(sizeof(b)));
    syslog("tmp size: "+String(sizeof(tmp)));
    syslog("tmp packet.serialNumber: "+String(tmp.serialNumber));
    syslog("tmp packet.counter: "+String(tmp.counter));
    syslog("tmp packet.lat: "+String(currentCoordinates.lat,8));
    syslog("tmp packet.lng: "+String(currentCoordinates.lng,8));

    syslog("tmp packet.date: "+String(tmp.date));
    syslog("tmp packet.time: "+String(tmp.time));
  
    // send packet
    LoRa.beginPacket();
    //LoRa.print("hello ");
    //LoRa.print(counter);
    LoRa.write((uint8_t*)&b, sizeof(b));
    LoRa.endPacket();

    int len = (counter > 999) ? 5 : 6;
    setTxLine(String(counter)+" "+String((currentCoordinates.lat),len)+" "+String((currentCoordinates.lng),len));
    DisplayHelper_draw();
    
    lastCoordinates = currentCoordinates;
  }
}

void LoraHelper_receive_loop() {
  LoRa.receive();
  /*

  int packetSize = LoRa.parsePacket();
  if (packetSize>0) {
    Serial.println("");
    Serial.println(String(packetSize));
    Serial.println("");
  } else {
    //Serial.print(String(packetSize));
  }
  if (packetSize>0) { cbk(packetSize);  }*/
}

void cbk(int packetSize) {
  if (isLisa()) {
    return;
  }

  if (packetSize <1 ) {
    return;
  }

  String packetStr = "";
  String packSize = String(packetSize,DEC);
  //Serial.println("packSize: "+packSize);

  uint8_t msg[52];
  for (int i=0; i<52; i++) {
    msg[i] = 0;
  }

  for (int i = 0; i < packetSize; i++) { 
    //uint8_t bb = LoRa.read();
    int bb = LoRa.read();
    char ch = (char) bb;
    //Serial.print(ch);
    msg[i] = bb;
    packetStr += ch;  
  }
  //Serial.println("");
  //Serial.println(packetStr);
  //Serial.println("");

  //Serial.println("PushToQueue");

  PushToQueue(msg);
  return;
/*

  char b[52] = "";
  for (int i = 0; i < packetSize; i++) { 
    uint8_t bb = LoRa.read();
    char ch = (char) bb;
    Serial.print(ch);
    b[i] = bb;
    packetStr += ch;  
  }
  for (int i=packetSize; i<52; i++) {
    b[i] = 0;
  }
  Serial.println("");
  Serial.println(packetStr);
  Serial.println("");

  Serial.println("PushToQueue");
  

  PushToQueue(packetStr);
  return;*/

  //Serial.println("");
  //Serial.println(packetStr);
  //Serial.println("");
  char b[52];

  //rssi = "RSSI " + String(LoRa.packetRssi(), DEC);
  rssi = ""+String(LoRa.packetRssi(), DEC);
  //Serial.println("--rssi-- "+ rssi);
  PACKET tmp; //Re-make the struct
  memcpy(&tmp, b, sizeof(tmp));


  unsigned long counter = tmp.counter;
  String serialNumber = String(tmp.serialNumber);
  Serial.println(serialNumber+", rssi: " +rssi);

  char tmpLat[12] = {0};
  for (int i=0; i<11; i++) {
    tmpLat[i] = tmp.lat[i];
  }

  char tmpLng[12] = {0};
  for (int i=0; i<11; i++) {
    tmpLng[i] = tmp.lng[i];
  }

  String latStr(tmpLat);
  String lngStr(tmpLng);
  
  char chLat[12] = {0};
  char chLng[12] = {0};

  sprintf(chLat,"%2.8f", latStr.toFloat() / multiplier);
  sprintf(chLng,"%2.8f", lngStr.toFloat() / multiplier);
  
  int len = (counter > 999) ? 8 : 9;
  String rxLine = String(counter)+" "+String(chLat).substring(0,len)+" "+String(chLng).substring(0,len);
  setRxLine(rxLine);

  char date[9] = {0};
  char time[7] = {0};

  for (int i=0; i<8; i++) {
    date[i] = tmp.date[i];
  }
  for (int i=0; i<6; i++) {
    time[i] = tmp.time[i];
  }

  String logString = ""+String(serialNumber)+","+String(counter)+","+String(chLat)+","+String(chLng)+","+String(date)+","+String(time);
  Serial.println(logString);

  //crash if update display from here
  //DisplayHelper_draw();
    
}
