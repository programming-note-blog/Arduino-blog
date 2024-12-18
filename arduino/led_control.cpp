/* led_control.cpp */
#include <Arduino.h>
#include "led_control.h"
#include "pin_control.h" // ピン制御モジュールをインクルード

// LED管理用構造体
struct LEDState {
  bool isBlinking;                // 点滅中かどうか
  unsigned long lastToggleTime;   // 最後に点灯/消灯を切り替えた時間
  unsigned long blinkInterval;    // 点滅間隔（ミリ秒）
  bool currentState;              // 現在のLEDの状態（HIGH/LOW）
};

// LEDの状態を最大20ピン分管理
static LEDState ledStates[20];

// 初期化関数
unsigned short LEDInit(unsigned short pin) {
  PinControlDigitalWrite(pin, LOW); // 初期状態は消灯
  ledStates[pin].isBlinking = false;
  ledStates[pin].lastToggleTime = 0;
  ledStates[pin].blinkInterval = 0;
  ledStates[pin].currentState = LOW;
  return 0;
}

// LED点灯関数
unsigned short LEDOn(unsigned short pin) {
  if (pin >= 20) return 1; // 範囲外のピン番号をチェック
  if (PinControlDigitalWrite(pin, HIGH) != 0) return 1; // ピン制御モジュールを使用
  ledStates[pin].isBlinking = false;  // 点滅を停止
  ledStates[pin].currentState = HIGH;
  return 0;
}

// LED消灯関数
unsigned short LEDOff(unsigned short pin) {
  if (pin >= 20) return 1; // 範囲外のピン番号をチェック
  if (PinControlDigitalWrite(pin, LOW) != 0) return 1; // ピン制御モジュールを使用
  ledStates[pin].isBlinking = false;  // 点滅を停止
  ledStates[pin].currentState = LOW;
  return 0;
}

// LED点滅設定関数
unsigned short LEDBlink(unsigned short pin, unsigned long interval) {
  if (pin >= 20) return 1; // 範囲外のピン番号をチェック
  ledStates[pin].isBlinking = true;
  ledStates[pin].blinkInterval = interval;
  ledStates[pin].lastToggleTime = millis();
  return 0;
}

// LED状態更新関数（点滅処理のため、定期的に呼び出す）
unsigned short LEDUpdate(void) {
  for (unsigned short pin = 0; pin < 20; pin++) {
    if (!ledStates[pin].isBlinking) continue;  // 点滅中でなければスキップ

    unsigned long currentTime = millis();
    if (currentTime - ledStates[pin].lastToggleTime >= ledStates[pin].blinkInterval) {
      // 点灯/消灯を切り替え
      ledStates[pin].currentState = !ledStates[pin].currentState;
      if (PinControlDigitalWrite(pin, ledStates[pin].currentState ? HIGH : LOW) != 0) {
        return 1; // エラー発生時は終了
      }
      ledStates[pin].lastToggleTime = currentTime;  // 時刻を更新
    }
  }
  return 0;
}
