#include <Arduino.h>

#include "command.h"
#include "pin_control.h"
#include "motor_control.h"
#include "led_control.h"
#include "sensor_control.h"

#ifdef SUCCESS
#undef SUCCESS
#endif
#define SUCCESS 0
#ifdef ERROR
#undef ERROR
#endif
#define ERROR 1

#define IS_ERROR(code) ((code) != SUCCESS)

#define COMMAND_TABLE_SIZE (sizeof(COMMAND_TABLE) / sizeof(SCommand))

static unsigned short cmd_echo(unsigned short argc, const char **argv);
static unsigned short cmd_dw(unsigned short argc, const char **argv);
static unsigned short cmd_dr(unsigned short argc, const char **argv);
static unsigned short cmd_aw(unsigned short argc, const char **argv);
static unsigned short cmd_ar(unsigned short argc, const char **argv);
static unsigned short cmd_motor(unsigned short argc, const char **argv);
static unsigned short cmd_led(unsigned short argc, const char **argv);
static unsigned short cmd_sensor(unsigned short argc, const char **argv);

typedef struct tCommand
{
	const char *key;												// キーとなる単語
	unsigned short (*func)(unsigned short argc, const char **argv); // 呼び出す関数
	const char *usage;												// 使い方
} SCommand;

static const SCommand COMMAND_TABLE[] = // コマンドテーブル
	{
		{"echo", cmd_echo, "echo <prm>            :display prm"},
		{"dw", cmd_dw, "dw <pin> <value(0/1)> :digitalWrite(pin,value)"},
		{"dr", cmd_dr, "dr <pin>              :digitalRead(pin)"},
		{"aw", cmd_aw, "aw <pin> <dec>        :analogWrite(pin,dec)"},
		{"ar", cmd_ar, "ar <pin>              :analogRead(pin)"},
		{"motor", cmd_motor, "motor                 :motor control commands"},
		{"led", cmd_led, "led                   :led control commands"},
		{"sensor", cmd_sensor, "sensor                :sensor control commands"},
};

void CommandExecute(unsigned short argc, const char **argv)
{
	unsigned short i;
	unsigned short result;

	if (argc == 0)
	{
		return;
	}

	for (i = 0; i < COMMAND_TABLE_SIZE; i++)
	{
		if (!strcmp(COMMAND_TABLE[i].key, argv[0]))
		{
			result = COMMAND_TABLE[i].func(argc - 1, &argv[1]);
			if (IS_ERROR(result))
			{
				Serial.println(COMMAND_TABLE[i].usage);
			}
			return;
		}
	}

	Serial.println("[error] Unknown command. Available commands:");
	for (i = 0; i < COMMAND_TABLE_SIZE; i++)
	{
		Serial.println(COMMAND_TABLE[i].usage);
	}
}

static unsigned short cmd_echo(unsigned short argc, const char **argv)
{
	unsigned short i;

	if (argc == 0)
	{
		return ERROR;
	}

	Serial.print(argv[0]);

	for (i = 1; i < argc; i++)
	{
		Serial.print(" ");
		Serial.print(argv[i]);
	}
	Serial.println();

	return SUCCESS;
}

static unsigned short cmd_dw(unsigned short argc, const char **argv)
{
	if (argc < 2)
		return ERROR;

	unsigned short pin = atoi(argv[0]);
	unsigned short value = atoi(argv[1]);

	return PinControlDigitalWrite((Pin)pin, value);
}

static unsigned short cmd_dr(unsigned short argc, const char **argv)
{
	if (argc < 1)
		return ERROR;

	unsigned short pin = atoi(argv[0]);
	short value;

	if (IS_ERROR(PinControlDigitalRead((Pin)pin, &value)))
	{
		return ERROR;
	}

	Serial.println(value);
	return SUCCESS;
}

