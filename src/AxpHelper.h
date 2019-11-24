/*****************************************
  ESP32 GPS VKEL 9600 Bds
This version is for T22_v01 20190612 board
As the power management chipset changed, it
require the axp20x library that can be found
https://github.com/lewisxhe/AXP202X_Library
You must import it as gzip in sketch submenu
in Arduino IDE
This way, it is required to power up the GPS
module, before trying to read it.
Also get TinyGPS++ library from: 
https://github.com/mikalhart/TinyGPSPlus
******************************************/


#ifndef AXPHELPER_H
#define AXPHELPER_H

#include <axp20x.h>

bool isAxpEnabled(void);
void AxpSetup(void);
void SetAxpChgLed(bool isOn);
float getBatteryVoltage(void);

bool isPEKShortPress();
bool isPEKLongPress();

void doShutdown(void);

#endif /* AXPHELPER_H */
