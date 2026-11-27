const int pinTemp = A0;    // Temperature sensor in A0
const int pinDial = A1;     // Rotary angle sensor in A1
const int pinBuzzer = 8;   // Buzzer pin in D8
const int pinButton = 3;

const int B = 4275;
const long R0 = 100000;

double readTemperatureC() {
  int rawTemp = analogRead(pinTemp);
  if (rawTemp <= 0) {
    return -273.15; // Avoid division by zero if disconnected
  }
  double R = R0 * (1023.0 / rawTemp - 1.0);
  return 1.0 / (log((double)R / R0) / B + 1.0 / 298.15) - 273.15;
}

void setup() {
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinDial, INPUT);
  pinMode(pinButton, INPUT);

  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setRGB(255, 255, 255);

  randomSeed(analogRead(A2));
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Temp Puzzle     ");
  lcd.setCursor(0, 1);
  lcd.print("Press to start  ");

  if (digitalRead(pinButton)) {
    bool puzzleSolved = false;

    // Lock in target range when the puzzle begins
    float randomMinTemp = random(23, 28);
    float randomMaxTemp = randomMinTemp + random(2, 6);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Make temp within");
    lcd.setCursor(0, 1);
    lcd.print("range");
    delay(5000);
    lcd.clear();

    // Stay on this screen and keep sampling until solved
    while (!puzzleSolved) {
      double temperatureC = readTemperatureC();

      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.print(temperatureC, 1);
      lcd.print("C ");

      lcd.setCursor(0, 1);
      lcd.print("Range:");
      lcd.print(randomMinTemp, 0);
      lcd.print("-");
      lcd.print(randomMaxTemp, 0);
      lcd.print("C       ");

      if (temperatureC >= randomMinTemp && temperatureC <= randomMaxTemp) {
        puzzleSolved = true;

        tone(pinBuzzer, 2000, 50);
        delay(80);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Puzzle Completed!");

        // Victory melody
        tone(pinBuzzer, 1000);
        delay(200);
        tone(pinBuzzer, 1500);
        delay(200);
        noTone(pinBuzzer);
        delay(2000);
      } else {
        // Short warning click while waiting for temp to reach range
        tone(pinBuzzer, 400);
        delay(50);
        noTone(pinBuzzer);
        delay(450);
      }
    }
  }
  delay(5000);
}
