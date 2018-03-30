/*
   Beispiel für Lauflicht

   TODO: Lasse die zwei LEDs doch hin und her laufen
*/

#define WAITTIME 1000

//Bibliothek einbinden und Objekt erzeugen
#include <LEDDisplay.h>
LEDArray led;

void setup() {
  led.init();
}

void loop() {
  uint16_t wert=0x3; //das ist das gleiche wie 256*B00000000 + B00000011

  for(uint8_t i=0;i<15;i++){
    led.set(wert);
    wert<<=1;
    delay(WAITTIME);
  }
}
