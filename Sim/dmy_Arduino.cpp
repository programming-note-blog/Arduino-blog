#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <random>
#define INPUT dmyINPUT
#include <windows.h>
#undef INPUT //windows.h の INPUT を解除

#define PIN_COUNT 22

class Pin {
public:
    PinMode Mode;
    int Value;
    char Name[64];

    Pin(int value, const char* p_Name)
        : Value(value) {
        strncpy(Name, p_Name, sizeof(Name) - 1);
        Name[sizeof(Name) - 1] = '\0'; // null終端
    }

    ~Pin() {}

    void setValue(int value) { Value = value; }
    int getValue() const { return Value; }
};

Pin g_PinList[PIN_COUNT] = {
    {1023, "reserved"},
    {1023, "reserved"},
    {1023, "D2"},
    {1023, "D3"},
    {1023, "D4"},
    {1023, "D5"},
    {1023, "D6"},
    {1023, "D7"},
    {1023, "D8"},
    {1023, "D9"},
    {1023, "D10"},
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
    {1023, "D21"},
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

PinStatus digitalRead(pin_size_t pinNumber) {
    if (pinNumber < PIN_COUNT) {
        return g_PinList[pinNumber].getValue() > 512 ? HIGH : LOW;
    }
    return LOW;
}

int analogRead(pin_size_t pinNumber) {
    if (pinNumber < PIN_COUNT) {
        return g_PinList[pinNumber].getValue();
    }
    return 0;
}

void digitalWrite(pin_size_t pinNumber, PinStatus status) {
    if (pinNumber < PIN_COUNT) {
        g_PinList[pinNumber].setValue(status == HIGH ? 1023 : 0);
    }
}

void analogWrite(pin_size_t pinNumber, int value) {
    if (pinNumber < PIN_COUNT) {
        g_PinList[pinNumber].setValue(value);
    }
}

void delay(unsigned long ms) {
    Sleep(ms);
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
