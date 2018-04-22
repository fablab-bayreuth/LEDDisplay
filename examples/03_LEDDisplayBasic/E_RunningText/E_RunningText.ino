#include <LEDDisplay.h>

//GFXfont* gfxFont = &FreeSerifBold9pt7b;
//GFXfont* gfxFont = &FreeSerif9pt7b;
//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;

//Create Instance of LEDDisplay
LEDDisplay led;


int shift_speed = 200;
uint8_t shift = 2;

//The text to display. Text is put in PROGMEM (not in RAM).
//This allows us to have really big test strings ~ 24kb
const char text[] PROGMEM = "Zum Aktionstag - ARDUINO DAY 2018 - begrüßt die offene High-Tech-Werkstatt FabLAB-Bayreuth, unsere Oberbürgermeisterin: Frau Brigitte Merk-Erbe!";


void setup(void) {
  led.begin();
  led.setFont(gfxFont);
  led.initRunning(shift, shift_speed);
}

void loop(void) {
  if (led.int0_flag) {
    if (led.wokeupFromSleep()) {
        led.clear();
        led.initRunning(shift, shift_speed);
    }

    led.setSpeed();
    led.runningTextPROGMEM(text);
    led.run();
    led.int0_flag = 0;
  }
  led.sleep();
}


