# Trash Transporter

## Hardware

1. ESP32
2. ESP32-CAM
3. NEO-6M GPS module

## ESP32 and ESP32-CAM Firmware

### Dependencies

This project requires VSCode with the PlatformIO extension installed.

If the libraries were not installed automatically after setup, the following libraries can be installed and added manually.

For ESP32:
1. ESP32Servo
2. SerialTransfer
3. EspSoftwareSerial
4. TinyGPSPlus-ESP32

For ESP32-CAM:
1. SerialTransfer

### Building

Open the `ESP32` folder with the PlatformIO extension and wait for it to finish configuring the project. Do the same for the `ESP32-CAM` folder.

Use the `Build` action to build the project and the `Upload` action to upload the firmware to a connected board.

## Front-end

### Dependencies

The front-end requires node.js and npm.

### Building

To run the front-end, use `npm run dev` and follow the displayed link.