#include "BluetoothHelper.h"
//#include "BluetoothSerial.h"
#include "Tools.h"

//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif

//BluetoothSerial SerialBT;

void BluetoothHelper_setup() {
    String serialNumber = getSerialNumber();
    String name = "Zarya_"+serialNumber;

    //SerialBT.begin(name); //Bluetooth device name
    debugLog("The device started, now you can pair it with bluetooth!");
    debugLog("Name of bluetooth: "+name);
}

void BluetoothHelper_loop() {

}

void BluetoothHelper_SerialWrite(String inputString) {

    return;
    //FIXME: временно отключено, crash в режиме лисы

    //Serial.println("bluetooth inputString: "+inputString);
    //SerialBT.write(0xAA);
    
    /*
    if (SerialBT.connected(5000)) {
        String writeString = ">"+inputString+"<\n";
        int len = writeString.length();
        //Serial.println("writeString.length is "+String(len));

        byte plain[len];
        writeString.getBytes(plain, len);
        SerialBT.write(plain,sizeof(plain));
    } else {
        debugLog("bluetooth not connected");
    }*/
}
