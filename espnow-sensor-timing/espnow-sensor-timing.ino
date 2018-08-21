/*
 ESP-NOW based sensor 

 Sends readings to an ESP-Now server with a fixed mac address, timing how long wake-send-sleep takes

 Fastest is with CPU Frequency 160MHz, Flash Mode QIO and Flash Frequency 80MHz 
 (available in Tools menu with Board: Generic ESP8266 module selected)

 Anthony Elder
 License: Apache License v2
*/
extern "C" {
  #include <espnow.h>
}

// this is the MAC Address of the remote ESP server which receives these sensor readings
uint8_t remoteMac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};

#define WIFI_CHANNEL 1
//#define SLEEP_SECS 15 * 60 // 15 minutes
#define SLEEP_SECS 5  // 15 minutes
#define SEND_TIMEOUT 100  // 245 millis seconds timeout 

// keep in sync with slave struct
struct __attribute__((packed)) SENSOR_DATA {
  float temp;
} sensorData;

unsigned long bootMs, setupMs, sendMs;

RF_PRE_INIT() {
  bootMs = millis();
}

void setup() {
  setupMs = millis();
  Serial.begin(115200); Serial.println();

  if (esp_now_init() != 0) {
    Serial.println("*** ESP_Now init failed");
    gotoSleep();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(remoteMac, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);

  esp_now_register_send_cb([](uint8_t* mac, uint8_t sendStatus) {
    Serial.printf("send_cb, send done, status = %i\n", sendStatus);
    gotoSleep();
  });

  uint8_t bs[sizeof(sensorData)];
  memcpy(bs, &sensorData, sizeof(sensorData));
  esp_now_send(NULL, bs, sizeof(sensorData)); // NULL means send to all peers
  sendMs = millis();
}

void loop() {
  if (millis() > SEND_TIMEOUT) {
    gotoSleep();
  }
}

void gotoSleep() {
  // add some randomness to avoid collisions with multiple devices
  int sleepSecs = SLEEP_SECS + ((uint8_t)RANDOM_REG32/8); 
  Serial.printf("Boot: %i ms, setup: %i ms, send: %i ms, now %i ms, going to sleep for %i secs...\n", bootMs, setupMs, sendMs, millis(), sleepSecs); 
  ESP.deepSleepInstant(sleepSecs * 1000000, RF_NO_CAL);
}
