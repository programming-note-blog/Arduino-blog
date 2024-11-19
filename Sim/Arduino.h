#ifndef ARDUINO_H_
#define ARDUINO_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// dmy_Arduino.cpp
unsigned short dmy_cmd_PrintPinList(unsigned short argc, char** argv);

/* Arduino.h */
#undef F
#define F(str) (str)

#define INTERNAL0V55 (VREF_ADC0REFSEL_0V55_gc >> VREF_ADC0REFSEL_gp)
#define INTERNAL1V1 (VREF_ADC0REFSEL_1V1_gc >> VREF_ADC0REFSEL_gp)
#define INTERNAL2V5 (VREF_ADC0REFSEL_2V5_gc >> VREF_ADC0REFSEL_gp)
#define INTERNAL4V3 (VREF_ADC0REFSEL_4V34_gc >> VREF_ADC0REFSEL_gp)
#define INTERNAL1V5 (VREF_ADC0REFSEL_1V5_gc >> VREF_ADC0REFSEL_gp)

#define DEFAULT     INTERNAL0V55
#define INTERNAL    ADC_REFSEL_INTREF_gc
#define VDD         ADC_REFSEL_VDDREF_gc
#define EXTERNAL    ADC_REFSEL_VREFA_gc

#define VCC_5V0 2
#define VCC_3V3 1
#define VCC_1V8 0

#define interrupts() sei()
#define noInterrupts() cli()

	 // avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

/* Allows performing a correction on the CPU value using the signature row
	values indicating oscillator error provided from the device manufacturer */
#define PERFORM_SIGROW_CORRECTION_F_CPU 0

	/* Variable containing corrected F_CPU value, after checks for safe operating
		frequency vs supply voltage, oscillator fuse setting and MCLK divider.
		Also includes the correction from signature row values if above #define
		PERFORM_SIGROW_CORRECTION_F_CPU = 1 */
	extern uint32_t F_CPU_CORRECTED;

	uint16_t clockCyclesPerMicrosecondComp(uint32_t clk);
	uint16_t clockCyclesPerMicrosecond();
	unsigned long clockCyclesToMicroseconds(unsigned long cycles);
	unsigned long microsecondsToClockCycles(unsigned long microseconds);

	// Get the bit location within the hardware port of the given virtual pin.
	// This comes from the pins_*.c file for the active board configuration.

	extern const uint8_t digital_pin_to_port[];
	extern const uint8_t digital_pin_to_bit_mask[];
	extern const uint8_t digital_pin_to_bit_position[];
	extern const uint8_t digital_pin_to_timer[];
	extern const uint8_t analog_pin_to_channel[];

	// Get the bit location within the hardware port of the given virtual pin.
	// This comes from the pins_*.c file for the active board configuration.
	//
	// These perform slightly better as macros compared to inline functions
	//
#define NOT_A_PIN 255
#define NOT_A_PORT 255
#define NOT_AN_INTERRUPT 255

#define PA 0
#define PB 1
#define PC 2
#define PD 3
#define PE 4
#define PF 5
#define NUM_TOTAL_PORTS 6

#define NOT_ON_TIMER 0
#define TIMERA0 1
#define TIMERB0 2
#define TIMERB1 3
#define TIMERB2 4
#define TIMERB3 5

	void setup_timers();
	bool isDoubleBondedActive(uint8_t pin);

#define digitalPinToPort(pin) ( (pin < NUM_TOTAL_PINS) ? digital_pin_to_port[pin] : NOT_A_PIN )
#define digitalPinToBitPosition(pin) ( (pin < NUM_TOTAL_PINS) ? digital_pin_to_bit_position[pin] : NOT_A_PIN )
#define digitalPinToBitMask(pin) ( (pin < NUM_TOTAL_PINS) ? digital_pin_to_bit_mask[pin] : NOT_A_PIN )
#define digitalPinToTimer(pin) ( (pin < NUM_TOTAL_PINS) ? digital_pin_to_timer[pin] : NOT_ON_TIMER )
#define analogPinToBitPosition(pin) ( (digitalPinToAnalogInput(pin) != NOT_A_PIN) ? digital_pin_to_bit_position[pin + ANALOG_INPUT_OFFSET] : NOT_A_PIN )
#define analogPinToBitMask(pin) ( (digitalPinToAnalogInput(pin) != NOT_A_PIN) ? digital_pin_to_bit_mask[pin + ANALOG_INPUT_OFFSET] : NOT_A_PIN )

