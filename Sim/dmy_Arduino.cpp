#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define INPUT dmyINPUT /* ArduinoÇ∆ä±è¬Ç∑ÇÈÇÃÇ≈âÒî */
#include <windows.h>
#undef INPUT

/*    ARDUINO-NANO-EVERY ADC Pins
 *
 *             ___________________
 * (AIN10) A9 |D13    |____|   D12| A10 (AIN9) MISO
 *            |3V3             D11| A11 (AIN8) MOSI
 * (AIN7)  A8 |AREF(D39)      ~D10|            SS
 * (AIN3)     |A0  (D14)       ~D9|
 * (AIN2)     |A1  (D15)        D8| A12 (AIN11)
 * (AIN1)     |A2  (D16)        D7|
 * (AIN0)     |A3  (D17)       ~D6| A13 (AIN14)
 * (AIN12)    |A4  (D18)       ~D5|
 * (AIN13)    |A5  (D19)        D4|
 * (AIN4)     |A6  (D20)       ~D3| A14 (AIN15)
 * (AIN5)     |A7  (D21)        D2|
 *            |5V              GND|
 *            |RES             RES|
 *            |GND              RX|
 *            |VIN              TX|
 *            |___________________|
 */

class Pin
{
public:
	PinMode Mode;
	int Value;
	char Name[64];
	Pin(int Value, const char* p_Name)
		:Value(Value)
	{
		memcpy(Name, p_Name, sizeof(Name));
	};
	~Pin()
	{};

private:

};

Pin PinLists[22] =
{
	{1023,"reserved"},
	{1023,"reserved"},
	{1023,"D2"},
	{1023,"D3"},
	{1023,"D4"},
	{1023,"D5"},
	{1023,"D6"},
	{1023,"D7"},
	{1023,"D8"},
	{1023,"D9"},
	{1023,"D10"},
	{1023,"D11"},
	{1023,"D12"},
	{1023,"D13"},
	{1023,"D14"},
	{1023,"D15"},
	{1023,"D16"},
	{1023,"D17"},
	{1023,"D18"},
	{1023,"D19"},
	{1023,"D20"},
	{1023,"D21"},
};

unsigned short dmy_cmd_PrintPinList(unsigned short argc, char** argv)
{
	for(int i = 2; i < 22; i++)
	{
		printf("%3s:%3d\n", PinLists[i].Name, PinLists[i].Value);
	}
	return 0;
}

void pinMode(pin_size_t pinNumber, PinMode pinMode)
{
	PinLists[pinNumber].Mode = pinMode;
}

PinStatus digitalRead(pin_size_t pinNumber)
{
	/* 5VÅ~0.3=1.5V à»â∫Ç 0 (LOW)  Ç∆îFéØ
	 * 5VÅ~0.6=3V   à»è„Ç 1 (HIGH) Ç∆îFéØ
	 * íÜä‘ÇÕñ¢íË */
	return PinLists[pinNumber].Value > 512 ? HIGH : LOW;
}

int analogRead(pin_size_t pinNumber)
{
	/*  0VÅ`5V Ç 1024íiäKÇ…ïœä∑ */
	return PinLists[pinNumber].Value;
}

int ValueRead(pin_size_t pinNumber)
{
	return PinLists[pinNumber].Value;
}

void digitalWrite(pin_size_t pinNumber, PinStatus status)
{
	PinLists[pinNumber].Value = status == HIGH ? 1023 : 0;
}

void analogWrite(pin_size_t pinNumber, int value)
{
	PinLists[pinNumber].Value = value;
}

void delay(unsigned long ms)
{
	Sleep(ms);
}

unsigned long millis(void)
{
	return clock()/100; //?????????????????
}

#include <random>
long random(long max)
{
	return rand() % max;
}

void randomSeed(long seed)
{
	std::srand(seed);
}
