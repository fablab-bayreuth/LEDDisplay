#include <LEDDisplay.h>

//Create Instance
LEDDisplay led;

void setup(void){
  //Init Display: Set Pins as Output, INT0, ...
  led.begin();
  
  //Set Font
  led.setFont(&FreeSerifBold9pt7b);
  
  //Add Content
  led.add("Hallo Welt"); 
}

void loop(void){
  //Look for INT0
  if(led.int0_flag){
    //Adjust the LED frequency according to rotation speed
    led.setSpeed();

    //OPTIONAL
    //Change buffer content

    //Print content with calculated frequency and clear int0_flag
    led.run();
  }
  
  //go to sleep when there is no rotation
  led.sleep();
}
