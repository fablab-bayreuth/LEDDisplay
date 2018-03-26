//Modified RTC-Library
// S.Holzheu (holzheu@bayceer.uni-bayreuth.de)
//
//
// A library for handling real-time clocks, dates, etc.
// 2010-02-04 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: RTClib.h 7763 2011-12-11 01:28:16Z jcw $

// Simple general-purpose date/time class (no TZ / DST / leap second handling!)
#ifndef RTClib_H
#define RTClib_H

#include <Wire.h>
#define SECONDS_PER_DAY 86400L
class DateTime {
public:
    DateTime (long t =0);
    DateTime (uint16_t year, uint8_t month, uint8_t day,
                uint8_t hour =0, uint8_t min =0, uint8_t sec =0);
    DateTime (const char* date, const char* time);

    uint16_t year() const;
    uint8_t month() const;
    uint8_t day() const;
    uint8_t hour() const;
    uint8_t minute() const;
    uint8_t second() const;
    uint8_t dayOfWeek() const;

    // 32-bit times as seconds since 1/1/2000
    long get() const;

protected:
    uint8_t yOff, m, d, hh, mm, ss;
};

//Abstract RTC
class RTC {
public:
	virtual void begin()=0;
	virtual void adjust(const DateTime& dt)=0;
	virtual DateTime now()=0;
};






// RTC using the time2 clock, has to be initialized before use
// NOTE: You have to add a appropriate ISR to your code to update _seconds
class RTC_Timer2 : public RTC {
public:
    void begin() {}
    void adjust(const DateTime& dt);
    DateTime now();
    volatile long _seconds;
};

// RTC using the internal millis() clock, has to be initialized before use
// NOTE: this clock won't be correct once the millis() timer rolls over (>49d?)
class RTC_Millis : public RTC {
public:
    void begin() {}
    void adjust(const DateTime& dt);
    DateTime now();

protected:
    static long offset;
};

#endif

