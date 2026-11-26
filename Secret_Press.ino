// Secret Press Code puzzle
// press the button the secret number of times, then wait 2 seconds
// if the count is right, the led will turn on and the lcd turns green
// otherwise the lcd turns red and tells you if you pressed too few or too many
// Grove - Button connect to D3
// Grove - LCD RGB Backlight connect to I2C

const int PIN_BUTTON   = 3;                      // pin of button
const int secretCount = 9;                      // the secret number of presses

unsigned long lastRelease = 0;                  // time the button was last released

void setup()
{
    lcd.begin(16, 2);                           // set up the LCD's number of columns and rows
    lcd.setRGB(0, 0, 255);                      // blue backlight while waiting

    pinMode(PIN_BUTTON, INPUT);                  // set button INPUT

    lcd.print("Secret code?");
    lcd.setCursor(0, 1);
    lcd.print("Press the button");
}

void loop()
{
    if(digitalRead(PIN_BUTTON))                  // when button is pressed
    {
        pressCount++;                           // add one press

        lcd.setRGB(0, 0, 255);
        lcd.clear();
        lcd.print("Presses: ");
        lcd.print(pressCount);

        delay(50);
        while(digitalRead(PIN_BUTTON));          // wait until button is released
        delay(50);

        lastRelease = millis();                 // start the 2 second timer from release
    }

    // 2 seconds after the last release, check the answer
    if(pressCount > 0 && millis() - lastRelease > 2000)
    {
        lcd.clear();

        if(pressCount == secretCount)
        {
            puzzleCount++;
            lcd.setRGB(0, 255, 0);              // green backlight
            lcd.print("Unlocked!");
            delay(3000);
        }
        else
        {
            lcd.setRGB(255, 0, 0);              // red backlight
            if(pressCount < secretCount)
            {
                lcd.print("Too few!");
            }
            else
            {
                lcd.print("Too many!");
            }
            lcd.setCursor(0, 1);
            lcd.print("Try again");
            delay(2000);
        }

        pressCount = 0;                         // start over

        lcd.setRGB(0, 0, 255);
        lcd.clear();
        lcd.print("Secret code?");
        lcd.setCursor(0, 1);
        lcd.print("Press the button");
    }
}
