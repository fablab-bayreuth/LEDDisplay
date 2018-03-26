#include <LEDDisplay.h>
#include <Sleep.h>

GFXfont* gfxFont = &FreeSerifBold9pt7b;
//Create Instance of LEDArray
LEDDisplay led(gfxFont);

const uint8_t pm1 [] = {
0x60, 0xF8, 0x7C, 0x7E, 0xFE, 0x7E, 0xFE, 0x7F, 0xFF, 0x67, 0xE7, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xF8, 0x80, 0x03, 0x07, 0x1F, 0x3F, 0x7F, 0x7F, 0x7F, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x1F
};

const uint8_t pm2 [] = {
0x00, 0x00, 0x04, 0x0E, 0x1E, 0x3E, 0x3E, 0x7F, 0x7F, 0x67, 0x67, 0x67, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xF8, 0x80, 0x00, 0x04, 0x1C, 0x3C, 0x7E, 0x7E, 0x7E, 0xFE,
0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x1F
};

uint8_t rot;

void initPoints(void){
  for(uint16_t i=0;i<PIXELCOUNT;i++){
    if(i%8>3) led.add((uint16_t) 256*B00000011 + B11000000);
    else led.add((uint16_t) 0);
  }
  
}


void setup(void){
  led.init(); //Set Pins as OUTPUT
  /*
   * Default Mode of the LED display is
   * "RUNNING_DISPLAY"
   * adding something to the display moves existing 
   * content to the left
   * 
   * Here we set mode=FIXED_DISPLAY
   * Using led.setCursor(pos) + led.add(...) 
   * overwrites content on a defined position.
   */
  led.setConf(FIXED_DISPLAY);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);

}

volatile uint8_t int0_flag;
void isr_int0(void) {
  int0_flag=1;
  initPoints();
}

uint16_t pos=PIXELCOUNT-24;

void loop(void){
  if(int0_flag){
    if(rot%2 == 0){
       led.setCursor(pos+20);
       led.add(' ');
       pos-=4;
       led.setCursor(pos);
        if(rot%4 == 0 ) led.addBitmap(pm1,48);
        else led.addBitmap(pm2,48);
       if(pos<4){
         led.clear(); 
         initPoints();
         pos=PIXELCOUNT-24;
       }
       
    }
    rot++;
    led.setSpeed();
    led.run();
    int0_flag=0;
  }
  if((micros()-led.getLastRotation())>500000){
    //no INT for more than 0.5 sec
    Sleep.sleep();
  } 
}


