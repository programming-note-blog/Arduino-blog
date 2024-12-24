#ifndef PIN_CONTROL_H
#define PIN_CONTROL_H

unsigned short PinControlSetMode(unsigned short pin, unsigned short mode);
unsigned short PinControlDigitalWrite(unsigned short pin, unsigned short value);
unsigned short PinControlDigitalRead(unsigned short pin, short* value);
unsigned short PinControlAnalogWrite(unsigned short pin, unsigned short value);
unsigned short PinControlAnalogRead(unsigned short pin, short* value);

#endif // PIN_CONTROL_H
