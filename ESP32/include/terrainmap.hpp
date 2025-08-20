#ifndef TERRAINMAP_HPP
#define TERRAINMAP_HPP

#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

class TerrainMap {
    private:
        SoftwareSerial *gpsSS;
        TinyGPSPlus *gps;

        int PIN_GPS_RX;
        int PIN_GPS_TX;

        int baud;

        double currentLatitude = 0;
        double currentLongitude = 0;
    
    public:
        TerrainMap(const int gpsRxPin, const int gpsTxPin, const int gpsBaud);

        void begin();
        void update();

        double getCurrentLatitude();
        double getCurrentLongitude();
};

#endif