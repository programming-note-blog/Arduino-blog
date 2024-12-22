#include <Arduino.h>
#include "pin_control.h"

unsigned short PinControlSetMode(unsigned short pin, unsigned short mode) {
  if (mode != INPUT && mode != INPUT_PULLUP && mode != OUTPUT) {
    return 1; // Error: Invalid mode
  }
  pinMode(pin, (PinMode)mode);
  return 0; // Success
}

unsigned short PinControlDigitalWrite(unsigned short pin, unsigned short value) {
  if (value != LOW && value != HIGH) {
    return 1; // Error
  }
  pinMode(pin, OUTPUT);
  digitalWrite(pin, (PinStatus)value);
  return 0; // Success
}

unsigned short PinControlDigitalRead(unsigned short pin, short* value) {
  if (value == nullptr) {
    return 1; // Error
  }
  pinMode(pin, INPUT);
  *value = digitalRead(pin);
  return 0; // Success
}

unsigned short PinControlAnalogWrite(unsigned short pin, unsigned short value) {
  analogWrite(pin, value);
  return 0; // Success
}

unsigned short PinControlAnalogRead(unsigned short pin, short* value) {
  if (value == nullptr) {
    return 1; // Error
  }
  *value = analogRead(pin);
  return 0; // Success
}
