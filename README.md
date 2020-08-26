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

### X.509 Self-Signed Certificate

Before uploading the code of this repo to your board, you need to generate a self-signed X.509 certificate to authenticate to the hub. To do that, compile and upload the example from [ArduinoECCX08](https://github.com/arduino-libraries/ArduinoECCX08) named ``ECCX08SelfSignedCert`` (you can find it in *File -> Examples -> ArduinoECCX08 -> Tools -> ECCX08SelfSignedCert*. When the upload is complete, open the serial monitor and follow the instructions to create a certificate. At the end of the procedure, the board should print a SHA1 fingerprint that you have to copy and store for a later step. More information on this process can be found [here](https://www.hackster.io/Arduino_Genuino/securely-connecting-an-arduino-nb-1500-to-azure-iot-hub-af6470).

## Azure IoT Hub Deployment

If you don't know how to create an Iot Hub and register a new device, use the following resources:
1. [Create an IoT hub](https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-create-through-portal#create-an-iot-hub)
2. [Register a new device](https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-create-through-portal#register-a-new-device-in-the-iot-hub)

### Important

When registering a new device, don't forget to select ``X.509 Self-Signed`` in the ``Authentication type`` field, and paste the SHA1 key that you copied before in both the ``Primary Thumbprint`` and ``Secondary Thumbprint`` fields.

<img align="center" alt="Register a device" src="https://github.com/s-gregorini003/azure-iot-arduino-nano-33-iot/blob/master/img/register-device-iothub.png"/>  

## Configuration

To connect the device to IoT Hub you need to copy some values in the ``arduino_secrets.h`` tab. Specifically, fill in ``SECRET_WIFI_SSID`` and ``SECRET_WIFI_PASS`` with your WiFi SSID and password respectively. From the IoT Hub "Overview" page, copy the hub hostname and paste it in ``SECRET_BROKER``. Then, from the "IoT devices" page, copy your device ID in ``SECRET_DEVICE_ID``.

```c++
#define SECRET_WIFI_SSID   " " 
#define SECRET_WIFI_PASS   " "
#define SECRET_BROKER " "
#define SECRET_DEVICE_ID " "

```
