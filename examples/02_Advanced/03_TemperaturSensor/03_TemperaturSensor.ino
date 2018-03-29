/*
 * 
 * Beispiel-Sketch zum Verständnis der Temperaturmessung
 * mit AnalogRead
 *  
 */



/* 
 *  Umrechnungsfunktion Widerstand (Ohm) in Temperatur (°C)
 *  Gewonnen wurde die Funktion durch eine Polynomanpassung
 *  T~ln(r)
 *  
 *  Um die Funktion log() zur Verfügung zu haben, müssen wir
 *  math.h einbinden.
 */
#include <math.h>
float ntc10_R2T(float r) {
  float log_r = log(r);
  return 440.61073 - 75.69303 * log_r +
         4.20199 * log_r * log_r - 0.09586 * log_r * log_r * log_r;
}


void setup() {
  Serial.begin(9600);
}

void loop() {
  /*
   * Hinweis: Der Sensor ist nicht dauerhaft mit Strom versorgt.
   * Dies hätte einen Dauerstromverbrauch von 100µA zur Folge
   * Eine Knopfzelle wäre da in 2000h (~ 3 Monate) leer.
   * Das kann man sich sparen, wenn man den Sensor nur zum 
   * Messen anschaltet.
   */
  //Spannung auf den Sensor legen
  pinMode(13,OUTPUT); 
  digitalWrite(13,HIGH);
  int adc=analogRead(A6);
  digitalWrite(13,LOW);
  pinMode(13,INPUT);
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
  delay(1000);

}
