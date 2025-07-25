#include <Arduino.h>

#include <SerialTransfer.h>
#include <esp_camera.h>

// Change as necessary.
const int PIN_TX = 14;
const int PIN_RX = 16;

const int CAM_PIN_PWDN = 32;
const int CAM_PIN_RESET = -1;
const int CAM_PIN_XCLK = 0;
const int CAM_PIN_SIOD = 26;
const int CAM_PIN_SIOC = 27;
const int CAM_PIN_D7 = 35;
const int CAM_PIN_D6 = 34;
const int CAM_PIN_D5 = 39;
const int CAM_PIN_D4 = 36;
const int CAM_PIN_D3 = 21;
const int CAM_PIN_D2 = 19;
const int CAM_PIN_D1 = 18;
const int CAM_PIN_D0 = 5;
const int CAM_PIN_VSYNC = 25;
const int CAM_PIN_HREF = 23;
const int CAM_PIN_PCLK = 22;

constexpr camera_config_t cameraConfig = {
    CAM_PIN_PWDN,
    CAM_PIN_RESET,
    CAM_PIN_XCLK,
    CAM_PIN_SIOD,
    CAM_PIN_SIOC,
    CAM_PIN_D7,
    CAM_PIN_D6,
    CAM_PIN_D5,
    CAM_PIN_D4,
    CAM_PIN_D3,
    CAM_PIN_D2,
    CAM_PIN_D1,
    CAM_PIN_D0,
    CAM_PIN_VSYNC,
    CAM_PIN_HREF,
    CAM_PIN_PCLK,
    20000000,
    LEDC_TIMER_0,
    LEDC_CHANNEL_0,
    PIXFORMAT_JPEG,
    FRAMESIZE_QVGA,
    12,
    1,
    CAMERA_FB_IN_PSRAM,
    CAMERA_GRAB_WHEN_EMPTY
};

SerialTransfer serial1Transfer;

void setup() {
    // Serial for logging to PC.
    Serial.begin(115200);

    // Serial to communicate with the main ESP32 controller.
    Serial1.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX);
    serial1Transfer.begin(Serial1);

    delay(200);

    // Initialize the camera.
    if (esp_camera_init(&cameraConfig) != ESP_OK) {
        Serial.println(F("Failed to initialize camera."));
        while (1) {
            delay(1000);
        }
    }
}

void loop() {
    camera_fb_t *pic = esp_camera_fb_get();
    Serial.print(F("Took a picture! It's length was "));
    Serial.print(pic->len);
    Serial.println(F(" bytes."));

    Serial.println(F("Attempting to transmit image data over serial..."));

    // Reserving two bytes here means that the maximum file index is 2^16 or 65536.
    // This means the maximum file size is 65 KB.
    int numPackets = pic->len / (MAX_PACKET_SIZE - 2);
    if (pic->len % MAX_PACKET_SIZE) ++numPackets;

    for (int packet = 0; packet < numPackets; ++packet) {
        int dataLength = MAX_PACKET_SIZE - 2;
        int fileIndex = packet * dataLength;

        if (fileIndex + dataLength > pic->len) {
            dataLength = pic->len - fileIndex;
        }

        int sendSize = serial1Transfer.txObj(fileIndex);
        sendSize = serial1Transfer.txObj(pic->buf[fileIndex], sendSize, dataLength);
        serial1Transfer.sendData(sendSize, 1);
    }

    esp_camera_fb_return(pic);

    delay(1000);
}