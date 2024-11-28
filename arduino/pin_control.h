#ifndef PIN_CONTROL_H
#define PIN_CONTROL_H

// コールバック関数の型定義
typedef void (*ButtonCallback)(void);

unsigned short PinControlDigitalWrite(unsigned short pin, unsigned short value);
unsigned short PinControlDigitalRead(unsigned short pin, short* value);
unsigned short PinControlAnalogWrite(unsigned short pin, unsigned short value);
unsigned short PinControlAnalogRead(unsigned short pin, short* value);


// ボタン制御用関数
unsigned short PinControlButtonInit(unsigned short pin, ButtonCallback callback);
unsigned short PinControlButtonRead(void);

#endif // PIN_CONTROL_H