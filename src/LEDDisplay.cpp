#include "LEDDisplay.h"

#include <EEPROM.h>

//INT0 staff
volatile uint8_t LEDDisplay::int0_flag = 0;
volatile uint8_t LEDDisplay::int0_millis = 0;

inline void LEDDisplay::int0ISR(void){
	LEDDisplay::int0_flag=1;
	LEDDisplay::int0_millis=millis();
}


void LEDDisplay::begin() {
	LEDArray::init();
	readRotationCount();
	pinMode(2, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(2),LEDDisplay::int0ISR,FALLING);
}


LEDDisplay::LEDDisplay() {
	gfxFont = NULL;
	wait = 500;
	mode = FIFO_DISPLAY;
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
	wait = (wait * pixel_count) / pc;
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
	uint8_t width;
	bool umlaut = true;
	if (gfxFont == NULL)
		return 0;
	if ((uint8_t) c == 0xc3) {
		_utf8_c = 1;
		return 0;
	}
	if (_utf8_c) {
		//c3 a4 c3 b6 c3 bc c3 84 c3 96
		//c3 9c c3 9f

		switch ((uint8_t) c) {
		case 0xa4: //ä
			c = 'a';
			break;
		case 0xb6: //ö
			c = 'o';
			break;
		case 0xbc: //ü
			c = 'u';
			break;
		case 0x84: //Ä
			c = 'A';
			break;
		case 0x96: //Ö
			c = 'O';
			break;
		case 0x9c: //Ü
			c = 'U';
			break;
		case 0x9f: //ß
			c = 's';
			umlaut = false;
			break;
		default:
			c = '8';
			_utf8_c = 0;
		}
	}
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
	width = pgm_read_byte(&glyph->xAdvance) + 1;
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
	if (_utf8_c) {
		if (!umlaut) {
			char_buffer[xo + 1] |= 0xfff0 << (9 + yo);
			char_buffer[xo + 2] &= 0x000f << (9 + yo);
		} else {
			char_buffer[xo] |= 0x1 << (9 + yo);
			char_buffer[xo + 1] |= 0x1 << (9 + yo);
			char_buffer[xo + w - 2] |= 0x1 << (9 + yo);
			char_buffer[xo + w - 1] |= 0x1 << (9 + yo);
		}
		_utf8_c = 0;
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
		*(((uint8_t*) &v)) = pgm_read_byte(bm + i);
		*(((uint8_t*) &v) + 1) = pgm_read_byte(bm + i + l / 2);
		add(v, mode);
	}
}

void LEDDisplay::addFloat(float f, int8_t width, uint8_t prec, uint8_t mode) {
	dtostrf(f, width, prec, cbuffer);
	add(cbuffer, mode);
}

void LEDDisplay::addInteger(long l, uint8_t mode) {
	ltoa(l, cbuffer, 10);
	add(cbuffer, mode);
}

void LEDDisplay::setSpeed(void) {
	uint16_t px = pixel_count * 5 / 4;
	wait = (micros() - last_rot) / px / (1+_overspeed)  - 16;
	if (wait * px > 200000L)
		wait = 200000L / px;
	last_rot = micros();
	rot_count++;
	rot_count_save = true;

}

unsigned long LEDDisplay::getLastRotation(void) {
	return last_rot;
}

unsigned long LEDDisplay::getRotationCount(void) {
	return rot_count;
}

void LEDDisplay::readRotationCount(void) {
	uint8_t* p = (uint8_t*) &rot_count;
	for (uint8_t i = 0; i < 4; i++) {
		p[i] = EEPROM.read(i);
	}
	rot_count_save = false;
}

void LEDDisplay::saveRotationCount(void) {
	if (!rot_count_save)
		return;
	uint8_t* p = (uint8_t*) &rot_count;
	for (uint8_t i = 0; i < 4; i++) {
		EEPROM.update(i, p[i]);
	}
	rot_count_save = false;
}

float LEDDisplay::getFramesPerSecond(void) {
	return 1000000.0 / (micros() - last_rot);
}

void LEDDisplay::setCursor(uint16_t p) {
	if (p >= pixel_count)
		p = 0;
	current_pos = p;
}
uint16_t LEDDisplay::getCursor(void) {
	return current_pos;
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
	uint8_t start=millis();
	int0_millis=start;
	if (mode == FIFO_DISPLAY) {
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
	_overspeed=(int0_millis-start)>40;//There was a INT0 in between!
	int0_flag=0;

}

void LEDDisplay::print(void) {
	LEDArray::print(buffer, pixel_count);
}

void LEDDisplay::initRunning(uint16_t shift_wait) {
	offset = 0;
	_shift_wait = shift_wait;
	_last_shift = millis();
	_is_done = false;
	clear();
}

void LEDDisplay::initRunning(uint8_t shift, uint16_t shift_wait) {
	initRunning(shift_wait / shift);
}

void LEDDisplay::runningText(const char* text) {
	_is_done = false;

	while (((uint16_t) millis() - _last_shift) > _shift_wait) {
		if (offset >= 0) {
			if (text[offset]) {
				while (text[offset] && !add(text[offset])) {
					offset++;
				}
			} else {
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
		_last_shift += _shift_wait;
	}

}

void LEDDisplay::runningTextPROGMEM(const char* text) {
	_is_done = false;
	while (((uint16_t) millis() - _last_shift) > _shift_wait) {
		if (offset >= 0) {
			char c = pgm_read_byte(text + offset);

			if (c) {
				while (!add(c) && c) {
					offset++;
					c = pgm_read_byte(text + offset);
				}
			} else {
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
		_last_shift += _shift_wait;
	}
}

void LEDDisplay::runningBitmapPROGMEM(const uint8_t* bitmap,
		uint16_t bitmap_length) {
	_is_done = false;
	while (((uint16_t) millis() - _last_shift) > _shift_wait) {
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
		_last_shift += _shift_wait;
	}
}

bool LEDDisplay::isDoneRunning(void) {
	return _is_done;
}

void LEDDisplay::sleep(bool clock_on) {
	if ((micros() - last_rot) < 500000)
		return;
	//no INT for more than 0.5 sec
	saveRotationCount();
	if (clock_on)
		Sleep.sleep(TIMER2_ON, SLEEP_MODE_PWR_SAVE);
	else
		Sleep.sleep();

}

bool LEDDisplay::wokeupFromSleep(void) {
	return (!rot_count_save);
}

#include <math.h>
float LEDDisplay::getTemperature(void) {
	digitalWrite(13, HIGH);
	int adc = analogRead(A6);
	digitalWrite(13, LOW);
	float log_r = log(20000.0 / (1023.0 / adc - 1));
	return 440.61073 - 75.69303 * log_r + 4.20199 * log_r * log_r
			- 0.09586 * log_r * log_r * log_r;

}
