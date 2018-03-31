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
  Serial.begin(9600);
  led.init(); //Set Pins as OUTPUT
  led.add(text); //Add the text to display buffer
  //This is just for font testing on serial monitor
  //comment if you do not need
  led.print();
   //end serial

}

void loop(void){
  led.run();
}

