#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ArduinoJSON.h>
#include <NewRemoteTransmitter.h>

// Syötä tähän wifi-verkkosi ssid ja salasana
#define WLAN_SSID "ssid"
#define WLAN_PASS "password"

// Vaihda tähän MQTT-brokerin osoite. Voit käyttää joko
// palvelimen mdns-nimeä tai lähiverkon ip-osoitetta.
#define MQTT_BROKER_IP "raspberry.local"

// Vaihda tähän haluamasi aihe, jolle anturi julkaisee viestejä
#define MQTT_TOPIC "rooms/bedroom/nexa"

// Nexa-lähetin on kiinni pinnissä D3
#define TRANCIEVER_PIN D3

// Nexa-kaukosäätimen ohjauskoodi
#define NEXA_ADDRESS 12345678

// Alustukset
WiFiClient wifiClient;
Adafruit_MQTT_Client mqtt(&wifiClient, MQTT_BROKER_IP, 1883);
Adafruit_MQTT_Subscribe nexaTopic = Adafruit_MQTT_Subscribe(&mqtt, MQTT_TOPIC);

NewRemoteTransmitter transmitter(NEXA_ADDRESS, TRANCIEVER_PIN, 260, 2);

// Jotkin kääntäjät vaativat funktiomäärittelyt (forward declarations) etukäteen
void setupWifi();
void connectToMQTTBroker();
void onMessage(char *data, uint16_t len);

void setup() {
    // Alustetaan sarjaporttiyhteys debuggausta varten
    // ja yhdistetään wemos kodin wifi-verkkoon
    Serial.begin(9600);
    setupWifi();

    // Annetaan kuunneltavalla mqtt-aiheelle
    // callback-funktio, jota kutsutaan kun
    // kuunneltava viesti vastaanotetaan
    nexaTopic.setCallback(onMessage);
    mqtt.subscribe(&nexaTopic);
}

void loop() {
    // Yhdistetään MQTT-brokeriin
    // processPackets() -funktio kuuntelee vastaantulevia viestejä,
    // ja tarvittaessa kutsuu aiheisiin liitettyjä callback-funktioita
    connectToMQTTBroker();
    mqtt.processPackets(10000);
}

// Alustaa wifi-yhteyden ja tarvittaessa yrittää yhdistää verkkoon
// uudelleen
void setupWifi() {
    WiFi.begin(WLAN_SSID, WLAN_PASS);

    Serial.print("\nConnecting to WIFI ");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWIFI Connected");
}

// Callback-funktio, jota kutsutaan aina,
// kun Wemos vastaanottaa viestin kuunnellulla aiheella
void onMessage(char *data, uint16_t len) {
    // Luodaan sopivankokoinen puskuri, johon vastaanotettu
    // mqtt-viesti puretaan.

    // Ohjeita staattisen JSON-puskurin mitoittamiseen
    // löytyy osoitteesta https://arduinojson.org/v5/assistant/
    const size_t bufferSize = JSON_OBJECT_SIZE(2) + 20;
    DynamicJsonBuffer jsonBuffer(bufferSize);
    JsonObject& root = jsonBuffer.parseObject(data);

    // Luetaan MQTT-viestistä kentät 'unit', joka vastaa
    // Nexa-laitetta 0..2. Suuremmilla arvoilla lähetetään
    // viesti koko ryhmälle.
    // Kenttä 'value' on boolean-arvo true tai false,
    // joka määrittää, kytketäänkö laite päälle vai pois päältä.
    int unit = root["unit"];
    bool value = root["value"];

    if (unit > -1 && unit < 3) {
        transmitter.sendUnit(unit, value);
    }
    else if (unit > 2) {
        transmitter.sendGroup(value);
    }
}

// Yhdistä mqtt-brokeriin
void connectToMQTTBroker() {
    if (mqtt.connected()) {
        return;
    }

    Serial.print("Connecting to MQTT broker ");
    while (mqtt.connect() != 0) { // if unable to connect, retry in 5s
             mqtt.disconnect();
             Serial.print(".");
             delay(5000);
    }
    Serial.println("\nMQTT Connected.");
}
