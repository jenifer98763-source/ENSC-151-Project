#include <Wire.h>
#include "rgb_lcd.h"
rgb_lcd lcd;

const int pinDial = A0;
const int pinBuzzer = 8;
const int pinButton = 3;

int targetAngle = 0;
int displayedAngle = 0;
int triesLeft = 5;
int puzzleCount = 0;
int prevButtonState = LOW;
String lastHint = "Turn & press btn";

void startNewRound() {
  targetAngle = random(0, 271);
  triesLeft = 5;
  lastHint = "Turn & press btn";
  lcd.clear();
  lcd.setRGB(40, 90, 200);
}

//accounts for human error when pressing the button (i.e. we can't take our finger off the button very quickly and the computer might register it as 2 button clicks)
void waitForButtonPress() {
  // Wait for any current press to finish
  while (digitalRead(pinButton) == HIGH) {
    delay(10);
  }
  delay(20);

  // Wait for a new button press
  while (digitalRead(pinButton) == LOW) {
    delay(10);
  }
  delay(40); 

  // Wait for release so it only triggers once per press
  while (digitalRead(pinButton) == HIGH) {
    delay(10);
  }
  delay(20);
}

void showInstructions() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pick 0-270 deg  ");
  lcd.setCursor(0, 1);
  lcd.print("Press btn->next ");
  waitForButtonPress();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LCD gives hint: ");
  lcd.setCursor(0, 1);
  lcd.print("Higher or Lower ");
  waitForButtonPress();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("5 tries per rnd ");
  lcd.setCursor(0, 1);
  lcd.print("Press btn->start");
  waitForButtonPress();
}

void setup() {
  pinMode(pinDial, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinButton, INPUT);

  Serial.begin(9600);
  randomSeed(analogRead(A2));

  lcd.begin(16, 2);
  lcd.setRGB(255, 255, 255);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle Guess");
  lcd.setCursor(0, 1);
  lcd.print("Puzzle");
  delay(1200);
  
  showInstructions();
  startNewRound();
}

void loop() {
  int currentAngle = analogRead(pinDial);
  int buttonState = digitalRead(pinButton);
  int angleRange = map(currentAngle, 0, 1023, 0, 270);

  // Current angle and remaining tries
  lcd.setCursor(0, 0);
  lcd.print("A:");
  lcd.print(angleRange);
  lcd.print(" Tries:");
  lcd.print(triesLeft);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print(lastHint);
  
  //clears lasthint letters and stores new hint in blank variable
  for (int i = lastHint.length(); i < 16; i++) {
    lcd.print(" ");
  }

  // Detect new button press
  if (buttonState == HIGH && prevButtonState == LOW) {
    delay(40); 
    if (digitalRead(pinButton) == HIGH) {
      int diff = targetAngle - angleRange;

      if (abs(diff) <= 5) { //gives buffer zone so that puzzle isn't impossible
        puzzleCount++;
        lcd.clear();
        lcd.setRGB(40, 200, 100);
        lcd.setCursor(0, 0);
        lcd.print("Puzzle Solved!");
        lcd.setCursor(0, 1);
        lcd.print("Target: ");
        lcd.print(targetAngle);

        tone(pinBuzzer, 1000, 180);
        delay(200);
        tone(pinBuzzer, 1500, 250);
        delay(1800);

        return;
      }

      //one less guess left
      triesLeft--;

      if (triesLeft <= 0) {
        lcd.clear();
        lcd.setRGB(210, 45, 45);
        lcd.setCursor(0, 0);
        lcd.print("Out of tries!");
        lcd.setCursor(0, 1);
        lcd.print("Target was ");
        lcd.print(targetAngle);

        tone(pinBuzzer, 260, 250);
        delay(270);
        tone(pinBuzzer, 180, 400);
        delay(1800);

        startNewRound();
      } else {
        if (angleRange < targetAngle) {
          lastHint = "Higher!         ";
          tone(pinBuzzer, 700, 120);
        } else {
          lastHint = "Lower!          ";
          tone(pinBuzzer, 500, 120);
        }
        lcd.setRGB(220, 160, 40);
      }
    }
  }

  prevButtonState = buttonState;
  delay(50);
}
