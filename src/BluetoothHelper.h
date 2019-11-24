#ifndef BLUETOOTHHELPER_H
#define BLUETOOTHHELPER_H

#include <Arduino.h>

void BluetoothHelper_setup(void);
void BluetoothHelper_loop(void);
void BluetoothHelper_SerialWrite(String inputString);

#endif /* BLUETOOTHHELPER_H */