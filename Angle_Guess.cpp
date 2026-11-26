#include "puzzles.h"

//Declare variables
static int targetAngle = 0;
static int displayedAngle = 0;
static int triesLeft = 5;
static int prevButtonState = LOW;
static String lastHint = "Turn & press btn";
static bool puzzleComplete = false;

//new round
static void startNewRound() {
  targetAngle = random(0, 271); //new random target angle
  triesLeft = 5; //reset tries
  lastHint = "Turn & press btn";
  lcd.clear();
  lcd.setRGB(40, 90, 200);
}

//accounts for human error when pressing the button (i.e. we can't take our finger off the button very quickly and the computer might register it as 2 button clicks)
static void waitForButtonPress() {
  while (digitalRead(PIN_BUTTON) == HIGH) delay(10); // Wait for any current press to finish
  while (digitalRead(PIN_BUTTON) == LOW) delay(10); // Wait for a new button press
  while (digitalRead(PIN_BUTTON) == HIGH) delay(10); //// Wait for release so it only triggers once per press
}

//function for all initial instructions
static void showInstructions() {
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

static void begin_() {
  //pins
  pinMode(PIN_ROTARY, INPUT);
  pinMode(PIN_BUTTON, INPUT);

  //introductory page
  lcd.begin(16, 2);
  lcd.setRGB(255, 255, 255);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Angle Guess");
  lcd.setCursor(0, 1);
  lcd.print("Puzzle");
  delay(1200);

  //show instructions and begin round
  showInstructions();
  startNewRound();
}

static bool update_ () {

  //read angle and button positions
  int currentAngle = analogRead(PIN_ROTARY);
  int buttonState = digitalRead(PIN_BUTTON);
  int angleRange = map(currentAngle, 0, 1023, 0, 270); //angle from 0-270 degrees

  // Display current angle and remaining tries
  lcd.setCursor(0, 0);
  lcd.print("A:");
  lcd.print(angleRange);
  lcd.print(" Tries:");
  lcd.print(triesLeft);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print(lastHint);
  
  //clears last hint (higher or lower) and stores new hint in blank variable
  for (int i = lastHint.length(); i < 16; i++) {
    lcd.print(" ");
  }

  if (buttonState == HIGH && prevButtonState == LOW) { //checks if button was pressed and not held
    delay(40); 
    if (digitalRead(PIN_BUTTON) == HIGH) { //verifies button is pressed
      int diff = targetAngle - angleRange; //calculate how close angle is to target angle

      //PUZZLE SOLVED
      if (abs(diff) <= 5) { //gives buffer zone so that puzzle isn't impossible
        lcd.clear();
        lcd.setRGB(0, 0, 255);
        lcd.setCursor(0, 0);
        lcd.print("Puzzle Solved!");
        lcd.setCursor(0, 1);
        lcd.print("Target: ");
        lcd.print(targetAngle);
        puzzleComplete = true;
      }

      triesLeft--; ////if not within target range, one less guess left

      if (triesLeft <= 0) { //PUZZLE FAILED
        lcd.clear();
        lcd.setRGB(210, 45, 45);
        lcd.setCursor(0, 0);
        lcd.print("Out of tries!");
        lcd.setCursor(0, 1);
        lcd.print("Target was ");
        lcd.print(targetAngle);

        startNewRound();
      } else { //Prompts user to choose a value higher or lower based on their last guess
        if (angleRange < targetAngle) {
          lastHint = "Higher!         ";
        } else {
          lastHint = "Lower!          ";
        }
        lcd.setRGB(220, 160, 40);
      }
    }
  }

  prevButtonState = buttonState;
  delay(50);
  return puzzleComplete;
}

Puzzle Angle_Guess = { begin_, update_};
