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
};
int eeAddressForSavedObject = 0;

SavedObject savedObject;

int touchCounter = 0;

void setLisaZaryaOnDisplay();
void onLongPress();
void onShortPress();
void clearEEPROM();

void ButtonSwitch_setup() {
    savedObject = {
        0
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
            delay(300);
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
            if (isAxpEnabled()) {
                SetAxpChgLed(true);
            } else {
                digitalWrite(ledPin, HIGH); 
            }
        } else {
            Serial.println("switch savedObject.isLisa = 0");
            savedObject.isLisa = 0;
            if (isAxpEnabled()) {
                SetAxpChgLed(false);
            } else {
                digitalWrite(ledPin, LOW); 
            }
        }
        Serial.println("save savedObject: " + String(savedObject.isLisa));
        EEPROM.put(eeAddressForSavedObject, savedObject);
        EEPROM.commit();

        setLisaZaryaOnDisplay();
}

void onShortPress() {
    Serial.println("Short press !!!");
}

void clearEEPROM() {
    Serial.println("clear EEPROM");
    int len = EEPROM.length();
    Serial.println("EEPROM length: " + String(len));
    for (int i=0; i<EEPROM.length(); i++) {     // Обнуляем EEPROM - приводим в первоначальное состояние
        EEPROM.write(i, 0);
    }
    savedObject.isLisa = 0;
    EEPROM.put(eeAddressForSavedObject, savedObject);

    EEPROM.commit();
}

void setLisaZaryaOnDisplay() {
    String lisaZarya =  + (isLisa() ? "FOX" : "DAWN"); 
    setLisaZarya(lisaZarya);

    DisplayHelper_draw();
}

bool isLisa() {
    return (savedObject.isLisa == 1);
}