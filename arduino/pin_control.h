#ifndef PIN_CONTROL_H
#define PIN_CONTROL_H

#define PIN_BUZZER 4
#define PIN_R_MOTOR_1 5
#define PIN_R_MOTOR_2 6
#define PIN_BUTTON 7
#define PIN_L_MOTOR_1 9
#define PIN_L_MOTOR_2 10
#define PIN_LED 13
#define PIN_SENSOR_0 14
#define PIN_SENSOR_1 15
#define PIN_SENSOR_2 16
#define PIN_SENSOR_3 17
#define PIN_SENSOR_4 18
#define PIN_SENSOR_5 19
#define PIN_SENSOR_6 20
#define PIN_SENSOR_7 21

unsigned short PinControlSetMode(unsigned short pin, unsigned short mode);
unsigned short PinControlDigitalWrite(unsigned short pin, unsigned short value);
unsigned short PinControlDigitalRead(unsigned short pin, short* value);
unsigned short PinControlAnalogWrite(unsigned short pin, unsigned short value);
unsigned short PinControlAnalogRead(unsigned short pin);

#endif // PIN_CONTROL_H
