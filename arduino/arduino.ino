#include "Arduino.h"
#include "serial.h"

void setup()
{
  SerialInit();
}

void loop()
{
  SerialRead();
}
