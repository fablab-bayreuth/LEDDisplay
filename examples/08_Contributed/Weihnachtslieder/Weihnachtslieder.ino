/*


*/

#include <LEDDisplay.h>

//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
//GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;
GFXfont* gfxFont = &FreeMonoBold9pt7b;
//GFXfont* gfxFont = &FreeSerifBold9pt7b;
//GFXfont* gfxFont = &FreeSans9pt7b;
uint8_t mode = 0;
uint8_t last_mode = 255;
uint8_t last_text = 255;

uint16_t rot_mode;

#include "lieder.h"

void new_mode(void) {
  randomSeed(millis());

  do {
    mode = random(1, WI_COUNT+1);
  } while (mode == last_text);
}

//Create Instance of LEDArray
LEDDisplay led;

#include <wuerfel.h>




void setup(void) {
  led.begin();
  led.setFont(gfxFont);
}

uint16_t text_end_pos, pm_pos;
uint8_t wert = 0;
uint8_t schritt = 9;
unsigned long last_int0;
float temp;
void loop(void) {
  if (led.int0_flag) {
    //New Rotation starts
    if (led.wokeupFromSleep()) {
      mode=0;
    }
    led.setSpeed();
    
    //new mode 
    if (last_mode != mode) {
      //Initialisierungen für die verschiedenen Anzeigen-Modi
      last_mode = mode;
      led.clear();
      switch (mode) {
        case 0:
          led.setConf(FIXED_DISPLAY, 160);
          randomSeed(millis());
          wert = 1;
          pm_pos = 0;
          schritt = 9;
          led.clear();
          break;
        default:
          led.setConf(FIFO_DISPLAY,380);

          led.initRunning(t_times[mode-1]);
          break;
      }

      rot_mode = 0;
      text_end_pos = led.getCursor();
    }

    //Change content of the display buffer
    switch (mode) {
        case 0:
        if (wert) {
          led.setCursor(pm_pos);
          led.clear(10);

          //move  schritt px to the right
          pm_pos += schritt;
          led.setCursor(pm_pos);

          if (pm_pos < (160 - 24)) {
            if (rot_mode % 4) {
              printWuerfelRollend(rot_mode % 4);
            } else {
              led.setCursor(pm_pos + 2);
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
        }

        if (rot_mode > 35) {
          new_mode();
          last_text=mode;
        }
        break;
      default:
        led.runningTextPROGMEM(wi[mode - 1]);
        if (led.isDoneRunning()) {
          mode = 0;
        }
        break;

    }
    //Run the display in the calculated speed (one rotation)
    led.run();

    //rotations in current mode
    rot_mode++;

    led.int0_flag = 0;

  }
  led.sleep();//Sleep
}


