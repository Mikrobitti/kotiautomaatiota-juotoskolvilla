#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ArduinoJson.h>
#include "DHTesp.h"
#include <SoftwareSerial.h>
#include <Ticker.h>

// Syötä tähän wifi-verkkosi ssid ja salasana
#define WLAN_SSID "ssid"
#define WLAN_PASS "password"

// Vaihda tähän MQTT-brokerin osoite. Voit käyttää joko
// palvelimen mdns-nimeä tai lähiverkon ip-osoitetta.
#define MQTT_BROKER_IP "raspberry.local"

// Vaihda tähän haluamasi aihe, jolle anturi julkaisee viestejä
#define MQTT_TOPIC "rooms/bedroom/sensors/1"

// Määritä tässä Wemoksen syväunen kesto mikrosekunneissa
#define SLEEP_DURATION 60 * 1000000 // 60 s

// Pinnimäärittelyt, DHT22 löytyy pinnistä D4, ja hiilidioksidianturi D1/D2
#define DHTTYPE DHT22
#define DHT22_PIN D4
#define MHZ14_R D2
#define MHZ14_T D1

// Alustuksia
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_BROKER_IP, 1883);
Adafruit_MQTT_Publish publisher = Adafruit_MQTT_Publish(&mqtt, MQTT_TOPIC);
DHTesp dht;
SoftwareSerial SerialCO2(MHZ14_T, MHZ14_R);

int co2;
float rh;
float temp;

// Jotkin kääntäjät vaativat funktiomäärittelyt (forward declarations) etukäteen
void connectToMQTTBroker();
void publishMessage();
void readMHZ14();
void readDHT22();


void setup() {
    Serial.begin(9600);

    WiFi.begin(WLAN_SSID, WLAN_PASS);
    dht.setup(DHT22_PIN, DHTesp::DHT22);

    Serial.print("\nConnecting to WIFI ");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWIFI Connected");

    connectToMQTTBroker();

    readDHT22();
    readMHZ14();

    publishMessage();

    mqtt.disconnect();

    Serial.println("Entering deepsleep");
    ESP.deepSleep(SLEEP_DURATION);
}

void loop() {
    // loop() jätetään tyhjäksi, koska setup()-funktion viimeisen
    // rivin ESP.deepSleep() sammuttaa wemoksen syväunen ajaksi.
}

void publishMessage() {
    Serial.println("Publishing message");

    // Luodaan staattinen JSON-puskuri, johon mittaukset tallennetaan
    StaticJsonBuffer<128> jsonBuffer;
    char strObj[128];

    JsonObject& jsonObject = jsonBuffer.createObject();

    // Tallennetaan mittaukset omiin propertyihin
    jsonObject["co2"] = co2;
    jsonObject["humidity"] = rh;
    jsonObject["temperature"] = temp;

    // Muunnetaan json-objekti merkkijonoksi ja julkaistaan annetulla mqtt-aiheella.
    jsonObject.printTo(strObj, sizeof(strObj));
    publisher.publish(strObj);
    delay(1);
}

// Funktio lukee ilmankosteuden ja lämpötilan DHT22-anturilta
void readDHT22() {
    Serial.println("Reading DHT22");

    rh = dht.getHumidity();
    temp = dht.getTemperature();
}

// Funktio lukee hiilidioksidipitoisuuden MHZ-14 -anturilta
void readMHZ14() {
    Serial.println("Reading MHZ-14");

    const uint8_t readCmd[9] = {0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79};
    uint8_t response[9];

    SerialCO2.write(readCmd, 9);
    delay(2); // odota hieman, jotta anturi ehtii mitata hiilidioksidin

    if (SerialCO2.available()) {
        SerialCO2.readBytes(response, 9);
    }

    // Mittaus luetaan kahtena tavuna, jotka pitää muuntaa yhdeksi
    // kokonaisluvuksi.
    int responseHigh = (int) response[2];
    int responseLow = (int) response[3];

    co2 = (responseHigh << 8) + responseLow;
}

// Yhdistä mqtt-brokeriin
void connectToMQTTBroker() {
    Serial.print("Connecting to MQTT broker ");
    while (mqtt.connect() != 0) { // odota 5s ja yritä tarvittaessa uudelleen
             mqtt.disconnect();
             Serial.print(".");
             delay(5000);
    }
    Serial.println("\nMQTT Connected.");
}