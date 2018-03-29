/*
   Beispiel für ein Bit-Operationen

   gleich wie 03 aber jetzt mit Bibliothek
*/

#define WAITTIME 1000

//Bibliothek einbinden und Objekt erzeugen
#include <LEDDisplay.h>
LEDArray led;

// setup: Wird aufgerufen, wenn das Programm startet (nach RESET)
void setup() {
  //Alle LED Pins als AUSGANG setzen
  led.init();
}

// wird immer wieder aufgerufen..
void loop() {
  /*
   * Hinweis: Wir schreiben die 16 Bit Ganzzahl als
   * Summe aus zwei 8 Bit Ganzzahlen.
   * Das ist notwendig, da nur 8 Bit Ganzzahlen in dieser
   * binären Form BXXXXXXXX geschrieben werden können.
   */
  
  led.set(256*B00001111 + B00000011);
  delay(WAITTIME);              //Warten

  led.set(256*B11001111 + B11000011);
  delay(WAITTIME);              //Warten

  led.set(256*B11110000 + B00011011);
  delay(WAITTIME);              //Warten

  
  //Hier ist Ende und die loop-Funktion wird verlassen
  //um dann gleich wieder neu aufgerufen zu werden.
}
