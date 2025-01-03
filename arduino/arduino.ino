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
	SensorControlUpdateThresholds();
	LedOneShot(PIN_BUZZER, 100);
}

static void UpdateCurrentState()
{
	CycleSetState(StateGetCurrentstate());
}

static void Func1()
{
	printf("Func1 called:%d\n", millis());

	const char *ArgV[2] = {"sensor", "get"};
	CommandExecute(2, ArgV);
}

static void Func2()
{
	printf("Func2 called:%d\n", millis());
}

static void Func3()
{
	printf("Func3 called:%d\n", millis());
}

static void Func4()
{
	printf("Func4 called:%d\n", millis());
}

void setup()
{
	SerialSetup();
	ButtonSetup(PIN_BUTTON_W, onButtonPress1);
	ButtonSetup(PIN_BUTTON_B, onButtonPress2);
	ButtonSetup(PIN_BUTTON_START, onButtonPress);
	MotorSetup();
	SensorSetup(PIN_SENSOR_LedOn);

	// 周期処理の設定
	// STATE_STANDBYの周期処理設定
	CycleSetup(UpdateCurrentState, STATE_STANDBY, 0);
	// STATE_LINETRACINGの周期処理設定
	CycleSetup(UpdateCurrentState, STATE_LINETRACING, 0);
	CycleSetup(Func1, STATE_LINETRACING, 1); // センサー情報取得
	CycleSetup(Func2, STATE_LINETRACING, 5); // 制御値算出
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
