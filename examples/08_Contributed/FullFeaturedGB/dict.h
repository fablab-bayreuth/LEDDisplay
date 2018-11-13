char dict_buffer[40];
int dict_offset=0;
const char dict[] PROGMEM = "German/English Excercise|\
eat|essen|I want to eat chocolate|Ich möchte Schokolade essen|\
drink|trinken|I like to drink beer|Ich trinke gerne Bier";

uint8_t dict_next_word(void){
  uint8_t i=0;
  char c;
  do{
    c=pgm_read_byte(dict+dict_offset);
    dict_buffer[i]=c;
    if(i<38) i++;
    dict_offset++;
  } while(c && c!='|');
  dict_buffer[i-1]=0;
  if(c=='|') return 1;
  else if(c==0){
    dict_offset=0;
    return 0;
  }
  else return 2;
}

