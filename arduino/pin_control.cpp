#include <Arduino.h>
#include "pin_control.h"

#define MAX_PIN_COUNT 22

// Array to store the mode of each pin
unsigned short pinModes[MAX_PIN_COUNT] = {
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT,
  OUTPUT
  };

unsigned short PinControlSetMode(unsigned short pin, unsigned short mode) {
  if (pin >= MAX_PIN_COUNT) {
    return 1; // Error: Pin out of range
  }
  if (mode != INPUT && mode != INPUT_PULLUP && mode != OUTPUT) {
    return 1; // Error: Invalid mode
  }
  pinMode(pin, (PinMode)mode);
  pinModes[pin] = mode; // Store the mode
  return 0; // Success
}

unsigned short PinControlDigitalWrite(unsigned short pin, unsigned short value) {
  if (pin >= MAX_PIN_COUNT) {
    return 1; // Error: Pin out of range
  }
  if (value != LOW && value != HIGH) {
    return 1; // Error: Invalid value
  }

  unsigned short originalMode = pinModes[pin]; // Save original mode
  if (originalMode != OUTPUT) {
    Serial.print("Warning: Changing pin ");
    Serial.print(pin);
    Serial.print(" mode from ");
    Serial.print(originalMode == INPUT ? "INPUT" : (originalMode == INPUT_PULLUP ? "INPUT_PULLUP" : "UNKNOWN"));
    Serial.println(" to OUTPUT");

    pinModes[pin] = OUTPUT;
  }

  pinMode(pin, OUTPUT);
  digitalWrite(pin, (PinStatus)value);

  return 0; // Success
}

unsigned short PinControlDigitalRead(unsigned short pin, short* value) {
  if (pin >= MAX_PIN_COUNT) {
    return 1; // Error: Pin out of range
  }
  if (value == nullptr) {
    return 1; // Error: Null pointer
  }

  unsigned short originalMode = pinModes[pin]; // Save original mode
  pinMode(pin, INPUT);
  *value = digitalRead(pin);
  pinMode(pin, (PinMode)originalMode); // Restore original mode

  return 0; // Success
}

unsigned short PinControlAnalogWrite(unsigned short pin, unsigned short value) {
  if (pin >= MAX_PIN_COUNT) {
    return 1; // Error: Pin out of range
  }
  analogWrite(pin, value);
  return 0; // Success
}

unsigned short PinControlAnalogRead(unsigned short pin) {
  if (pin >= MAX_PIN_COUNT) {
    return 1; // Error: Pin out of range
  }

  return analogRead(pin); // Success
}
