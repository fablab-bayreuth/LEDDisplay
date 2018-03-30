/*
   Das Buch Genesis

   Beispiel für einen langen Text.




*/

#include <LEDDisplay.h>

//GFXfont* gfxFont = &FreeSerifBold9pt7b;
//GFXfont* gfxFont = &FreeSerif9pt7b;
//GFXfont* gfxFont = &FreeMonoOblique9pt7b;
GFXfont* gfxFont = &FreeMonoBoldOblique9pt7b;
//Create Instance of LEDArray
LEDDisplay led(gfxFont);


bool reverse = true;
int shift_speed = 50;
uint8_t shift = 1;

//The text to display. Text is put in PROGMEM (not in RAM).
//This allows us to have really big test strings ~ 24kb
const char text[] PROGMEM = "\
\
    Das Buch Genesis         \
\
Die Anfaenge: 1,1 - 11,9        \
Die Erschaffung der Welt: 1,1 - 2,4a  \
\
  Im Anfang schuf Gott Himmel und Erde;\
\
  die Erde aber war wuest und wirr, Finsternis lag ueber der Urflut und Gottes Geist schwebte ueber dem Wasser.\
\
  Gott sprach: Es werde Licht. Und es wurde Licht.\
\
  Gott sah, dass das Licht gut war. Gott schied das Licht von der Finsternis\
\
  und Gott nannte das Licht Tag und die Finsternis nannte er Nacht. Es wurde Abend und es wurde Morgen: erster Tag.\
\
  Dann sprach Gott: Ein Gewoelbe entstehe mitten im Wasser und scheide Wasser von Wasser.\
\
  Gott machte also das Gewoelbe und schied das Wasser unterhalb des Gewoelbes vom Wasser oberhalb des Gewoelbes. So geschah es\
\
  und Gott nannte das Gewoelbe Himmel. Es wurde Abend und es wurde Morgen: zweiter Tag.\
\
  Dann sprach Gott: Das Wasser unterhalb des Himmels sammle sich an einem Ort, damit das Trockene sichtbar werde. So geschah es.\
\
  Das Trockene nannte Gott Land und das angesammelte Wasser nannte er Meer. Gott sah, dass es gut war.\
\
  Dann sprach Gott: Das Land lasse junges Gruen wachsen, alle Arten von Pflanzen, die Samen tragen, und von Baeumen, die auf der Erde Fruechte bringen mit ihrem Samen darin. So geschah es.\
\
  Das Land brachte junges Gruen hervor, alle Arten von Pflanzen, die Samen tragen, alle Arten von Baeumen, die Fruechte bringen mit ihrem Samen darin. Gott sah, dass es gut war.\
\
  Es wurde Abend und es wurde Morgen: dritter Tag.\
\
  Dann sprach Gott: Lichter sollen am Himmelsgewoelbe sein, um Tag und Nacht zu scheiden. Sie sollen Zeichen sein und zur Bestimmung von Festzeiten, von Tagen und Jahren dienen;\
\
  sie sollen Lichter am Himmelsgewoelbe sein, die ueber die Erde hin leuchten. So geschah es.\
\
  Gott machte die beiden grossen Lichter, das groessere, das ueber den Tag herrscht, das kleinere, das ueber die Nacht herrscht, auch die Sterne.\
\
  Gott setzte die Lichter an das Himmelsgewoelbe, damit sie ueber die Erde hin leuchten,\
\
  ueber Tag und Nacht herrschen und das Licht von der Finsternis scheiden. Gott sah, dass es gut war.\
\
  Es wurde Abend und es wurde Morgen: vierter Tag.\
\
  Dann sprach Gott: Das Wasser wimmle von lebendigen Wesen und Voegel sollen ueber dem Land am Himmelsgewoelbe dahinfliegen.\
\
  Gott schuf alle Arten von grossen Seetieren und anderen Lebewesen, von denen das Wasser wimmelt, und alle Arten von gefiederten Voegeln. Gott sah, dass es gut war.\
\
  Gott segnete sie und sprach: Seid fruchtbar und vermehrt euch und bevoelkert das Wasser im Meer und die Voegel sollen sich auf dem Land vermehren.\
\
  Es wurde Abend und es wurde Morgen: fuenfter Tag.\
\
  Dann sprach Gott: Das Land bringe alle Arten von lebendigen Wesen hervor, von Vieh, von Kriechtieren und von Tieren des Feldes. So geschah es.\
\
  Gott machte alle Arten von Tieren des Feldes, alle Arten von Vieh und alle Arten von Kriechtieren auf dem Erdboden. Gott sah, dass es gut war.\
\
  Dann sprach Gott: Lasst uns Menschen machen als unser Abbild, uns aehnlich. Sie sollen herrschen ueber die Fische des Meeres, ueber die Voegel des Himmels, ueber das Vieh, ueber die ganze Erde und ueber alle Kriechtiere auf dem Land.\
\
  Gott schuf also den Menschen als sein Abbild; als Abbild Gottes schuf er ihn. Als Mann und Frau schuf er sie.\
\
  Gott segnete sie und Gott sprach zu ihnen: Seid fruchtbar und vermehrt euch, bevoelkert die Erde, unterwerft sie euch und herrscht ueber die Fische des Meeres, ueber die Voegel des Himmels und ueber alle Tiere, die sich auf dem Land regen.\
\
  Dann sprach Gott: Hiermit uebergebe ich euch alle Pflanzen auf der ganzen Erde, die Samen tragen, und alle Baeume mit samenhaltigen Fruechten. Euch sollen sie zur Nahrung dienen.\
\
  Allen Tieren des Feldes, allen Voegeln des Himmels und allem, was sich auf der Erde regt, was Lebensatem in sich hat, gebe ich alle gruenen Pflanzen zur Nahrung. So geschah es.\
\
  Gott sah alles an, was er gemacht hatte: Es war sehr gut. Es wurde Abend und es wurde Morgen: der sechste Tag.\
\
  So wurden Himmel und Erde vollendet und ihr ganzes Gefuege.\
\
  Am siebten Tag vollendete Gott das Werk, das er geschaffen hatte, und er ruhte am siebten Tag, nachdem er sein ganzes Werk vollbracht hatte.\
\
  Und Gott segnete den siebten Tag und erklaerte ihn fuer heilig; denn an ihm ruhte Gott, nachdem er das ganze Werk der Schoepfung vollendet hatte.\
\
  Das ist die Entstehungsgeschichte von Himmel und Erde, als sie erschaffen wurden.";


void setup(void) {
  led.init(); //Set Pins as OUTPUT
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), isr_int0, FALLING);
  led.initRunning(shift, shift_speed);
}

//Interrupt Routine
volatile uint8_t int0_flag;
void isr_int0(void) {
  int0_flag = 1;

}

void loop(void) {

  if (int0_flag) {
    //Wenn zwischendruch aufgehört wird zu drehen, mache wir einen
    //Neustart des Textes. Wer das nicht möchte, bitte auskommentieren :-)
    if (led.wokeupFromSleep()) {
      led.clear();
      led.initRunning(shift, shift_speed);

    }

    led.setSpeed();
    led.runningTextPROGMEM(text);
    led.run();
    int0_flag = 0;
  }
  led.sleep();
}


