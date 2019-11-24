#include "main.h"

#include <Wire.h>

#include "AxpHelper.h"
#include "ButtonSwitch.h"
#include "DisplayHelper.h"
#include "Tools.h"
#include "WiFiHelper.h"
#include "SyslogHelper.h"
#include "GpsHelper.h"

#include <Thread.h>
//#include <ThreadRunOnce.h>
#include <ThreadController.h>

#include "QueueHelper.h"
#include "BluetoothHelper.h"

// ThreadController that will controll all threads
ThreadController controll = ThreadController();

Thread* buttonsThread = new Thread();
void buttonsThreadCallback() {
  ButtonSwitch_loop();
  if (isAxpEnabled()) {
    if (isPEKShortPress()) {
      Serial.println("isPEKShortPressIRQ");
    }
    if (isPEKLongPress()) {
      Serial.println("isPEKLongPressIRQ");
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

void sendlog();
void shutdown(const char *msg);

bool init_ok = false;

void sendlog()
{
  syslog(getTxLine());
  syslog(getRxLine());
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
  
  SetupWiFi();
  
  LoraHelper_setup();  
  DisplayHelper_draw();

  ButtonSwitch_setup();

  SyslogHelper_setup();

  if (isLisa()) {
    GpsHelper_setup();
    //TODO: send gps coordinates via bluetooth to smartphone 
  } else {
    BluetoothHelper_setup();
  }


  syslog("init ok");


  setRxLine("");

  buttonsThread->onRun(buttonsThreadCallback);
  buttonsThread->setInterval(1000);
  controll.add(buttonsThread);

  loraReceiveThread->onRun(loraReceiveThreadCallback);
  loraReceiveThread->setInterval(20);
  controll.add(loraReceiveThread);

  loraSendThread->onRun(loraSendThreadCallback);
  loraSendThread->setInterval(10000);
  controll.add(loraSendThread);

  if (isLisa()) {
    gpsReadThread->onRun(gpsReadThreadCallback);
    gpsReadThread->setInterval(10000);
    controll.add(gpsReadThread);
  } 
  
  queueHelperThread->onRun(queueHelperThreadCallback);
  queueHelperThread->setInterval(10000);
  controll.add(queueHelperThread);


//  displayUpdateThread->onRun(displayUpdateThreadCallback);
//  displayUpdateThread->setInterval(1000);
//  controll.add(displayUpdateThread);
  //GpsHelper_loop();

  DisplayHelper_draw();

  init_ok = true;
}

void loop() {
  controll.run();    
}
