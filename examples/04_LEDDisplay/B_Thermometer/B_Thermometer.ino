#include <LEDDisplay.h>

GFXfont* gfxFont = &FreeSerifBold9pt7b;
LEDDisplay led;

uint16_t text_end_pos;
void setup(void) {
  led.begin();
  led.setFont(gfxFont);
  led.add("Temperatur C: ");
  text_end_pos = led.getCursor();
}

void loop() {
  if (led.int0_flag) {
    float temp = led.getTemperature();
    led.setSpeed();
    led.setCursor(text_end_pos);
    led.addFloat(temp, 5, 1);
    led.run();
  }
  led.sleep();

}
