#include <LEDDisplay.h>

LEDDisplay led;


int shift_speed = 100;
uint8_t shift = 3;

//The Bitmap to display. Bitmap is put in PROGMEM (not in RAM).
//This allows us to have really big bitmaps ~ 24kb
//Bitmap can be create with http://en.radzio.dxp.pl/bitmap_converter/

uint16_t bm_length = 2 * 100; //16x100 bitmap
const uint8_t bitmap[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFC, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xE0,
  0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFE, 0xFE, 0x86, 0x86, 0x86, 0x86, 0xFE, 0xFE, 0xC4, 0xF4,
  0x1C, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0x86, 0x86, 0x86, 0xFE, 0xFE, 0xFE, 0xFE, 0x86,
  0x86, 0x86, 0xFE, 0xFE, 0xFE, 0xFE, 0x86, 0x86, 0x86, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x60,
  0xE0, 0x30, 0x20, 0x34, 0x28, 0x34, 0x38, 0x30, 0x28, 0x38, 0x38, 0x38, 0x3C, 0x38, 0x2A, 0x30,
  0x38, 0x28, 0x38, 0x28, 0x30, 0x20, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x10, 0x10, 0x3F, 0x07, 0x07, 0x77, 0x8F,
  0x8F, 0x8F, 0x77, 0x07, 0x07, 0x77, 0x8F, 0x8F, 0x8F, 0x77, 0x07, 0x07, 0x77, 0x8F, 0x8F, 0x8F,
  0x77, 0x1F, 0x1F, 0x10, 0x10, 0x38, 0x10, 0x10, 0x10, 0x3F, 0x27, 0x77, 0x8F, 0x8F, 0x8F, 0x77,
  0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x77, 0x8F, 0x8F, 0x8F, 0x77, 0x27, 0x3F, 0x10,
  0x10, 0x10, 0x38, 0x10, 0x1F, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x78, 0x08, 0x08, 0x08, 0x08,
  0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x88, 0x88, 0x88, 0x7C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



void setup(void) {
  led.begin(); //Set Pins as OUTPUT
  led.initRunning(shift, shift_speed);
}


void loop(void) {
  if (led.int0_flag) {
    led.setSpeed();
    led.runningBitmapPROGMEM(bitmap, bm_length);
    led.run();
    led.int0_flag = 0;
  }
  led.sleep();
}


