/*
   Beispeilsketch zum Verständnis von Interrupts

*/
#include <Sleep.h>

//Definition der Interrupt Service Routinen
//1. für den Read Kontakt - Wird im Setup aktiviert
void isr_int0(void) {
  Serial.print(" **** INT0 **** ");
}

//2. Für den Timer2 mit dem Uhrenquarz
ISR(TIMER2_OVF_vect) {
  Serial.print(" **** TIMER2 **** ");
}



void setup() {
  //Serielle Schnittstelle mit 9600 Baud starten
  Serial.begin(9600);
  //Pullup Widerstand einschalten
  pinMode(2, INPUT_PULLUP); 
  //Interrupt aktivieren
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);
  //TIMER2 so initialisieren, dass er jede Sekunde 1x überläuft.
  //Dann wird die oben definierte Funktion aufgerufen.
  Sleep.setupTimer2();
}

void loop() {
  Serial.print("ich ");
  delay(100);
  Serial.print("bin ");
  delay(100);
  Serial.print("die ");
  delay(100);
  Serial.print("loop ");
  delay(100);
  Serial.println("Funktion");
}
