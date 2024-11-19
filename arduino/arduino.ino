#include "Arduino.h"
#include "command.h"
#include "serial.h"

#define BIT_RATE 9600

void setup()
{
  SerialInit();

  Serial.begin(BIT_RATE);
  Serial.println("Hello World");

  Serial.print("> ");
}

void loop()
{
  SerialRead();
}
