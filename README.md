# Send Large JSON Data to IoT Hub Using MQTT

This project is to demonstrate how to send JSON documents larger than 256 Bytes from an Arduino Nano 33 IoT board to Azure IoT Hub.

## Prerequisites

To compile the code, you need the following libraries:

- [ArduinoJson](https://arduinojson.org) - To encapsulate my data in a JSON message, one of the formats accepted by IoT Hub;

- [WiFiNINA](https://github.com/arduino-libraries/WiFiNINA) - To connect the board to the WiFi;

- [ArduinoMqttClient](https://github.com/arduino-libraries/ArduinoMqttClient) - Client that allows to send and receive MQTT messages;

- [ArduinoBearSSL](https://github.com/arduino-libraries/ArduinoBearSSL) - Port of [BearSSL](https://bearssl.org/) to Arduino, to implement the SSL/TLS protocol;

- [ArduinoECCX08](https://github.com/arduino-libraries/ArduinoECCX08) - Library for the Atmel/Microchip ECC508 and ECC608 crypto chips, used for authentication with IoT Hub with a SelfSigned X.509 certificate;

All libraries must be installed on your system before compiling the code (instructions on how to install Arduino libraries can be found [here](https://www.arduino.cc/en/guide/libraries)).

## Configuration

```c++
#define SECRET_WIFI_SSID   " " 
#define SECRET_WIFI_PASS   " "
#define SECRET_BROKER " "
#define SECRET_DEVICE_ID " "

```
