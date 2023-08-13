#include "command.h"

#define BIT_RATE 9600

void setup()
{
  CommandInit();

  Serial.begin(BIT_RATE);
  Serial.println("Hello World");

  Serial.print("> ");
}

void loop()
{
  CommandSerialRead();
}
