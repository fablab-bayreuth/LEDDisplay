
#include <math.h>
float ntc10_R2T(float r) {
  float log_r = log(r);
  return 440.61073 - 75.69303 * log_r +
         4.20199 * log_r * log_r - 0.09586 * log_r * log_r * log_r;
}

#include <LEDDisplay.h>

GFXfont* gfxFont = &FreeSerifBold9pt7b;
//Create Instance of LEDArray
LEDDisplay led(gfxFont);

volatile uint8_t int0_flag;
void isr_int0(void) {
  int0_flag=1;
  
}


uint16_t text_end_pos;
void setup(void) {
  led.init(); //Set Pins as OUTPUT
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);
  led.add("Temp C: ");
  text_end_pos = led.getCursor();
}

void loop() {
  if (int0_flag) {
    digitalWrite(13, HIGH);
    int adc = analogRead(A6);
    digitalWrite(13, LOW);
    //Umrechnen in Widerstandswert
    float rt = 20000.0 / (1023.0 / adc - 1);
    //Umrechnen in Temperatur
    float t = ntc10_R2T(rt);
    led.setSpeed();
    led.setCursor(text_end_pos);
    led.addFloat(t, 5, 1);
    led.run();
    int0_flag = 0;
  }
  led.sleep();

}
