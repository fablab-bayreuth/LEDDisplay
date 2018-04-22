#include <LEDDisplay.h>

//GFXfont* gfxFont = &FreeSerifBold9pt7b;
//GFXfont* gfxFont = &FreeSerif9pt7b;
//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;

//Create Instance of LEDDisplay
LEDDisplay led;


int shift_speed = 70; //Wait minimum 70ms for next shift
//effectivly this is one shift per rotation
uint8_t shift = 1; //shift by 1 letter

//The text to display. Text is put in PROGMEM (not in RAM).
//This allows us to have really big test strings ~ 24kb
const char text[] PROGMEM = "Fablab Bayreuth -- Arduino Day 2018 - 07.04.2018 -- ab 13:00";


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


