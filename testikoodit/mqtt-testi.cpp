/**
* Koodi MQTT-toiminnallisuuden testaamiseen.
*
* Oheinen testikoodi vastaa toiminnaltaan ilmanlaatuanturin koodia,
* mutta anturimittaukset on tästä koodista poistettua. Voit siis
* huoletta testata tällä pelkkää MQTT-julkaisijan toimintaa
* Node-REDiä vasten.
*/

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ArduinoJson.h>

#define WLAN_SSID "ssid"
#define WLAN_PASS "password"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, "192.168.1.240", 1883);
Adafruit_MQTT_Publish publisher = Adafruit_MQTT_Publish(&mqtt, "rooms/kitchen/sensor1");

void connectToMQTTBroker();
void measureAndPublish();

void setup() {
    Serial.begin(9600);

    WiFi.begin(WLAN_SSID, WLAN_PASS);
    Serial.println("Yhdistetään WIFI");
    while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
    }
}

void loop() {
    connectToMQTTBroker();
    measureAndPublish();

    delay(5000);
}

void measureAndPublish() {
    StaticJsonBuffer<128> jsonBuffer;
    char strObj[128];

    JsonObject& jsonObject = jsonBuffer.createObject();

    jsonObject["co2"] = 568;
    jsonObject["humidity"] = 42;
    jsonObject["temperature"] = 27;
    jsonObject.printTo(strObj, sizeof(strObj));

    publisher.publish(strObj);
}

void connectToMQTTBroker() {
    if (mqtt.connected()) return;
    while (mqtt.connect() != 0) {
            mqtt.disconnect();
            delay(5000);
    }
    Serial.println("MQTT yhdistetty!");
}