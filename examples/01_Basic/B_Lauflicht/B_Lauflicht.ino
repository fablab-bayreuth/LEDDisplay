/*
 * Beispiel für ein Lauflicht
 * 
 * TODO: Mache doch mal die Wartezeit kürzer und schaue was passiert.
 */

#define WAITTIME 100


//Die LED Pin in von Oben nach Unten
const uint8_t LEDPins[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, A5, A4, A3, A2, A1, A0, 4 };


// setup: Wird aufgerufen, wenn das Programm startet (nach RESET)
void setup() {
  //Alle LED Pins als AUSGANG setzen
  for (uint8_t i = 0; i < 16; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }
}

// wird immer wieder aufgerufen..
void loop() {
  for (uint8_t i = 0; i < 16; i++) {
    digitalWrite(LEDPins[i], HIGH);  //den i. Pin auf Versorgungsspannung (3.3V) setzen
    delay(WAITTIME);              //Warten
    digitalWrite(LEDPins[i], LOW);   //wieder auf 0V setzen
  }
  //Hier ist Ende und die loop-Funktion wird verlassen
  //um dann gleich wieder neu aufgerufen zu werden.
}
