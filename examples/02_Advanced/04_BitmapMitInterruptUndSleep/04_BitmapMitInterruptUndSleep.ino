/*
 *  Einfaches Beispiel für Zusammenspiel von 
 *  Sleep, Interrupt und led.write(INT16-Array);
 * 
 * 
 */
#define WAITTIME_MICROSEC 1000 
#include <LEDDisplay.h>
//#include <Sleep.h> // das ist jetzt nicht mehr notwendig, da Sleep.h schon in LEDDisplay.h drin ist :-)
//Create Instance of LEDArray
LEDArray led;

//Definition der Interrupt Service Routinen
//1. für den Read Kontakt - Wird im Setup aktiviert
void isr_int0(void) {
  //macht nichts --- muss aber trotzdem definiert sein
}

uint16_t smiley[]={
256*B00000111 + B11100000, 
256*B00011000 + B00011000,
256*001000000 + B00000100,
256*B01000110 + B00000010,
256*B01001000 + B00000010,
256*B10010000 + B00111001,
256*B10100000 + B00111001,
256*B10100000 + B00000001,
256*B10100000 + B00000001,
256*B10100000 + B00111001,
256*B10010000 + B00111001,
256*B01001000 + B00010010,
256*B01000110 + B00000010,
256*B00100000 + B00000100,
256*B00011000 + B00011000,
256*B00000111 + B11100000, 
};


void setup() {
  //Pullup Widerstand einschalten
  pinMode(2, INPUT_PULLUP); 
  //Interrupt aktivieren
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);
  led.init(); //Set Pins as OUTPUT
}

void loop() {
  Sleep.sleep(); //Hier bleibt das Programm stehen bis der REED-Kontakt ausgelöst wird
  led.write(smiley,16,WAITTIME_MICROSEC);    
}
