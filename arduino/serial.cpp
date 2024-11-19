#include <Arduino.h>

#include "serial.h"
#include "command.h"

#define READ_BUFFER_SIZE 64
#define ARGC_MAX 8

#define IS_TERMINATION(c) ((c) == '\r' || (c) == '\n' || (c) == '\0')
#define IS_SPACE(c)       ((c) == ' ')

static char ReadBuffer[READ_BUFFER_SIZE];
static char* pReadBufferEnd;
static unsigned short ArgC;
static char* ArgV[ARGC_MAX];

static unsigned short get_arg(char* line, char** argv);


void SerialInit(void)
{
  pReadBufferEnd = &ReadBuffer[0];
}


void SerialRead(void)
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
    Serial.println();
    
    *pReadBufferEnd = '\0';

    ArgC = get_arg(&ReadBuffer[0], ArgV);
    CommandExecute(ArgC, ArgV);

    pReadBufferEnd = &ReadBuffer[0];

    Serial.print("> ");

    return;
  }

  pReadBufferEnd++;
}


static unsigned short get_arg(char* line, char** argv)
{
  unsigned short i;
  unsigned short argc = 0;

	// 1文字目
	if(IS_SPACE(line[0]) || IS_TERMINATION(line[0]))
	{
		return argc;
	}
	else
	{
		argv[argc] = &line[0];
		argc++;
	}

  // 2文字目以降
  for(i = 1; i < READ_BUFFER_SIZE; i++)
  {
    if(IS_TERMINATION(line[i]))
    {
      line[i] = '\0';
      return argc;
    }

    if(IS_SPACE(line[i]))
    {
      line[i] = '\0';
      continue;
    }

    if(IS_TERMINATION(line[i - 1]))
    {
      argv[argc] = &line[i];
      argc++;

      if(argc == ARGC_MAX)
      {
        return ARGC_MAX - 1;
      }
    }
  }

  Serial.println("[error]line without end");
  return 0;
}
