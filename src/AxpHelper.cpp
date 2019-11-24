#include "AxpHelper.h"
#include "ButtonSwitch.h"

AXP20X_Class axp;

bool axpEnabled;

bool isAxpEnabled(void) {
    return axpEnabled;
}

AXP20X_Class getAxp(void) {
    return axp;
}

void AxpSetup() {
    //new code
    Wire.begin(21, 22);
    int ret = axp.begin(Wire, AXP192_SLAVE_ADDRESS);

    if (ret == AXP_PASS) {
        axpEnabled = true;
        Serial.println("AXP192 Begin PASS");
        
        if (isLisa()) {
            axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
            axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
            axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
            axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
            axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
        }

        //axp.setChgLEDMode(AXP20X_LED_LOW_LEVEL);
        axp.setChgLEDMode(AXP20X_LED_OFF);

        //axp.setShutdownTime(5000);
        //axp.setTimeOutShutdown(true);

    } else {
        Serial.println("AXP192 Begin FAIL");
        axpEnabled = false;
    }
}

void SetAxpChgLed(bool isOn) {
    if (!axpEnabled) {
        return;
    }
    if (isOn) {
        //Serial.println("SetAxpChgLed ON");
        axp.setChgLEDMode(AXP20X_LED_LOW_LEVEL); 
    } else {
        //Serial.println("SetAxpChgLed OFF");
        axp.setChgLEDMode(AXP20X_LED_OFF); 
    }
}

float getBatteryVoltage() {
    if (!axpEnabled) {
        return 0;
    }
    return axp.getBattVoltage();
}

bool isPEKShortPress() {
    bool result = false;
    if (!axpEnabled) {
        return false;
    }
    if (axp.readIRQ() != AXP_PASS) {
        return false;
    } 
    result = axp.isPEKShortPressIRQ();
    if (result) {
        axp.clearIRQ();
    }
    return result;
}

bool isPEKLongPress() {
    bool result = false;
    if (!axpEnabled) {
        return false;
    }
    if (axp.readIRQ() != AXP_PASS) {
        return false;
    } 
    result = axp.isPEKLongtPressIRQ();
    /*if (result) {
        axp.clearIRQ();
    }*/
    return result;
}

void doShutdown() {
    axp.shutdown();
}
