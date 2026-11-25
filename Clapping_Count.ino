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
rgb_lcd lcd; // I2C Socket

// Define variables
bool wasTouched = false;
int buzzCount = 0;
int clapCount = 0;
bool puzzleComplete = false;
int thresholdValue = 300; // sound sensor threshold value

void setup() {
  // Pins setup
  pinMode(pinSound, INPUT);
  pinMode(pinBuzzer, OUTPUT);

  // LCD
  lcd.begin(16, 2);
  lcd.setRGB(0, 0, 255); // Blue backlight
  lcd.print("Clap the same number of times as the buzzer");

  // Randomize buzzer beeps
  randomSeed(analogRead(A0)); // random seed once, using an unconnected analog pin
  buzzCount = random(1,16) // 1 to 15
  for (i = 0; i < buzzCount; i++) {
    digitalWrite(pinBuzzer, HIGH);
    delay(500);
  }

  // Display count
  lcd.clear();
  lcd.print(clapCount);
}

void loop() {
    // Read sound sensor
    int sensorValue = analogRead(pinSound);
    if (sensorValue > thresholdValue) {
        clapCount++;
        // LCD update
        lcd.clear();
        lcd.print(clapCount);
        delay(500);
        if (clapCount == buzzCount) {
            lcd.clear();
            lcd.print("Moving to next puzzle!");
            delay(1000);
            puzzleComplete = true;
        }
    }
}
