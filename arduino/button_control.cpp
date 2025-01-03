#include <Arduino.h>
#include "button_control.h"
#include "pin_control.h" // ピン制御モジュールをインクルード

#define DEBOUNCE_DELAY 50 // デバウンス遅延 (ms)

// ボタン状態管理構造体
struct ButtonState
{
	unsigned long lastDebounceTime; // デバウンスのタイムスタンプ
	bool lastStableState;			// 安定した最後の状態 (HIGH/LOW)
	bool currentState;				// 現在の読み取り状態
	ButtonCallback callback;		// ボタンが押されたときのコールバック
};

// 最大20個のピンのボタン状態を管理
static ButtonState buttonStates[20];

unsigned short ButtonSetup(unsigned short pin, ButtonCallback callback)
{
	// ピンモードを INPUT_PULLUP に設定
	if (PinControlSetMode(pin, INPUT_PULLUP) != 0)
	{
		return 1; // エラー
	}

	// ボタン状態初期化
	buttonStates[pin].lastDebounceTime = 0;
	buttonStates[pin].lastStableState = HIGH; // 初期状態は HIGH（未押下）
	buttonStates[pin].currentState = HIGH;
	buttonStates[pin].callback = callback;

	return 0;
}

unsigned short ButtonLoop(void)
{
	for (unsigned short pin = 0; pin < 20; pin++)
	{
		if (buttonStates[pin].callback == nullptr)
		{
			continue; // コールバックが登録されていないピンはスキップ
		}

		// 現在のピンの状態を読み取る
		short reading;
		if (PinControlDigitalRead(pin, &reading) != 0)
		{
			return 1; // エラー
		}

		// 状態が変化したらデバウンス処理をリセット
		if (reading != buttonStates[pin].currentState)
		{
			buttonStates[pin].lastDebounceTime = millis();
			buttonStates[pin].currentState = reading; // 状態を即時更新
		}

		// デバウンス遅延後に安定した状態を確認
		if ((millis() - buttonStates[pin].lastDebounceTime) > DEBOUNCE_DELAY)
		{
			// 安定状態が変化した場合
			if (reading != buttonStates[pin].lastStableState)
			{
				buttonStates[pin].lastStableState = reading; // 安定状態を更新

				// ボタンが押された場合（LOW に遷移）
				if (reading == LOW && buttonStates[pin].callback != nullptr)
				{
					buttonStates[pin].callback(); // コールバックを呼び出す
				}
			}
		}
	}
	return 0;
}
