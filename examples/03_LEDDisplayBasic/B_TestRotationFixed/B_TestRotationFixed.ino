#include <LEDDisplay.h>

GFXfont* gfxFont = &FreeSerifBold9pt7b;
//Create Instance of LEDArray
LEDDisplay led(gfxFont);

uint16_t bitmap[]={
256*B01111111 + B11111111, 
256*B01000000 + B00000001,
256*B01000000 + B00000001,
256*B01000000 + B00000001,
256*B01000000 + B00000001,
256*B01000000 + B00000001,
256*B01111111 + B11111111 
};

void setup(void){
  led.init(); //Set Pins as OUTPUT
  led.add("Hallo Welt");
  led.add(bitmap,7);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);

}

volatile uint8_t int0_flag;
void isr_int0(void) {
  int0_flag=1;
  
}

void loop(void){
  if(int0_flag){
    led.setSpeed();
    led.run();
    int0_flag=0;
  }
  led.sleep();
}


