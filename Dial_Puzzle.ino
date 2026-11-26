#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

// Sensor pins
const int pinDial = A0;
const int pinBuzzer = 5;
const int pinButton = 3;


int randomMinAngle = 300;
int randomMaxAngle = 700;
const int totalStages = 3;
const unsigned long holdMS = 1500; //keep angle sensor on value for 1.5s to verify
const unsigned long timeLimitMS = 10000; 
const int zeroThreshold = 20; //accepts values close to 0 so code doesn't get stuck

// Start all counters and timers at initial positions
int stage = 1;
int puzzleCount = 0;
unsigned long holdStart = 0;
bool holding = false;
bool waitingForZero = false;
unsigned long outOfRangeStart = 0;

//checks if value is in range and sets inRange=true
bool inRange(int val, int low, int high) {
  return val >= low && val <= high;
}

//randomizes range
void newRange() {
  randomMinAngle = random(150, 500);
  int span = random(200, 400); //makes range difference between 200 and 400 degrees 
  randomMaxAngle = min(1023, randomMinAngle + span); //picks smaller number so angle can't go above 1023
}

//displays values screen 
void valueScreen(int angle, bool isInRange, unsigned long progress, unsigned long remainingSec) {
  lcd.setCursor(0, 0);
  lcd.print("S");
  lcd.print(stage);
  lcd.print(" Rng:");
  lcd.print(randomMinAngle);
  lcd.print("-");
  lcd.print(randomMaxAngle);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("A:");
  lcd.print(angle);
  lcd.print(" ");

  if (isInRange) { //in range = displays % progress
    lcd.print(progress);
    lcd.print("%      ");
  } else { //else = displays time
    lcd.print("Time ");
    lcd.print(remainingSec);
    lcd.print("s   ");
  }
}

//function to reset dial to 0 degrees to be used before starting each stage
void reset0(int angle) {
  lcd.setRGB(210, 150, 30);
  lcd.setCursor(0, 0);
  lcd.print("Reset dial to 0 ");
  lcd.setCursor(0, 1);
  lcd.print("Angle:");
  lcd.print(angle);
  lcd.print(" -> 0      ");
}

//Function to restart from stage 1 if angle is out of range
void resetPuzzle() {
  stage = 1;
  holding = false;
  waitingForZero = false;
  outOfRangeStart = 0;
  newRange();

  lcd.clear();
  lcd.setRGB(200, 40, 40);
  lcd.setCursor(0, 0);
  lcd.print("Out of range!   ");
  lcd.setCursor(0, 1);
  lcd.print("Restarting...   ");

  tone(pinBuzzer, 220, 350);
  delay(800);
  lcd.clear();
  lcd.setRGB(40, 90, 200);
}

//Function to delay button press to mitigate human error 
void waitForButtonPress() {
  while (digitalRead(pinButton) == HIGH) {
    delay(10);
  }
  delay(20);

  while (digitalRead(pinButton) == LOW) {
    delay(10);
  }
  delay(40);

  while (digitalRead(pinButton) == HIGH) {
    delay(10);
  }
  delay(20);
}

//Function to show initial instructions for puzzle
void showInstructions() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Turn dial to be");
  lcd.setCursor(0, 1);
  lcd.print("in range. cont->");
  waitForButtonPress();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hold for 1.5s");
  lcd.setCursor(0, 1);
  lcd.print("Solve 3 stages");
  waitForButtonPress();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press btn to");
  lcd.setCursor(0,1);
  lcd.print("begin.");
  waitForButtonPress();
}

//Function for setup
void setup() {
  Serial.begin(9600);
  pinMode(pinDial, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinButton, INPUT);

  randomSeed(analogRead(PIN_SOUND) + micros()); //random number generator
  newRange();

  lcd.begin(16, 2);
  lcd.setRGB(40, 90, 200);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dial Puzzle     ");
  waitForButtonPress();

  showInstructions();
  waitingForZero = true;
  lcd.clear();
}

void loop() {
  int angle = analogRead(pinDial);

  if (waitingForZero) {
    reset0(angle);
    if (angle <= zeroThreshold) {
      waitingForZero = false;
      outOfRangeStart = 0;
      holding = false;
      newRange();
      lcd.clear();
      lcd.setRGB(40, 90, 200);
    }
    delay(50);
    return;
  }

  if (inRange(angle, randomMinAngle, randomMaxAngle)) {
    outOfRangeStart = 0;

    if (!holding) {
      holding = true;
      holdStart = millis();
      tone(pinBuzzer, 880, 80);
    }

    unsigned long elapsed = millis() - holdStart; //calculate time elapsed when angle held
    unsigned long percentage = min(100UL, (elapsed * 100UL) / holdMS); //calculate percentage

    lcd.setRGB(40, 180, 90);
    valueScreen(angle, true, percentage, 0); 

    //dial at correct angle for >1.5s, trigger stage clear. Reset angle to 0.
    if (elapsed >= holdMS) {
      holding = false;
      outOfRangeStart = 0;
      tone(pinBuzzer, 1320, 180);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Stage ");
      lcd.print(stage);
      lcd.print(" Cleared!");
      lcd.setCursor(0, 1);
      lcd.print("Press btn to cont");
      waitForButtonPress();

      stage++;
      lcd.clear();

      if (stage <= totalStages) {
        waitingForZero = true;
      }
    }
  } else {
    holding = false;

    if (outOfRangeStart == 0) { //start time count when out of range
      outOfRangeStart = millis();
    }

    unsigned long outElapsed = millis() - outOfRangeStart; //calculate time elapsed
    if (outElapsed > timeLimitMS) { //time elapsed > 10 puzzle will reset
      resetPuzzle();
      return;
    }

    unsigned long remainingSec = (timeLimitMS - outElapsed + 999UL) / 1000UL; //calculate remaining time

    lcd.setRGB(40, 90, 200);
    valueScreen(angle, false, 0, remainingSec);
  }

  if (stage > totalStages) { //puzzle solved block
    puzzleCount++;
    lcd.clear();
    lcd.setRGB(40, 200, 140);
    lcd.setCursor(0, 0);
    lcd.print("Puzzle Solved!  ");
    lcd.setCursor(0, 1);
    lcd.print("Completed: ");
    lcd.print(puzzleCount);

    tone(pinBuzzer, 1000);
    delay(200);
    tone(pinBuzzer, 1500);
    delay(200);
    noTone(pinBuzzer);
    delay(2000);

    // Reset for next round
    stage = 1;
    holding = false;
    waitingForZero = false;
    outOfRangeStart = 0;
    newRange();
    lcd.clear();
    lcd.setRGB(40, 90, 200);
    return;
  }

  delay(50);
}
