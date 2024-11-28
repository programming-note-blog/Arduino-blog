#include <Arduino.h>
#include "pin_control.h"


// ボタン用デバウンス処理のための状態管理
struct ButtonState {
  unsigned long lastDebounceTime;  // デバウンス用のタイムスタンプ
  bool lastStableState;           // 安定した状態（HIGH/LOW）
  bool currentState;              // 現在の入力状態
  ButtonCallback callback;        // ボタンが押された時のコールバック関数
};

// ピンごとのボタン状態を管理する配列 (仮に20ピンをサポート)
static ButtonState buttonStates[20];

#define DEBOUNCE_DELAY 50 // デバウンス用の遅延(ms)


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


// ボタン初期化 (コールバック関数を登録)
unsigned short PinControlButtonInit(unsigned short pin, ButtonCallback callback) {
  pinMode(pin, INPUT_PULLUP); // プルアップモードでボタンを設定
  buttonStates[pin].lastDebounceTime = 0;
  buttonStates[pin].lastStableState = HIGH; // 初期状態はHIGH（未押下）
  buttonStates[pin].currentState = HIGH;
  buttonStates[pin].callback = callback; // コールバック関数を登録
  return 0;
}

// ボタン状態を確認して押下時の処理を実行
unsigned short PinControlButtonRead(void) {
  for (unsigned short pin = 0; pin < 20; pin++) {
    if (buttonStates[pin].callback == nullptr) {
      continue; // コールバックが登録されていないピンはスキップ
    }

    // 現在のボタン状態を読み取る
    bool reading = digitalRead(pin);

    // デバウンス処理
    if (reading != buttonStates[pin].currentState) {
      buttonStates[pin].lastDebounceTime = millis(); // 状態が変化したのでタイムスタンプを記録
    }

    // 一定時間経過後に状態を確定
    if ((millis() - buttonStates[pin].lastDebounceTime) > DEBOUNCE_DELAY) {
      if (reading != buttonStates[pin].lastStableState) {
        buttonStates[pin].lastStableState = reading; // 状態が安定したので更新

        // 押下状態になったときのみコールバックを実行
        if (reading == LOW && buttonStates[pin].callback != nullptr) {
          buttonStates[pin].callback();
        }
      }
    }

    buttonStates[pin].currentState = reading; // 現在の状態を更新
  }
  return 0;
}
