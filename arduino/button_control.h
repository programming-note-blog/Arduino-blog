#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

typedef void (*ButtonCallback)(void);

unsigned short ButtonInit(unsigned short pin, ButtonCallback callback);
unsigned short ButtonRead(void);

#endif // BUTTON_CONTROL_H
