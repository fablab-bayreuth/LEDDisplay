#ifndef _LEDARRAY_H_
#define _LEDARRAY_H_

#include <Arduino.h>

const uint8_t LEDArrayPin[] = {4, A0, A1, A2, A3, A4, A5, 13, 12, 11, 10, 9, 8, 7, 6, 5 };

/*
LEDArray is the raw device
it has only functions for setting the LEDs
the print methods are for Serial debug
*/


class LEDArray {

  public:
    LEDArray();
    void set(uint16_t w);
    void write(uint16_t* w,uint16_t l, uint16_t wt=700,bool reverse=false);
    void init(void);
    void print(uint16_t v);
    void print(uint16_t* w, uint16_t l);
};

#endif
