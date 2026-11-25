// ENSC 151 Project - Touch Count puzzle
/** Description:
 * Buzzer beeps X times → user must touch the sensor X times
 * Must be done within a certain time window
 * Does not consider how long pauses are between touches, as long as they are done for correct amount
 * Display instructions on LCD
 */

#include <Wire.h>
#include "rgb_lcd.h"

// Define pins
const int pinTouch = 2; // D2
const int pinBuzzer = 4; // D4
const int pinButton = 3; // D3
rgb_lcd lcd; // I2C Socket

// Define variables
bool wasTouched = false;
int buzzCount = 0;
int touchCount = 0;
bool puzzleComplete = false;

void buzzer(int count);

void setup() {
    // Pins setup
    pinMode(pinTouch, INPUT);
    pinMode(pinBuzzer, OUTPUT);

    // LCD
    lcd.begin(16, 2);
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
    randomSeed(analogRead(A0)); // random seed once, using an unconnected analog pin
    buzzCount = random(1,16); // 1 to 15
    buzzer(buzzCount);

    // Display touch count
    lcd.clear();
    lcd.print("Touches: ");
    lcd.setCursor(9, 0);
    lcd.print(touchCount);
}

void buzzer(int count) {
  for (int i = 0; i < count; i++) {
      digitalWrite(pinBuzzer, HIGH);
      delay(200);
      digitalWrite(pinBuzzer, LOW);
      delay(500);
    }
}

void loop() {
    // Read touch sensor
    bool touched = (digitalRead(pinTouch) == HIGH);

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
    bool pressed = (digitalRead(pinButton) == HIGH);
    if (pressed) {
      // Check if counts match up
      if (touchCount == buzzCount) {
            // Complete puzzle if counts match up
            lcd.clear();
            lcd.print("Puzzle complete!");
            delay(1000);
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
}
