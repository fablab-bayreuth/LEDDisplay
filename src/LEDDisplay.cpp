#include "LEDDisplay.h"
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

LEDDisplay::LEDDisplay() {
	gfxFont = NULL;
	wait = 500;
	mode = RUNNING_DISPLAY;
	direction = ANTICLOCKWISE;
	pixel_count = PIXELCOUNT;
}

LEDDisplay::LEDDisplay(const GFXfont *f) {
	gfxFont = (GFXfont *) f;
	wait = 500;
	mode = RUNNING_DISPLAY;
	direction = ANTICLOCKWISE;
	pixel_count = PIXELCOUNT;
}

void LEDDisplay::setFont(const GFXfont *f) {
	gfxFont = (GFXfont *) f;
}

void LEDDisplay::setConf(uint8_t m, uint16_t pc, uint8_t d) {
	mode = m;
	direction = d;
	if (pc > PIXELCOUNT)
		pc = PIXELCOUNT;

	pixel_count = pc;
}

void LEDDisplay::add(uint16_t w, uint8_t mode) {
	switch (mode) {
	case MODE_OVERWRITE:
		buffer[current_pos] = w;
		break;
	case MODE_ADD:
		buffer[current_pos] |= w;
		break;
	case MODE_SUBSTRACT:
		buffer[current_pos] &= (!w);
		break;
	}
	current_pos++;
	if (current_pos >= pixel_count)
		current_pos = 0;
	if (offset < 0 && current_pos == last_run_pos)
		_is_done = true;
}

uint8_t LEDDisplay::add(char c, uint8_t mode) {
	if (gfxFont == NULL)
		return 0;
	if (c < (uint8_t) pgm_read_byte(&gfxFont->first))
		return 0;
	if (c > (uint8_t) pgm_read_byte(&gfxFont->last))
		return 0;

	c -= (uint8_t) pgm_read_byte(&gfxFont->first);
	GFXglyph *glyph = &(((GFXglyph *) pgm_read_pointer(&gfxFont->glyph))[c]);
	uint8_t *bitmap = (uint8_t *) pgm_read_pointer(&gfxFont->bitmap);

	uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
	uint8_t w = pgm_read_byte(&glyph->width), h = pgm_read_byte(&glyph->height);
	int8_t xo = pgm_read_byte(&glyph->xOffset), yo = pgm_read_byte(
			&glyph->yOffset);
	uint8_t width = pgm_read_byte(&glyph->xAdvance) + 1;
	uint8_t xx, yy, bits = 0, bit = 0;

	//        Serial.println(c);
	//        Serial.println(w);
	//        Serial.println(h);
	//        Serial.println(xo);
	//        Serial.println(yo);
	//        Serial.println(width);
	uint16_t char_buffer[25];
	for (xx = 0; xx < width; xx++) {
		char_buffer[xx] = 0;
	}

	for (yy = 0; yy < h; yy++) {
		for (xx = 0; xx < w; xx++) {
			if (!(bit++ & 7)) {
				bits = pgm_read_byte(&bitmap[bo++]);
			}
			if (bits & 0x80) {
				//       char_buffer[xx + xo] |= 0x8000 >> (yy + 11 + yo);
				char_buffer[xx + xo] |= 0x1 << (yy + 11 + yo);
			}
			bits <<= 1;
		}
	}
	for (xx = 0; xx < width; xx++) {
		add(char_buffer[xx], mode);
	}
	return width;
}

void LEDDisplay::add(char* c, uint8_t mode) {
	uint8_t i = 0;
	while (c[i]) {
		add(c[i], mode);
		i++;
	}
}

void LEDDisplay::add(uint16_t* w, uint16_t l, uint8_t mode) {
	for (uint16_t i = 0; i < l; i++) {
		add(w[i], mode);
	}
}

void LEDDisplay::addBitmap(uint8_t* bm, uint16_t l, uint8_t mode) {
	uint16_t v;
	for (uint16_t i = 0; i < l / 2; i++) {
		*(((uint8_t*) &v)) = bm[i];
		*(((uint8_t*) &v) + 1) = bm[i + l / 2];
		add(v, mode);
	}
}

