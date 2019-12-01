#ifndef TOOLS_H
#define TOOLS_H

void SetupSerialNumber(void);
String getSerialNumber(void);
void Shutdown(void);

void setDown(void);
bool getDown(void);

void debugLog(String message);

#endif /* TOOLS_H */