#include "QueueHelper.h"
#include <cppQueue.h>
#include "BluetoothHelper.h"

#include <Arduino.h>
#include <stdio.h>
#include <HardwareSerial.h>
//#include "HttpServerHelper.h"

#include "Time.h"
#include "Tools.h"
#include "ButtonSwitch.h"

struct FoxDictionary{
  char serial[6];
  int lastCounter;
};

FoxDictionary foxDictionaryArray[20];


float multiplier2 = 1.5152814573F;

//52 lora packet size
uint8_t msg[52];
char chStr[100];

void parseBuffer(uint8_t msg[]);

Queue q(sizeof(msg), 20, FIFO);	// Instantiate queue for parse messages
Queue qSend(sizeof(msg), 20, FIFO);	// Instantiate queue for send messages

Queue qHttp(sizeof(chStr), 100, FIFO);	// Instantiate queue for parse messages


void setFoxCounter(char serial[6], int counter);
int getFreeFoxIndex();
int getFoxIndex(char serial[6]);
int getFoxCounter(char serial[6]);

char serial[6] = {};

void QueueHelper_setup() {
    q.clean();
    qSend.clean();
    
    FoxDictionary fox = {
      {},
      0
    };
    sprintf(fox.serial, "%s","000000");
    for (int i=0; i<20; i++) {
      foxDictionaryArray[i] = fox;
    };

}

void setFoxCounter(char serial[6], int counter) {
  debugLog("setFoxCounter: "+String(serial)+", counter: "+String(counter, DEC));
  int index = getFoxIndex(serial); 
  debugLog("getFoxIndex: "+String(index));
  FoxDictionary fox = {
    {}, 
    0
  };
  String ser = String(serial);
  debugLog("setup counter for serial: "+ser);
  sprintf(fox.serial, "%s", ser);
  fox.lastCounter = counter; 

  debugLog("will setup fox.serial: "+String(fox.serial));
  debugLog("will setup fox.counter: "+String(fox.lastCounter));
  

  foxDictionaryArray[index] = fox;
}

int getFreeFoxIndex() {
  int result = 19;
  for (int i=0; i<20; i++) {
    FoxDictionary fox = foxDictionaryArray[i];
    String str = String(fox.serial);
    if (str == "000000") {
      result = i;
      break;
    }
  }
  //debugLog("getFreeFoxIndex: "+String(result,DEC));
  return result;
}

int getFoxIndex(char serial[6]) {
  debugLog("getFoxIndex for "+String(serial));
  int result = -1;
  for (int i=0; i<20; i++) {
    FoxDictionary fox = foxDictionaryArray[i];
    if (String(fox.serial) == String(serial)) {
      result = i;
      debugLog("just found index "+String(i)+" for serial "+String(serial));
      break;
    }
  }
  if (result == -1) {
    debugLog("not found index for serial "+String(serial)+", will get free index");

    result = getFreeFoxIndex();
    debugLog("new index for serial "+String(serial)+" is "+String(result));
  } 
  return result;
}

int getFoxCounter(char serial[6]) {
  debugLog("getFoxCounter for serial: "+String(serial));
  int result = -1;
  for (int i=0; i<20; i++) {
    FoxDictionary fox = foxDictionaryArray[i];
    debugLog("test fox serial: "+String(fox.serial));
    if (String(fox.serial) == String(serial)) {
      debugLog("found fox for serial: "+String(fox.serial)+", counter: "+String(fox.lastCounter, DEC));
      result = fox.lastCounter;
      break;
    }
  }
  return result;
}

void QueueHelper_loop() {
  //debugLog("QueueHelper_loop");
  unsigned int i;
  int count = 20 - q.getRemainingCount();
  for (i = 0 ; i < count; i++) { //sizeof(tab)/sizeof(Rec) ; i++)
    //debugLog("queue i: " + String(i, DEC));
		//Rec rec;
    uint8_t msg[52];
    for (int j = 0; j<52; j++) {
      msg[j] = 0;
    }
		if (q.pop(msg) == false) {
      return;
    }
    //debugLog("queue parse i: " + String(i, DEC));
	  parseBuffer(msg);
	}
  //debugLog("QueueHelper_loop end");
}

String getHttpLog(void) {
  String result = "{\"results\": [";
  unsigned int i;
  int count = 100 - qHttp.getRemainingCount();
  debugLog("http queue count: " + String(count, DEC));
  for (i = 0 ; i < count; i++) { 
    debugLog("http queue i: " + String(i, DEC));
		//Rec rec;
    char msg[100];
    for (int j = 0; j<100; j++) {
      msg[j] = 0;
    }
		if (qHttp.pop(msg) == false) {
      debugLog("http queue is empty");
    } else {
      //we have msg!
      //return msg
      //char chLat[12] = {0};
      String str = (i<(count-1)) ? "," : "";

      result = result+"{ \"msg\": \""+String(msg)+"\"}"+str;

      debugLog("http queue parse i: " + String(i, DEC));//+", result: "+result);
    }
	  //parseBuffer(msg);
	}
  result = result + "]}";
  return result;
}


int qSendGetRemainingCount() {
  return qSend.getRemainingCount();
}

ResultMessage qSendPop() {
  ResultMessage resultMessage = {};
  for (int j = 0; j<52; j++) {
    resultMessage.msg[j] = 0;
  }
  qSend.pop(resultMessage.msg);
  
  return resultMessage;
}


void PushToQueue(uint8_t msg[]) {
  q.push(msg);
}

void PushToQueueForLoraSend(uint8_t msg[]) {
  qSend.push(msg);
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

  char chStr[100] = {0};
  for (int i=0; i<logString.length(); i++) {
    chStr[i] = logString[i];
  }


  debugLog("qHttp.push(msg)");
  qHttp.push(chStr);
//  HttpServerHelper_AddLog(logString);

  DisplayHelper_draw();

  //проверить, нужно ли повторить отправку этого пакета...
  //1) проверим что это не наш собственный серийный номер
  //серийный номер из пакета это serialNumber
      
  //sprintf(serial, "%s","abcdef");
  //setFoxCounter(serial, 1);

  //void setFoxCounter(char serial[6], int counter);
  //int getFreeFoxIndex();
  //int getFoxIndex(char serial[6]);
  //int getFoxCounter(char serial[6]);

  if (isLisa() == true) {
    String ownSerialNumber = getSerialNumber();
    debugLog("test serials: own: "+ownSerialNumber+", pct: "+serialNumber);

    if (ownSerialNumber != serialNumber) {
      debugLog("need resend packet");
      //это не наш пакет. Проверить последний сохраненный counter для этого серийника
      char ser[7] = {0,0,0,0,0,0,0};
      sprintf(ser, "%s", serialNumber);
      int foxCounter = getFoxCounter(ser);
      debugLog("test counters: foxCounter: "+String(foxCounter, DEC)+", pct.counter: "+String(counter,DEC));
      if (foxCounter < int(counter)) {
        debugLog("setFoxCounter ser "+serialNumber+", counter: "+String(counter));
        setFoxCounter(ser, counter);
        //отправить пакет 
        debugLog("retransmit packet for "+serialNumber+", counter: "+String(counter));
        //PushToQueue(msg);
        PushToQueueForLoraSend(msg);
      }
    }
  }
}
