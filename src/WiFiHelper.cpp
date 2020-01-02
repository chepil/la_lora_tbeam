#include "WiFiHelper.h"
#include <WiFi.h>
#include "DisplayHelper.h"
#include "Tools.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Time.h>

// A UDP instance to let us send and receive packets over UDP
WiFiUDP ntpUDP;
// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);
// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);


void updateTime();
bool needUpdate = true;

void SetupWiFi() {
    //WiFi.mode(WIFI_STA);
    WiFi.begin("lora", "o7t4r14RIUj");

    int maxwait = 5;
    while ((WiFi.status() != WL_CONNECTED) && maxwait-- > 0  ) {
        delay(1000);
        setBottomLine("WiFi init...");
        DisplayHelper_draw();
        debugLog("WiFi init...");
    }
    if (WiFi.status() == WL_CONNECTED) {
        debugLog("Connected to the WiFi network");
        setBottomLine("WiFi OK");
        DisplayHelper_draw();

        IPAddress local = WiFi.localIP();

        String localipaddress = local.toString();
        debugLog(localipaddress);

        String gateway = WiFi.gatewayIP().toString();
        debugLog(gateway);
        
        
        String lastOctet = String(local[3]);
        setLocalIP(lastOctet);

        timeClient.begin();

    } else {
        debugLog("Connected to the WiFi network");
        setBottomLine("WiFi not connected");
        DisplayHelper_draw();
        setLocalIP("no wifi");
    }
}

void NtpClientThread_loop() {
    //debugLog("NtpClientThread_loop");
    if (needUpdate) {
        updateTime();
    }
}

void updateTime() {
    //debugLog("updateTime");
    //FIXME: for update mus be inside LOOP
    timeClient.update();

    /*
    getEpochTime returns the unix epoch, 
    which are the seconds elapsed since 00:00:00 UTC on 1 January 1970 
    (leap seconds are ignored, every day is treated as having 86400 seconds). 
    Attention: If you have set a time offset this time offset will be added to your epoch timestamp.
    */

   time_t t = now();
   if (t > 1574612710) {
       needUpdate = false;
   }

    unsigned long rawTime = timeClient.getEpochTime();
    if (rawTime > 1574612710) {
        debugLog("set time with rawTime: "+String(rawTime));
        setTime(rawTime);
        DisplayHelper_draw();
        needUpdate = false;
    } //else {
        //debugLog("rawTime: "+String(rawTime));
    //}
}