static unsigned short cmd_aw(unsigned short argc, const char **argv)
{
	if (argc < 2)
		return ERROR;

	unsigned short pin = atoi(argv[0]);
	unsigned short value = atoi(argv[1]);

	return PinControlAnalogWrite((Pin)pin, value);
}

static unsigned short cmd_ar(unsigned short argc, const char **argv)
{
	if (argc < 1)
		return ERROR;

	unsigned short pin = atoi(argv[0]);

	Serial.println(PinControlAnalogRead((Pin)pin));

	return SUCCESS;
}

static unsigned short cmd_motor_set_right(unsigned short argc, const char **argv);
static unsigned short cmd_motor_set_left(unsigned short argc, const char **argv);
static unsigned short cmd_motor_stop(unsigned short argc, const char **argv);
static const SCommand COMMAND_MOTOR_TABLE[] = // コマンドテーブル
	{
		{"r", cmd_motor_set_right, "motor r <speed> :Set right motor speed (-255 to 255)"},
		{"l", cmd_motor_set_left, "motor l <speed> :Set left motor speed (-255 to 255)"},
		{"s", cmd_motor_stop, "motor s         :Stop both motors"},
};

static unsigned short cmd_motor(unsigned short argc, const char **argv)
{
	unsigned short i;
	unsigned short result;

	if (argc == 0)
	{
		for (i = 0; i < (sizeof(COMMAND_MOTOR_TABLE) / sizeof(SCommand)); i++)
		{
			Serial.println(COMMAND_MOTOR_TABLE[i].usage);
		}

		return SUCCESS;
	}

	for (i = 0; i < (sizeof(COMMAND_MOTOR_TABLE) / sizeof(SCommand)); i++)
	{
		if (!strcmp(COMMAND_MOTOR_TABLE[i].key, argv[0]))
		{
			result = COMMAND_MOTOR_TABLE[i].func(argc - 1, &argv[1]);
			if (IS_ERROR(result))
			{
				Serial.println(COMMAND_MOTOR_TABLE[i].usage);
			}
			return SUCCESS;
		}
	}

	Serial.println("[error] Unknown command. Available commands:");
	for (i = 0; i < (sizeof(COMMAND_MOTOR_TABLE) / sizeof(SCommand)); i++)
	{
		Serial.println(COMMAND_MOTOR_TABLE[i].usage);
	}

	return SUCCESS;
}

static unsigned short cmd_motor_set_right(unsigned short argc, const char **argv)
{
	if (argc < 1)
		return ERROR;
	short speed = atoi(argv[0]);
	return MotorControlSetRightMotorSpeed(speed);
}

static unsigned short cmd_motor_set_left(unsigned short argc, const char **argv)
{
	if (argc < 1)
		return ERROR;
	short speed = atoi(argv[0]);
	return MotorControlSetLeftMotorSpeed(speed);
}

static unsigned short cmd_motor_stop(unsigned short argc, const char **argv)
{
	MotorControlStopRightMotor();
	MotorControlStopLeftMotor();
	return SUCCESS;
}

static unsigned short cmd_led_oneshot(unsigned short argc, const char **argv);
static const SCommand COMMAND_LED_TABLE[] = // コマンドテーブル
	{
		{"oneshot", cmd_led_oneshot, "oneshot <pin> <ms>            :display prm"},
};
static unsigned short cmd_led(unsigned short argc, const char **argv)
{
	unsigned short i;
	unsigned short result;

	if (argc == 0)
	{
		for (i = 0; i < (sizeof(COMMAND_LED_TABLE) / sizeof(SCommand)); i++)
		{
			Serial.println(COMMAND_LED_TABLE[i].usage);
		}

		return SUCCESS;
	}

	for (i = 0; i < (sizeof(COMMAND_LED_TABLE) / sizeof(SCommand)); i++)
	{
		if (!strcmp(COMMAND_LED_TABLE[i].key, argv[0]))
		{
			result = COMMAND_LED_TABLE[i].func(argc - 1, &argv[1]);
			if (IS_ERROR(result))
			{
				Serial.println(COMMAND_LED_TABLE[i].usage);
			}
			return SUCCESS;
		}
	}

	Serial.println("[error] Unknown command. Available commands:");
	for (i = 0; i < (sizeof(COMMAND_LED_TABLE) / sizeof(SCommand)); i++)
	{
		Serial.println(COMMAND_LED_TABLE[i].usage);
	}

	return SUCCESS;
}

