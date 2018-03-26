#include <LEDDisplay.h>
#include <Sleep.h>
#include <RTClib.h>


RTC_Timer2 RTC;

ISR(TIMER2_OVF_vect) {
  RTC._seconds += 1;
}

//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
//GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;
GFXfont* gfxFont = &FreeMonoBold9pt7b;
//GFXfont* gfxFont = &FreeSerifBold9pt7b;
//Create Instance of LEDArray
LEDDisplay led(gfxFont);

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
char text[] = "HH:MI:SS -- DD.MM.YYYY    ";


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
    led.setSpeed();
    //get time from Clock
    now2text();
    //write the time-string to LED
    led.setCursor(0);
    led.add(text);
    led.run();
    //clear INT0 flag
    int0_flag = 0;
  }
  if ((micros() - led.getLastRotation()) > 500000) {
    //no INT for more than 0.5 sec
    Sleep.sleep(TIMER2_ON, SLEEP_MODE_PWR_SAVE);    // sleep function called here
  }
}


