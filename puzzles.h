// include/puzzles.h
#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <rgb_lcd.h>
#include <Servo.h>
#include "pins.h"
#include <string>

// Puzzle type: every puzzle provides these two functions
struct Puzzle {
  void (*begin)();    // reset state, show clue on LCD
  bool (*update)();   // called every loop; returns true when solved
};

// Shared hardware: defined ONCE in ENSC-151-Project.ino, used everywhere
extern rgb_lcd lcd;
extern Servo groveServo;

// The puzzles: each is defined in its own .cpp file
extern Puzzle Touch_Count;
extern Puzzle Clapping_Count;
extern Puzzle Dial_Range;
extern Puzzle Angle_Guess;
extern Puzzle Secret_Press;
extern Puzzle Light_Lock_Puzzle;

// Shared helper functions (prototypes only)
void beepSuccess();                     // short "solved" beep
void showError(const char* message);    // message on the LCD
