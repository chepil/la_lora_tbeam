#include <Arduino.h>

#include <WString.h>
#include "stdint.h"
#include <stdio.h>

#include <SPI.h>
#include "LoraHelper.h"


#ifndef MAIN_H
#define MAIN_H

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

#define ButtonPinV1 39
#define ButtonPinV2 38

#define LedPinV1 14

#endif /* MAIN_H */

