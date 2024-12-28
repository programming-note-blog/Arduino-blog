#include <Arduino.h>

#include "command.h"
#include "pin_control.h"
#include "motor_control.h"
#include "led_control.h"

#ifdef SUCCESS
#undef SUCCESS
#endif
#define SUCCESS 0
#ifdef ERROR
#undef ERROR
#endif
#define ERROR   1

#define IS_ERROR(code)    ((code) != SUCCESS)

#define COMMAND_TABLE_SIZE (sizeof(COMMAND_TABLE) / sizeof(SCommand))

static unsigned short cmd_echo(unsigned short argc, char** argv);
static unsigned short cmd_dw(unsigned short argc, char** argv);
static unsigned short cmd_dr(unsigned short argc, char** argv);
static unsigned short cmd_aw(unsigned short argc, char** argv);
static unsigned short cmd_ar(unsigned short argc, char** argv);
static unsigned short cmd_motor_set_right(unsigned short argc, char** argv);
static unsigned short cmd_motor_set_left(unsigned short argc, char** argv);
static unsigned short cmd_motor_stop(unsigned short argc, char** argv);
static unsigned short cmd_led(unsigned short argc, char** argv);

typedef struct tCommand
{
  const char* key;                                                // キーとなる単語
  unsigned short (*func)(unsigned short argc, char** argv);       // 呼び出す関数
  const char* usage;                                              // 使い方
} SCommand;

static const SCommand COMMAND_TABLE[] =                           // コマンドテーブル
{
  {"echo", cmd_echo, "echo <prm>            :display prm"},
  {"dw",   cmd_dw,   "dw <pin> <value(0/1)> :digitalWrite(pin,value)"},
  {"dr",   cmd_dr,   "dr <pin>              :digitalRead(pin)"},
  {"aw",   cmd_aw,   "aw <pin> <dec>        :analogWrite(pin,dec)"},
  {"ar",   cmd_ar,   "ar <pin>              :analogRead(pin)"},
  {"motor_set_right", cmd_motor_set_right, "motor_set_right <speed> :Set right motor speed (-255 to 255)"},
  {"motor_set_left",  cmd_motor_set_left,  "motor_set_left <speed>  :Set left motor speed (-255 to 255)"},
  {"motor_stop", cmd_motor_stop, "motor_stop            :Stop both motors"},
  {"led",  cmd_led,  "led                   :led control commands"}
};

void CommandExecute(unsigned short argc, char** argv)
{
  unsigned short i;
  unsigned short result;

  if (argc == 0)
	{
		return;
	}

  for(i = 0; i < COMMAND_TABLE_SIZE; i++)
  {
    if(!strcmp(COMMAND_TABLE[i].key, argv[0]))
    {
      result = COMMAND_TABLE[i].func(argc - 1, &argv[1]);
      if(IS_ERROR(result))
      {
        Serial.println(COMMAND_TABLE[i].usage);
      }
      return;
    }
  }

  Serial.println("[error] Unknown command. Available commands:");
  for(i = 0; i < COMMAND_TABLE_SIZE; i++)
  {
    Serial.println(COMMAND_TABLE[i].usage);
  }
}

static unsigned short cmd_echo(unsigned short argc, char** argv)
{
  unsigned short i;

  if(argc == 0)
  {
    return ERROR;
  }

  Serial.print(argv[0]);

  for(i = 1; i < argc; i++)
  {
    Serial.print(" ");
    Serial.print(argv[i]);
  }
  Serial.println();

  return SUCCESS;
}

static unsigned short cmd_dw(unsigned short argc, char** argv) {
  if (argc < 2) return ERROR;

  unsigned short pin = atoi(argv[0]);
  unsigned short value = atoi(argv[1]);

  return PinControlDigitalWrite(pin, value);
}

static unsigned short cmd_dr(unsigned short argc, char** argv) {
  if (argc < 1) return ERROR;

  unsigned short pin = atoi(argv[0]);
  short value;

  if (IS_ERROR(PinControlDigitalRead(pin, &value))) {
    return ERROR;
  }

  Serial.println(value);
  return SUCCESS;
}

static unsigned short cmd_aw(unsigned short argc, char** argv) {
  if (argc < 2) return ERROR;

  unsigned short pin = atoi(argv[0]);
  unsigned short value = atoi(argv[1]);

  return PinControlAnalogWrite(pin, value);
}

static unsigned short cmd_ar(unsigned short argc, char** argv) {
  if (argc < 1) return ERROR;

  unsigned short pin = atoi(argv[0]);
  short value;

  if (IS_ERROR(PinControlAnalogRead(pin, &value))) {
    return ERROR;
  }

  Serial.println(value);
  return SUCCESS;
}

static unsigned short cmd_motor_set_right(unsigned short argc, char** argv) {
  if (argc < 1) return ERROR;
  short speed = atoi(argv[0]);
  return MotorControlSetRightMotorSpeed(speed);
}

static unsigned short cmd_motor_set_left(unsigned short argc, char** argv) {
  if (argc < 1) return ERROR;
  short speed = atoi(argv[0]);
  return MotorControlSetLeftMotorSpeed(speed);
}

static unsigned short cmd_motor_stop(unsigned short argc, char** argv) {
  MotorControlStopRightMotor();
  MotorControlStopLeftMotor();
  return SUCCESS;
}

static unsigned short cmd_led_oneshot(unsigned short argc, char** argv);
static const SCommand COMMAND_LED_TABLE[] =                           // コマンドテーブル
{
  {"oneshot", cmd_led_oneshot, "oneshot <pin> <ms>            :display prm"},
};
static unsigned short cmd_led(unsigned short argc, char** argv)
{
  unsigned short i;
  unsigned short result;

  if (argc == 0)
	{
    for(i = 0; i < (sizeof(COMMAND_LED_TABLE) / sizeof(SCommand)); i++)
    {
      Serial.println(COMMAND_LED_TABLE[i].usage);
    }

    return SUCCESS;
	}

  for(i = 0; i < (sizeof(COMMAND_LED_TABLE) / sizeof(SCommand)); i++)
  {
    if(!strcmp(COMMAND_LED_TABLE[i].key, argv[0]))
    {
      result = COMMAND_LED_TABLE[i].func(argc - 1, &argv[1]);
      if(IS_ERROR(result))
      {
        Serial.println(COMMAND_LED_TABLE[i].usage);
      }
      return SUCCESS;
    }
  }

  Serial.println("[error] Unknown command. Available commands:");
  for(i = 0; i < (sizeof(COMMAND_LED_TABLE) / sizeof(SCommand)); i++)
  {
    Serial.println(COMMAND_LED_TABLE[i].usage);
  }

  return SUCCESS;
}

static unsigned short cmd_led_oneshot(unsigned short argc, char** argv)
{
  if (argc < 2) return ERROR;

  unsigned short pin = atoi(argv[0]);
  unsigned short duration = atoi(argv[1]);

  return LEDOneShot(pin, duration);
}
