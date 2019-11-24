//EEPROM manual http://codius.ru/articles/Arduino_EEPROM_работа_с_энергонезависимой_памятью

#include "ButtonSwitch.h"
#include <EEPROM.h>
#include "AxpHelper.h"
#include "DisplayHelper.h"
#include "main.h"

int buttonPin = ButtonPinV2;
int buttonState = 0;
int ledPin = LedPinV1;

struct SavedObject {
  byte isLisa;
  byte loraSendTimer;
};
int eeAddressForSavedObject = 0;

SavedObject savedObject;

int touchCounter = 0;

void setLisaZaryaOnDisplay();
void onLongPress();
void onShortPress();
void clearEEPROM();
byte senderTimer();
int getSenderTimer();
void restoreLedState();
void blinkLed(int count);


void ButtonSwitch_setup() {
    savedObject = {
        0,
        10,
    };

    EEPROM.begin(sizeof(savedObject));

    EEPROM.get(eeAddressForSavedObject, savedObject);
    if ((savedObject.isLisa != 0) && (savedObject.isLisa != 1)) {
        clearEEPROM();
    }
    
    if (isAxpEnabled()) {
        buttonPin = ButtonPinV2;
    } else {
        buttonPin = ButtonPinV1;
        pinMode(ledPin, OUTPUT);
    }
    pinMode(buttonPin, INPUT);
    
    Serial.println("button pin is " + String(buttonPin));

    //savedObject.isLisa = 0;

    /* read saved data */
    Serial.println("read saved data");
    EEPROM.get(eeAddressForSavedObject, savedObject);
  
    if (sizeof(savedObject)>0) {
        Serial.println("savedObject NOT NULL");
    } else {
        Serial.println("savedObject IS NULL");
    }

    if (savedObject.isLisa != 1) {
        //Serial.println("init savedObject.isLisa: 0");
        Serial.println("init savedObject.isLisa: "+String(savedObject.isLisa));

        if (isAxpEnabled()) {
            SetAxpChgLed(false);
        } else {
            digitalWrite(ledPin, LOW); 
        }
    } else {
        Serial.println("init savedObject.isLisa: 1");

        if (isAxpEnabled()) {
            SetAxpChgLed(true);
        } else {
            digitalWrite(ledPin, HIGH); 
        }
    }
    setLisaZaryaOnDisplay();
}



void ButtonSwitch_loop() {
    buttonState = digitalRead(buttonPin);

      //detect if button is pushed
    if (buttonState == LOW) {
        //Serial.println("BUTTON PUSHED");

        touchCounter++;

        if (touchCounter > 3) {
            onLongPress();
            touchCounter = 0;
            delay(2000);
        }

/*
        if (savedObject.isLisa != 1) {
            
        } else {
            
        }*/
        
    } else {
        //Serial.println("BUTTON DETACH");

        if (touchCounter > 0) {
            touchCounter = 0;
            onShortPress();
            delay(100);
        }
    }
}

void onLongPress() {
    EEPROM.get(eeAddressForSavedObject, savedObject);
        /*Serial.println("----savedObject.isLisa---- "+String(savedObject.isLisa));
        if (savedObject.isLisa != 1) {
            Serial.println("current savedObject.isLisa: 0");
        } else {
            Serial.println("current savedObject.isLisa: 1");
        }*/

        if (savedObject.isLisa != 1) {
            Serial.println("switch savedObject.isLisa = 1");
            savedObject.isLisa = 1;
            
        } else {
            Serial.println("switch savedObject.isLisa = 0");
            savedObject.isLisa = 0;
            
        }
        Serial.println("save savedObject: " + String(savedObject.isLisa));
        EEPROM.put(eeAddressForSavedObject, savedObject);
        EEPROM.commit();

        restoreLedState();
}

void onShortPress() {
    Serial.println("Short press !!!");
    EEPROM.get(eeAddressForSavedObject, savedObject);
    byte loraSendTimer = savedObject.loraSendTimer;
    //Serial.println("current loraSendTimer: "+String(loraSendTimer, DEC));
    if (loraSendTimer >= 0 && loraSendTimer < 120) {
        loraSendTimer = loraSendTimer + 5;
        if (loraSendTimer >= 120) {
            loraSendTimer = 0;
        }
    } else {
        loraSendTimer = 0;
    }
    savedObject.loraSendTimer = loraSendTimer;
    Serial.println("loraSendTimer: "+String(loraSendTimer, DEC));

    EEPROM.put(eeAddressForSavedObject, savedObject);
    EEPROM.commit();

    setLisaZaryaOnDisplay();

    //мигнуть светодиодом кратное число раз, для 0==1, 5==2, 10==3 и т.д.
    //TODO: blink led only if display not availabel
    //int blinkCounter = loraSendTimer/5+1;
    //Serial.println("will blink: "+String(blinkCounter)+" times");
    //blinkLed(blinkCounter);
    
}

void clearEEPROM() {
    Serial.println("clear EEPROM");
    int len = EEPROM.length();
    Serial.println("EEPROM length: " + String(len));
    for (int i=0; i<EEPROM.length(); i++) {     // Обнуляем EEPROM - приводим в первоначальное состояние
        EEPROM.write(i, 0);
    }
    savedObject.isLisa = 0;
    savedObject.loraSendTimer = 0;
    EEPROM.put(eeAddressForSavedObject, savedObject);

    EEPROM.commit();
}

void setLisaZaryaOnDisplay() {
    String lisaZarya =  + (isLisa() ? "FOX" : "DAWN"); 
    setLisaZarya(lisaZarya);

    setSenderTimer(senderTimer());

    DisplayHelper_draw();
}

bool isLisa() {
    return (savedObject.isLisa == 1);
}

byte senderTimer() {
    return savedObject.loraSendTimer;
}

int getSenderTimer() {
    EEPROM.get(eeAddressForSavedObject, savedObject);
    byte loraSendTimer = savedObject.loraSendTimer;
    return loraSendTimer;
}

void blinkLed(int count) {
    if (isAxpEnabled()) {
        SetAxpChgLed(false);
    } else {
        digitalWrite(ledPin, LOW); 
    }
    delay(1000);
    
    for (int i=1; i<count; i++) {
        if (isAxpEnabled()) {
            SetAxpChgLed(false);
        } else {
            digitalWrite(ledPin, LOW); 
        }
        delay(300);
        if (isAxpEnabled()) {
            SetAxpChgLed(true);
        } else {
            digitalWrite(ledPin, HIGH); 
        }
        delay(100);
    }
    delay(500);
    //restore LED state
    restoreLedState();

}

void restoreLedState() {
    EEPROM.get(eeAddressForSavedObject, savedObject);
    if (savedObject.isLisa == 1) {
        if (isAxpEnabled()) {
            SetAxpChgLed(true);
        } else {
            digitalWrite(ledPin, HIGH); 
        }
    } else
        if (isAxpEnabled()) {
            SetAxpChgLed(false);
        } else {
            digitalWrite(ledPin, LOW); 
        }
    setLisaZaryaOnDisplay();
}