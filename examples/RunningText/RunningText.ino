#include <LEDDisplay.h>
#include <Sleep.h>

//GFXfont* gfxFont = &FreeSerifBold9pt7b;
GFXfont* gfxFont = &FreeSerif9pt7b;
//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
//GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;
//Create Instance of LEDArray
LEDDisplay led(gfxFont);


bool reverse=true;
int shift_speed=200;
uint8_t shift=2;

//The text to display. Text is put in PROGMEM (not in RAM).
//This allows us to have really big test strings ~ 24kb
const char text[] PROGMEM ="I don't really have no ambition you know. I only have one thing I'd really like to see happen. I'd like to see Mankind live together. Black, white, Chinese, everyone. That's all. - Donald Trump";
//char text[] PROGMEM ="Der Vorteil der Klugheit besteht darin, dass man sich dumm stellen kann. Das Gegenteil ist schon schwieriger. - Bastian Schweinsteiger";


void setup(void){
  led.init(); //Set Pins as OUTPUT
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);
  led.initRunning(shift,shift_speed);
}

//Interrupt Routine
volatile uint8_t int0_flag;
void isr_int0(void) {
  int0_flag=1;
  
}

void loop(void){
  
  if(int0_flag){
    led.setSpeed();
    led.runningTextPROGMEM(text);
    led.run(reverse);
    int0_flag=0;
  }
  if((micros()-led.getLastRotation())>500000){
    //no INT for more than 0.5 sec
    Sleep.sleep();
  } 
}


