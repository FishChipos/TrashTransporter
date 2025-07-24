#include <Arduino.h>

#include <esp_camera.h>

const int CAM_PIN_PWDN = 0;
const int CAM_PIN_RESET = 0;
const int CAM_PIN_XCLK = 0;
const int CAM_PIN_SIOD = 0;
const int CAM_PIN_SIOC = 0;
const int CAM_PIN_D7 = 0;
const int CAM_PIN_D6 = 0;
const int CAM_PIN_D5 = 0;
const int CAM_PIN_D4 = 0;
const int CAM_PIN_D3 = 0;
const int CAM_PIN_D2 = 0;
const int CAM_PIN_D1 = 0;
const int CAM_PIN_D0 = 0;
const int CAM_PIN_VSYNC = 0;
const int CAM_PIN_HREF = 0;
const int CAM_PIN_PCLK = 0;

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

void setup() {
    Serial.begin(115200);
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
    esp_camera_fb_return(pic);

    delay(1000);
}