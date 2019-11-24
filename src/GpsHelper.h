#ifndef GPSHELPER_H
#define GPSHELPER_H

struct Coordinates {
    bool isValid;
    float lat;
    float lng;
    char date[9];
    char time[7];
};

void GpsHelper_setup(void);
void GpsHelper_loop(void);
//void smartDelay(unsigned long ms);

Coordinates getCoordinates(void);

#endif /* GPSHELPER_H */
