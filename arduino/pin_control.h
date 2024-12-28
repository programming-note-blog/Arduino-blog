#ifndef PIN_CONTROL_H
#define PIN_CONTROL_H

#define PIN_BUZZER 4
#define PIN_R_MOTOR_1 5
#define PIN_R_MOTOR_2 6
#define PIN_BUTTON 7
#define PIN_L_MOTOR_1 9
#define PIN_L_MOTOR_2 10
#define PIN_LED 13

unsigned short PinControlSetMode(unsigned short pin, unsigned short mode);
unsigned short PinControlDigitalWrite(unsigned short pin, unsigned short value);
unsigned short PinControlDigitalRead(unsigned short pin, short* value);
unsigned short PinControlAnalogWrite(unsigned short pin, unsigned short value);
unsigned short PinControlAnalogRead(unsigned short pin, short* value);

#endif // PIN_CONTROL_H
