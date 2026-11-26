// ENSC 151 Project - Touch Count puzzle
/** Description:
 * Buzzer beeps X times → user must touch the sensor X times
 * Must be done within a certain time window
 * Does not consider how long pauses are between touches, as long as they are done for correct amount
 * Display instructions on LCD
 */

#include "puzzles.h"

// Define variables
static bool wasTouched = false;
static int buzzCount = 0;
static int touchCount = 0;
static bool puzzleComplete = false;

static void buzzer(int count) {
  for (int i = 0; i < count; i++) {
      digitalWrite(PIN_BUZZER, HIGH);
      delay(200);
      digitalWrite(PIN_BUZZER, LOW);
      delay(500);
    }
}

static void begin_() {
  // Pins setup
  pinMode(PIN_TOUCH, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setRGB(0, 0, 255); // Blue backlight
  lcd.print("Touch sensor the");
  lcd.setCursor(0, 1);
  lcd.print("number of buzzes.");
  delay(2000);
  lcd.clear();
  lcd.print("Press button");
  lcd.setCursor(0, 1);
  lcd.print("to confirm.");
  delay(2000);

  // Randomize buzzer beeps
  randomSeed(analogRead(PIN_SOUND) + micros()); // random seed once using a mixed signal
  buzzCount = random(1,16); // 1 to 15
  buzzer(buzzCount);

  // Display touch count
  lcd.clear();
  lcd.print("Touches: ");
  lcd.setCursor(9, 0);
  lcd.print(touchCount);
}
static bool update_() {
  // Read touch sensor
  bool touched = (digitalRead(PIN_TOUCH) == HIGH);

  // Run only on first loop of new touch
  if (touched && !wasTouched) {
      touchCount++;
      // LCD update
      lcd.clear();
      lcd.print("Touches: ");
      lcd.setCursor(9, 0);
      lcd.print(touchCount);
  }

  // Prevent from counting while holding
  wasTouched = touched; // update at the end of every loop

  // Read button presses
  bool pressed = (digitalRead(PIN_BUTTON) == HIGH);
  if (pressed) {
    // Check if counts match up
    if (touchCount == buzzCount) {
          // Complete puzzle if counts match up
          lcd.clear();
          lcd.print("Puzzle complete!");
          delay(3000);
          puzzleComplete = true;
      } else {
        // Otherwise, try buzzes again
        lcd.clear();
        lcd.print("Wrong, try again!");
        touchCount = 0;
        delay(1000);
        buzzer(buzzCount);

        // Display touch count
        lcd.clear();
        lcd.print("Touches: ");
        lcd.setCursor(9, 0);
        lcd.print(touchCount);
      }
  }

  return puzzleComplete;
}

Puzzle Touch_Count = { begin_, update_ };