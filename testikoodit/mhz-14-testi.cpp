/**
* Koodi MZH-14-anturin testaamiseen.
*
* Oheisella testikoodilla voit testata, oletko kytkenyt
* MHZ-14 -hiilidioksidianturin oikein wemokseesi,
* ja palauttaako anturi mitään järkevää mittaustulosta.
*/

#include <SoftwareSerial.h>

// Kytkennät MHZ-14 -anturin pinneihin
#define CO2_TX 3
#define CO2_RX 2

SoftwareSerial SerialCO2(CO2_RX, CO2_TX); // RX, TX

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

    int co2 = (responseHigh << 8) + responseLow;
    return co2;
}

void setup() {
    Serial.begin(9600);
    SerialCO2.begin(9600);
    Serial.println("Alustettu.");
}

void loop() {
    // Tulosta CO2 sarjaporttiin ppm:nä (parts per million).
    // Sisäilman hiilidioksidipitoisuus on tyypillisesti (karkeasti) 400 - 1000.
    Serial.println(readMHZ14());
    delay(200);
}