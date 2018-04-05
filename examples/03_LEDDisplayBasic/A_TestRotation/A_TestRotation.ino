#include <LEDDisplay.h>
#include <Sleep.h>

//Create Instance of LEDDisplay
LEDDisplay led;

uint16_t bitmap[] = {
  256 * B01111111 + B11111111,
  256 * B01000000 + B00000001,
  256 * B01000000 + B00000001,
  256 * B01000000 + B00000001,
  256 * B01000000 + B00000001,
  256 * B01000000 + B00000001,
  256 * B01111111 + B11111111
};


void setup(void) {
  led.begin(); //Set Pins as OUTPUT, Activate INT0 ...
  led.add(bitmap,7);
  led.add(bitmap,7);
  led.add(bitmap,7);
  led.add(bitmap,7);
  led.add(bitmap,7);
  led.add(bitmap,7);

}

void loop(void) {
  Sleep.sleep();
  led.run();
}

