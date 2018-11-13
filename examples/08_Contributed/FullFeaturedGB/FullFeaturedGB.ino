/*
   Alles was geht :-)

   Mehr Platz hat der Arduino nicht!


*/

#include <LEDDisplay.h>
#include <RTClib.h>


#define CORR_SEC_COUNT 24000
#define CORR_DIRECTION -1

RTC_Timer2 RTC;
volatile unsigned long corr_sec_counter;
ISR(TIMER2_OVF_vect) {
  RTC._seconds += 1;
  corr_sec_counter++;
  if (corr_sec_counter >= CORR_SEC_COUNT) {
    corr_sec_counter = 0;
    RTC._seconds = RTC._seconds + CORR_DIRECTION;
  }
}
//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
GFXfont* gfxFont2 = &FreeMonoBoldOblique9pt7b;
//GFXfont* gfxFont = &FreeMonoBold9pt7b;
GFXfont* gfxFont = &FreeSerifBold9pt7b;
//GFXfont* gfxFont = &FreeSans9pt7b;
uint8_t mode = 0;
uint8_t last_mode = 255;

uint16_t rot_mode;



//Create Instance of LEDArray
LEDDisplay led;

#include "bitmaps.h"
#include "rtc_func.h"
#include "zug_lang.h"
#include "towerBridge.h"
#include "pm.h"
#include "dict.h"
#include <wuerfel.h>


int shift_wait = 60;


const char rtext[] PROGMEM = "Fablab Bayreuth --- Arduino Day 2018 --- 07.04.2018";


void setup(void) {
  led.begin();
  led.setFont(gfxFont);

  //init timer2 to 1 sec for RTC increment
  Sleep.setupTimer2();

  //Serial clock adjustment stuff
  //Program stays in this loop until INT0 is fired
  Serial.begin(9600);
  led.int0_flag = 0;
  while (! led.int0_flag) {
    Serial.print("Current time: ");
    now2text();
    Serial.println(text);
    Serial.println("To set clock send string in format YYYY/MM/DD HH24:MI:SS");
    if (Serial.available() >= 19) {

      uint8_t m, d, hh, mm, ss;
      uint16_t y;
      uint8_t buffer[25];
      for (uint8_t i = 0; i < 19; i++) {
        buffer[i] = Serial.read();
      }
      y = atoi(buffer + 0);
      m = atoi(buffer + 5);
      d = atoi(buffer + 8);
      hh = atoi(buffer + 11);
      mm = atoi(buffer + 14);
      ss = atoi(buffer + 17);
      dt = DateTime (y, m, d, hh, mm, ss);
      RTC.adjust(dt);
      delay(100);
      while (Serial.available()) Serial.read();

    }
    delay(1000);

  }
  Serial.end();

}

uint16_t text_end_pos, pm_pos;
uint8_t wert = 0;
uint8_t schritt = 9;
unsigned long last_int0;
float temp;
uint8_t res;

void loop(void) {
  if (led.int0_flag) {
    if (led.wokeupFromSleep()) {
      if ((RTC.now().get() - last_int0) > 5) {
        led.clear();
        led.initRunning(shift_wait/6);
        mode = 0; //Neustart nach Schlafmode > 5 Sec
        led.setConf();
      }
    }
    float temp_float = led.getFramesPerSecond();
    led.setSpeed();
    last_int0 = RTC.now().get();
    if (last_mode != mode) {
      //Initialisierungen für die verschiedenen Anzeigen-Modi
      last_mode = mode;
      led.clear();
      switch (mode) {
        case 0:
          led.setFont(gfxFont);
          break;
        case 1:
          led.setFont(gfxFont2);
          led.add("Rotations: ");
          break;
        case 2:
          led.setFont(gfxFont);
          led.add("Temp: ");
          temp = led.getTemperature();
          led.addFloat(temp, 4, 1);
          led.add("C / ");
          led.addFloat(temp * 9 / 5 + 32, 4, 1);
          led.add("F ");
          break;
        case 3:
          led.setFont(gfxFont2);
          led.add("Runtime [s]: ");
          break;
        case 4:
          led.setFont(gfxFont);
          led.add("Frames/s: ");
          break;
        case 5:
          led.initRunning(shift_wait/6);
          break;
        case 6:
          led.initRunning( shift_wait);
          break;
        case 7:
          for (uint16_t i = 0; i < 160; i++) {
            if (i % 4 > 1) led.add((uint16_t) 256 * B00000001 + B10000000);
            else led.add((uint16_t) 0);
          }
          led.setConf(FIXED_DISPLAY, 160);
          pm_pos = 160 - 16;
          break;
        case 8:
          led.setConf(FIXED_DISPLAY, 160);
          randomSeed(millis());
          wert = 1;
          pm_pos = 0;
          schritt = 9;
          led.clear();
          break;
        case 9:
          led.setConf(FIXED_DISPLAY, 160);
          led.setCursor(60);
          led.addBitmapPROGMEM(towerBridge, 49 * 2);

          break;
        case 10:
          led.setConf();
          led.setFont(gfxFont2);
          led.initRunning( shift_wait);
          res = dict_next_word();
          break;

      }

      rot_mode = 0;
      text_end_pos = led.getCursor();
    }

    switch (mode) {
      case 0:
        now2text();
        //write the time-string to LED
        led.setCursor(0);
        led.add(text);
        if (rot_mode > 30) mode++;
        break;
      case 1:
        led.setCursor(text_end_pos);
        led.addInteger(led.getRotationCount());
        if (rot_mode > 30) mode++;
        break;
      case 2:
        if (rot_mode > 30) mode++;
        break;
      case 3:
        led.setCursor(text_end_pos);
        led.addInteger(millis() / 1000);
        if (rot_mode > 30) mode++;
        break;
      case 4:
        led.setCursor(text_end_pos);
        led.addFloat(temp_float, 4, 1);
        if (rot_mode > 30) mode++;
        break;
      case 5:
        led.runningBitmapPROGMEM(zug_lang, 522 * 2);
        //        led.runningBitmapPROGMEM(bitmap, bm_length);
        if (led.isDoneRunning()) {
          mode++;
        }
        break;
      case 6:
        led.runningTextPROGMEM(rtext);
        if (led.isDoneRunning()) {
          mode++;
        }
        break;
      case 7:

        //clear last 8px of old packman
        led.setCursor(pm_pos + 12);
        led.clear(4);
        //move 4px to the left
        pm_pos -= 4;
        led.setCursor(pm_pos);
        //Add open or closed mouth packman
        if (rot_mode % 4 == 0 ) led.addBitmapPROGMEM(pm_1, 32);
        else if (rot_mode % 4 == 2) led.addBitmapPROGMEM(pm_3, 32);
        else led.addBitmapPROGMEM(pm_2, 32);


        if ((pm_pos * 2) < (160 - 32)) {
          led.setCursor(pm_pos * 2 + 32);
          led.clear(8);
          led.setCursor(pm_pos * 2 + 16);
          led.addBitmapPROGMEM(geist, 32);
        }
        //We got to the end...
        if (pm_pos < 4) {
          mode++;
        }

        break;
      case 8:
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
          mode++;
        }
        break;
      case 9:
        if (rot_mode > 30) mode++;

        break;
      case 10:
        led.runningText(dict_buffer);
        if (led.isDoneRunning()) {
          if (! res) mode++;
          else
            res = dict_next_word();
        }


    }
    led.run();


    if (mode > 10) mode = 0;

    //rotations in current mode
    rot_mode++;

    led.int0_flag = 0;

  }
  led.sleep(true);//Sleep but leave clock on
}


