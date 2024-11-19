#include <Arduino.h>

#include "command.h"

#ifdef SUCCESS
#undef SUCCESS
#endif
#define SUCCESS 0
#ifdef ERROR
#undef ERROR
#endif
#define ERROR   1

#define IS_ERROR(code)    ((code) != SUCCESS)

static unsigned short cmd_echo(unsigned short argc, char** argv);
static unsigned short cmd_dw(unsigned short argc, char** argv);
static unsigned short cmd_dr(unsigned short argc, char** argv);
static unsigned short cmd_aw(unsigned short argc, char** argv);
static unsigned short cmd_ar(unsigned short argc, char** argv);

typedef struct tCommand
{
  const char* key;                                                // キーとなる単語
  unsigned short (*func)(unsigned short argc, char** argv);       // 呼び出す関数
  const char* usage;                                              // 使い方
} SCommand;

static const SCommand COMMAND_TABLE[] =                           // コマンドテーブル
{
  {"echo", cmd_echo, "echo <prm>            :display prm"},
  {"dw",   cmd_dw,   "dw <pin> <value(0/1)> :digitalWrite(pin,value)"},
  {"dr",   cmd_dr,   "dr <pin>              :digitalRead(pin)"},
  {"aw",   cmd_aw,   "aw <pin> <dec>        :analogWrite(pin,dec)"},
  {"ar",   cmd_ar,   "ar <pin>              :analogRead(pin)"},
};

void CommandExecute(unsigned short argc, char** argv)
{
  unsigned short i;
  unsigned short result;

  if (argc == 0)
	{
		return;
	}

  for(i = 0; i < sizeof(COMMAND_TABLE) / sizeof(SCommand); i++)
  {
    if(!strcmp(COMMAND_TABLE[i].key, argv[0]))
    {
      result = COMMAND_TABLE[i].func(argc - 1, &argv[1]);
      if(IS_ERROR(result))
      {
        Serial.println(COMMAND_TABLE[i].usage);
      }
      return;
    }
  }

  for(i = 0; i < sizeof(COMMAND_TABLE) / sizeof(SCommand); i++)
  {
    Serial.println(COMMAND_TABLE[i].usage);
  }
}

static unsigned short cmd_echo(unsigned short argc, char** argv)
{
  unsigned short i;

  if(argc == 0)
  {
    return ERROR;
  }

  Serial.print(argv[0]);

  for(i = 1; i < argc; i++)
  {
    Serial.print(" ");
    Serial.print(argv[i]);
  }
  Serial.println();

  return SUCCESS;
}

static unsigned short cmd_dw(unsigned short argc, char** argv)
{
  unsigned short pin_num;
  unsigned short value;

  if(argc < 2)
  {
    return ERROR;
  }

  pin_num = atoi(argv[0]);
  value = atoi(argv[1]);

  pinMode(pin_num, OUTPUT);
  digitalWrite(pin_num, (PinStatus)value);

  return SUCCESS;
}

static unsigned short cmd_dr(unsigned short argc, char** argv)
{
  unsigned short pin_num;
  short value;

  if(argc < 1)
  {
    return ERROR;
  }

  pin_num = atoi(argv[0]);

  value = digitalRead(pin_num);

  Serial.println(value);

  return SUCCESS;
}

static unsigned short cmd_aw(unsigned short argc, char** argv)
{
  unsigned short pin_num;
  unsigned short value;

  if(argc < 2)
  {
    return ERROR;
  }

  pin_num = atoi(argv[0]);
  value = atoi(argv[1]);

  analogWrite(pin_num, value);

  return SUCCESS;
}

static unsigned short cmd_ar(unsigned short argc, char** argv)
{
  unsigned short pin_num;
  short value;

  if(argc < 1)
  {
    return ERROR;
  }

  pin_num = atoi(argv[0]);

  value = analogRead(pin_num);

  Serial.println(value);

  return SUCCESS;
}
