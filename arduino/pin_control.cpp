#include <Arduino.h>
#include "pin_control.h"

unsigned short PinControlDigitalWrite(unsigned short pin, unsigned short value) {
  if (value != LOW && value != HIGH) {
    return 1; // エラー
  }
  pinMode(pin, OUTPUT);
  digitalWrite(pin, (PinStatus)value);
  return 0; // 成功
}

unsigned short PinControlDigitalRead(unsigned short pin, short* value) {
  if (value == nullptr) {
    return 1; // エラー
  }
  pinMode(pin, INPUT);
  *value = digitalRead(pin);
  return 0; // 成功
}

unsigned short PinControlAnalogWrite(unsigned short pin, unsigned short value) {
  analogWrite(pin, value);
  return 0; // 成功
}

unsigned short PinControlAnalogRead(unsigned short pin, short* value) {
  if (value == nullptr) {
    return 1; // エラー
  }
  *value = analogRead(pin);
  return 0; // 成功
}
