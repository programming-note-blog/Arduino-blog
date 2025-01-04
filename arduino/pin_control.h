#ifndef PIN_CONTROL_H
#define PIN_CONTROL_H

enum Pin
{
	PIN_RESERVED_2 = 2,
	PIN_RESERVED_3,
	PIN_BUZZER,
	PIN_R_MOTOR_1,
	PIN_R_MOTOR_2,
	PIN_BUTTON_B,
	PIN_BUTTON_W,
	PIN_L_MOTOR_1,
	PIN_L_MOTOR_2,
	PIN_BUTTON_START,
	PIN_RESERVED_12,
	PIN_SENSOR_LEDON,
	PIN_SENSOR_0,
	PIN_SENSOR_1,
	PIN_SENSOR_2,
	PIN_SENSOR_3,
	PIN_SENSOR_4,
	PIN_SENSOR_5,
	PIN_SENSOR_6,
	PIN_SENSOR_7,

	PIN_COUNT
};

unsigned short PinControlSetMode(Pin pin, unsigned short mode);
unsigned short PinControlDigitalWrite(Pin pin, unsigned short value);
unsigned short PinControlDigitalRead(Pin pin, short *value);
unsigned short PinControlAnalogWrite(Pin pin, unsigned short value);
unsigned short PinControlAnalogRead(Pin pin);

#endif // PIN_CONTROL_H