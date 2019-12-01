#include <Arduino.h>
#include "SSD1306.h"  
#include "AxpHelper.h"
#include "DisplayHelper.h"
#include "main.h"
#include "Tools.h"
#include "ButtonSwitch.h"
#include "Time.h"


//#include <U8x8lib.h>

//#ifdef U8X8_HAVE_HW_SPI
//#include <SPI.h>
//#endif

String TXline = "";
String RXline = "";
String localIP = "";
String bottomline = "";
String lisaZarya = "";
String serialNumber = "";
byte senderTimer = 0;

SSD1306 display(0x3c, 21, 22);
#define SDA 21
#define SCL 22
#define RESET 0x3C

//U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ RESET);

void showInitMessage();

void DisplayHelper_setup() {
    //u8x8.begin();
    //u8x8.setPowerSave(0);

    pinMode(16,OUTPUT);
    digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
    delay(100); 
    digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high
    delay(500);
    showInitMessage();
}

void DisplayHelper_loop() {
    DisplayHelper_draw();
}

void DisplayHelper_draw() {

    //Serial.println("DisplayHelper_draw");

    //u8x8.setFont(u8x8_font_chroma48medium8_r);
    //u8x8.drawString(0,0,"Hello World!");
    //u8x8.refreshDisplay();		// only required for SSD1606/7  
    //delay(2000);

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);
  if (!getDown()) {
    //display.setFont(ArialMT_Plain_16);
        
    display.drawStringMaxWidth(0, 0,   128, serialNumber + ":" + lisaZarya);

    display.setFont(ArialMT_Plain_10);

    if (isLisa()) {
        //send timer seconds show
        display.drawStringMaxWidth(110, 0,   28, String(senderTimer, DEC));
    }

    display.drawStringMaxWidth(0, 17 , 128, TXline);
    
    display.drawStringMaxWidth(0, 34 , 128, RXline);

    String battery = "";
    if (isAxpEnabled()) {
        float battVoltage = getBatteryVoltage();
        battery = "V: "+String(battVoltage/1000,2)+", ";
    } 


    time_t t = now();
    //uint8_t min = minute(t);
    uint8_t sec = second(t); 
    uint8_t min = minute(t); 
    uint8_t hr = hour(t); 
    String currentTime = String(hr)+":"+String(min)+":"+String(sec);

    //display.setFont(ArialMT_Plain_10);
    display.drawStringMaxWidth(0, 50 , 128, battery+localIP+", "+currentTime);
  } else {
      display.drawStringMaxWidth(4, 26,   128, "...SHUTDOWN...");
  }
  display.display();
}

void showInitMessage() {
    /*
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    u8x8.drawString(0,0,"Hello World!");
    u8x8.refreshDisplay();		// only required for SSD1606/7  */
    
    display.init();
    display.flipScreenVertically();  
    display.setFont(ArialMT_Plain_16);
    display.clear();
    RXline = "LOADING";
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawStringMaxWidth(0, 17 , 128, RXline);
    display.display();
}

void setTxLine(String str) {
    TXline = str;
}

void setRxLine(String str) {
    RXline = str;
}

void setLocalIP(String str) {
    localIP = str;
}

void setBottomLine(String str) {
    bottomline = str;
}

void setLisaZarya(String str) {
    lisaZarya = str;
}

void setSerialNumber(String str) {
    serialNumber = str;
}

void setSenderTimer(byte timer) {
    senderTimer = timer;
}

String getTxLine() {
    return TXline;
}

String getRxLine() {
    return RXline;
}

String getLocalIP() {
    return localIP;
}

String getBottomLine() {
    return bottomline;
}

String getLisaZarya() {
    return lisaZarya;
}
