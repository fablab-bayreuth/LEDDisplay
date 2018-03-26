//>>> The latest version of this code can be found at https://github.com/jcw/ !!

// A library for handling real-time clocks, dates, etc.
// 2010-02-04 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: RTClib.cpp 7763 2011-12-11 01:28:16Z jcw $
// A library for handling real-time clocks, dates, etc.
// 2010-02-04 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <avr/pgmspace.h>
#include "RTClib.h"
#include <Arduino.h>



//PROGMEM prog_uint8_t daysInMonth[]  = {31,28,31,30,31,30,31,31,30,31,30,31};

static uint8_t daysInMonth[] =
		{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// number of days since 2000/01/01, valid for 2001..2099
static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d) {
	if (y >= 2000)
		y -= 2000;
	uint16_t days = d;
	for (uint8_t i = 1; i < m; ++i)
		days += daysInMonth[(i - 1)];
//    days += pgm_read_byte(daysInMonth + i - 1);
	if (m > 2 && y % 4 == 0)
		++days;
	return days + 365 * y + (y + 3) / 4 - 1;
}

static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s) {
	return ((days * 24L + h) * 60 + m) * 60 + s;
}

////////////////////////////////////////////////////////////////////////////////
// DateTime implementation - ignores time zones and DST changes
// NOTE: also ignores leap seconds, see http://en.wikipedia.org/wiki/Leap_second

DateTime::DateTime(long t) {
	ss = t % 60;
	t /= 60;
	mm = t % 60;
	t /= 60;
	hh = t % 24;
	uint16_t days = t / 24;
	uint8_t leap;
	for (yOff = 0;; ++yOff) {
		leap = yOff % 4 == 0;
		if (days < (uint16_t) 365 + leap)
			break;
		days -= 365 + leap;
	}
	for (m = 1;; ++m) {
		uint8_t daysPerMonth = daysInMonth[(m - 1)]; //pgm_read_byte(daysInMonth + m - 1);
		if (leap && m == 2)
			++daysPerMonth;
		if (days < daysPerMonth)
			break;
		days -= daysPerMonth;
	}
	d = days + 1;
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
		uint8_t min, uint8_t sec) {
	if (year >= 2000)
		year -= 2000;
	yOff = year;
	m = month;
	d = day;
	hh = hour;
	mm = min;
	ss = sec;
}

static uint8_t conv2d(const char* p) {
	uint8_t v = 0;
	if ('0' <= *p && *p <= '9')
		v = *p - '0';
	return 10 * v + *++p - '0';
}

// A convenient constructor for using "the compiler's time":
// DateTime now (__DATE__, __TIME__);
// NOTE: using PSTR would further reduce the RAM footprint
DateTime::DateTime(const char* date, const char* time) {
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	yOff = conv2d(date + 9);
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	switch (date[0]) {
	case 'J':
		m = date[1] == 'a' ? 1 : m = date[2] == 'n' ? 6 : 7;
		break;
	case 'F':
		m = 2;
		break;
	case 'A':
		m = date[2] == 'r' ? 4 : 8;
		break;
	case 'M':
		m = date[2] == 'r' ? 3 : 5;
		break;
	case 'S':
		m = 9;
		break;
	case 'O':
		m = 10;
		break;
	case 'N':
		m = 11;
		break;
	case 'D':
		m = 12;
		break;
	}

	d = conv2d(date + 4);
	hh = conv2d(time);
	mm = conv2d(time + 3);
	ss = conv2d(time + 6);
}

uint8_t DateTime::dayOfWeek() const {
	uint16_t day = get() / SECONDS_PER_DAY;
	return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

long DateTime::get() const {
	uint16_t days = date2days(yOff, m, d);
	return time2long(days, hh, mm, ss);
}

uint16_t DateTime::year() const { return 2000 + yOff; }
uint8_t DateTime::month() const { return m; }
uint8_t DateTime::day() const { return d; }
uint8_t DateTime::hour() const { return hh; }
uint8_t DateTime::minute() const { return mm; }
uint8_t DateTime::second() const { return ss; }


////////////////////////////////////////////////////////////////////////////////
// RTC_Timer2 implementation

void RTC_Timer2::adjust(const DateTime& dt) {
	noInterrupts();
    _seconds = dt.get();
    interrupts();
}

DateTime RTC_Timer2::now() {
	noInterrupts();
    unsigned long s=_seconds;
    interrupts();
    return s;
}

////////////////////////////////////////////////////////////////////////////////
// RTC_Millis implementation

long RTC_Millis::offset = 0;

void RTC_Millis::adjust(const DateTime& dt) {
    offset = dt.get() - millis() / 1000;
}

DateTime RTC_Millis::now() {
    return offset + millis() / 1000;
}

