#include <LEDDisplay.h>

GFXfont* gfxFont = &FreeSerifBold9pt7b;
//Create Instance of LEDArray
LEDDisplay led(gfxFont);

volatile uint8_t int0_flag;
void isr_int0(void) {
  int0_flag=1;
  
}


uint16_t text_end_pos;
void setup(void){
  led.init(); //Set Pins as OUTPUT
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);
  led.add("Frames/s ");
  text_end_pos=led.getCursor();
}


void loop(void){
  if(int0_flag){
    float fps=led.getFramesPerSecond(); //Dies muss von setSpeed() aufgerufen werden!!
    led.setSpeed();
    led.setCursor(text_end_pos);
    led.addFloat(fps,5,1);
    led.run();
    int0_flag=0;
  }
  led.sleep();
}


