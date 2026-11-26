// Secret Press Code puzzle
// press the button the secret number of times, then wait 2 seconds
// if the count is right, the lcd turns green and says Unlocked
// otherwise the lcd turns red and tells you if you pressed too few or too many
// Grove - Button connect to D3
// Grove - LCD RGB Backlight connect to I2C
#include "puzzles.h"

static bool puzzleComplete = false;
static int secretCount = 1; // the secret number of presses
static bool unlocked = false; // true once the puzzle is solved

static int pressCount = 0;              // how many times the button was pressed
static unsigned long lastRelease = 0;   // time the button was last released

static void showPrompt() {
    lcd.setRGB(0, 0, 255);              // blue backlight while waiting
    lcd.clear();
    lcd.print("Press the button");
    lcd.setCursor(0, 1);
    lcd.print("1-10 times");
}

static void begin_() {
    if (unlocked) {
        return;
    }

    secretCount = random(1, 11);
    
    pressCount = 0;
    puzzleComplete = false;

    lcd.begin(16, 2);
    pinMode(PIN_BUTTON, INPUT);
    showPrompt();
}

static bool update_() {
    // when button is pressed
    if (digitalRead(PIN_BUTTON)) {
        pressCount++;

        lcd.setRGB(0, 0, 255);
        lcd.clear();
        lcd.print("Presses: ");
        lcd.print(pressCount);

        delay(50);
        while (digitalRead(PIN_BUTTON));    // wait until button is released
        delay(50);

        lastRelease = millis();             // start the 2 second timer from release
    }

    // 2 seconds after the last release, check the answer
    if (pressCount > 0 && millis() - lastRelease > 2000) {
        lcd.clear();

        if (pressCount == secretCount) {
            unlocked = true;
            lcd.setRGB(0, 255, 0);          // green backlight
            lcd.print("Unlocked!");
            delay(3000);
            puzzleComplete = true;
            return puzzleComplete;          // skip the reset screen
        }

        lcd.setRGB(255, 0, 0);              // red backlight
        lcd.print(pressCount < secretCount ? "Too few!" : "Too many!");
        lcd.setCursor(0, 1);
        lcd.print("Try again");
        delay(2000);

        pressCount = 0;                     // start over
        showPrompt();
    }
    return puzzleComplete;
}

Puzzle Secret_Press = { begin_, update_ };
