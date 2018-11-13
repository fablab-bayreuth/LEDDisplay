# LEDDisplay
Arduino library for rotating LED display. 

![Clock example](https://github.com/fablab-bayreuth/LEDDisplay/blob/master/extra/clock.png)

The LED Display board is a simple Arduino clone with a ATMega328p processor, 32768Hz RTC crystal, 16 LEDs and an reed switch.

Fonts are included from the Adafruit GFX library, which can be installed via Arduino library manager.


## Installation
- Download the zip-File and add the library to your arduino IDE.
- Install the Adafruit GFX Library using the library manager.

## Getting started
Connect the board to your PC with a FTDI adapter. 
Choose "Arduino Pro or Pro Mini" as Board and "ATMEGA328P (3.3V,8MHz)" as processor.

## Usage
A typical example sketch for having a bitmap running over the display looks like this:

	#include <LEDDisplay.h>

	//Define a font
	GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;

	//Create Instance of LEDDisplay
	LEDDisplay led;

	//Set shift wait
	int shift_wait = 70; //70ms for next shift

	//The text to display. Text is put in PROGMEM (not in RAM).
	//This allows us to have really big test strings ~ 24kb
	const char text[] PROGMEM = "Fablab Bayreuth -- Arduino Day 2018 - 07.04.2018 -- ab 13:00";

	void setup(void) {
  		led.begin();
  		led.setFont(gfxFont);
  		led.initRunning(shift_wait);
	}

	void loop(void) {
  		if (led.int0_flag) {
    		if (led.wokeupFromSleep()) {
        		led.clear();
        		led.initRunning(shift_wait);
    		}
		    led.setSpeed();
    		led.runningTextPROGMEM(text);
    		led.run();
    		led.int0_flag = 0;
  		}
  		led.sleep();
	}


 
 