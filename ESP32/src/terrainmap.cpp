#include "terrainmap.hpp"

TerrainMap::TerrainMap(const int gpsRxPin, const int gpsTxPin, const int gpsBaud) {
    PIN_GPS_RX = gpsRxPin;
    PIN_GPS_TX = gpsTxPin;

    baud = gpsBaud;
}

void TerrainMap::begin() {
    gpsSS = new SoftwareSerial(PIN_GPS_RX, PIN_GPS_TX);
    gpsSS->begin(baud);

    gps = new TinyGPSPlus();
}

void TerrainMap::update() {
    while (gpsSS->available() > 0) {
        char gpsChar = gpsSS->read();
        Serial.print(gpsChar);
        gps->encode(gpsChar);

        if (gps->location.isValid()) {
            currentLatitude = gps->location.lat();
            currentLongitude = gps->location.lng();

            Serial.println(currentLatitude, 6);
            Serial.println(currentLongitude, 6);
        }
    }
}

double TerrainMap::getCurrentLatitude() {
    return currentLatitude;
}

double TerrainMap::getCurrentLongitude() {
    return currentLongitude;
}