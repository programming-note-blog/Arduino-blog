#define BIT_RATE 9600
#define READ_BUFFER_SIZE 64

#define IS_TERMINATION(c) ((c) == '\r' || (c) == '\n' || (c) == '\0')

static char ReadBuffer[READ_BUFFER_SIZE];
static char* pReadBufferEnd;

void setup()
{
  pReadBufferEnd = &ReadBuffer[0];

  Serial.begin(BIT_RATE);
  Serial.println("Hello World");
}

void loop()
{
  if(Serial.available() <= 0)
  {
    return;
  }

  if(pReadBufferEnd == &ReadBuffer[READ_BUFFER_SIZE])
  {
    ReadBuffer[READ_BUFFER_SIZE - 1] = '\0';
  }
  else
  {
    *pReadBufferEnd = (char)Serial.read();
  }

  if(IS_TERMINATION(*pReadBufferEnd))
  {
    *pReadBufferEnd = '\0';
    Serial.println(ReadBuffer);
    pReadBufferEnd = &ReadBuffer[0];

    return;
  }

  pReadBufferEnd++;
}
