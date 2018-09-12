//BuzzWire_With_Text
//This is the code for an advanced version of the game buzz wire with conditional LCD output and piezo song upon completion.
//Avi Kazen 08-2018
//https://www.instructables.com/id/Buzz-Wire-Scavenger-Hunt-Clue/
//Piezo "Happy Birthday" taken from https://create.arduino.cc/projecthub/trduunze/piezo-happy-birthday-7ea362


// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int startPin = A0;
const int endPin = A2;
const int failPin = A1;
const int redLED = 9;
const int greenLED = 8;
bool failFlag = false;
String prompt1="Happy Birthday" ;
String prompt2="Jamie!!" ;
String prompt3="Here is your";
String prompt4="final challenge." ;
      
long now=0;
int stage = 4;

//for piezo
const int speakerPin = 7;
int length = 28; // the number of notes
char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
int beats[] = { 2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8,8, 16, 1, 2,2,8,8,8,8,16, 1,2,2,8,8,8,16 };
int tempo = 150;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  // Print a message to the LCD.
  now=millis();
  displayText();
  delay(5010);
  lcd.clear();
  displayText();
  delay(5000);
  lcd.clear();
}

void loop() {
  displayText();

  if(analogRead(startPin)>200){
     digitalWrite(redLED,LOW);
     digitalWrite(greenLED,HIGH);
     failFlag = false;
     stage=0;
   }
   else if(analogRead(failPin)>200){
     digitalWrite(redLED,HIGH);
     digitalWrite(greenLED,LOW);
     failFlag=true;
     stage=2;
   }
   else if(analogRead(endPin)>200){
     if (failFlag==false){
       digitalWrite(redLED,HIGH);
       digitalWrite(greenLED,HIGH);
       stage=3;
       displayText();
       happyBirthday();
     }
     else {
          stage=5;
     }
   }
   else{
     //stage=1;
   }
}

void happyBirthday(){
    for (int i = 0; i < length; i++) {
       if (notes[i] == ' ') {
         delay(beats[i] * tempo); // rest
     }
     else {
       playNote(notes[i], beats[i] * tempo);
     }
     // pause between notes
     delay(tempo);
     displayText();
    }
}

void displayText(){
  switch (stage) {
    case 0:    // touching start washer
      prompt1="Get the ring to " ;
      prompt2="the other washer" ;
      prompt3="Without touching" ;
      prompt4="the copper wire!" ;
      break;
    case 1:    // on the way
      prompt1="  Good Luck!    " ;
      prompt2="                " ;
      prompt3="  Don't mess up!" ;
      prompt4="                " ;
      break;
    case 2:    // touched the wire
      prompt1="Awwwwwwwwwwwwwww" ;
      prompt2="bummerrrrrrrrrrr" ;
      prompt3="Try again from  " ;
      prompt4="the start!      " ;
      break;
    case 3:    // winner
      prompt1="Wahoo!! Congrats" ;
      prompt2="Now go look....." ;
      prompt3="In the          " ;
      prompt4="dishwasher!     " ;
      break;
    case 4:    // startup
      prompt1="Happy Birthday" ;
      prompt2="Jamie!!" ;
      prompt3="Here is your";
      prompt4="final challenge." ;
      break;
    case 5:    // cheater!
      prompt1="Cheater!!       " ;
      prompt2="                " ;
      prompt3="Start over!     ";
      prompt4="                " ;
      break;
  }

  if (millis()-now < 5000){
    lcd.setCursor(0, 0);
    lcd.print(prompt1);
    lcd.setCursor(0, 1);
    lcd.print(prompt2);
  }
  else if (millis()-now < 10000){
    lcd.setCursor(0, 0);
    lcd.print(prompt3);
    lcd.setCursor(0, 1);
    lcd.print(prompt4);
  }
  else now=millis();
}

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
     digitalWrite(speakerPin, HIGH);
     delayMicroseconds(tone);
     digitalWrite(speakerPin, LOW);
     delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',
                   'c', 'd', 'e', 'f', 'g', 'a', 'b',
                   'x', 'y' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014,
                   956,  834,  765,  593,  468,  346,  224,
                   655 , 715 };
  int SPEE = 5;
  // play the tone corresponding to the note name
  for (int i = 0; i < 17; i++) {
     if (names[i] == note) {
      int newduration = duration/SPEE;
       playTone(tones[i], newduration);
     }
  }
}


