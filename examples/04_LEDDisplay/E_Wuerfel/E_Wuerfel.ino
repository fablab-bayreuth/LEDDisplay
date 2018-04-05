#include <LEDDisplay.h>

LEDDisplay led;


//Diese Include-Datei hat die Bitmaps und zwei Funktionen
//Sie erwartet, dass es "led" gibt
#include <wuerfel.h>


void setup(void) {
  led.begin(); 
  /*
     Default Mode of the LED display is
     "FIFO_DISPLAY"
     adding something to the display moves existing
     content to the left

     Here we set mode=FIXED_DISPLAY
     Using led.setCursor(pos) + led.add(...)
     overwrites content on a defined position.
  */
  led.setConf(FIXED_DISPLAY, 160);

}


uint16_t pos = 0;
uint8_t rot;

uint8_t wert = 0;
uint8_t schritt = 9;
void loop(void) {
  if (led.int0_flag) {
    if (led.wokeupFromSleep()) {
      randomSeed(millis());
      wert = 1;
      pos = 0;
      schritt = 9;
      led.clear();
    }
    if (wert) {
      led.setCursor(pos);
      led.clear(10);

      //move 8px to the right
      pos += schritt;
      led.setCursor(pos);

      if (pos < (160 - 24)) {
        if (rot % 4) {
          printWuerfelRollend(rot % 4);
        } else {
          led.setCursor(pos + 2);
          wert = random(1, 7);
          printWuerfel(wert);
          schritt--;
        }
      } else {
        if (wert) {
          printWuerfel(wert);
          wert = 0;
        }
      }
      rot++;
    }
    led.setSpeed();
    led.run();
    led.int0_flag = 0;
  }
  led.sleep();
}


