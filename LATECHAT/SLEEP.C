#include <stdio.h>
#include <io.h>
#ifdef PROTECTED
	#define INCL_DOS
	#include <os2.h>
#endif


#ifndef PROTECTED

static long cpu_speed = 0L;


void get_cpu_time(void)		/* cannot be loop optimized!!!!!! */
	{
	int far *clock = (int far *)0x40006C;
	int count;

	for (count = *clock; count == *clock; )
		/* do nothing */;
	count = *clock;
	for (cpu_speed = 0L; count == *clock; )
		cpu_speed++;
	}



void sleep(int time)
	{
    long count, dur;

	if (cpu_speed == 0L)
		get_cpu_time();
	dur = (cpu_speed / 9L) * (long)time;
	for (count = 0; count <= dur;)
		++count;
	}


#else

void sleep(int time)
	{
	DosSleep((long)time);
	}

#endif
