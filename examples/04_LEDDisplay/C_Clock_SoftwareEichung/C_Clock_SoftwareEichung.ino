/*
 * Der Uhren-Quarz auf der Schaltung hat nur eine bestimmte 
 * Genauigkeit. Außerdem wurde bei manchen Versionen auf die
 * Lastkondensatioren verzichtet, was dazu führt, dass
 * die Uhr meist etwas zu schnell läuft.
 * 
 * Dies lässt sich jedoch recht einfach korrigieren, indem man
 * in bestimmten Abständen eine Sekunde zurück oder vorzählt
 * 
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
  if(corr_sec_counter>=CORR_SEC_COUNT){
    corr_sec_counter=0;
    RTC._seconds = RTC._seconds + CORR_DIRECTION;
  }
}

//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
//GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;
GFXfont* gfxFont = &FreeMonoBold9pt7b;
//GFXfont* gfxFont = &FreeSerifBold9pt7b;
//Create Instance of LEDDisplay
LEDDisplay led;

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
    Serial.end();

  }

}

void loop(void) {
  if (led.int0_flag) {
    led.setSpeed();
    //get time from Clock
    now2text();
    //write the time-string to LED
    led.setCursor(0);
    led.add(text);
    led.run();
    //clear INT0 flag
    led.int0_flag = 0;
  }
  led.sleep(true); //Sleep but leave clock on
}


