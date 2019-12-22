/*
   Beispiel für ein Bit-Operationen

   TODO: Ändere die Bit-Muster. Kannst du ein Doppellauflicht machen?
*/

#define WAITTIME 1000

/*
 * Ein 16bit Integer sieht binär wie folft aus:
 * 
 * 00010010 01100100
 * 
 * Damit lassen sich hervorradend die Leuchtzustände der 16 LEDs festlegen 
 * 
 * Man definiert:
 * 1. Bit (ganz rechts)   == Oberste LED
 * 2. Bit (2. von rechts) == 2. LED von Oben
 * ...
 */

//Die LED Pin in von Oben nach Unten
const uint8_t LEDPins[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, A5, A4, A3, A2, A1, A0, 4 };

void schalteLEDs(uint16_t wert) {
  uint16_t maske = 1; // 00000000 00000001
  for (uint8_t i = 0; i < 16; i++) {
    /*
       Um zu entscheiden, ob eine bestimmte LED an oder aus ist
       nutzen wir das binäre "&"
       Dazu werden die Bits übereinander gelegt und nur da, wo
       bei beiden Variablen eine "1" steht ist das Ergebnis 1
       sonst 0.

       z.B. wert:  00001001 00000011
          & maske: 00000000 00000001
       -----------------------------
           an_aus: 00000000 00000001
    */
    uint16_t an_aus = wert & maske;
    digitalWrite(LEDPins[i], an_aus); 

    /*
       Jetzt nutzen wir den SHIFT-Operator (>>) um den wert nach
       rechts zu "shiften"

       Beispiel:
       z.B. wert:  00001001 00000011
               >> 2
       ------------------------
       ergebnis:   00000010 01000000
       Wir schieben natürlich nur um 1

    */
    wert=wert>>1; //wird oft auch wert>>=1; geschrieben

  }

}


// setup: Wird aufgerufen, wenn das Programm startet (nach RESET)
void setup() {
  //Alle LED Pins als AUSGANG setzen
  for (uint8_t i = 0; i < 16; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }
}

// wird immer wieder aufgerufen..
void loop() {
  /*
   * Hinweis: Wir schreiben die 16 Bit Ganzzahl als
   * Summe aus zwei 8 Bit Ganzzahlen.
   * Das ist notwendig, da nur 8 Bit Ganzzahlen in dieser
   * binären Form BXXXXXXXX geschrieben werden können.
   */
  
  schalteLEDs(256*B11111111 + B11111111);
  delay(WAITTIME);              //Warten

  schalteLEDs(256*B11001111 + B11000011);
  delay(WAITTIME);              //Warten

  schalteLEDs(256*B11110000 + B00011011);
  delay(WAITTIME);              //Warten
  
  //Hier ist Ende und die loop-Funktion wird verlassen
  //um dann gleich wieder neu aufgerufen zu werden.
}
