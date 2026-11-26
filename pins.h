// include/pins.h
#pragma once
#include <Arduino.h>

// Inputs
constexpr uint8_t PIN_TOUCH  = 4; // Touch sensor in D4
constexpr uint8_t PIN_BUTTON = 3; // Button in D3
constexpr uint8_t PIN_LIGHT  = A2; // Light sensor in A2
constexpr uint8_t PIN_SOUND  = A3; // Sound sensor in A3
constexpr uint8_t PIN_ROTARY = A1; // Rotary angle sensor in A1
constexpr uint8_t PIN_TEMP   = A0; // Temperature sensor in A0

// Outputs
constexpr uint8_t PIN_BUZZER = 8; // Buzzer in D8
constexpr uint8_t PIN_SERVO  = 2; // Servo in D2