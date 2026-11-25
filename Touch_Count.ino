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
rgb_lcd lcd; // I2C Socket

// Define variables
bool wasTouched = false;
int buzzCount = 0;
int touchCount = 0;
bool puzzleComplete = false;

void setup() {
    // Pins setup
    pinMode(pinTouch, INPUT);
    pinMode(pinBuzzer, OUTPUT);

    // LCD
    lcd.begin(16, 2);
    lcd.setRGB(0, 0, 255); // Blue backlight
    lcd.print("Touch the sensor with the same amount of buzzes as the buzzer");

    // Randomize buzzer beeps
    randomSeed(analogRead(A0)); // random seed once, using an unconnected analog pin
    buzzCount = random(1,16) // 1 to 15
    for (i = 0; i < buzzCount; i++) {
    digitalWrite(pinBuzzer, HIGH);
    delay(500);
    }

    lcd.clear();
    lcd.print(touchCount);
}

void loop() {
    // Read touch sensor
    bool touched = (digitalRead(pinTouch) == HIGH);

    // Run only on first loop of new touch
    if (touched && !wasTouched) {
        touchCount++;
        // LCD update
        lcd.clear();
        lcd.print(touchCount);
        if (touchCount == buzzCount) {
            lcd.clear();
            lcd.print("Moving to next puzzle!");
            delay(1000);
            puzzleComplete = true;
        }
    }

    // Prevent from counting while holding
    wasTouched = touched; // update at the end of every loop
}
