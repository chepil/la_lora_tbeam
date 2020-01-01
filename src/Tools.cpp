#include <ArduinoUniqueID.h>
#include "DisplayHelper.h"
#include "Tools.h"
#include "main.h"
#include "AxpHelper.h"

#include <vector>

String showSerialNumber = "";
String id8SerialNumber = "";

bool down = false;

void printDebugLog();

std::vector<String> arrayOfDebugLog;

String getSerialNumber() {
    return showSerialNumber;
}

void SetupSerialNumber() {

  arrayOfDebugLog.push_back("SetupSerialNumber"); 
  //UniqueIDdump(Serial);
  UniqueID8dump(Serial);
  for (size_t i = 0; i<8; i++) {
    id8SerialNumber = id8SerialNumber + ((UniqueID8[i] < 0x10) ? "0" : String(UniqueID8[i], HEX));
	}
  int skip = 0;
  showSerialNumber = id8SerialNumber;
  for (int i=0; i<showSerialNumber.length(); i++) {
    if (showSerialNumber.charAt(i)=='0') {
      skip = skip + 1;
    } else {
      break;
    }
  }
  showSerialNumber.remove(0,skip);
  //show last 2 bytes only
  if (showSerialNumber.length() > 6) {
    showSerialNumber.remove(0, showSerialNumber.length()-6);
  }
  debugLog("id8 Serial number: "+id8SerialNumber);
  debugLog("screen Serial number: "+showSerialNumber);

  setSerialNumber(showSerialNumber);


}

void Shutdown() {
    setDown();
    DisplayHelper_draw();

    if (isAxpEnabled()) {
        SetAxpChgLed(false);
    } else {
        int ledPin = LedPinV1;
        digitalWrite(ledPin, LOW); 
    }
    delay(3000);
    doShutdown();
}


void setDown() {
  down = true;
}

bool getDown() {
  return down;
}

void debugLog(String message) {
  arrayOfDebugLog.push_back(message); 
  printDebugLog();
}

void printDebugLog() {
  while (!arrayOfDebugLog.empty()) { 
    String message = arrayOfDebugLog[0];
    int sm = sizeof(message);
    int len = Serial.availableForWrite();
    if (len >= sm) {
      arrayOfDebugLog.pop_back();
      Serial.println(message);
      delay(100);
    }
  } 
}