#include <Arduino.h>

#include "arduino_secrets.h"

// ArduinoJson - Version: Latest 
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

  #include <ArduinoBearSSL.h>
  #include <ArduinoECCX08.h>
  #include <utility/ECCX08SelfSignedCert.h>
  #include <ArduinoMqttClient.h>
  #include <WiFiNINA.h>

  const char broker[]      = SECRET_BROKER;
  String     deviceId  =  SECRET_DEVICE_ID;  // Device ID if ECCX08 certificate

  #define ECCX08_CERTIFICATE

  WiFiClient    wifiClient;            // Used for the TCP socket connection
  BearSSLClient sslClient(wifiClient); // Used for SSL/TLS connection, integrates with ECC508
  MqttClient    mqttClient(sslClient);

  const char ssid[]        = SECRET_WIFI_SSID;
  const char pass[]        = SECRET_WIFI_PASS;
  
//=========================================================================================

  unsigned long getTime() {
    // get the current time from the WiFi module
    return WiFi.getTime();
  }

  void connectWiFi() {
    Serial.print("Attempting to connect to SSID: ");
    Serial.print(ssid);
    Serial.print(" ");

    while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
      // failed, retry
      Serial.print(".");
      delay(5000);
    }
    Serial.println();

    Serial.println("You're connected to the network");
    Serial.println();
  }

/*
 * Establishses connection with the MQTT Broker (IoT Hub)
 * Some errors you may receive:
 * -- (-.2) Either a connectivity error or an error in the url of the broker
 * -- (-.5) Check credentials - has the SAS Token expired? Do you have the right connection string copied into arduino_secrets?
 */
  void connectMQTT() {
    Serial.print("Attempting to MQTT broker: ");
    Serial.print(broker);
    Serial.println(" ");

    while (!mqttClient.connect(broker, 8883)) {
      // failed, retry
      Serial.print(".");
      Serial.println(mqttClient.connectError());
      delay(5000);
    }
    Serial.println();

    Serial.println("You're connected to the MQTT broker");
    Serial.println();
    
    // subscribe to a topic
    mqttClient.subscribe("devices/" + deviceId + "/messages/devicebound/#");
  }

  
void publishMessage() {
  Serial.println("Publishing message");
  
  const int capacity = JSON_ARRAY_SIZE(10) + 10*JSON_OBJECT_SIZE(2)+ JSON_OBJECT_SIZE(3) + 280;     // Calculation of the JSON doc size, as explained in the documentation

/*  
Here is where you should write the body of your message. In this example, the JSON doc is purposely longer than 256 characters, to highlight the issue.
*/

  StaticJsonDocument<capacity> doc;
  doc["topic"] = "messageTopic";  
  doc["deviceId"] = deviceId;
  JsonArray data = doc.createNestedArray("data");
  JsonObject data_0 = data.createNestedObject();
  data_0["label"] = "Ankara";
  data_0["state"] = true;
  JsonObject data_1 = data.createNestedObject();
  data_1["label"] = "Beirut";
  data_1["state"] = true;
  JsonObject data_2 = data.createNestedObject();
  data_2["label"] = "Cincinnati";
  data_2["state"] = false;
  JsonObject data_3 = data.createNestedObject();
  data_3["label"] = "Detroit";
  data_3["state"] = false;
  JsonObject data_4 = data.createNestedObject();
  data_4["label"] = "Eindhoven";
  data_4["state"] = true;
  JsonObject data_5 = data.createNestedObject();
  data_5["label"] = "Fresno";
  data_5["state"] = false;
  JsonObject data_6 = data.createNestedObject();
  data_6["label"] = "Genoa";
  data_6["state"] = false;
  JsonObject data_7 = data.createNestedObject();
  data_7["label"] = "Huddersfield";
  data_7["state"] = true;
  JsonObject data_8 = data.createNestedObject();
  data_8["label"] = "Istanbul";
  data_8["state"] = true;
  JsonObject data_9 = data.createNestedObject();
  data_9["label"] = "Jakarta";
  data_9["state"] = false;
  

//   DEBUG - serialize the document in the serial monitor
//   serializeJson(doc, Serial);
//   Serial.println(" ");
  
  char payload[1024]; // length of the char buffer that contains the JSON file, concretely the number of characters included in one message
  size_t payloadSize = serializeJson(doc, payload);

//   DEBUG - write the size of the serialized document
//   Serial.print("json size:");
//   Serial.println(payloadSize);
  
// send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage("devices/" + deviceId + "/messages/events/", static_cast<unsigned long>(payloadSize));
  mqttClient.print(payload);
  mqttClient.endMessage();
/*  
To replicate the issue, uncomment the following 3 lines and comment the 3 above. This way you'll only be able to send MQTT messages smaller than 256 Bytes.
*/
//  mqttClient.beginMessage("devices/" + deviceId + "/messages/events/");
//  serializeJson(doc, mqttClient);
//  mqttClient.endMessage();
}

//=========================================================================================


void setup() {

      Serial.begin(9600);

      if (!ECCX08.begin()) {
        Serial.println("No ECCX08 present!"); // If no ECCX08 certificate is present, generate one using the "ECCX08SelfSignedCert.ino" sketch from the library examples
        while (1);
      }

      // reconstruct the self signed cert
      ECCX08SelfSignedCert.beginReconstruction(0, 8);
      ECCX08SelfSignedCert.setCommonName(ECCX08.serialNumber());
      ECCX08SelfSignedCert.endReconstruction();

      // Set the ECCX08 slot to use for the private key
     // and the accompanying public certificate for it
      sslClient.setEccSlot(0, ECCX08SelfSignedCert.bytes(), ECCX08SelfSignedCert.length());

      // Set a callback to get the current time
      // used to validate the servers certificate
      
      ArduinoBearSSL.onGetTime(getTime);

      // Set the username to "<broker>/<device id>/?api-version=2018-06-30"
      String username;

      // Set the client id used for MQTT as the device id
      mqttClient.setId(deviceId);

      username += broker;
      username += "/";
      username += deviceId;
      username += "/api-version=2018-06-30";
      mqttClient.setUsernamePassword(username, "");

}

//=========================================================================================

void loop() {
  
    if(WiFi.status() != WL_CONNECTED) {
      connectWiFi();
    }
  
  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

    publishMessage();

  delay(10000); //	publish a message roughly every 10 seconds
}
