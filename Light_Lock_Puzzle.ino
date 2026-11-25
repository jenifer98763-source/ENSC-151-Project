/*
  Light Lock - Two-Stage Light Puzzle

  Description:
    An Arduino puzzle where the player must complete two light-based
    challenges in order. The LCD shows when the puzzle is solved.

    Stage 1: Cover the light sensor until the reading drops below 200
             (on the sensor's 0-1023 scale), and hold it for 4 seconds.
    Stage 2: Shine a flashlight on the sensor until the reading rises above
             700, and hold it for 4 seconds. This stage has a 10-second
             time limit; if time runs out, the puzzle resets to Stage 1.

    On completing both stages, the LCD turns green and shows
    "Puzzle solved!".

  How it works:
    - A Grove RGB LCD shows instructions, a progress bar while holding,
      and a countdown during Stage 2. The backlight color indicates state
      (red = waiting, amber = holding, green = success).
    - Light readings are printed to the Serial Monitor so DARK_THRESHOLD
      and LIGHT_THRESHOLD can be adjusted if the room is brighter or
      dimmer than expected.
    - There is no reset button. To play again after the puzzle has been
      solved, power-cycle the Arduino (or press its reset button).

  Hardware:
    - Light sensor on A0
    - Grove RGB LCD (16x2, I2C)

  Libraries: Wire, rgb_lcd
*/

#include <Wire.h>
#include "rgb_lcd.h"

const int LIGHT_PIN = A0;

const int DARK_THRESHOLD  = 200;   // stage 1: reading must be at or below this
const int LIGHT_THRESHOLD = 700;   // stage 2: reading must be at or above this

const unsigned long COVER_HOLD_MS  = 4000;
const unsigned long LIGHT_HOLD_MS  = 4000;
const unsigned long STAGE2_TIME_MS = 10000;  // time allowed to finish stage 2

rgb_lcd lcd;

int stage = 1;                       // 1 = cover, 2 = flashlight, 3 = done
unsigned long holdStart = 0;
unsigned long lastPrint = 0;
unsigned long stage2Start = 0;

enum Screen { NONE, WAIT, HOLD };
Screen shown = NONE;

void setColor(int r, int g, int b) { lcd.setRGB(r, g, b); }

void printLine(int row, String text) {
  while (text.length() < 16) text += ' ';
  lcd.setCursor(0, row);
  lcd.print(text.substring(0, 16));
}

void completeStage() {
  if (stage == 1) {
    setColor(0, 255, 0);
    printLine(0, "Stage 1 clear!");
    printLine(1, "Now... light!");
    delay(1500);
    stage = 2;
    holdStart = 0;
    shown = NONE;
    stage2Start = millis();          // start the stage 2 clock
  } else {
    stage = 3;
    setColor(0, 255, 0);
    printLine(0, "Puzzle solved!");
    printLine(1, "You escaped!");
  }
}

void timeUp() {
  setColor(255, 0, 0);
  printLine(0, "Time's up!");
  printLine(1, "Start over...");
  delay(2000);
  stage = 1;
  holdStart = 0;
  shown = NONE;
}

void showCountdown(int secs) {
  lcd.setCursor(14, 0);
  if (secs < 10) lcd.print(' ');
  lcd.print(secs);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  // Puzzle solved: nothing more to do until the Arduino is power-cycled
  if (stage == 3) return;

  int light = analogRead(LIGHT_PIN);
  unsigned long now = millis();

  // Print readings twice a second so you can tune the thresholds
  if (now - lastPrint > 500) {
    Serial.print("Stage "); Serial.print(stage);
    Serial.print("  Light: "); Serial.println(light);
    lastPrint = now;
  }

  // Stage 2 countdown
  int secsLeft = 0;
  if (stage == 2) {
    long remaining = (long)STAGE2_TIME_MS - (long)(now - stage2Start);
    if (remaining <= 0 && holdStart == 0) {   // don't cut off someone mid-hold
      timeUp();
      return;
    }
    if (remaining < 0) remaining = 0;
    secsLeft = (remaining + 999) / 1000;
  }

  bool conditionMet = (stage == 1) ? (light <= DARK_THRESHOLD)
                                   : (light >= LIGHT_THRESHOLD);
  unsigned long holdMs = (stage == 1) ? COVER_HOLD_MS : LIGHT_HOLD_MS;

  if (conditionMet) {
    if (holdStart == 0) holdStart = now;
    unsigned long held = now - holdStart;

    if (held >= holdMs) {
      completeStage();
      return;
    }

    setColor(255, 180, 0);
    if (shown != HOLD) {
      printLine(0, (stage == 1) ? "1/2 Hold it..." : "2/2 Hold it...");
      shown = HOLD;
    }
    int bars = map(held, 0, holdMs, 0, 16);
    String bar = "";
    for (int i = 0; i < 16; i++) bar += (i < bars) ? '#' : '-';
    printLine(1, bar);

  } else {
    holdStart = 0;

    if (shown != WAIT) {
      setColor(255, 0, 0);
      if (stage == 1) {
        printLine(0, "1/2 Too bright!");
        printLine(1, "Cover the sensor");
      } else {
        printLine(0, "2/2 Too dark!");
        printLine(1, "Need more light");
      }
      shown = WAIT;
    }
  }

  if (stage == 2) showCountdown(secsLeft);

  delay(50);
}
