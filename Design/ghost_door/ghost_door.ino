/*** Import Libraries  ***/
#include "DHTesp.h"             // DHT sensor library for ESPx by beegee_tokyo
#include "EspMQTTClient.h"      // EspMQTTClient by Patrck Lapointe
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>   // Adafruit SSD1306 by Adafruit
#include <ArduinoJson.h>        // ArduinoJson by Benoit Blanchon
#include <WiFiManager.h>        // WiFiManager by tzapu

#define RELAY2_PIN D0
#define RELAY_ON LOW            // active low
#define RELAY_OFF HIGH

/*** Subscribe topic ***/
const char *sub_topic = "ghost/door";

char WifiSSID[64] = "GHOST-2G";
char WifiPassword[64] = "ghostlinux";

const char *mqtt_broker = "sweetdream.iptime.org";
char mqtt_username[40] = "iot";
char mqtt_password[40] = "csee1414";
char mqtt_clientname[40] = "ghost door";
int mqtt_port = 1883;

// WiFiManager wifiManager;

// EspMQTTClient mqtt_client(
//   mqtt_broker,
//   mqtt_port,  // It is mandatory here to allow these constructors to be distinct from those with the Wifi handling parameters
//   mqtt_username,    // Omit this parameter to disable MQTT authentification
//   mqtt_password,    // Omit this parameter to disable MQTT authentification
//   mqtt_clientname
// );

EspMQTTClient mqtt_client(
  WifiSSID, 
  WifiPassword,
  mqtt_broker, 
  mqtt_username, 
  mqtt_password,
  mqtt_clientname,
  mqtt_port
);

void setup() {
  Serial.begin(115200);  

  pinMode(RELAY2_PIN, OUTPUT);
  mqtt_client.enableDebuggingMessages(true);

  digitalWrite(RELAY2_PIN, RELAY_OFF);
}

void loop() {
  mqtt_client.loop();
  // wifiManager.startConfigPortal();

}


void onConnectionEstablished() {

  // mqtt_client.publish(sub_topic, "sub_topic: Connection Established!");

  mqtt_client.subscribe(
    sub_topic, 
    [](const String& topic, const String & payload)
    {
      Serial.print(">> "); Serial.print(topic); Serial.print(" "); Serial.println(payload);
      
      if (payload == "toggle") {
        digitalWrite(RELAY2_PIN, RELAY_ON);
        delay(300);
        digitalWrite(RELAY2_PIN, RELAY_OFF);
      }

    }
  );
}