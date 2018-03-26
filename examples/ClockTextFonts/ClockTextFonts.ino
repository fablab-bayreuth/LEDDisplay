#include <LEDDisplay.h>
#include <Sleep.h>
#include <RTClib.h>

RTC_Timer2 RTC;

ISR(TIMER2_OVF_vect) {
  RTC._seconds += 1;
}

//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
//GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;
//GFXfont* gfxFont1 = &FreeMonoBold9pt7b;
GFXfont* gfxFont1 = &FreeSerifBold9pt7b;
GFXfont* gfxFont2 = &FreeSans9pt7b;
uint8_t current_font = 1;
uint16_t rot_count = 0;
uint16_t mode = 0;
uint16_t rot_mode;
float fps;



//Create Instance of LEDArray
LEDDisplay led(gfxFont1);

volatile uint8_t int0_flag;
void isr_int0(void) {
  int0_flag = 1;
}

//Helper Function for now2text
void i2str(uint16_t i, unsigned char* b) {
  if (i < 10) {
    b[0] = '0';
    b++;
  }
  itoa(i, b, 10);
}

DateTime dt;
char text[] = "HH:MI:SS -- DD.MM.YYYY  ";
char text2[] = "Rotations:              ";
char text3[] = "Frames/s:               ";
char text4[] = "Runtime [s]:               ";


//gets time from RTC and formats it into the text array
void now2text(void) {
  dt = RTC.now();
  //Format the time-string
  i2str(dt.hour(), text);
  text[2] = ':';
  i2str(dt.minute(), text + 3);
  text[5] = ':';
  i2str(dt.second(), text + 6);
  text[8] = ' ';
  i2str(dt.day(), text + 12);
  text[14] = '.';
  i2str(dt.month(), text + 15);
  text[17] = '.';
  i2str(dt.year(), text + 18);

}

int shift_speed = 100;
uint8_t shift = 6;

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

const char rtext[] PROGMEM ="I don't really have no ambition you know. I only have one thing I'd really like to see happen. I'd like to see Mankind live together. Black, white, Chinese, everyone. That's all. - Donald Trump";
uint8_t tshift = 2;


void setup(void) {
  led.init(); //Set Pins as OUTPUT

  //Set up Interrupt
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);

  //init timer2 to 1 sec for RTC increment
  Sleep.setupTimer2();

  //Serial clock adjustment stuff
  //Program stays in this loop until INT0 is fired
  Serial.begin(9600);
  int0_flag = 0;
  while (! int0_flag) {
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

}

void loop(void) {
  if (int0_flag) {
    fps=1000000.0/(micros()-led.getLastRotation());
    led.setSpeed();
    switch (mode) {
      case 0:
        now2text();
        //write the time-string to LED
        led.setCursor(0);
        led.add(text);
        break;
      case 1:
        itoa(rot_count, text2 + 11, 10);
        led.setCursor(0);
        led.add(text2);
        break;
      case 2:
        dtostrf(fps, 4, 1, text3+10); // avr-libc function for floats  
        led.setCursor(0);
        led.add(text3);
        break;
      case 3:
        itoa(millis()/1000, text4 + 13, 10);
        led.setCursor(0);
        led.add(text4);
        break;
      case 4:
        led.runningBitmapPROGMEM(bitmap,bm_length);
        if(led.isDoneRunning()){
          mode++;
          led.initRunning(tshift,shift_speed);
          rot_mode=0;
        }
        break;
      case 5:
        led.runningTextPROGMEM(rtext);
        if(led.isDoneRunning()){
          mode=0;
          rot_mode=0;
        }
        break;
    }
    led.run();
    //get time from Clock
    //clear INT0 flag
    rot_count++;
    rot_mode++;
    if (rot_mode % 16 == 0 && mode<4) {
      if (current_font == 1) {
        led.setFont(gfxFont2);
        led.clear();
        current_font = 2;
      } else {
        led.setFont(gfxFont1);
        led.clear();
        current_font = 1;
      }
    }
    if (rot_mode % 32 == 0 && mode<4) {
      mode++;
      rot_mode=0;
      if(mode==4)   led.initRunning(shift,shift_speed);

      if (mode > 5) mode = 0;
    }
    int0_flag = 0;

  }
  if ((micros() - led.getLastRotation()) > 500000) {
    //no INT for more than 0.5 sec
    Sleep.sleep(TIMER2_ON, SLEEP_MODE_PWR_SAVE);    // sleep function called here
  }
}


