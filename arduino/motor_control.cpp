#include "motor_control.h"
#include "pin_control.h"

#define PWM_RESOLUTION 255 // Default PWM resolution

static unsigned short rightMotorPins[2] = {5, 6}; // Pins for the right motor
static unsigned short leftMotorPins[2] = {9, 10}; // Pins for the left motor

unsigned short MotorControlInit() {
  for (unsigned short i = 0; i < 2; i++) {
    PinControlAnalogWrite(rightMotorPins[i], 0); // Initialize with 0 duty cycle
    PinControlAnalogWrite(leftMotorPins[i], 0); // Initialize with 0 duty cycle
  }
  return 0; // Success
}

unsigned short MotorControlSetRightMotorSpeed(short speed) {
  if (speed > PWM_RESOLUTION || speed < -PWM_RESOLUTION) {
    return 1; // Error: Speed out of range
  }

  if (speed >= 0) {
    PinControlAnalogWrite(rightMotorPins[0], speed);
    PinControlAnalogWrite(rightMotorPins[1], 0);
  } else {
    PinControlAnalogWrite(rightMotorPins[0], 0);
    PinControlAnalogWrite(rightMotorPins[1], -speed);
  }

  return 0; // Success
}

unsigned short MotorControlSetLeftMotorSpeed(short speed) {
  if (speed > PWM_RESOLUTION || speed < -PWM_RESOLUTION) {
    return 1; // Error: Speed out of range
  }

  if (speed >= 0) {
    PinControlAnalogWrite(leftMotorPins[0], speed);
    PinControlAnalogWrite(leftMotorPins[1], 0);
  } else {
    PinControlAnalogWrite(leftMotorPins[0], 0);
    PinControlAnalogWrite(leftMotorPins[1], -speed);
  }

  return 0; // Success
}

unsigned short MotorControlStopRightMotor() {
  PinControlAnalogWrite(rightMotorPins[0], 0);
  PinControlAnalogWrite(rightMotorPins[1], 0);
  return 0; // Success
}

unsigned short MotorControlStopLeftMotor() {
  PinControlAnalogWrite(leftMotorPins[0], 0);
  PinControlAnalogWrite(leftMotorPins[1], 0);
  return 0; // Success
}
