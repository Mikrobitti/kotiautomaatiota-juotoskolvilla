# Kotiautomaatiota juotoskolvilla

Oheisesta repositoriosta löytyy tarvittava tieto huhtikuun 2019 Mikrobitin 'Kotiautomaatiota kolvinvarresta' värkkäilyihin.

## Tiedostot

Rakennettavat laitteet on jaettu omiin kansioihinsa ohjelmakoodeineen ja 3d-mallineen. Lisäinfoa itse Mikrobitissä.

```
.
├── README.md
├── ilmanlaatuanturi
│   ├── 3d-mallit
│   └── main.cpp
├── mqtt-nexa-silta
│   ├── 3d-mallit
│   ├── NewRemoteSwitch.zip
│   └── main.cpp
└── testikoodit
    ├── dht22-testi.cpp
    ├── mhz-14-testi.cpp
    └── mqtt-testi.cpp
```

## Asennus ja käyttöönotto

Molemmat laitteet käyttävät ESP8266-pohjaista Wemos-mikrokontrolleria pohjanaan. Ohjelmakoodin voi ajaa Wemokseen suoraan Arduino-ympäristöstä seuraavilla ohjeilla:

1. Asenna uusin Arduino IDE esimerkiksi [täältä](https://www.arduino.cc/en/Main/Software)
2. Avaa **Preferences (Asetukset)** ja lisää osoite *http://arduino.esp8266.com/stable/package_esp8266com_index.json* kenttään *Additional Board Manager URLs*
3. Avaa **Tools > Board** -valikosta *Boards Manager* ja asenna paketti *esp8266*
4. Ota Wemos D1 Mini kohdealustaksi valikosta **Tools > Board**
5. Asenna tarvittavat kirjastot *Library Managerilla*. Sen löydät valikosta **Sketch > Include Library > Manage Libraries...**

>**Ilmanlaatuanturi** tarvitsee kirjastot *Adafruit_MQTT*, *ArduinoJson*, *DHTesp* ja *SoftwareSerial*.

>**MQTT-Nexa-silta** tarvitsee kirjastot *Adafruit_MQTT*, *ArduinoJson* ja *NewRemoteTransmitter*. NewRemoteSwitch -kirjasto pitää asentaa erikseen, sillä sitä ei löydy suoraan Library Managerista. Se kannattaakin asentaa suoraan [zip-paketista](./mqtt-nexa-silta/NewRemoteSwitch.zip). **Sketch > Include Library > Add .ZIP Library...**

> Varmasti uusimman version ko. kirjastosta löydät kirjaston repositoriosta [BitBucketissa](https://bitbucket.org/fuzzillogic/433mhzforarduino/wiki/Home). Jos haluat asentaa kirjaston täältä käsin, avaa ko. linkistä sivupalkin Downloads-valikko ja lataa koko zip-paketti koneellesi. Siirrä zipin NewRemoteSwitch-kansio Arduinon libraries-kansioon [näillä ohjeilla](https://www.arduino.cc/en/Guide/Libraries).

7. Muokkaa koodia (ainakin Wifi-tunnuksesi!) ja ohjelmoi se Wemokseen upload-nappia painamalla!

*Molempien laitteiden yksityiskohtaiset rakennusohjeet löydät Mikrobitin artikkelista.*

## Testikoodit

Kansiosta [testikoodit](./testikoodit) löyyty koodinpätkät dht22- ja mhz-14 -anturien, sekä mqtt-palvelimen testaamiseen wemoksella. Anturit ja MQTT-yhteys kannattaa testata näillä esimerkkikoodeilla erikseen ennen laitteiden rakentamista.

## Nexa-kaukosäätimen lähetinkoodin selvittäminen

Mqtt-Nexa-silta tarvitsee toimiakseen Nexa-kaukosäätimen lähetinkoodin, jonka saa nuuskittua kaukosäätimestä NewRemoteSwitch-kirjaston avulla.

Asennettuasi NewRemoteSwitch-kirjaston, avaa kirjaston ShowReceivedCode -esimerkkikoodi, ja kiinnitä 433 MHz vastaanotinmoduuli (GND, 5V ja D2) kiinni Wemokseesi.

`NewRemoteReceiver::init(D2, 2, showCode);`

Vaihda init-funktion pinni D2:een ja avaa sarjaportti (oikealla nopeudella)! Painele kaukosäätimen nappeja yksitellen. Toivottavasti sarjaportissa alkaa näkyä tämänkaltaisia viestejä nexa-kaukosäätimeltäsi:

```
Addr 12345678 unit 0 on, period: 266us.
Addr 12345678 unit 0 off, period: 266us.
Addr 12345678 unit 1 on, period: 266us.
Addr 12345678 unit 1 off, period: 265us.
Addr 12345678 unit 2 on, period: 266us.
Addr 12345678 unit 2 off, period: 266us.
Addr 12345678 group off, period: 266us.
```

Ota osoite ja periodi talteen, ja syötä ne mqtt-nexa-sillan ohjelmakoodiin riville 29:

```
#define NEXA_ADDRESS 12345678
...
NewRemoteTransmitter transmitter(NEXA_ADDRESS, TRANCIEVER_PIN, 266, 2);
```
