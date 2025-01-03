#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H

typedef void (*ButtonCallback)(void);

unsigned short ButtonSetup(unsigned short pin, ButtonCallback callback);
unsigned short ButtonLoop(void);

#endif // BUTTON_CONTROL_H
