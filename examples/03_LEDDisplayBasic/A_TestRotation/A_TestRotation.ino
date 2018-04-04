#include <LEDDisplay.h>
#include <Sleep.h>

//Create Instance of LEDArray
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


void isr_int0(void) {
}

void setup(void) {
  led.init(); //Set Pins as OUTPUT
  led.add(bitmap,7);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);

}

void loop(void) {
  Sleep.sleep();
  led.run();
}

