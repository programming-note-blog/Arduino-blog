#include <Arduino.h>
#include "pin_control.h"

#define MAX_PIN_COUNT 22

// Array to store the mode of each pin
unsigned short pinModes[MAX_PIN_COUNT] = {
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT,
	OUTPUT};

/**
 * @brief 指定されたピンのモードを設定します。
 *
 * @param pin 設定するピン番号。
 * @param mode 設定するモード(INPUT, INPUT_PULLUP, OUTPUT)。
 * @return 0: 成功, 1: エラー(無効なピンまたはモード)。
 */
unsigned short PinControlSetMode(Pin pin, unsigned short mode)
{
	if (pin >= MAX_PIN_COUNT)
	{
		return 1; // Error: Pin out of range
	}
	if (mode != INPUT && mode != INPUT_PULLUP && mode != OUTPUT)
	{
		return 1; // Error: Invalid mode
	}
	pinMode((Pin)pin, (PinMode)mode);
	pinModes[pin] = mode; // Store the mode
	return 0;			  // Success
}

/**
 * @brief 指定されたピンにデジタル値を出力します。
 *
 * @param pin 出力するピン番号。
 * @param value 出力する値(LOW, HIGH)。
 * @return 0: 成功, 1: エラー(無効なピンまたは値)。
 */
unsigned short PinControlDigitalWrite(Pin pin, unsigned short value)
{
	if (pin >= MAX_PIN_COUNT)
	{
		return 1; // Error: Pin out of range
	}
	if (value != LOW && value != HIGH)
	{
		return 1; // Error: Invalid value
	}

	unsigned short originalMode = pinModes[pin]; // Save original mode
	if (originalMode != OUTPUT)
	{
		Serial.print("Warning: Changing pin ");
		Serial.print(pin);
		Serial.print(" mode from ");
		Serial.print(originalMode == INPUT ? "INPUT" : (originalMode == INPUT_PULLUP ? "INPUT_PULLUP" : "UNKNOWN"));
		Serial.println(" to OUTPUT");

		pinModes[pin] = OUTPUT;
	}

	pinMode((Pin)pin, OUTPUT);
	digitalWrite((Pin)pin, (PinStatus)value);

	return 0; // Success
}

/**
 * @brief 指定されたピンからデジタル値を読み取ります。
 *
 * @param pin 読み取るピン番号。
 * @param value 読み取った値を格納するポインタ。
 * @return 0: 成功, 1: エラー(無効なピンまたはポインタがnull)。
 */
unsigned short PinControlDigitalRead(Pin pin, short *value)
{
	if (pin >= MAX_PIN_COUNT)
	{
		return 1; // Error: Pin out of range
	}
	if (value == nullptr)
	{
		return 1; // Error: Null pointer
	}

	unsigned short originalMode = pinModes[pin]; // Save original mode
	pinMode((Pin)pin, INPUT);
	*value = digitalRead(pin);
	pinMode((Pin)pin, (PinMode)originalMode); // Restore original mode

	return 0; // Success
}

/**
 * @brief 指定されたピンにアナログ値を出力します。
 *
 * @param pin 出力するピン番号。
 * @param value 出力するアナログ値(0-255)。
 * @return 0: 成功, 1: エラー(無効なピン)。
 */
unsigned short PinControlAnalogWrite(Pin pin, unsigned short value)
{
	if (pin >= MAX_PIN_COUNT)
	{
		return 1; // Error: Pin out of range
	}
	analogWrite((Pin)pin, value);
	return 0; // Success
}

/**
 * @brief 指定されたピンからアナログ値を読み取ります。
 *
 * @param pin 読み取るピン番号。
 * @return アナログ値(0-1023), エラー時は1を返します。
 */
unsigned short PinControlAnalogRead(Pin pin)
{
	if (pin >= MAX_PIN_COUNT)
	{
		return 1; // Error: Pin out of range
	}

	return analogRead(pin); // Success
}
