#include "QueueHelper.h"
#include <cppQueue.h>
#include "BluetoothHelper.h"

//#include <Arduino.h>
#include <stdio.h>
#include <HardwareSerial.h>

#include "Time.h"
#include "Tools.h"


float multiplier2 = 1.5152814573F;

//52 lora packet size
uint8_t msg[52];

void parseBuffer(uint8_t msg[]);

Queue q(sizeof(msg), 20, FIFO);	// Instantiate queue

void QueueHelper_setup() {
    q.clean();
}

void QueueHelper_loop() {
    unsigned int i;

    int count = 20 - q.getRemainingCount();

    //if (count > 0) {
    //    Serial.println("start QueueHelper_loop, count: "+String(count));
    //}

    for (i = 0 ; i < count; i++) //sizeof(tab)/sizeof(Rec) ; i++)
	{
		//Rec rec;
        uint8_t msg[52];
        for (int i = 0; i<52; i++) {
            msg[i] = 0;
        }

		if (q.pop(msg) == false) {
            return;
        }
	    parseBuffer(msg);
	}
    //if (count > 0) {
    //    Serial.println("stop QueueHelper_loop");
    //}
}

void PushToQueue(uint8_t msg[]) {
  //Serial.println("--PushToQueue--");
  q.push(msg);
}

void parseBuffer(uint8_t msg[]) {
    
    //Serial.println("");
  //Serial.println(packetStr);
  //Serial.println("");

  //rssi = "RSSI " + String(LoRa.packetRssi(), DEC);
  //rssi = ""+String(LoRa.packetRssi(), DEC);
  //Serial.println("--rssi-- "+ rssi);

  PACKET tmp; //Re-make the struct
  memcpy(&tmp, msg, sizeof(tmp));

  unsigned long counter = tmp.counter;
  String serialNumber = String(tmp.serialNumber);
  //Serial.println(serialNumber);//+", rssi: " +rssi);

  String latStr="";
  String lngStr="";
  
  //int tmpLat[12];
  for (int i=0; i<11; i++) {
    //tmpLat[i] = tmp.lat[i];
    latStr = latStr + (char)tmp.lat[i];
  }

  //int tmpLng[12];
  for (int i=0; i<11; i++) {
    //tmpLng[i] = tmp.lng[i];
    lngStr = lngStr + (char)tmp.lng[i];
  }

  
//  String latStr(tmpLat);
//  String lngStr(tmpLng);
  
  char chLat[12] = {0};
  char chLng[12] = {0};

  sprintf(chLat,"%2.8f", latStr.toFloat() / multiplier2);
  sprintf(chLng,"%2.8f", lngStr.toFloat() / multiplier2);
  
  int len = (counter > 999) ? 8 : 9;
  String rxLine = String(counter)+" "+String(chLat).substring(0,len)+" "+String(chLng).substring(0,len);
  setRxLine(rxLine);

  String dateStr = "";
  String timeStr = "";

  //int date[9];
  //int time[7];

  for (int i=0; i<8; i++) {
    //date[i] = tmp.date[i];
    dateStr = dateStr + (char) tmp.date[i];
  }
  for (int i=0; i<6; i++) {
    //time[i] = tmp.time[i];
    timeStr = timeStr + (char) tmp.time[i];
  }

  time_t t = now();
  uint8_t sec = second(t);
 
  String logString = String(serialNumber)+","+String(counter)+","+String(chLat)+","+String(chLng)+","+String(dateStr)+","+String(timeStr);
  debugLog("-> "+String(sec)+"sec. "+logString);

  debugLog("");

  BluetoothHelper_SerialWrite(logString);

  DisplayHelper_draw();
  
}
