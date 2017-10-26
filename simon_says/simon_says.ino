/*Simon Says game. Now with sound effects. 

Originaly made by Robert Spann
Code trimmed and sound effects added by digimike

Buttons are to be set on there designated pins without pull down resistors
and connected to ground rather then +5. 
*/
#include "pitches.h"

int speakerpin = 12;
int starttune[] = {NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_C4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4};
int duration2[] = {100, 200, 100, 200, 100, 400, 100, 100, 100, 100, 200, 100, 500};
int note[] = {NOTE_C4, NOTE_C4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5};
int duration[] = {100, 100, 100, 300, 100, 300};
int button[] = {2, 3, 4, 5}; //The four button input pins
int ledpin[] = {8, 9, 10, 11};  // LED pins
int turn = 0;  // turn counter
int buttonstate = 0;  // button state checker
int randomArray[100]; //Intentionally long to store up to 100 inputs (doubtful anyone will get this far)
int inputArray[100];  


void setup() 
{
  Serial.begin(9600);
  //speakerpin.begin(12); // speaker is on pin 12

  for(int x=0; x<4; x++)  // LED pins are outputs
  {
    pinMode(ledpin[x], OUTPUT);
  }
  
  for(int x=0; x<4; x++) 
  {
    pinMode(button[x], INPUT);  // button pins are inputs
    digitalWrite(button[x], HIGH);  // enable internal pullup; buttons start in high position; logic reversed
  }

  randomSeed(analogRead(0)); //Added to generate "more randomness" with the randomArray for the output function
  for (int thisNote = 0; thisNote < 13; thisNote ++) {
     // play the next note:
     tone(speakerpin, starttune[thisNote]);
     // hold the note:
     if (thisNote==0 || thisNote==2 || thisNote==4 || thisNote== 6)
     {
       digitalWrite(ledpin[0], HIGH);
     }
     if (thisNote==1 || thisNote==3 || thisNote==5 || thisNote== 7 || thisNote==9 || thisNote==11)
     {
       digitalWrite(ledpin[1], HIGH);
     }
     if (thisNote==8 || thisNote==12)
     {
       digitalWrite(ledpin[2], HIGH);
     }  
     if (thisNote==10)
     {   
       digitalWrite(ledpin[3], HIGH);
     }
     delay(duration2[thisNote]);
     // stop for the next note:
     noTone(speakerpin);
     writeAllLeds(LOW);
     delay(25);
    }
  delay(1000);
}
 
void loop() 
{   
  for (int y=0; y<=99; y++)
  {
    //function for generating the array to be matched by the player
    writeAllLeds(HIGH);  // set all LEDs to high
  
    for (int thisNote = 0; thisNote < 6; thisNote ++) {
     // play the next note:
     tone(speakerpin, note[thisNote]);
     // hold the note:
     delay(duration[thisNote]);
     // stop for the next note:
     noTone(speakerpin);
     delay(25);
    }
    
    writeAllLeds(LOW); // set all LEDs to low
    
    delay(1000);
  
    for (int y=turn; y <= turn; y++)
    { //Limited by the turn variable
      Serial.println(""); //Some serial output to follow along
      Serial.print("Turn: ");
      Serial.print(y);
      Serial.println("");
      randomArray[y] = random(1, 5); //Assigning a random number (1-4) to the randomArray[y], y being the turn count
      for (int x=0; x <= turn; x++)
      {
        Serial.print(randomArray[x]);
      
        for(int y=0; y<4; y++)
        {
      
          if (randomArray[x] == 1 && ledpin[y] == 8) 
          {  //if statements to display the stored values in the array
            flashLED(ledpin[y], NOTE_G3, 400, 100);
          }

          if (randomArray[x] == 2 && ledpin[y] == 9) 
          {
            flashLED(ledpin[y], NOTE_A3, 400, 100);
          }
  
          if (randomArray[x] == 3 && ledpin[y] == 10) 
          {
            flashLED(ledpin[y], NOTE_B3, 400, 100);
          }

          if (randomArray[x] == 4 && ledpin[y] == 11) 
          {
            flashLED(ledpin[y], NOTE_C4, 400, 100);
          }
        }
      }
    }
    input();
  }
}
 
void flashLED(int ledpin, int note, int note_length, int delay_time) { // flashes the led and plays the required tone
  digitalWrite(ledpin, HIGH);
  tone(speakerpin, note, 100);
  delay(note_length);
  digitalWrite(ledpin, LOW);
  delay(delay_time);
}
 
void input() { //Function for allowing user input and checking input against the generated array

  for (int x=0; x <= turn;)
  { //Statement controlled by turn count

    for(int y=0; y<4; y++)
    {
      
      buttonstate = digitalRead(button[y]);

      if (buttonstate == LOW)
      {
        switch (button[y]) {
          case 2:
            flashLED(ledpin[0], NOTE_G3, 200, 250);
            inputArray[x] = 1;
            Serial.print(" ");
            Serial.print(1);
            if (inputArray[x] != randomArray[x]) { //Checks value input by user and checks it against
              fail();                              //the value in the same spot on the generated array
            }                                      //The fail function is called if it does not match
            x++;
            break;
          case 3:
            flashLED(ledpin[1], NOTE_A3, 200, 250);
            inputArray[x] = 2;
            Serial.print(" ");
            Serial.print(2);
            if (inputArray[x] != randomArray[x]) {
              fail();
            }
            x++;
            break;
          case 4:
            flashLED(ledpin[2], NOTE_B3, 200, 250);
            inputArray[x] = 3;
            Serial.print(" ");
            Serial.print(3);
            if (inputArray[x] != randomArray[x]) {
              fail();
            }
            x++;
            break;
          case 5:
            flashLED(ledpin[3], NOTE_C4, 200, 250);
            inputArray[x] = 4;
            Serial.print(" ");
            Serial.print(4);
            if (inputArray[x] != randomArray[x]) 
            {
              fail();
            }
            x++;
            break;
        }
      }
    }
  }
  delay(500);
  turn++; //Increments the turn count, also the last action before starting the output function over again
}

void fail() { //Function used if the player fails to match the sequence
 
  for (int y=0; y<=2; y++)
  { //Flashes lights for failure
    
    writeAllLeds(HIGH);  // set all LEDs to high
    tone(speakerpin, NOTE_G3, 300);
    delay(200);
    writeAllLeds(LOW);  // set all LEDs to low
    tone(speakerpin, NOTE_C3, 300);
    delay(200);
  }
  delay(500);
  turn = -1; //Resets turn value so the game starts over without need for a reset button
}

void writeAllLeds(boolean state) { // sets all LEDS to high or low 
    digitalWrite(ledpin[0], state);
    digitalWrite(ledpin[1], state);
    digitalWrite(ledpin[2], state);
    digitalWrite(ledpin[3], state);
}


