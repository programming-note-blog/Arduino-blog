#include "Arduino.h"

#include "serial.h"
#include "pin_control.h"
#include "button_control.h"
#include "motor_control.h"
#include "led_control.h"

#define LED_PIN 13

// ボタンが押されたときの処理(仮)
void onButtonPress() {
  Serial.println("Button Pressed!");
}

void setup()
{
  SerialInit();

  // ボタン用ピンの初期化（7番ピンにコールバックを登録）
  ButtonInit(7, onButtonPress);

  MotorControlInit();

  LEDInit(LED_PIN);       // LEDの初期化
  LEDBlink(LED_PIN, 500); // 500ms間隔で点滅
}

void loop()
{
  SerialRead();
  ButtonRead();
  LEDUpdate();
}

