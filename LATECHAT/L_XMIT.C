/* s_xmit.c
**
** Copyright (c) 1989, Christopher Laforet
** All Rights Reserved
**
** Started: 27 November 1989
**
** Revision Information: $Logfile:   D:/mine/bbs/vcs/s_xmit.c_v  $
**                       $Date:   10 Dec 1990 02:25:28  $
**                       $Revision:   1.13  $
**
*/


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#ifdef PROTECTED
	#define INCL_DOSPROCESS
	#include <os2.h>
#endif
#include "latechat.h"




			  
void pascal purge_output(int port)
	{
	purge_output_buffer(port);
	}





void pascal send_char_fast(char character)
	{
	while (1)
		{
        if (!check_cd(Port))
            exit (0);
        if (output_nowait(Port,character))      /* out to comm port */
			break;
		}
	}



void pascal send_char(char character)
	{
	register sent = 0;

	do
		{
        if (!check_cd(Port))
            exit (0);
        else
        if (!check_output(Port))       /* transmitter almost empty */
			{
            output_wait(Port,character);        /* out to comm port */
			++sent;
			}
		}
	while (!sent);
	}



int send_string(char *string)
	{
    return send_buffer(string,strlen(string));
	}



int send_buffer(char *buf,int buflen)
	{
	int current = 0;
	int sent;

	while (current < buflen)
		{
		sent = 0;
		do
			{
            if (!check_cd(Port))
                exit (0);
            else        /* transmitter almost empty */
				{
                output_wait(Port,buf[current]);     /* out to comm port */
				fputc(buf[current],stderr);
				fflush(stderr);
				++sent;
				}
#ifdef PROTECTED
			DosSleep(0L);
#endif
			}
		while (!sent);
        ++current;
        }
    return 0;
    }


void Locate(int x,int y)
	{
	char TempBuf [15];

	sprintf(TempBuf, "\033[%d;%dH",y,x);   /* Cursor positioner */
	send_string(TempBuf);
	}

