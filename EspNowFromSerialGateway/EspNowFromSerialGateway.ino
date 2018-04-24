/**
 * ESP-NOW from serial Gateway Example 
 * 
 * This shows how to use an ESP8266/Arduino as an ESP-Now Gateway by having one
 * ESP8266 receive ESP-Now messages and write them to Serial and have another
 * ESP8266 receive those messages over Serial and send them over WiFi. This is to
 * overcome the problem of ESP-Now not working at the same time as WiFi.
 * 
 * Author: Anthony Elder
 * License: Apache License v2
 */
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

#define BAUD_RATE 115200

SoftwareSerial swSer(14, 12, false, 1024);

//-------- Customise these values -----------
const char* ssid = "<yourSSID>";
const char* password = "<yourWifiPassword>";
//-------- Customise the above values --------

WiFiClient wifiClient;

String deviceMac;

// keep in sync with ESP_NOW sensor struct
struct __attribute__((packed)) SENSOR_DATA {
    float temp;
    float humidity;
    float pressure;
} sensorData;

volatile boolean haveReading = false;

void setup() {
  Serial.begin(115200); Serial.println();

  swSer.begin(BAUD_RATE);

  wifiConnect();
}

int heartBeat;

void loop() {
  if (millis()-heartBeat > 30000) {
    Serial.println("Waiting for ESP-NOW messages...");
    heartBeat = millis();
  }

  while (swSer.available()) {
    if ( swSer.read() == '$' ) {
      while ( ! swSer.available() ) { delay(1); }
      if ( swSer.read() == '$' ) {
        readSerial();
      }
    }
  }
}

void readSerial() {

    deviceMac = "";

    while (swSer.available() < 6) { delay(1); }
    deviceMac += String(swSer.read(), HEX);
    deviceMac += String(swSer.read(), HEX);
    deviceMac += String(swSer.read(), HEX);
    deviceMac += String(swSer.read(), HEX);
    deviceMac += String(swSer.read(), HEX);
    deviceMac += String(swSer.read(), HEX);

    while (swSer.available() < 1) { delay(1); }
    byte len =  swSer.read();

    while (swSer.available() < len) { delay(1); }
    swSer.readBytes((char*)&sensorData, len);

    sendSensorData();
}

void sendSensorData() {
   // do something with the received sensorData  
}

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(250);
     Serial.print(".");
  }  
  Serial.print("\nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

