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
LEDDisplay led;


char text[]="Hallo Welt :-)";

void setup(void){
  led.begin();
  led.setFont(gfxFont);
  led.add(text); 

}

void loop(void){
  if(led.int0_flag){
    led.setSpeed();
    led.run();
  }
  led.sleep();
}
