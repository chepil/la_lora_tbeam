#include "HttpServerHelper.h"
#include "Tools.h"

//#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "QueueHelper.h"

AsyncWebServer server(80);

const char* PARAM_MESSAGE = "message";
const int maxArraySize = 100;
int lastArrayIndex = 0;

void notFound(AsyncWebServerRequest *request);

//char* myStrings[maxArraySize] = {};

void HttpServerHelper_setup() {
    debugLog("HttpServerHelper setup");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });

    // Send a GET request to <IP>/get?message=<message>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
        String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();

            if (message == "getHttpLog") {
                message = getHttpLog();
            }

        } else {
            message = "[{\"msg\":\"Incorrect message param\"}]";
        };

        


        request->send(200, "application/json", "" + message);
    });

    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/post", HTTP_POST, [](AsyncWebServerRequest *request){
        String message;
        if (request->hasParam(PARAM_MESSAGE, true)) {
            message = request->getParam(PARAM_MESSAGE, true)->value();
        } else {
            message = "No message sent";
        };
        request->send(200, "text/plain", "Hello, POST: " + message);
    });

    server.onNotFound(notFound);

    server.begin();
}


void HttpServerHelper_loop() {

}

void HttpServerHelper_AddLog(String inputString) {
    /*
    unsigned char* buffer = new unsigned char[100];
    inputString.getBytes(buffer, 100, 0);
    const char *str = (const char*)buffer;

    lastArrayIndex = lastArrayIndex + 1;
    if (lastArrayIndex < 100) {
        myStrings[lastArrayIndex] = (char*)malloc(strlen(str)+1);
        strcpy(myStrings[lastArrayIndex], str);
    }*/
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
