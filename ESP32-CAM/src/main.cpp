#include <Arduino.h>

#include <SerialTransfer.h>
#include <esp32cam.h>

const int PIN_RX = 13;
const int PIN_TX = 14;

SerialTransfer camSerialTransfer;

const esp32cam::Resolution resolution = esp32cam::Resolution::find(320, 240);

void setup() {
    // Serial for logging to PC.
    Serial.begin(115200);

    // Serial to communicate with the main ESP32 controller.
    Serial1.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX);
    camSerialTransfer.begin(Serial1);

    esp32cam::setLogger(Serial);
    esp32cam::Config cameraConfig;
    cameraConfig.setBufferCount(3);
    cameraConfig.setJpeg(80);
    cameraConfig.setPins(esp32cam::pins::AiThinker);
    cameraConfig.setResolution(resolution);

    esp32cam::Camera.begin(cameraConfig);

    Serial.print("Hello world!");
}

void loop() {
    std::unique_ptr<esp32cam::Frame> frame = esp32cam::capture();

    size_t packetIDSize = sizeof(uint16_t);


    size_t packetCount = frame->size() / (MAX_PACKET_SIZE - packetIDSize);

    if (frame->size() % (MAX_PACKET_SIZE - packetIDSize)) ++packetCount;

    for (uint16_t packet = 0; packet < packetCount; ++packet) {
        uint8_t dataSize = MAX_PACKET_SIZE - packetIDSize;
        size_t dataIndex = packet * dataSize;
        
        if (packet == packetCount - 1) {
            dataSize = frame->size() - dataIndex;
        }

        uint8_t sendSize = camSerialTransfer.txObj(packet);
        sendSize = camSerialTransfer.txObj(frame->data()[dataIndex], sendSize, dataSize);

        camSerialTransfer.sendData(sendSize);
        delay(50);
    }

    delay(100);
}