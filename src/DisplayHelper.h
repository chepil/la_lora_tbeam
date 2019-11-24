#ifndef DISPLAYHELPER_H
#define DISPLAYHELPER_H

#include <WString.h>
#include <Arduino.h>

void setTxLine(String str);
void setRxLine(String str);
void setLocalIP(String str);
void setBottomLine(String str);
void setLisaZarya(String str);
void setSerialNumber(String str);
void setSenderTimer(byte timer);

String getTxLine();
String getRxLine();
String getLocalIP();
String getBottomLine();
String getLisaZarya();

void DisplayHelper_setup(void);
void DisplayHelper_draw(void);
void DisplayHelper_loop(void);

#endif /* DISPLAYHELPER_H */