#define portToPortStruct(port) ( (port < NUM_TOTAL_PORTS) ? ((PORT_t *)&PORTA + port) : NULL)
#define digitalPinToPortStruct(pin) ( (pin < NUM_TOTAL_PINS) ? ((PORT_t *)&PORTA + digitalPinToPort(pin)) : NULL)
#define getPINnCTRLregister(port, bit_pos) ( ((port != NULL) && (bit_pos < NOT_A_PIN)) ? ((volatile uint8_t *)&(port->PIN0CTRL) + bit_pos) : NULL )
#define digitalPinToInterrupt(P) (P)

#define portOutputRegister(P) ( (volatile uint8_t *)( &portToPortStruct(P)->OUT ) )
#define portInputRegister(P) ( (volatile uint8_t *)( &portToPortStruct(P)->IN ) )
#define portModeRegister(P) ( (volatile uint8_t *)( &portToPortStruct(P)->DIR ) )



/* Common.h */

void yield(void);

typedef enum
{
	LOW = 0,
	HIGH = 1,
	CHANGE = 2,
	FALLING = 3,
	RISING = 4,
} PinStatus;

typedef enum
{
	INPUT = 0x0,
	OUTPUT = 0x1,
	INPUT_PULLUP = 0x2,
	INPUT_PULLDOWN = 0x3,
} PinMode;

typedef enum
{
	LSBFIRST = 0,
	MSBFIRST = 1,
} BitOrder;

#define PI          3.1415926535897932384626433832795
#define HALF_PI     1.5707963267948966192313216916398
#define TWO_PI      6.283185307179586476925286766559
#define DEG_TO_RAD  0.017453292519943295769236907684886
#define RAD_TO_DEG  57.295779513082320876798154814105
#define EULER       2.718281828459045235360287471352

#define SERIAL      0x0
#define DISPLAY     0x1

#ifndef constrain
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#endif

#ifndef radians
#define radians(deg) ((deg)*DEG_TO_RAD)
#endif

#ifndef degrees
#define degrees(rad) ((rad)*RAD_TO_DEG)
#endif

#ifndef sq
#define sq(x) ((x)*(x))
#endif

typedef void (*voidFuncPtr)(void);
typedef void (*voidFuncPtrParam)(void*);

// interrupts() / noInterrupts() must be defined by the core

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#ifndef bit
#define bit(b) (1UL << (b))
#endif

typedef uint8_t   byte;
typedef uint16_t  word;

void init(void);
void initVariant(void);

#ifdef EXTENDED_PIN_MODE
// Platforms who wnat to declare more than 256 pins need to define EXTENDED_PIN_MODE globally
typedef uint32_t pin_size_t;
#else
typedef uint8_t pin_size_t;
#endif


void pinMode(pin_size_t pinNumber, PinMode pinMode);
void digitalWrite(pin_size_t pinNumber, PinStatus status);
PinStatus digitalRead(pin_size_t pinNumber);
int analogRead(pin_size_t pinNumber);
void analogReference(uint8_t mode);
void analogWrite(pin_size_t pinNumber, int value);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);
void delayMicroseconds(unsigned int us);
unsigned long pulseIn(pin_size_t pin, uint8_t state, unsigned long timeout);
unsigned long pulseInLong(pin_size_t pin, uint8_t state, unsigned long timeout);

void shiftOut(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder, uint8_t val);
pin_size_t shiftIn(pin_size_t dataPin, pin_size_t clockPin, BitOrder bitOrder);

void attachInterrupt(pin_size_t interruptNumber, voidFuncPtr callback, PinStatus mode);
void attachInterruptParam(pin_size_t interruptNumber, voidFuncPtrParam callback, PinStatus mode, void* param);
void detachInterrupt(pin_size_t interruptNumber);

long random(long max);
long random(long min, long max);
void randomSeed(long seed);

#include <CDC.h>
//#include <pins_arduino.h>

#endif /* ARDUINO_H_ */
