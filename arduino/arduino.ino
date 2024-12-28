#include "Arduino.h"

#include "serial.h"
#include "pin_control.h"
#include "button_control.h"
#include "motor_control.h"
#include "led_control.h"

// ボタンが押されたときの処理(仮)
void onButtonPress() {
  Serial.println("Button Pressed!");

  LEDOneShot(PIN_BUZZER, 100);
}

void setup()
{
  SerialInit();
  ButtonInit(PIN_BUTTON, onButtonPress);
  MotorControlInit();
  LEDBlink(PIN_LED, 500); // 500ms間隔で点滅

  LEDPattern(PIN_BUZZER); // ピピ
}

void loop()
{
  SerialRead();
  ButtonRead();
  LEDUpdate();
}

