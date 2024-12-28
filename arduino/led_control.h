#ifndef LED_CONTROL_H
#define LED_CONTROL_H

unsigned short LEDOn(unsigned short pin);
unsigned short LEDOff(unsigned short pin);
unsigned short LEDBlink(unsigned short pin, unsigned long interval);
unsigned short LEDOneShot(unsigned short pin, unsigned long duration);
unsigned short LEDPattern(unsigned short pin);
unsigned short LEDUpdate(void);

#endif // LED_CONTROL_H
