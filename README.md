# Kotiautomaatiota kolvinvarresta

Oheisesta repositoriosta löytyy tarvittava tieto Mikrobitin numeron (x/19) artikkelin 'Kotiautomaatiota kolvinvarresta' värkkäilyihin.

## Tiedostot

Rakennettavat laitteet on jaettu omiin kansioihinsa ohjelmakoodeineen ja 3d-mallineen. Lisäinfoa itse Mikrobitissä.

```
.
├── README.md
├── ilmanlaatuanturi
│   ├── 3d-mallit
│   └── main.cpp
└── mqtt-nexa-silta
    ├── 3d-mallit
    ├── NewRemoteSwitch.zip
    └── main.cpp
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