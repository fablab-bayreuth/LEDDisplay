#include <LEDDisplay.h>

LEDDisplay led;

//Three packman bitmaps + Ghost
#include "pm.h" 

#define WIDTH 160

void initPoints(void) {
  led.clear();
  //Print some dots
  for (uint16_t i = 0; i < WIDTH; i++) {
    if (i % 4 > 1) led.add((uint16_t) 256 * B00000001 + B10000000);
    else led.add((uint16_t) 0);
  }

}


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
  led.setConf(FIXED_DISPLAY,WIDTH);
  initPoints();
}


uint16_t pos = WIDTH - 16;
uint8_t rot;

void loop(void) {
  if (led.int0_flag) {
    //clear last 8px of old packman
    led.setCursor(pos + 12);
    led.clear(4);
    //move 4px to the left
    pos -= 4;
    led.setCursor(pos);
    //Add open or closed mouth packman
    if (rot % 4 == 0 ) led.addBitmapPROGMEM(pm_1, 32);
    else if(rot % 4 == 2) led.addBitmapPROGMEM(pm_3,32);
    else led.addBitmapPROGMEM(pm_2, 32);

    //der Geist
    if((pos*2)<(WIDTH-32)){
     led.setCursor(pos*2 + 32);
     led.clear(8);
     led.setCursor(pos*2+16);
     led.addBitmapPROGMEM(geist,32);
    }

    //We got to the end...
    if (pos < 4) {
       initPoints();
       pos = WIDTH - 16;
    }
    rot++;
    led.setSpeed();
    led.run();
  }
  led.sleep();
}
