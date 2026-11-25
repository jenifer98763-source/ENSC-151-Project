// ENSC 151 Project - Clapping Count puzzle
/** Description:
 * Similar to touch count
 * Buzzer beeps X times → user must clap their hands X times
 * Adjust sound sensor threshold for claps
 * Must have cooldown (time in between) for claps
 * Display instructions on LCD
 */

#include <Wire.h>
#include "rgb_lcd.h"

// Define pins
const int pinSound = A1; // A1
const int pinBuzzer = 4; // D4
const int pinButton = 3; // D3
rgb_lcd lcd; // I2C Socket

// Define variables
bool wasTouched = false;
int buzzCount = 0;
int clapCount = 0;
bool puzzleComplete = false;
int thresholdValue = 300; // sound sensor threshold value

void buzzer(int count);

void setup() {
  // Pins setup
  pinMode(pinSound, INPUT);
  pinMode(pinBuzzer, OUTPUT);

  // LCD
  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 255); // Blue backlight
  lcd.print("Clap the number");
  lcd.setCursor(0, 1);
  lcd.print("of buzzes, then");
  delay(2000);
  lcd.clear();
  lcd.print("press button");
  lcd.setCursor(0, 1);
  lcd.print("to confirm.");
  delay(2000);

  // Randomize buzzer beeps
  randomSeed(analogRead(A0)); // random seed once, using an unconnected analog pin
  buzzCount = random(1,16); // 1 to 15
  buzzer(buzzCount);

  // Display clap count
  lcd.clear();
  lcd.print("Claps: ");
  lcd.setCursor(7, 0);
  lcd.print(clapCount);
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
    // Read sound sensor
    int sensorValue = analogRead(pinSound);
    if (sensorValue > thresholdValue) {
        clapCount++;
        // LCD update
        lcd.clear();
        lcd.print("Claps: ");
        lcd.setCursor(7, 0);
        lcd.print(clapCount);
        delay(500);
    }

    // Read button presses
    bool pressed = (digitalRead(pinButton) == HIGH);
    if (pressed) {
      // Check if counts match up
      if (clapCount == buzzCount) {
            // Complete puzzle if counts match up
            lcd.clear();
            lcd.print("Puzzle complete!");
            delay(1000);
            puzzleComplete = true;
        } else {
          // Otherwise, try buzzes again
          lcd.clear();
          lcd.print("Wrong, try again!");
          clapCount = 0;
          delay(1000);
          buzzer(buzzCount);

          // Display touch count
          lcd.clear();
          lcd.print("Claps: ");
          lcd.setCursor(7, 0);
          lcd.print(clapCount);
        }
    }
}
