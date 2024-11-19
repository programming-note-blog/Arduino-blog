#include <stdio.h>
#include <thread>
#define INPUT tmpINPUT
#include <windows.h>
#undef INPUT

#include "arduino.ino"

static void ThreadA(void);

int main(void)
{
	std::thread th_a(ThreadA);

	setup();

	for(;;)
	{
		loop();
	}

	return 0;
}

extern void dmy_SerialSetLine(const char* str);
static void ThreadA(void)
{
	char buffer[256];

	for(;; )
	{
		fgets(buffer, 256, stdin);

		dmy_SerialSetLine(buffer);
	}
}