static unsigned short cmd_led_oneshot(unsigned short argc, const char **argv)
{
	if (argc < 2)
		return ERROR;

	unsigned short pin = atoi(argv[0]);
	unsigned short duration = atoi(argv[1]);

	return LedOneShot((Pin)pin, duration);
}

static unsigned short cmd_sensor_calibrate_white(unsigned short argc, const char **argv);
static unsigned short cmd_sensor_calibrate_black(unsigned short argc, const char **argv);
static unsigned short cmd_sensor_update_thresholds(unsigned short argc, const char **argv);
static unsigned short cmd_sensor_get_binary_output(unsigned short argc, const char **argv);

static const SCommand COMMAND_SENSOR_TABLE[] =
	{
		{"w", cmd_sensor_calibrate_white, "sensor w   :Calibrate white surface"},
		{"b", cmd_sensor_calibrate_black, "sensor b   :Calibrate black surface"},
		{"u", cmd_sensor_update_thresholds, "sensor u   :Update detection thresholds"},
		{"get", cmd_sensor_get_binary_output, "sensor get :Get binary output"},
};

static unsigned short cmd_sensor(unsigned short argc, const char **argv)
{
	unsigned short i;
	unsigned short result;

	if (argc == 0)
	{
		for (i = 0; i < (sizeof(COMMAND_SENSOR_TABLE) / sizeof(SCommand)); i++)
		{
			Serial.println(COMMAND_SENSOR_TABLE[i].usage);
		}
		return SUCCESS;
	}

	for (i = 0; i < (sizeof(COMMAND_SENSOR_TABLE) / sizeof(SCommand)); i++)
	{
		if (!strcmp(COMMAND_SENSOR_TABLE[i].key, argv[0]))
		{
			result = COMMAND_SENSOR_TABLE[i].func(argc - 1, &argv[1]);
			if (IS_ERROR(result))
			{
				Serial.println(COMMAND_SENSOR_TABLE[i].usage);
			}
			return SUCCESS;
		}
	}

	Serial.println("[error] Unknown command. Available commands:");
	for (i = 0; i < (sizeof(COMMAND_SENSOR_TABLE) / sizeof(SCommand)); i++)
	{
		Serial.println(COMMAND_SENSOR_TABLE[i].usage);
	}

	return SUCCESS;
}

static unsigned short cmd_sensor_calibrate_white(unsigned short argc, const char **argv)
{
	return SensorControlCalibrateWhite();
}

static unsigned short cmd_sensor_calibrate_black(unsigned short argc, const char **argv)
{
	return SensorControlCalibrateBlack();
}

static unsigned short cmd_sensor_update_thresholds(unsigned short argc, const char **argv)
{
	return SensorControlUpdateThresholds();
}

static unsigned short cmd_sensor_get_binary_output(unsigned short argc, const char **argv)
{
	unsigned char binaryOutput = SensorControlGetBinaryOutput();
	Serial.print(!!(binaryOutput & (1 << 7)));
	Serial.print(!!(binaryOutput & (1 << 6)));
	Serial.print(!!(binaryOutput & (1 << 5)));
	Serial.print(!!(binaryOutput & (1 << 4)));
	Serial.print(!!(binaryOutput & (1 << 3)));
	Serial.print(!!(binaryOutput & (1 << 2)));
	Serial.print(!!(binaryOutput & (1 << 1)));
	Serial.print(!!(binaryOutput & (1 << 0)));
	Serial.println("");
	return SUCCESS;
}
