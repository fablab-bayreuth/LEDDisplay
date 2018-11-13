
//Helper Function for now2text
void i2str(uint16_t i, unsigned char* b) {
  if (i < 10) {
    b[0] = '0';
    b++;
  }
  itoa(i, b, 10);
}

DateTime dt;
char text[] = "DD/MM/YYYY HH.MI:SS AM";

//gets time from RTC and formats it into the text array
void now2text(void) {
  dt = RTC.now();
  i2str(dt.day(), text);
  text[2] = '/';
  i2str(dt.month(), text + 3 );
  text[5] = '/';
  i2str(dt.year(), text + 6 );
  text[10] = ' ';

  //Format the time-string
  uint8_t h=dt.hour();
  if(h>12) h-=12;
  
  i2str(h, text+11);
  text[13] = '.';
  i2str(dt.minute(), text + 14);
  text[16] = ':';
  i2str(dt.second(), text + 17);
  text[19] = ' ';
  if(dt.hour()>=12) text[20]='P';
  else text[20]='A';

}