void LEDDisplay::addBitmapPROGMEM(uint8_t* bm, uint16_t l, uint8_t mode) {
	uint16_t v;
	for (uint16_t i = 0; i < l / 2; i++) {
		*(((uint8_t*) &v)) = pgm_read_byte(i);
		*(((uint8_t*) &v) + 1) = pgm_read_byte(i + l / 2);
		add(v, mode);
	}
}

void LEDDisplay::setSpeed(void) {
	uint16_t px=pixel_count*5/4;
	wait = (micros() - last_rot) / px - 16;
	if (wait * px > 200000L)
		wait = 200000L / px;
	last_rot = micros();
}

unsigned long LEDDisplay::getLastRotation(void) {
	return last_rot;
}

void LEDDisplay::setCursor(uint16_t p) {
	if (p >= pixel_count)
		p = 0;
	current_pos = p;
}

void LEDDisplay::clear(void) {
	for (int i = 0; i < pixel_count; i++) {
		buffer[i] = 0;
	}
	current_pos = 0;
}

void LEDDisplay::clear(uint16_t c) {
	for (int i = 0; i < c; i++) {
		add((uint16_t) 0);
	}
}

void LEDDisplay::run(void) {
	if (mode == RUNNING_DISPLAY) {
		if (direction == ANTICLOCKWISE) {
			write(buffer, current_pos, wait, true);
			write(buffer + current_pos, pixel_count - current_pos, wait, true);
		} else {
			write(buffer + current_pos, pixel_count - current_pos, wait, false);
			write(buffer, current_pos, wait, false);
		}
	} else {
		write(buffer, pixel_count, wait, direction == ANTICLOCKWISE);
	}
}

void LEDDisplay::print(void) {
	LEDArray::print(buffer, pixel_count);
}

void LEDDisplay::initRunning(uint8_t shift, uint16_t shift_wait) {
	offset = 0;
	_shift = shift;
	_shift_wait = shift_wait;
	_last_shift = millis();
	_is_done = false;
	clear();
}

void LEDDisplay::runningText(const char* text) {
	_is_done = false;
	if (((uint16_t) millis() - _last_shift) > _shift_wait) {
		for (uint8_t i = 0; i < _shift; i++) {
			if (offset >= 0) {
				if (text[offset])
					add(text[offset]);
				else {
					offset = -100;
					last_run_pos = current_pos;
				}
			} else
				add(' ');
			offset++;

			if (_is_done) {
				offset = 0;
				break;
			}
		}
		_last_shift = millis();
	}

}

void LEDDisplay::runningTextPROGMEM(const char* text) {
	_is_done = false;
	if (((uint16_t) millis() - _last_shift) > _shift_wait) {
		for (uint8_t i = 0; i < _shift; i++) {
			if (offset >= 0) {
				char c = pgm_read_byte(text + offset);
				if (c)
					add(c);
				else {
					offset = -100;
					last_run_pos = current_pos;
				}
			} else
				add(' ');
			offset++;

			if (_is_done) {
				offset = 0;
				break;
			}
		}
		_last_shift = millis();
	}
}

void LEDDisplay::runningBitmapPROGMEM(const uint8_t* bitmap,
		uint16_t bitmap_length) {
	_is_done = false;
	if (((uint16_t) millis() - _last_shift) > _shift_wait) {
		for (uint8_t i = 0; i < _shift; i++) {
			uint16_t v = 0;
			if (offset >= 0 && offset < bitmap_length / 2) {
				*(((uint8_t*) &v)) = pgm_read_byte(bitmap + offset);
				*(((uint8_t*) &v) + 1) = pgm_read_byte(
						bitmap + offset + bitmap_length / 2);
			} else if (offset == bitmap_length / 2) {
				offset = -pixel_count;
				last_run_pos = current_pos;
			}
			add(v);
			offset++;

			if (offset == 0) {
				_is_done = true;
				break;
			}
		}
		_last_shift = millis();
	}
}

bool LEDDisplay::isDoneRunning(void) {
	return _is_done;
}

