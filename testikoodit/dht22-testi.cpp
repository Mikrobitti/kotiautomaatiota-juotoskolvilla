/**
* Koodi DHT22-anturin testaamiseen.
*
* Oheisella testikoodilla voit erikseen testata,
* onko DHT22-anturi ehjä ja kytketty Wemokseen oikein.
* Koodi tulostaa anturilta luetun ilmankosteuden ja
* lämpötilan sarjaporttiin.
*/


#include "DHTesp.h"

DHTesp dht;

void setup() {
    Serial.begin(9600);
    dht.setup(D4, DHTesp::DHT22);
}

void loop() {
    delay(dht.getMinimumSamplingPeriod());

    float humidity = dht.getHumidity();
    float temperature = dht.getTemperature();

    Serial.print(humidity);
    Serial.print(" RH% - ");
    Serial.print(temperature);
    Serial.println(" °C");
}
