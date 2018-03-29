/*
   Beispeilsketch zum Verständnis von Interrupts

*/
#include <Sleep.h>

//Definition der Interrupt Service Routinen
//1. für den Read Kontakt - Wird im Setup aktiviert
volatile uint8_t int0_flag;
void isr_int0(void) {
  // Serial.print(" **** INT0 **** "); //Funktioniert nicht, da noch disabled
  int0_flag=1;
}

//2. Für den Timer2 mit dem Uhrenquarz
volatile uint8_t timer2_flag;
ISR(TIMER2_OVF_vect) {
 // Serial.print(" **** TIMER2 **** ");
 timer2_flag=1;
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
  Serial.print("sooo... ");
  Serial.flush();
  Sleep.sleep(); //Hier bleibt das Programm stehen und braucht nur noch 0,3µA
  //Hier ist auch die UHR ausgeschaltet. Damit kann nur noch INTERRUPT0 (Reedkontakt) uns aufwecken (oder natürlich ein Reset)
  //
  // Wenn die UHR anbeiben soll, müssen wir statt Sleep.sleep() das folgende schreiben:
  //Sleep.sleep(TIMER2_ON, SLEEP_MODE_PWR_SAVE);

  delay(100);
  Serial.println(" was? wer hat mich geweckt?? ");
  if(int0_flag){
    Serial.println("Ach so, der INT0");
    int0_flag=0;
  }
  if(timer2_flag){
    Serial.println("Jetzt gab es noch einen Timer2 Interrupt");
    timer2_flag=0;
    
  }
}
