#include "Arduino.h"

#include "command.h"
#include "serial.h"
#include "pin_control.h"
#include "button_control.h"
#include "motor_control.h"
#include "led_control.h"
#include "sensor_control.h"
#include "cycle.h"
#include "state.h"

/**
 * @brief ライントレース用のフレーム情報を管理する構造体
 *
 * - sensorData: センサーから取得したバイナリデータ
 * - frameIndex: 現在のフレームインデックス
 * - leftSpeed: 左モーターの速度
 * - rightSpeed: 右モーターの速度
 * - center: ラインの中心位置
 * - error: ライン中心からの偏差
 * - previousError: 前回の偏差
 * - integral: 偏差の積分値
 */
struct FrameInfo
{
	unsigned char sensorData = 0;  ///< センサー出力データ
	unsigned short frameIndex = 0; ///< フレームインデックス
	short leftSpeed = 0;		   ///< 左モーター速度
	short rightSpeed = 0;		   ///< 右モーター速度
	float center = 0.0f;		   ///< ラインの中心位置
	float error = 0.0f;			   ///< ライン中心からの偏差
	short adjustment = 0;		   ///< モーター制御量
	float previousError = 0.0f;	   ///< 前回の偏差
	float integral = 0.0f;		   ///< 偏差の積分値
};

static FrameInfo frameInfo;

/**
 * @brief ボタン押下時の処理
 * @details 現在の状態を更新し、LEDを短時間点灯させる。
 */
void onButtonPress()
{
	StateOnButtonPress();
	LedOneShot(PIN_BUZZER, 100);
}

/**
 * @brief 白色キャリブレーションを行うボタン処理
 */
void onButtonPress1()
{
	SensorControlCalibrateWhite();
	LedOneShot(PIN_BUZZER, 100);
}

/**
 * @brief 黒色キャリブレーションを行うボタン処理
 */
void onButtonPress2()
{
	SensorControlCalibrateBlack();
	LedOneShot(PIN_BUZZER, 100);
}

/**
 * @brief 閾値更新を行うボタン処理
 */
void onButtonRelease2()
{
	SensorControlUpdateThresholds();
	LedPattern(PIN_BUZZER);
}

/**
 * @brief 現在の状態を更新する処理
 */
static void PreMainCycle()
{
	frameInfo.frameIndex++;
	CycleSetState(StateGetCurrentstate());
}

/**
 * @brief センサーからデータを読み取る処理
 * @details センサー出力を取得し、フレームインデックスをインクリメントする。
 */
static void ReadSensorData()
{
	frameInfo.sensorData = SensorControlGetBinaryOutput();
}

/**
 * @brief モーター速度を計算する処理
 * @details
 * - センサー情報からラインの中心位置を計算
 * - 偏差と制御値を計算し、モーター速度を設定
 * - PID制御（比例・積分・微分制御）を適用
 */
static void CalculateMotorSpeeds()
{
	// ラインの中心位置を計算
	{
		int count = 0; // ライン上のビット数
		int sum = 0;   // ラインの位置の合計

		for (int i = 0; i < 8; i++)
		{
			if (frameInfo.sensorData & (1 << i))
			{
				sum += i;
				count++;
			}
		}

		frameInfo.center = (count == 0) ? 3.5f : static_cast<float>(sum) / count;
	}

	// 偏差と制御値を計算
	frameInfo.error = 3.5f - frameInfo.center;
	frameInfo.integral += frameInfo.error; // 偏差の積分
	frameInfo.integral = constrain(frameInfo.integral, -20, 20);

	const short baseSpeed = 150; ///< 基本速度
	const float Kp = 30.0f;		 ///< 比例ゲイン
	const float Ki = 0.5f;		 ///< 積分ゲイン
	const float Kd = 10.0f;		 ///< 微分ゲイン

	float derivative = frameInfo.error - frameInfo.previousError;
	frameInfo.adjustment = static_cast<short>(Kp * frameInfo.error + Ki * frameInfo.integral + Kd * derivative);

	frameInfo.leftSpeed = baseSpeed - frameInfo.adjustment;
	frameInfo.rightSpeed = baseSpeed + frameInfo.adjustment;

	// モーター速度を制限
	frameInfo.leftSpeed = constrain(frameInfo.leftSpeed, 0, 255);
	frameInfo.rightSpeed = constrain(frameInfo.rightSpeed, 0, 255);

	frameInfo.previousError = frameInfo.error;
}

/**
 * @brief モーター速度を適用する処理
 * @details 計算された速度を左右のモーターに適用する。
 */
static void ApplyMotorSpeeds()
{
	MotorControlSetLeftMotorSpeed(frameInfo.leftSpeed);
	MotorControlSetRightMotorSpeed(frameInfo.rightSpeed);

	// Serial.print(!!(frameInfo.sensorData & (1 << 7)));
	// Serial.print(!!(frameInfo.sensorData & (1 << 6)));
	// Serial.print(!!(frameInfo.sensorData & (1 << 5)));
	// Serial.print(!!(frameInfo.sensorData & (1 << 4)));
	// Serial.print(!!(frameInfo.sensorData & (1 << 3)));
	// Serial.print(!!(frameInfo.sensorData & (1 << 2)));
	// Serial.print(!!(frameInfo.sensorData & (1 << 1)));
	// Serial.print(!!(frameInfo.sensorData & (1 << 0)));
	// Serial.print(",");
	// Serial.print(frameInfo.center);
	// Serial.print(",");
	// Serial.print(frameInfo.error);
	// Serial.print(",");
	// Serial.print(frameInfo.adjustment);
	// Serial.print(",");
	// Serial.print(frameInfo.leftSpeed);
	// Serial.print(",");
	// Serial.println(frameInfo.rightSpeed);
}

/**
 * @brief 停止状態のログを出力する処理
 */
static void LogStoppedState()
{
	printf("Stopped state log: %lu\n", millis());
}

/**
 * @brief 初期化処理
 * @details 各モジュールの初期化と周期処理の設定を行う。
 */
void setup()
{
	SerialSetup();
	ButtonSetup(PIN_BUTTON_W, onButtonPress1, nullptr);
	ButtonSetup(PIN_BUTTON_B, onButtonPress2, onButtonRelease2);
	ButtonSetup(PIN_BUTTON_START, onButtonPress, nullptr);
	MotorSetup();
	SensorSetup();

	// 周期処理の設定
	CycleSetup(PreMainCycle, STATE_STANDBY, 0);

	// STATE_LINETRACINGの周期処理設定
	CycleSetup(PreMainCycle, STATE_LINETRACING, 0);
	CycleSetup(ReadSensorData, STATE_LINETRACING, 1);
	CycleSetup(CalculateMotorSpeeds, STATE_LINETRACING, 2);
	CycleSetup(ApplyMotorSpeeds, STATE_LINETRACING, 7);

	// STATE_STOPPEDの周期処理設定
	CycleSetup(PreMainCycle, STATE_STOPPED, 0);
	CycleSetup(LogStoppedState, STATE_STOPPED, 0);

	LedPattern(PIN_BUZZER); // 起動時のサウンド
}

/**
 * @brief メインループ
 * @details シリアル通信、ボタン処理、LED処理、周期処理を実行する。
 */
void loop()
{
	SerialLoop();
	ButtonLoop();
	LedLoop();
	CycleLoop();
}
