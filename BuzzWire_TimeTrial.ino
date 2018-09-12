//BuzzWire Game with LCD OUTPUT
//Avi Kazen 09-2018

// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h> //for keeping high score data

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int startPin = A0;
int endPin = A2;
const int failPin = A1;
const int redLED = 9;
const int greenLED = 8;
bool failFlag = true;
bool winFlag = false;

long highscoreRaw = 0;
int highscoreMin = 99;
int highscoreSec = 59;
long yourscoreRaw = 0;
int yourscoreMin = 0;
int yourscoreSec = 0;
int scoreAddress = 0;   //Location to store score in EEPROM


String prompt1="" ;
String prompt2="" ;
String prompt3="";
String prompt4="" ;
      
long now=0;
long counterStart=0;

//for piezo
const int speakerPin = 7;
int length = 2; // the number of notes
char winNotes[] = "dx";
char loseNotes[] = "AG";
int beats[] = { 2, 2};
int tempo = 150;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  
  //long resetScore=5999000;
  //EEPROM.put(scoreAddress, resetScore);//reset high score to 99 min 59 sec
  //delay(2);
  
  EEPROM.get(scoreAddress, highscoreRaw);//get last high score
  delay(2);
  highscoreSec = (highscoreRaw/1000)%60;
  highscoreMin = ((highscoreRaw/1000) - highscoreSec)/60;
}

void loop() {
  if(analogRead(startPin)>200){ //ring touching the start washer
     digitalWrite(redLED,LOW);
     digitalWrite(greenLED,HIGH);
     winFlag = false;
     counterStart=millis();
     lcd.setCursor(0, 0);
     lcd.print("High score:");
     lcd.setCursor(0, 1);
     lcd.print(highscoreMin);
     lcd.print(" min ");
     lcd.print(highscoreSec);
     lcd.print(" sec  ");
   }
   
   else if(analogRead(failPin)>200){ //ring touching track
     digitalWrite(redLED,HIGH);
     digitalWrite(greenLED,LOW);
     lcd.setCursor(0, 0);
     lcd.print("Bummer.         ");
     lcd.setCursor(0, 1);
     lcd.print("Try again       ");
     if (failFlag==false){
      lose();
     }
     failFlag=true;
   }
   else if (failFlag==true){ //failed. Set next washer as the start washer and reset game
      if(analogRead(A2)>200){
        startPin = A2;
        endPin = A0;
        failFlag = false;
      }
      if(analogRead(A0)>200){
        startPin = A0;
        endPin = A2;
        failFlag = false;
      }
    }
   else if(analogRead(endPin)>200 && winFlag==false){
     if (failFlag==false){ //win!
       yourscoreRaw = millis() - counterStart;
       yourscoreSec = (yourscoreRaw/1000)%60;
       yourscoreMin = ((yourscoreRaw/1000) - highscoreSec)/60;
       winFlag=true;
       digitalWrite(redLED,HIGH);
       digitalWrite(greenLED,HIGH);
       lcd.setCursor(0, 0);
       lcd.print("Your score:");
       lcd.setCursor(0, 1);
       lcd.print(yourscoreMin);
       lcd.print(" min ");
       lcd.print(yourscoreSec);
       lcd.print(" sec  ");
       win();
       
     }
     else { //cheater!
       lcd.setCursor(0, 0);
       lcd.print("Cheater!            ");
       lcd.setCursor(0, 1);
       lcd.print("Start over!     ");
     }
   }
   
   else if ((failFlag==false)&&(winFlag==false)){ //game has started{
       lcd.setCursor(0, 0);
       lcd.print((millis()-counterStart)/1000);
       lcd.print("            ");
       lcd.setCursor(0, 1);
       lcd.print("seconds           ");
   }

}


void win(){
    for (int i = 0; i < length; i++) {
       if (winNotes[i] == ' ') {
         delay(beats[i] * tempo); // rest
     }
     else {
       playNote(winNotes[i], beats[i] * tempo);
     }
     // pause between notes
     delay(tempo);
    }
    delay(2000);
    winFlag=false;
    counterStart=millis();
    if(startPin == A0){
      startPin = A2;
      endPin = A0;}
    else{
      startPin = A0;
      endPin = A2;
      }
    if (yourscoreRaw<highscoreRaw){
      newHighScore();
    }
}

void newHighScore(){
       lcd.setCursor(0, 0);
       lcd.print("Congrats! New   ");
       lcd.setCursor(0, 1);
       lcd.print("high score!     ");
       highscoreRaw=yourscoreRaw;
       highscoreSec = (highscoreRaw/1000)%60;
       highscoreMin = ((highscoreRaw/1000) - highscoreSec)/60;
       EEPROM.put(scoreAddress, highscoreRaw);
       delay(2000);
}

void lose(){
    for (int i = 0; i < length; i++) {
       if (loseNotes[i] == ' ') {
         delay(beats[i] * tempo); // rest
     }
     else {
       playNote(loseNotes[i], beats[i] * tempo);
     }
     // pause between notes
     delay(tempo);
    }
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

