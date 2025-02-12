#include "main.h"

#include <Wire.h>

#include "ButtonSwitch.h"
#include "DisplayHelper.h"
#include "Tools.h"
#include "WiFiHelper.h"
#include "SyslogHelper.h"
#include "GpsHelper.h"


//#include <ThreadRunOnce.h>

#include "QueueHelper.h"
#include "BluetoothHelper.h"

#include "AxpHelper.h"
#include <Thread.h>
#include <ThreadController.h>
#include "HttpServerHelper.h"

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

Thread* buttonsThread = new Thread();
void buttonsThreadCallback() {
  
  ButtonSwitch_loop();

  if (isAxpEnabled()) {
    if (isPEKShortPress()) {
      debugLog("isPEKShortPressIRQ");
    }
    if (isPEKLongPress()) {
      debugLog("isPEKLongPressIRQ");
      Shutdown();
    }
  }
}

Thread* loraReceiveThread = new Thread();
void loraReceiveThreadCallback() {
  LoraHelper_receive_loop();
}

Thread* loraSendThread = new Thread();
void loraSendThreadCallback() {
  if (isLisa()) {
    LoraHelper_send_loop();
  }
}

Thread* gpsReadThread = new Thread();
void gpsReadThreadCallback() {
  GpsHelper_loop();
}

Thread* displayUpdateThread = new Thread();
void displayUpdateThreadCallback() {
  DisplayHelper_loop();
}

Thread* queueHelperThread = new Thread();
void queueHelperThreadCallback() {
  QueueHelper_loop();
}

Thread* ntpClientThread = new Thread();
void ntpClientThreadCallback() {
  NtpClientThread_loop();
}

void sendlog();
void shutdown(const char *msg);

bool init_ok = false;

void sendlog()
{
  debugLog(getTxLine());
  debugLog(getRxLine());
}
 
void setup() {
  SPI.begin(SCK,MISO,MOSI,SS);
  Serial.begin(115200);
  while (!Serial);
  DisplayHelper_setup();

  SetupSerialNumber();

  QueueHelper_setup();

  ButtonSwitch_setup();
  AxpSetup();
  
  //end new code
  
  //FIXME: add ntp sync time
//  https://www.geekstips.com/arduino-time-sync-ntp-server-esp8266-udp/
  SetupWiFi();
  
  LoraHelper_setup();  
  DisplayHelper_draw();

  ButtonSwitch_setup();

  SyslogHelper_setup();

  GpsHelper_setup();
  if (isLisa()) {  
    //TODO: send gps coordinates via bluetooth to smartphone 
  } else {
    //BluetoothHelper_setup();
    HttpServerHelper_setup();
  }

  debugLog("init ok");

  setRxLine("");

  buttonsThread->onRun(buttonsThreadCallback);
  buttonsThread->setInterval(300);
  controll.add(buttonsThread);

  loraReceiveThread->onRun(loraReceiveThreadCallback);
  loraReceiveThread->setInterval(20);
  controll.add(loraReceiveThread);

  loraSendThread->onRun(loraSendThreadCallback);
  loraSendThread->setInterval(1000);
  controll.add(loraSendThread);

  if (isLisa()) {
    gpsReadThread->onRun(gpsReadThreadCallback);
    gpsReadThread->setInterval(30000);
    controll.add(gpsReadThread);
  } 
  
  queueHelperThread->onRun(queueHelperThreadCallback);
  queueHelperThread->setInterval(10000);
  controll.add(queueHelperThread);

  ntpClientThread->onRun(ntpClientThreadCallback);
  ntpClientThread->setInterval(1000);
  controll.add(ntpClientThread);

  displayUpdateThread->onRun(displayUpdateThreadCallback);
  displayUpdateThread->setInterval(1000);
  controll.add(displayUpdateThread);
  //GpsHelper_loop();

  DisplayHelper_draw();

  init_ok = true;
}

void loop() {
  controll.run();    
}
