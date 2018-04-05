#include <LEDDisplay.h>

GFXfont* gfxFont = &FreeSerifBold9pt7b;
//Create Instance of LEDDisplay
LEDDisplay led;


uint16_t text_end_pos;
void setup(void){
  led.begin(); //Set Pins as OUTPUT
  led.setFont(gfxFont);
  led.add("Frames/s ");
  text_end_pos=led.getCursor();
}


void loop(void){
  if(led.int0_flag){
    float fps=led.getFramesPerSecond(); //Dies muss von setSpeed() aufgerufen werden!!
    led.setSpeed();
    led.setCursor(text_end_pos);
    led.addFloat(fps,5,1);
    led.run();
    led.int0_flag=0;
  }
  led.sleep();
}


