#ifndef _LEDDISPLAY_H_
#define _LEDDISPLAY_H_


#if !defined(__INT_MAX__) || (__INT_MAX__ > 0xFFFF)
 #define pgm_read_pointer(addr) ((void *)pgm_read_dword(addr))
#else
 #define pgm_read_pointer(addr) ((void *)pgm_read_word(addr))
#endif
#include <Arduino.h>
#include <Sleep.h>


#include <gfxfont.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansOblique9pt7b.h>
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include <Fonts/FreeSerifBoldItalic9pt7b.h>


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


/*
This is the LEDDisplay class. It has a big buffer 16x280 Bits
the run-method flushes the buffer to the leds

the add-methods add text or bitmap to the buffer

there are some functions for running text and bitmap
*/
#define PIXELCOUNT 280

#define MODE_OVERWRITE 0
#define MODE_ADD 1
#define MODE_SUBSTRACT 2


#define CLOCKWISE 0
#define ANTICLOCKWISE 1

#define RUNNING_DISPLAY 0
#define FIXED_DISPLAY 1

class LEDDisplay : public LEDArray {
  private: 
    uint16_t buffer[PIXELCOUNT];
    char cbuffer[12];
    uint16_t current_pos;
    uint16_t pixel_count;
    GFXfont* gfxFont;
    unsigned long wait;
    unsigned long last_rot;
    unsigned long rot_count;
    bool rot_count_save;
    uint8_t direction;
    uint8_t mode;

 //Variables for running Text+Bitmap    
    uint8_t _shift;
    uint16_t _shift_wait;
    uint16_t _last_shift;
    bool _is_done;
    int offset;
    uint16_t last_run_pos;
   
  public:
    /*
     * Constuctor for LEDDisplay
     */
    LEDDisplay();
    LEDDisplay(const GFXfont *f);

    /*
     * init Display:
     * set PINs as output, read rot_count from EEPROM
     */
    void init(void);

    /*
     * Set font pointer
     */
    void setFont(const GFXfont *f);

    /*
     * Set configuration: this has only to be called, if you want to change the default values
     */
    void setConf(uint8_t mode=RUNNING_DISPLAY, uint16_t width=PIXELCOUNT, uint8_t dir=ANTICLOCKWISE);

    /*
     * Set/Get Cursor
     */
    void setCursor(uint16_t p);
    uint16_t getCursor(void);

    /*
     * Clears the display buffer
     */
    void clear(void);
    void clear(uint16_t c);

    /*
     * Call to calculate the rotation speed and the wait time
     */
    void setSpeed(void);

    /*
     * Calculate last Frames/s value
     * This has to be called directly before setSpeed()
     */
    float getFramesPerSecond();

   /*
     * Give the last rotation time im µs
     */
    unsigned long getLastRotation();

    /*
     * Methods for getting, reading and saving rotation counts to EEPROM
     */
    unsigned long getRotationCount();
    void saveRotationCount();
    void readRotationCount();

    /*
     * Prints out the buffer content with the calculated frequency
     */
    void run(void);

    /*
     * Debug method for printing to Serial
     */
    void print(void);

    /*
     * Add-Methodes
     *
     * All methods add content to the display. After adding the cursor is incremented
     *
     */
    uint8_t add(char c, uint8_t mode=MODE_OVERWRITE);
    void add(char* c, uint8_t mode=MODE_OVERWRITE);
    void add(uint16_t w, uint8_t mode=MODE_OVERWRITE);
    void add(uint16_t* w, uint16_t l, uint8_t mode=MODE_OVERWRITE);
    void addBitmap(uint8_t* bm, uint16_t l, uint8_t mode=MODE_OVERWRITE);
    void addBitmapPROGMEM(uint8_t* bm, uint16_t l, uint8_t mode=MODE_OVERWRITE);
    void addFloat(float f, int8_t width=0, uint8_t prec=2, uint8_t mode=MODE_OVERWRITE);
    void addInteger(long l, uint8_t mode=MODE_OVERWRITE);


    /*
     * Running Display Methods
     */

    /*
     * Init Running Display: set shift (px) and shift_wait (ms)
     */
    void initRunning(uint8_t shift, uint16_t shift_wait);

    /*
     * returns true, when content shifing is done
     */
    bool isDoneRunning(void);

    /*
     * Methods for fetching and adding conent in a continuous mode
     */
    void runningText(const char* text);
    void runningTextPROGMEM(const char* text);
    void runningBitmapPROGMEM(const uint8_t* bitmap, uint16_t bitmap_length);
    
    void sleep(bool clock_on=false);

};


#endif
