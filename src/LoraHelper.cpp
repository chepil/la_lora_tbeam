#include "LoraHelper.h"
#include <LoRa.h>
#include "DisplayHelper.h"
#include "GpsHelper.h"
#include "Tools.h"
#include "SyslogHelper.h"
#include "ButtonSwitch.h"

#include "QueueHelper.h"
#include "Time.h"


#define BAND  433000E3
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)


float multiplier = 1.5152814573F;

PACKET struct_data;

String rssi = "RSSI --";
String packet;

unsigned long counter = 0;


Coordinates lastCoordinates = {};

bool STOP_RECEIVER = false;

void cbk(int packetSize);
bool itsTimeToSend();

void LoraHelper_setup(void) {

    LoRa.setPins(SS,RST,DI0);
    if (!LoRa.begin(BAND)) {
        debugLog("Starting LoRa ...");
        while (1);
    } else {
      debugLog("LoRa Started");
    }
    //LoRa.receive();
    //LoRa.onReceive(cbk);
    LoRa.setSpreadingFactor(12);
    LoRa.crc();
    LoRa.setOCP(150);
    LoRa.setTxPower(17);

    counter = 0;
}

bool itsTimeToSend() {
  bool result = false;

  time_t t = now();
  //uint8_t min = minute(t);
  uint8_t sec = second(t); 

  if (sec%5 == 0) {
    //debugLog("sec time: "+String(sec));
  }

  //TODO переделать на учет 2х минут, с четными/нечетными минутами, и распределить 120 секунд на 2 минуты
  int loraSenderTimer = getSenderTimer();
  if (loraSenderTimer < 60) {
    result = (loraSenderTimer == sec);
  }


  return result;
}

void LoraHelper_send_loop() {

  if (!itsTimeToSend()) {
    return;
  }
  debugLog("LoraHelper_send_loop itsTimeToSend");
  STOP_RECEIVER = true;


  Coordinates currentCoordinates = getCoordinates(); 
  //Serial.println("currentCoordinates received");
    /*
    if ((currentCoordinates.isValid) && (
      (currentCoordinates.lat != lastCoordinates.lat) ||
      (currentCoordinates.lng != lastCoordinates.lng) ||
      (currentCoordinates.date != lastCoordinates.date) ||
      (currentCoordinates.time != lastCoordinates.time) 
      )
    */
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

    //debugLog(" ");
    //debugLog("----");
    //debugLog("size: "+String(sizeof(b)));
    //debugLog("tmp size: "+String(sizeof(tmp)));
    //debugLog("tmp packet.serialNumber: "+String(tmp.serialNumber));
    //debugLog("tmp packet.counter: "+String(tmp.counter));
    //debugLog("tmp packet.lat: "+String(currentCoordinates.lat,8));
    //debugLog("tmp packet.lng: "+String(currentCoordinates.lng,8));
    //debugLog("tmp packet.date: "+String(tmp.date));
    //debugLog("tmp packet.time: "+String(tmp.time));
    
    //delay(100);
    // send packet

    // Each packet can contain up to 255 bytes.

    LoRa.beginPacket();

    //LoRa.print("hello ");
    //LoRa.print(counter);
    LoRa.write((uint8_t*)&b, sizeof(b));
    int success = LoRa.endPacket(true); //async, true enables non-blocking mode, false waits for transmission to be completed (default)
    
    debugLog("LORA success: "+String(success));

    int len = (counter > 999) ? 5 : 6;
    
    setTxLine(String(counter)+" "+String((currentCoordinates.lat),len)+" "+String((currentCoordinates.lng),len));
    //БУДЕТ ГЛЮК ЕСЛИ ЗДЕСЬ СДЕЛАТЬ ВЫЗОВ DisplayHelper_draw
    DisplayHelper_draw();

    lastCoordinates = currentCoordinates;
    
    delay(1000);
    
  }
  STOP_RECEIVER = false;
  debugLog("LoraHelper_send_loop end");
  
}

void LoraHelper_receive_loop() {

  if (STOP_RECEIVER == true) {
    return;
  } else {
    //Serial.print(".");
    //LoRa.receive();

    /*int availableBytes = LoRa.available();
    if (availableBytes > 0) {
      debugLog(">>>>> availableBytes: "+String(availableBytes));
    }*/
    int packetSize = LoRa.parsePacket();
    if (packetSize>0) {
      debugLog("");
      debugLog(">>>>>");
      debugLog(String(packetSize));
      debugLog("");
      cbk(packetSize);
    } else {
      //Serial.print(String(packetSize));
    }
    //if (packetSize>0) { cbk(packetSize);  }
  }
}

void cbk(int packetSize) {
  debugLog("-=-=-=-=-=-=-=-= cbk =-=-=-=-=-=-=-=-=-");
  //if (isLisa()) {
  //  return;
  //}

  if (packetSize <1 ) {
    return;
  }

  String packetStr = "";
  String packSize = String(packetSize,DEC);
  //debugLog("packSize: "+packSize);

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
  //debugLog("");
  //debugLog(packetStr);
  //debugLog("");

  //debugLog("PushToQueue");
  debugLog("-=-=-=-=-=-=-=-= end cbk =-=-=-=-=-=-=-=-=-");

  PushToQueue(msg);
    
  debugLog("-=-=-=-=-=-=-=-= end push to queue =-=-=-=-=-=-=-=-=-");

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
  debugLog("");
  debugLog(packetStr);
  debugLog("");

  debugLog("PushToQueue");
  

  PushToQueue(packetStr);
  return;*/

  //debugLog("");
  //debugLog(packetStr);
  //debugLog("");
  char b[52];

  //rssi = "RSSI " + String(LoRa.packetRssi(), DEC);
  rssi = ""+String(LoRa.packetRssi(), DEC);
  //debugLog("--rssi-- "+ rssi);
  PACKET tmp; //Re-make the struct
  memcpy(&tmp, b, sizeof(tmp));


  unsigned long counter = tmp.counter;
  String serialNumber = String(tmp.serialNumber);
  debugLog(serialNumber+", rssi: " +rssi);

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
  debugLog(logString);

  //crash if update display from here
  //DisplayHelper_draw();
    
}
