#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int BUTTON_PIN = 4;   // change to your button's port
int difficulty = 0;         // 0 = Easy, 1 = Medium, 2 = Hard
String names[] = {"Easy", "Medium", "Hard"};

void setDifficultyColor() {
  if (difficulty == 0) {
    lcd.setRGB(0, 255, 0);
  } else if (difficulty == 1) {
      lcd.setRGB(255, 90, 0);
    } else {
      lcd.setRGB(255, 0, 0);
    }
}

void showDifficulty() {
  setDifficultyColor();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode: " + names[difficulty]);
  lcd.setCursor(0, 1);
  lcd.print("Tap:next Hold:OK");
}

void chooseDifficulty() {
  showDifficulty();
  while (true) {
    if (digitalRead(BUTTON_PIN) == HIGH) {
      unsigned long start = millis();
      while (digitalRead(BUTTON_PIN) == HIGH) {}   // wait for release

      if (millis() - start >= 1000) {
        return;                                    // held: confirm
      }
      difficulty = (difficulty + 1) % 3;           // tapped: next option
      showDifficulty();
      delay(50);                                   
    }
  }
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  lcd.begin(16, 2);
  lcd.setRGB(0, 150, 255);

  lcd.setCursor(3, 0);
  lcd.print("Welcome to");
  lcd.setCursor(2, 1);
  lcd.print("Mini Escape!");
  delay(3000);

  chooseDifficulty();

  lcd.clear();
  lcd.print("Selected:");
  lcd.setCursor(0, 1);
  lcd.print(names[difficulty]);
  delay(1500);
  lcd.clear();
}

void loop() {
  
}
