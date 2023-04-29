#include <LEDDisplay.h>

//Define a font
GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;

//Create Instance of LEDDisplay
LEDDisplay led;

//Set shift wait
int shift_wait = 70; //70ms for next shift

//The text to display. Text is put in PROGMEM (not in RAM).
//This allows us to have really big text strings ~ 24kb
const char text[] PROGMEM = "Fablab Bayreuth -- Arduino Day 2018 - 07.04.2018 -- ab 13:00";

void setup(void) {
  led.begin(); //initialize display (e.g. attach INT0)
  led.setFont(gfxFont); //set font pointer
}

void loop(void) {
  if (led.int0_flag) {
    if (led.wokeupFromSleep()) {
      led.clear(); //clear the display
      led.initRunning(shift_wait); //set shift_wait and set offset to zero
    }
    led.setSpeed();//calculates the speed to flash the LEDs
    led.runningTextPROGMEM(text);//add new char to the buffer if shift time is exceeded
    led.run(); //flashes the LEDs and clears int0_flag at the end
  }
  led.sleep(); //go to sleep when there is no int0 for more than 500ms
}
