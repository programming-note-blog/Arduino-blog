#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <random>
#define INPUT dmyINPUT
#include <windows.h>
#undef INPUT //windows.h の INPUT を解除
#include <iostream>
#include <cstring>
#include <map>

#define PIN_COUNT 22

class Pin {
public:
    PinMode Mode;
    int Value;
    bool isAnalogWrite; // アナログライトされたかを記録
    char Name[64];

    Pin(int value, const char* p_Name)
        : Value(value), isAnalogWrite(false) {
        strncpy(Name, p_Name, sizeof(Name) - 1);
        Name[sizeof(Name) - 1] = '\0'; // null終端
    }

    ~Pin() {}

    void setValue(int value) { Value = value; }
    int getValue() const { return Value; }
};

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

// ピンリストの初期化
Pin g_PinList[PIN_COUNT] = {
    {1023, "reserved"},
    {1023, "reserved"},
    {1023, "D2"},
    {1023, "D3"},  // PWM
    {1023, "D4"},
    {1023, "D5"},  // PWM
    {1023, "D6"},  // PWM
    {1023, "D7"},
    {1023, "D8"},
    {1023, "D9"},  // PWM
    {1023, "D10"}, // PWM
    {1023, "D11"},
    {1023, "D12"},
    {1023, "D13"},
    {1023, "D14"},
    {1023, "D15"},
    {1023, "D16"},
    {1023, "D17"},
    {1023, "D18"},
    {1023, "D19"},
    {1023, "D20"},
    {1023, "D21"}
};

unsigned short dmy_cmd_PrintPinList(unsigned short argc, char** argv) {
    for (int i = 2; i < PIN_COUNT; i++) {
        printf("%3s: %3d\n", g_PinList[i].Name, g_PinList[i].Value);
    }
    return 0;
}

void pinMode(pin_size_t pinNumber, PinMode pinMode) {
    if (pinNumber < PIN_COUNT) {
        g_PinList[pinNumber].Mode = pinMode;
    }
}

// PWMが許可されているピン番号
std::map<int, bool> pwmPins = {
    {3, true}, {5, true}, {6, true}, {9, true}, {10, true}
};

// デジタルライト
void digitalWrite(pin_size_t pinNumber, PinStatus status) {
    if (pinNumber < 2 || pinNumber >= PIN_COUNT) {
        std::cerr << "Error: Invalid pin number" << std::endl;
        return;
    }

    // アナログライトが適用されているピンではデジタルライトを禁止
    if (g_PinList[pinNumber].isAnalogWrite) {
        std::cerr << "Error: Cannot digitalWrite to a pin after analogWrite." << std::endl;
        return;
    }

    g_PinList[pinNumber].Value = (status == HIGH) ? 1023 : 0;
    g_PinList[pinNumber].isAnalogWrite = false; // デジタルライトとして扱う
    //std::cout << "Pin " << g_PinList[pinNumber].Name << " set to " << (status == HIGH ? "HIGH" : "LOW") << std::endl;
}

// アナログライト
void analogWrite(pin_size_t pinNumber, int value) {
    if (pinNumber < 2 || pinNumber >= PIN_COUNT) {
        std::cerr << "Error: Invalid pin number" << std::endl;
        return;
    }

    // PWM可能なピンかチェック
    if (pwmPins.find(pinNumber) == pwmPins.end()) {
        std::cerr << "Error: analogWrite not supported on pin " << g_PinList[pinNumber].Name << std::endl;
        return;
    }

    if (value < 0 || value > 255) {
        std::cerr << "Error: Value out of range (0-255)" << std::endl;
        return;
    }

    g_PinList[pinNumber].Value = value * 4; // 0-255 を 0-1023 にスケーリング
    g_PinList[pinNumber].isAnalogWrite = true; // アナログライトされたことを記録
    std::cout << "Pin " << g_PinList[pinNumber].Name << " set to PWM value " << value << std::endl;
}

// デジタルリード
PinStatus digitalRead(pin_size_t pinNumber) {
    if (pinNumber < 2 || pinNumber >= PIN_COUNT) {
        std::cerr << "Error: Invalid pin number" << std::endl;
        return LOW;
    }

    // アナログライト後のピンは値を保証しない
    if (g_PinList[pinNumber].isAnalogWrite) {
        std::cerr << "Warning: digitalRead called on a pin after analogWrite, value is not guaranteed." << std::endl;
    }

    return g_PinList[pinNumber].Value > 512 ? HIGH : LOW;
}

// アナログリード
int analogRead(pin_size_t pinNumber) {
    if (pinNumber < 2 || pinNumber >= PIN_COUNT) {
        std::cerr << "Error: Invalid pin number" << std::endl;
        return 0;
    }

    return g_PinList[pinNumber].Value / 4;
}

unsigned long millis(void) {
    return clock();
}

std::mt19937 gen;

void randomSeed(long seed) {
    gen.seed(seed);
}

long random(long max) {
    std::uniform_int_distribution<> dist(0, max - 1);
    return dist(gen);
}
