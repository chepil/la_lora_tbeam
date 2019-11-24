#include "WiFiHelper.h"
#include <WiFi.h>
#include "DisplayHelper.h"

void SetupWiFi() {
    WiFi.begin("lora", "o7t4r14RIUj");

    int maxwait = 5;
    while ((WiFi.status() != WL_CONNECTED) && maxwait-- > 0  ) {
        delay(1000);
        setBottomLine("WiFi init...");
        DisplayHelper_draw();
        Serial.println("WiFi init...");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected to the WiFi network");
        setBottomLine("WiFi OK");
        DisplayHelper_draw();
        setLocalIP(WiFi.localIP().toString());
        Serial.println(WiFi.gatewayIP());
    } else {
        Serial.println("Connected to the WiFi network");
        setBottomLine("WiFi not connected");
        DisplayHelper_draw();
        setLocalIP("no wifi");
    }
}