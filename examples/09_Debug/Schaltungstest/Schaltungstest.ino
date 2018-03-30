/*
 * Beispiel für ein Lauflicht
 * 
 * TODO: Mache doch mal die Wartezeit kürzer und schaue was passiert.
 */

#define WAITTIME 100

#include <math.h>
float ntc10_R2T(float r) {
  float log_r = log(r);
  return 440.61073 - 75.69303 * log_r +
         4.20199 * log_r * log_r - 0.09586 * log_r * log_r * log_r;
}

//Die LED Pin in von Oben nach Unten
const uint8_t LEDPins[] = {5, 6, 7, 8, 9, 10, 11, 12, 13, A5, A4, A3, A2, A1, A0, 4 };


// setup: Wird aufgerufen, wenn das Programm startet (nach RESET)
void setup() {
  //Alle LED Pins als AUSGANG setzen
  for (uint8_t i = 0; i < 16; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }
  Serial.begin(9600);
}

// wird immer wieder aufgerufen..
void loop() {
  digitalWrite(13,HIGH);
  int adc=analogRead(A6);
  digitalWrite(13,LOW);
  Serial.print("ADC-Wert: ");
  Serial.println(adc);

  //Umrechnen in Widerstandswert
  float rt=20000.0/(1023.0/adc-1);
  Serial.print("Widerstandswert: ");
  Serial.println(rt);

  //Umrechnen in Temperatur
  float t=ntc10_R2T(rt);
  Serial.print("Temperatur: ");
  Serial.println(t);
  
  Serial.println("--------------------------");
  for (uint8_t i = 0; i < 16; i++) {
    digitalWrite(LEDPins[i], HIGH);  //den i. Pin auf Versorgungsspannung (3.3V) setzen
    delay(WAITTIME);              //Warten
    digitalWrite(LEDPins[i], LOW);   //wieder auf 0V setzen
  }
  //Hier ist Ende und die loop-Funktion wird verlassen
  //um dann gleich wieder neu aufgerufen zu werden.
}
