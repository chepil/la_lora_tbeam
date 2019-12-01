#include <Arduino.h>
#include "SyslogHelper.h"
#include "Tools.h"
//#include <Syslog.h>
//#include <WiFiUdp.h>

// A UDP instance to let us send and receive packets over UDP
//WiFiUDP udpClient;

// Create a new syslog instance with LOG_KERN facility
//Syslog Extsyslog(udpClient, SYSLOG_PROTO_BSD);

void SyslogHelper_setup() {
    //Extsyslog.server("192.168.220.2", 10514);  
    //Extsyslog.defaultPriority(LOG_KERN);

}

void syslog(String message) {
   
    debugLog(message);
    //Serial.println(message);

    //FIXME: не делать в главном потоке! тормозит работу приложения!!!

    //Extsyslog.log(LOG_INFO, message);

    ////if (init_ok) {
    //Extsyslog.log(LOG_INFO, message);
    ////}
}