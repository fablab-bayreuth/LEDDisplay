#include "LEDArray.h"

LEDArray::LEDArray() {
}

inline uint8_t Bit_Reverse(uint8_t x) {
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return x;
}

void LEDArray::init(void) {
	for (uint8_t j = 0; j < 16; j++) {
		pinMode(LEDArrayPin[j], OUTPUT);
	}
}

void LEDArray::set(uint16_t w) {

	PORTD = B11110000 & ((w << 5) | (w >> 11)) | B00001111 & PORTD;
	PORTB = B00111111 & (w >> 3) | PORTB & B11000000;
	PORTC = B00111111 & (Bit_Reverse(w >> 7)) | PORTC & B11000000;

	/*

	 PORTD=B00010000&(w<<4) | B00001111 & PORTD; //set bit 0 clear 13,14,15
	 w>>=1; //Move bits to right;
	 PORTC=B00111111 & w | PORTC & B11000000;
	 w>>=6; //Move 6 bits to right;
	 PORTB=B00111111&( Bit_Reverse(w)>>2 ) | PORTB & B11000000;
	 w>>=6; //Move 6 bits to right;
	 PORTD|=Bit_Reverse(w);
	 */
	/*  Too slow!!
	 for (uint8_t j = 0; j < 16; j++) {
	 digitalWrite(LEDArrayPin[j],w&0x1);
	 w>>=1;
	 }
	 */
}

void LEDArray::write(uint16_t* w, uint16_t l, uint16_t wt, bool reverse) {
	uint16_t j;
	if (reverse) {
		j = l;
		while (j) {
			j--;
			set(w[j]);
			delayMicroseconds(wt);
		}
	} else {
		for (j = 0; j < l; j++) {
			set(w[j]);
			delayMicroseconds(wt);
		}
	}
	set(0);
}

void LEDArray::print(uint16_t v) {
	Serial.print('.');
	for (int8_t y = 15; y >= 0; y--) {
		if (v & (1 << y))
			Serial.print("X");
		else
			Serial.print(" ");
	}
	Serial.println();
}

void LEDArray::print(uint16_t* w, uint16_t l) {
	for (uint16_t i = 0; i < l; i++) {
		print(w[i]);
	}
}
