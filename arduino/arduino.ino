#include "Arduino.h"

#include "command.h"
#include "serial.h"
#include "pin_control.h"
#include "button_control.h"
#include "motor_control.h"
#include "led_control.h"
#include "sensor_control.h"


// ボタンが押されたときの処理(仮)
void onButtonPress() {

  const char* ArgV[2] = {"sensor","get"};
  CommandExecute(2, ArgV);
  LEDOneShot(PIN_BUZZER, 100);
}

void onButtonPress1() {
  SensorControlCalibrateWhite();
  LEDOneShot(PIN_BUZZER, 100);
}

void onButtonPress2() {
  SensorControlCalibrateBlack();
  SensorControlUpdateThresholds();
  LEDOneShot(PIN_BUZZER, 100);
}

void setup()
{
  SerialInit();
  ButtonInit(PIN_BUTTON_W, onButtonPress1);
  ButtonInit(PIN_BUTTON_B, onButtonPress2);
  ButtonInit(PIN_BUTTON_G, onButtonPress);
  MotorControlInit();

  SensorControlInit();
  LEDOn(PIN_SENSOR_LEDON);

  LEDPattern(PIN_BUZZER); // 起動時にピピと鳴らす
}

void loop()
{
  SerialRead();
  ButtonRead();
  LEDUpdate();
}

