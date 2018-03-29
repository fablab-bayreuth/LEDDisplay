/*
 * Das Arduino Blink Beispiel
 * 
 * TODO: Ändere doch mal den LED_PIN. 
 * Schaue dazu in die Dokumentation, um aus dem Schaltplan
 * herauszufinden, welcher Pin mit welcher LED verbunden ist
 */


#define WAITTIME 1000
#define LED_PIN 13

// setup: Wird aufgerufen, wenn das Programm startet (nach RESET)
void setup() {
  // Pin als AUSGANG setzen
  pinMode(LED_PIN, OUTPUT);
}

// wird immer wieder aufgerufen..
void loop() {
  digitalWrite(LED_PIN, HIGH);  //LED_PIN auf Versorgungsspannung (3.3V) setzen
  delay(WAITTIME);              //Warten
  digitalWrite(LED_PIN, LOW);   //LED_PIN auf 0V setzen
  delay(WAITTIME);              //Warten
  //Hier ist Ende und die loop-Funktion wird verlassen
  //um dann gleich wieder neu aufgerufen zu werden.
}
