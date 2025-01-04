/**
 * @file button_control.cpp
 * @brief ボタン制御モジュール
 *
 * ボタンの押下時および解放時にコールバック関数を呼び出す機能を提供します。
 */

#include <Arduino.h>
#include "button_control.h"
#include "pin_control.h"

#define DEBOUNCE_DELAY 50 ///< デバウンス遅延 (ms)

/**
 * @struct ButtonState
 * @brief ボタンの状態を管理する構造体
 *
 * ボタンの状態、デバウンス処理用のタイムスタンプ、
 * 押下および解放時のコールバック関数を保持します。
 */
struct ButtonState
{
	unsigned long lastDebounceTime;	  ///< デバウンスのタイムスタンプ
	bool lastStableState;			  ///< 安定した最後の状態 (HIGH/LOW)
	bool currentState;				  ///< 現在の読み取り状態
	ButtonCallback onPressCallback;	  ///< ボタンが押されたときのコールバック
	ButtonCallback onReleaseCallback; ///< ボタンが離されたときのコールバック
};

/// 最大20個のピンのボタン状態を管理
static ButtonState buttonStates[20];

/**
 * @brief ボタンの設定を行います。
 *
 * 指定したピンに対して、押下および解放時のコールバック関数を登録します。
 *
 * @param[in] pin 設定対象のピン番号
 * @param[in] onPress ボタン押下時のコールバック関数 (nullptr 可)
 * @param[in] onRelease ボタン解放時のコールバック関数 (nullptr 可)
 * @return 成功時は0、エラー時は1を返します。
 *
 * @details
 * - ピンモードは INPUT_PULLUP に設定されます。
 * - コールバック関数が登録されていない場合、その状態はスキップされます。
 */
unsigned short ButtonSetup(unsigned short pin, ButtonCallback onPress, ButtonCallback onRelease)
{
	// ピンモードを INPUT_PULLUP に設定
	if (PinControlSetMode((Pin)pin, INPUT_PULLUP) != 0)
	{
		return 1; // エラー
	}

	// ボタン状態初期化
	buttonStates[pin].lastDebounceTime = 0;
	buttonStates[pin].lastStableState = HIGH; // 初期状態は HIGH（未押下）
	buttonStates[pin].currentState = HIGH;
	buttonStates[pin].onPressCallback = onPress;
	buttonStates[pin].onReleaseCallback = onRelease;

	return 0;
}

/**
 * @brief ボタン状態を監視し、必要に応じてコールバック関数を呼び出します。
 *
 * @return 成功時は0、エラー時は1を返します。
 *
 * @details
 * - 各ボタンピンの状態をチェックし、押下および解放イベントを検出します。
 * - デバウンス処理により、安定した状態のみを基にイベントを判定します。
 */
unsigned short ButtonLoop(void)
{
	for (unsigned short pin = 0; pin < 20; pin++)
	{
		if (buttonStates[pin].onPressCallback == nullptr && buttonStates[pin].onReleaseCallback == nullptr)
		{
			continue; // コールバックが登録されていないピンはスキップ
		}

		// 現在のピンの状態を読み取る
		short reading;
		if (PinControlDigitalRead((Pin)pin, &reading) != 0)
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
				if (reading == LOW && buttonStates[pin].onPressCallback != nullptr)
				{
					buttonStates[pin].onPressCallback(); // 押下コールバックを呼び出す
				}

				// ボタンが離された場合（HIGH に遷移）
				if (reading == HIGH && buttonStates[pin].onReleaseCallback != nullptr)
				{
					buttonStates[pin].onReleaseCallback(); // 解放コールバックを呼び出す
				}
			}
		}
	}
	return 0;
}
