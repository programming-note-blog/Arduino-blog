#ifndef LED_CONTROL_H
#define LED_CONTROL_H

unsigned short LedOn(unsigned short pin);
unsigned short LedOff(unsigned short pin);
unsigned short LedBlink(unsigned short pin, unsigned long interval);
unsigned short LedOneShot(unsigned short pin, unsigned long duration);
unsigned short LedPattern(unsigned short pin);
unsigned short LedLoop(void);

#endif // LED_CONTROL_H
