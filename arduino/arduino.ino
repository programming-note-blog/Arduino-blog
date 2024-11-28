#include "Arduino.h"

#include "serial.h"
#include "pin_control.h"

// ボタンが押されたときの処理(仮)
void onButtonPress() {
  Serial.println("Button Pressed!");
}

void setup()
{
  SerialInit();

  // ボタン用ピンの初期化（7番ピンにコールバックを登録）
  PinControlButtonInit(7, onButtonPress);
}

void loop()
{
  SerialRead();
  PinControlButtonRead();
}

