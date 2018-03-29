#include <LEDDisplay.h>

//Create Instance of LEDArray
LEDArray led;

void write(uint16_t w){
  unsigned long t;
  t=micros();
  led.set(w);
  t=micros()-t;
  Serial.println(t);
  delay(500);
  
}

void setup(void){
  Serial.begin(9600);
  led.init(); //Set Pins as OUTPUT
  write(256*B11111111 + B11111111);
  write(256*B11110000 + B11110000);
  write(256*B00001111 + B11110000);
  write(256*B11110000 + B00001111);

  
  write(0xffff);
  write(0xf0f0);
  write(0x0ff0);
  write(0xf00f);
  write(0);

  uint16_t v=1;
  for(uint8_t i=0;i<16;i++){
    write(v);
    v<<=1;
  }

}

void loop(void){
}

