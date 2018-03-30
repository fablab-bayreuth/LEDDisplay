
//Helper Function for now2text
void i2str(uint16_t i, unsigned char* b) {
  if (i < 10) {
    b[0] = '0';
    b++;
  }
  itoa(i, b, 10);
}

DateTime dt;
char text[] = "HH:MI:SS -- DD.MM.YYYY  ";

//gets time from RTC and formats it into the text array
void now2text(void) {
  dt = RTC.now();
  //Format the time-string
  i2str(dt.hour(), text);
  text[2] = ':';
  i2str(dt.minute(), text + 3);
  text[5] = ':';
  i2str(dt.second(), text + 6);
  text[8] = ' ';
  i2str(dt.day(), text + 12);
  text[14] = '.';
  i2str(dt.month(), text + 15);
  text[17] = '.';
  i2str(dt.year(), text + 18);

}

