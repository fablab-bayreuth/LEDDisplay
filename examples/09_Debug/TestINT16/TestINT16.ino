#include <LEDDisplay.h>

//Create Instance of LEDArray
LEDArray led;




void setup(void) {
  led.init(); //Set Pins as OUTPUT
  led.set(256 * B11111111 + B11111111);
  delay(1000);
  led.set(256 * B11110000 + B11110000);
  delay(1000);
  led.set(256 * B00001111 + B11110000);
  delay(1000);
  led.set(256 * B11110000 + B00001111);
  delay(1000);


  led.set(0xffff);
  delay(1000);
  led.set(0xf0f0);
  delay(1000);
  led.set(0x0ff0);
  delay(1000);
  led.set(0xf00f);
  delay(1000);
  led.set(0);


}

void loop(void) {
  uint16_t v = 1;
  for (uint8_t i = 0; i < 16; i++) {
    led.set(v);
    delay(200);
    v <<= 1;
  }

}

