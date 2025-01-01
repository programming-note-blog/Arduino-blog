#include <CDC.h>
#include <stdio.h>
#include <string.h>

class Serial_ Serial;

static unsigned long gs_Speed;
static char gs_Buffer[256];
static int gs_Avarable;

void dmy_SerialSetLine(const char* str)
{
	uint32_t i;
	for(i = 0; i < 256; i++)
	{
		gs_Buffer[i] = str[i];
		if(str[i]=='\n')
		{
			break;
		}
	}
	gs_Avarable = i + 1;
}

size_t Serial_::println(const char* str)
{
	return printf(str)+printf("\n");
}
size_t Serial_::println(const char Char)
{
	return printf("%d",Char) + printf("\n");
}

size_t Serial_::println(unsigned char value, int base)
{
	if (base == 2)
	{
		return printf("%c%c%c%c%c%c%c%c\n", value & 128 ? '1' : '0', value & 64 ? '1' : '0', value & 32 ? '1' : '0', value & 16 ? '1' : '0', value & 8 ? '1' : '0', value & 4 ? '1' : '0', value & 2 ? '1' : '0', value & 1 ? '1' : '0');
	}
	//TODO:バイナリ以外の形式
	return 0;
}

// コンパイラによっては出力されない
size_t Serial_::print(const char* str)
{
	return printf(str);
}
size_t Serial_::print(const char)
{
	return 0;
}

size_t Serial_::print(int num)
{
	return printf("%d",num);
}

size_t Serial_::println(int num)
{
	size_t n = printf("%d",num);
	n += println();
	return n;
}

size_t Serial_::print(unsigned int num)
{
	return printf("%u",num);
}

size_t Serial_::println(unsigned int num)
{
	size_t n = printf("%u",num);
	n += println();
	return n;
}

size_t Serial_::print(unsigned long num)
{
	return printf("%u",num);
}

size_t Serial_::println(unsigned long num)
{
	size_t n = printf("%u",num);
	n += println();
	return n;
}

size_t Serial_::println(void)
{
	return printf("\n");
}

void  Serial_::begin(unsigned long Speed)
{
	gs_Speed = Speed;
}
int  Serial_::available(void)
{
	return gs_Avarable;
}
int  Serial_::read(void)
{
	char Ret = gs_Buffer[0];

	if(gs_Avarable <=0)
	{
		return 0;
	}

	gs_Avarable--;
	memcpy(gs_Buffer, &gs_Buffer[1],sizeof(gs_Buffer) - 1);
	
	return Ret;
}

Serial_::operator bool(void)
{
	return this;
}

int  Serial_::peek(void)
{
	return 0;
}
int  Serial_::availableForWrite(void)
{
	return 0;
}
void  Serial_::flush(void)
{}
// unsigned int  Serial_::write(unsigned char)
// {
// 	return 0;
// }
// unsigned int  Serial_::write(unsigned char const*, unsigned int)
// {
// 	return 0;
// }