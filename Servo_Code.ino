#include <Wire.h>
#include "rgb_lcd.h"
#include <Servo.h>
rgb_lcd lcd;

int pinServo = 3;
int pinBuzzer = 8;

Servo groveServo;

void setup() {
 
  groveServo.attach(pinServo);
  pinMode(pinBuzzer, OUTPUT);

}

void loop() {
  
  groveServo.write(0);
  delay(10000);

  //if() { //If all the things are completed then it will open

    groveServo.write(90);
    lcd.clear();
    lcd.print("Congratulations!");

    double D5 = 622.25;
    double A5 = 932.33;

    tone(pinBuzzer, D5);
    delay(1000);
    tone(pinBuzzer, D5);
    delay(500);
    tone(pinBuzzer, D5);
    delay(500);
    tone(pinBuzzer, A5);
    delay(500);
    noTone(pinBuzzer);


    int i;

    for(i=0;i<5;i++) {

      lcd.setRGB(255,0,0);
      delay(10000);
      lcd.setRGB(0,255,0);
      delay(10000);
      lcd.setRGB(0,0,255);
      delay(1000);

    }

  //}


}
