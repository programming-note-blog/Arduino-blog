#include "Arduino.h"
#include "led_control.h"
#include "pin_control.h"

// LED管理用構造体
struct LEDState {
  bool isBlinking;                // 点滅中かどうか
  unsigned long lastToggleTime;   // 最後に点灯/消灯を切り替えた時間
  unsigned long blinkInterval;    // 点滅間隔（ミリ秒）
  bool currentState;              // 現在のLEDの状態（HIGH/LOW）
  bool isOneShot;                 // 一度だけ点灯するかどうか
  unsigned long oneShotDuration;  // 一度だけ点灯する時間（ミリ秒）
  bool isPatternActive;           // 特定パターンが有効かどうか
  unsigned short patternStep;     // パターンの現在のステップ
};

// LEDの状態を最大20ピン分管理
static LEDState ledStates[20] = {0};

// LEDの状態を初期化する内部関数
static void InitializeLEDState(unsigned short pin) {
  if (!ledStates[pin].isBlinking && !ledStates[pin].isOneShot && !ledStates[pin].isPatternActive && ledStates[pin].currentState == LOW && ledStates[pin].blinkInterval == 0) {
//   PinControlDigitalWrite(pin, LOW); // 初期状態は維持するため初期化しない
    ledStates[pin].isBlinking = false;
    ledStates[pin].lastToggleTime = 0;
    ledStates[pin].blinkInterval = 0;
    ledStates[pin].currentState = LOW;
    ledStates[pin].isOneShot = false;
    ledStates[pin].oneShotDuration = 0;
    ledStates[pin].isPatternActive = false;
    ledStates[pin].patternStep = 0;
  }
}

// LED点灯関数
unsigned short LedOn(unsigned short pin) {
  if (pin >= 20) return 1; // 範囲外のピン番号をチェック
  InitializeLEDState(pin);
  if (PinControlDigitalWrite(pin, HIGH) != 0) return 1; // ピン制御モジュールを使用
  ledStates[pin].isBlinking = false;  // 点滅を停止
  ledStates[pin].isOneShot = false;  // 一度だけ点灯も停止
  ledStates[pin].isPatternActive = false; // パターンも停止
  ledStates[pin].currentState = HIGH;
  return 0;
}

// LED消灯関数
unsigned short LedOff(unsigned short pin) {
  if (pin >= 20) return 1; // 範囲外のピン番号をチェック
  InitializeLEDState(pin);
  if (PinControlDigitalWrite(pin, LOW) != 0) return 1; // ピン制御モジュールを使用
  ledStates[pin].isBlinking = false;  // 点滅を停止
  ledStates[pin].isOneShot = false;  // 一度だけ点灯も停止
  ledStates[pin].isPatternActive = false; // パターンも停止
  ledStates[pin].currentState = LOW;
  return 0;
}

// LED点滅設定関数
unsigned short LedBlink(unsigned short pin, unsigned long interval) {
  if (pin >= 20) return 1; // 範囲外のピン番号をチェック
  InitializeLEDState(pin);
  ledStates[pin].isBlinking = true;
  ledStates[pin].blinkInterval = interval;
  ledStates[pin].lastToggleTime = millis();
  ledStates[pin].isOneShot = false; // 一度だけ点灯を無効化
  ledStates[pin].isPatternActive = false; // パターンも無効化
  return 0;
}

// LED一度だけ点灯関数
unsigned short LedOneShot(unsigned short pin, unsigned long duration) {
  if (pin >= 20) return 1; // 範囲外のピン番号をチェック
  InitializeLEDState(pin);
  if (PinControlDigitalWrite(pin, HIGH) != 0) return 1; // 点灯
  ledStates[pin].isBlinking = false; // 点滅を無効化
  ledStates[pin].isOneShot = true;
  ledStates[pin].oneShotDuration = duration;
  ledStates[pin].lastToggleTime = millis();
  ledStates[pin].currentState = HIGH;
  ledStates[pin].isPatternActive = false; // パターンも無効化
  return 0;
}

// LED特定パターン設定関数
unsigned short LedPattern(unsigned short pin) {
  if (pin >= 20) return 1; // 範囲外のピン番号をチェック
  InitializeLEDState(pin);
  ledStates[pin].isBlinking = false; // 点滅を無効化
  ledStates[pin].isOneShot = false;  // 一度だけ点灯を無効化
  ledStates[pin].isPatternActive = true;
  ledStates[pin].patternStep = 0;
  ledStates[pin].lastToggleTime = millis();
  ledStates[pin].currentState = HIGH;
  if (PinControlDigitalWrite(pin, HIGH) != 0) return 1; // 最初の点灯
  return 0;
}

// LED状態更新関数（点滅処理のため、定期的に呼び出す）
unsigned short LedLoop(void) {
  for (unsigned short pin = 0; pin < 20; pin++) {
    if (ledStates[pin].isBlinking) {
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
    
    if (ledStates[pin].isOneShot) {
      unsigned long currentTime = millis();
      if (currentTime - ledStates[pin].lastToggleTime >= ledStates[pin].oneShotDuration) {
        // 一度だけ点灯終了
        if (PinControlDigitalWrite(pin, LOW) != 0) return 1;
        ledStates[pin].isOneShot = false;
        ledStates[pin].currentState = LOW;
      }
    }

    if (ledStates[pin].isPatternActive) {
      unsigned long currentTime = millis();
      switch (ledStates[pin].patternStep) {
        case 0: // 最初の100ms点灯
          if (currentTime - ledStates[pin].lastToggleTime >= 100) {
            if (PinControlDigitalWrite(pin, LOW) != 0) return 1; // 消灯
            ledStates[pin].patternStep = 1;
            ledStates[pin].lastToggleTime = currentTime;
          }
          break;
        case 1: // 50ms消灯
          if (currentTime - ledStates[pin].lastToggleTime >= 50) {
            if (PinControlDigitalWrite(pin, HIGH) != 0) return 1; // 点灯
            ledStates[pin].patternStep = 2;
            ledStates[pin].lastToggleTime = currentTime;
          }
          break;
        case 2: // 次の100ms点灯
          if (currentTime - ledStates[pin].lastToggleTime >= 100) {
            if (PinControlDigitalWrite(pin, LOW) != 0) return 1; // 消灯
            ledStates[pin].patternStep = 3;
            ledStates[pin].lastToggleTime = currentTime;
          }
          break;
        case 3: // パターン終了
          ledStates[pin].isPatternActive = false;
          break;
      }
    }
  }
  return 0;
}
