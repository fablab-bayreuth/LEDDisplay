#include <LEDDisplay.h>

//Choose a font

//GFXfont* gfxFont = &FreeMono9pt7b;
//GFXfont* gfxFont = &FreeMonoBold9pt7b;
//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
//GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;
//GFXfont* gfxFont = &FreeSans9pt7b;
//GFXfont* gfxFont = &FreeSansBold9pt7b;
//GFXfont* gfxFont = &FreeSansOblique9pt7b;
//GFXfont* gfxFont = &FreeSansBoldOblique9pt7b;
//GFXfont* gfxFont = &FreeSerif9pt7b;
GFXfont* gfxFont = &FreeSerifBold9pt7b;
//GFXfont* gfxFont = &FreeSerifItalic9pt7b;
//GFXfont* gfxFont = &FreeSerifBoldItalic9pt7b;


//Create Instance of LEDDisplay
LEDDisplay led(gfxFont);


//Your text:
//There is a hack to support the following UTF8-Chars ä, ö, ü, Ä, Ö, Ü, ß.
//All other chars will result in "?"
char text[]="Hallo Welt :-)";

void setup(void){
  led.init(); //Set Pins as OUTPUT
  led.add(text); //Add the text to display buffer
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

