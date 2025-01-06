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

// ボタンが押されたときの処理(仮)
void onButtonPress()
{
	StateOnButtonPress();
	LedOneShot(PIN_BUZZER, 100);
}

void onButtonPress1()
{
	SensorControlCalibrateWhite();
	LedOneShot(PIN_BUZZER, 100);
}

void onButtonPress2()
{
	SensorControlCalibrateBlack();
	LedOneShot(PIN_BUZZER, 100);
}

void onButtonRelease2()
{
	SensorControlUpdateThresholds();
	LedPattern(PIN_BUZZER);
}

static void UpdateCurrentState()
{
	CycleSetState(StateGetCurrentstate());
}

unsigned char sensorData;
unsigned short frameindex = 0;
static void Func1()
{
	sensorData = SensorControlGetBinaryOutput(); // 実測0.93us
	frameindex++;
}

short leftSpeed;
short rightSpeed;
static void Func2()
{
	// 中心位置計算
	float center;
	{
		int count = 0; // ライン上のビット数
		int sum = 0;   // ラインの位置の合計

		for (int i = 0; i < 8; i++)
		{
			if (sensorData & (1 << i))
			{
				sum += i;
				count++;
			}
		}

		if (count == 0)
		{
			center = 3.5; // ラインが見つからない場合
		}
		else
		{
			center = (float)sum / count; // 中心位置を計算
		}
	}

	float error = 3.5 - center;
	// 制御値算出
	{
		const short baseSpeed = 150; // 基本速度
		const float Kp = 30.0;		 // 比例ゲイン
		const float Kd = 10.0;		 // 微分ゲイン

		static float previousError = 0;
		float derivative = error - previousError;

		short adjustment = (short)(Kp * error + Kd * derivative);

		leftSpeed = baseSpeed - adjustment;
		rightSpeed = baseSpeed + adjustment;

		// モーター速度を0～255に制限
		if (leftSpeed < 0)
			leftSpeed = 0;
		if (leftSpeed > 255)
			leftSpeed = 255;
		if (rightSpeed < 0)
			rightSpeed = 0;
		if (rightSpeed > 255)
			rightSpeed = 255;

		previousError = error;
	}
}

static void Func3()
{
	MotorControlSetLeftMotorSpeed(leftSpeed);
	MotorControlSetRightMotorSpeed(rightSpeed);

	Serial.print(leftSpeed);
	Serial.print(",");
	Serial.println(rightSpeed);
}

static void Func4()
{
	printf("Func4 called:%d\n", millis());
}

void setup()
{
	SerialSetup();
	ButtonSetup(PIN_BUTTON_W, onButtonPress1, nullptr);
	ButtonSetup(PIN_BUTTON_B, onButtonPress2, onButtonRelease2);
	ButtonSetup(PIN_BUTTON_START, onButtonPress, nullptr);
	MotorSetup();
	SensorSetup();

	// 周期処理の設定
	// STATE_STANDBYの周期処理設定
	CycleSetup(UpdateCurrentState, STATE_STANDBY, 0);
	// STATE_LINETRACINGの周期処理設定
	CycleSetup(UpdateCurrentState, STATE_LINETRACING, 0);
	CycleSetup(Func1, STATE_LINETRACING, 1); // センサー情報取得
	CycleSetup(Func2, STATE_LINETRACING, 2); // 制御値算出
	CycleSetup(Func3, STATE_LINETRACING, 7); // 制御値設定
	// STATE_STOPPEDの周期処理設定
	CycleSetup(UpdateCurrentState, STATE_STOPPED, 0);
	CycleSetup(Func4, STATE_STOPPED, 0); // アラーム鳴らす

	LedPattern(PIN_BUZZER); // 起動時にピピと鳴らす
}

void loop()
{
	SerialLoop();
	ButtonLoop();
	LedLoop();
	CycleLoop();
}
