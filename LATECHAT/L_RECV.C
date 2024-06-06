/* s_recv.c
**
** Copyright (c) 1990, Christopher Laforet
** All Rights Reserved
**
** Started: 2 January 1990
**
** Revision Information: $Logfile:   D:/mine/bbs/vcs/s_recv.c_v  $
**                       $Date:   10 Dec 1990 02:24:52  $
**                       $Revision:   1.9  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#ifdef PROTECTED
	#define INCL_DOSPROCESS
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include "latechat.h"




void pascal purge_input(int port)
	{
    while (peek_input(Port) != -1)
		read_input(port);
	purge_input_buffer(port);
	}



int get_kb(void)
	{
	if (peek_kb() != 0xffff)
		return (unsigned int)read_kb();
#ifdef PROTECTED
	DosSleep(0L);
#endif
	return 0;
	}



int pascal recv_char(int timeout)
	{
	long retry = 0;
	long max_timeout = (long)timeout * 20L;
	register quit = 0;
	int rtn = -1;

	do
		{
        if (!check_cd(Port))
            exit(0);
        else if (peek_input(Port) != -1)
			{
            rtn = read_input(Port);
			++quit;
			}
		else if (timeout)
			{
            /* sleep(50); xxx */
			++retry;
			if (retry >= max_timeout)
				++quit;
			}
		else
			quit = 1;
		}
	while (!quit);

	return rtn;
	}




int get_char(void)
	{
	int key;
	long total_inactive;
	
        total_inactive = Remaining;     /* 5 minutes default */

	while (1)
		{
        if (!check_cd)
            exit (0);
		else if (key = get_kb())
			return key;
        else if (peek_input(Port) != -1)
            return read_input(Port);
		}
	}



void hangup(void)
	{
    lower_dtr(Port);    /* force hangup */
    sleep(1000);
    raise_dtr(Port);
    if (check_cd(Port))
		{
        lower_dtr(Port);
        sleep(1000);
        raise_dtr(Port);
        purge_output(Port);
        purge_input(Port);
		}
	}



void get_reason(char *string)
	{
	char buffer[3];
	char *cptr;
	int key;
	int quit = 0;

	cptr = string;
	do
		{
		key = get_char();
		if (key == '\b' || key == 0x7f)
			{
			if (cptr > string)
				{
				--cptr;
				send_string("\b \b");
				}
			}
		else if (key == '\r' || key == '\n')
			{
			White;
			send_string("\r\n");
			quit = 1;
			}
		else if (key >= ' ' && !(key & 0xff00))
			{
			if (cptr < (string + 40))
				{
				*cptr = (char)key;
				sprintf(buffer,"%c",*cptr);
				send_string(buffer);
				++cptr;
				}
			}
		}
	while (!quit);
	*cptr = '\0';
	}

