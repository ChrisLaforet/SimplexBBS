#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bios.h>
#include <time.h>
#include <process.h>
#ifdef PROTECTED
	#define INCL_DOSPROCESS
	#include <os2.h>
#endif
#include "latechat.h"

int     NowX=1,
        Dummy;

char
     envbuffer1[256],
     *cptr,
     ComSpec [40],
	 RemBuf[80 * MAXLINE],
	 SysBuf[80 * MAXLINE],
     Reason [80],
     NewInt[4],
     Choice = '\0';

unsigned  ModemParams, ModemStatus, data, status;

int       Result1, Result2, Result3, Result4, DoPause (), Port=0, baud,
          Step, CountMax=5, Elapsed=0, OldElapsed, Remaining;

#ifndef PROTECTED
	time_t CurrTime, InitTime;
#endif


int main(int argc,char *argv[])
	{
	int count;

	if (argc < 7)
    	{
#ifdef PROTECTED
        printf("\r\n LateChat/2 (v %d.%02d) -- A split screen chat program. \n",MAJOR_VERSION,MINOR_VERSION);
        printf("                by Chris Laforet and Kelly Hughes '91\n");
    	printf("\n Usage:  LateChat handle baud seconds ansi firstname lastname [string to show]\n\n");
        printf("   Where    handle            =  com port handle being used");
#else
        printf("\r\n LateChat (v %d.%02d) -- A split screen chat program. \n",MAJOR_VERSION,MINOR_VERSION);
        printf("                by Chris Laforet and Kelly Hughes '91\n");
        printf("\n Usage:  LateChat port baud seconds ansi firstname lastname [string to show] \n\n");
        printf("   Where    port              =  com port being used (1=com1 2=com2 etc)");
#endif
    	printf("\n            [baud]            =  baud rate of caller, or 0 if to leave alone.");
    	printf("\n            [seconds]         =  time to give sysop to hit a key");
    	printf("\n            [ansi status]     =  non-zero value if ansi active");
    	printf("\n            [string to show]  =  string to prompt Sysop with");
    	printf("\n\n    Sample Qbbs call for Com1 (type 7) :  \n        *H e:\\qbbs\\latechat.exe 1 *B 12 *G Paging Sysop...\n");
        printf("\n    Sample Simplex call with port passed in (type 240) :  \n        $* latechat $p $b 15 $a $f $l Rousting Sysop...\n");
                                                                           
        printf("\n  Concept of use:  Enforce hours on the normal chat, but provide this \n");
        printf("    chat for higher-priveledged users (ie. friends) that you want to be  \n");
        printf("    able to reach you anytime.  \n");
        printf("  In case you run LateChat from a batch file, here are the exit values \n");
        printf("      0=user abort, chat finish  1=errors  2=time expired  3=sysop abort\n");
        printf("  LateChat watches the carrier automatically.  It does not reboot, since\n");
        printf("  the BBS should detect a dropped carrier as soon as it returns to it.  \n");
    	exit(1);
    	}

#ifdef PROTECTED
	Port = atoi(argv[1]);
#else
	if ((Port = atoi(argv[1])) > 0)
		--Port;
	else
		{
    	printf("\n\n\aInvalid Port number <= 0!\n\n");
		return 1;
		}
#endif
	
	baud = atoi (argv [2]);
	CountMax = atoi (argv [3]);
	if (!atoi (argv [4]))
		{
    	send_string("\n\n\aMust have ANSI enabled.\n\n");
		return 1;
		}

#ifdef PROTECTED
	if (setup_fossil(Port) <= 0)
    	{
    	printf("\n\n\aUnable to initialize Port Handle.\n\n");
		return 1;
    	}
#else
	if (init_fossil (Port) <= 0)
    	{
    	printf("\n\n\aNo Fossil installed.\n\n");
		return 1;
    	}
#endif

	if (check_cd(Port) == 0)
      	{
      	BriteWhite;
      	printf("\n\nCannot be used in local test mode...\n");
		return 1;
      	}

	if (baud)
		set_baud( Port, baud);

    /*  Add the Name to the prompt variable in case of shell  */

#ifdef PROTECTED
    if (cptr = getenv("PROMPT"))
        sprintf(envbuffer1,"PROMPT=\033[1;33m-%s OS/2 Shell-$_%s","LateChat!",cptr);
    else
        sprintf(envbuffer1,"[PROMPT=LateChat!  OS/2 Shell]$_$l$p$g ");
#else
    if (cptr = getenv("PROMPT"))
        sprintf(envbuffer1,"PROMPT=\033[1;33m-%s DOS Shell-$_%s","LateChat!",cptr);
    else
        sprintf(envbuffer1,"[PROMPT=LateChat!  DOS Shell]$_$l$p$g ");
#endif
    putenv(envbuffer1);        /* do not use envbuffer1 for anything else */


	BriteGreen;
	send_string ("\r\nWhy do you want to chat (ENTER=Quit)? ");
	PushSpot;
	send_string ("[37;1;44m");
	for (count = 0; count < 41; count++)
		send_string (" ");
	PopSpot;
	get_reason(Reason);
	send_string ("\x1b[0m\r\n\n");
	if (!strlen(Reason))
		return 0;

	BriteYellow;
	fprintf(stderr,"\r\n\"%s %s\" ", argv [5], argv [6]);
	BriteCyan;
	fprintf(stderr,"is yelling for a chat. \n");
	fprintf(stderr,"Press ESC or N to cancel, anything else to Chat\r\n\n");

#ifndef PROTECTED
	/*  kill off partial second at start */
	time (&CurrTime);
	do
    	time (&InitTime);              
	while (InitTime == CurrTime);
#endif

	/* Show String passed in or default */
	BriteWhite;
	if (argc > 7)
		{
		for (Step = 7; Step < argc; Step++)
			{
			send_string (argv[Step]);
			send_string (" ");
			}
		}
	else
		send_string ("Paging Sysop ....");
 	send_string ("   ");

	BriteYellow;
#ifndef PROTECTED
	time (&CurrTime);
	Elapsed = (int)(CurrTime - InitTime);
	OldElapsed = CountMax;
#endif

#ifdef PROTECTED
	while (CountMax && peek_kb() == 0xffff)
		{
        send_string("\b\b  \b\b");
        fprintf(stderr,"\a");
        sprintf(NewInt,"%2d",CountMax--);
        send_string(NewInt);
		DosSleep(1000L);
		}
#else
	while (Elapsed < CountMax && peek_kb() == 0xffff)
      	{
      	time (&CurrTime);
      	Elapsed = (int)(CurrTime - InitTime);
      	if (Elapsed != OldElapsed)
          	{
          	send_string ("\b\b  \b\b");
          	fprintf(stderr,"\a");
          	sprintf (NewInt,"%2d",CountMax - Elapsed);
          	send_string (NewInt);
          	}
      	OldElapsed = Elapsed;
      	}
#endif
	if (peek_kb() == 0xffff)
		{
    	BriteWhite;
    	send_string ("\b\bTime Expired without response from Sysop!\r\n");
		return 2;
		}

	Choice = (char)get_kb();
	switch (Choice)
    	{
    	case 27:
    	case 'N':
        	BriteWhite;
        	send_string ("\b\bSYSOP aborted chat.  He must be busy!\r\n");
			while (peek_kb() != 0xffff)
				read_kb();
			return 3;
			break;
    	default:
        	BriteWhite;
        	send_string ("\b\bEntering Chat mode....\r\n");
        	Chat();
			break;
     	}
	return 0;
	}



void draw_screen(void)
	{
	int count;
	char buffer [41];

	CLS;
	BriteGreen;
#ifdef PROTECTED
	send_string ("ีออ LateChat/2! ออออออออออออออออต");
#else
	send_string ("ีออ LateChat! ออออออออออออออออออต");
#endif
	BriteYellow;
	send_string ("  Sysop  ");
	BriteGreen;
/*	sprintf (buffer, "ฦออออออออออออออออออออออออออออออออออธ"); */
	sprintf (buffer, "ฦอออออออออออออออออออออ v %d.%02d อออออธ",MAJOR_VERSION, MINOR_VERSION);
	send_string (buffer);
	Locate (1,RemoteLine-2);
	BriteMagenta;
	send_string ("ฦออออออออออออออออออออออออออออออออต");
	BriteYellow;
	send_string ("  You  ");
	BriteMagenta;
	send_string ("ฦอออออออออออออออออออออออออออออออออออต");

	BriteRed;
	for (count = 0; count < MAXLINE; count++)
		{
		Locate (1,SysopLine + count);
		send_string(SysBuf + (count * 80));
		}
	BriteGreen;
	for (count = 0; count < MAXLINE; count++)
		{
		Locate (1,RemoteLine + count);
		send_string(RemBuf + (count * 80));
		}
	MeLocate (1, BottomLine);
	printf ("[30;47m   Commands:  @H = HangUp  ^R = Redraw  ^G = Bell  @J = Shell  ESC = Exit      [0m");
	}			


/*============================== Chat =================================*/

void Chat(void)
	{
	int	    KeyHit,
        	SerHit,
        	LastKey,
			SysX=1,	SysY=0,
			RemX=1, RemY=0;

	memset(SysBuf,0,(80 * MAXLINE) * sizeof(char));
	memset(RemBuf,0,(80 * MAXLINE) * sizeof(char));
	draw_screen();
	Locate (1, SysopLine);
	LastKey = Local;
	BriteRed;
	while (1)
		{
    	if (!check_cd(Port))
        	{
        	fprintf(stderr,"\x1b[0;37;1m\x1b[24;1HCarrier was Lost!");
			exit(1);
        	}
    	if (peek_input(Port) != 0xffff)
        	{
        	SerHit = read_input(Port);
        	if (LastKey != Remote)
             	{
             	LastKey = Remote;
				Locate (RemX, RemoteLine+RemY);
             	BriteGreen;
             	}
        	DoSwitch((unsigned int)SerHit, &RemX, &RemY,RemoteLine,RemBuf);
        	}
    	if (KeyHit = get_kb()) 
			{
			if (KeyHit == '\033')
				{
            	BriteWhite;
				Locate(1,24);
				send_string("Returning you to the BBS...hold on!!");
				while (peek_kb() != 0xffff)
					read_kb();
            	return;
				}
        	if (LastKey != Local)
             	{
             	LastKey = Local;
				Locate (SysX, SysopLine+SysY);
             	BriteRed;
             	}
			DoSwitch((unsigned int)KeyHit, &SysX, &SysY,SysopLine,SysBuf);
        	}
    	}
	}



void DoSwitch(unsigned int BothHit,int *X,int *Y,int StartY,char *LineBuf)
    {
	char buffer[80];
	char *cptr;
	char *cptr1;
	int count;
	int len;
	int wrapflag = 0;

	cptr = LineBuf + (*Y * 80);
    switch (BothHit)
        {
        case 0x2300:   /*  alt-H = Hang up */
                {
                hangup();
                exit (1);
                }
            break;
        case 0x2400:   /*  alt-J = shell to dos */
                {
                if ((cptr1 = getenv("COMSPEC")) != NULL)
                    sprintf (ComSpec, "%s", cptr1);
                else
#ifdef PROTECTED
                    strcpy (ComSpec, "cmd.exe");
#else
                    strcpy (ComSpec, "command.com");
#endif
                BriteWhite;
                Locate(1,24);
#ifdef PROTECTED
                send_string ("\r\aSysop shelling to OS/2.  Please hold a few seconds... ");
#else
                send_string ("\r\aSysop shelling to DOS.  Please hold a few seconds... ");
#endif
                fprintf (stderr, "\033[2J");
                if (spawnlp (P_WAIT, ComSpec,ComSpec,NULL) == -1)
                    {
                    BriteRed;
					fprintf (stderr, "\n\a System Error!!  Cannot Shell!  \n");
                    }
                else
                    {
                    draw_screen();
                    Locate(*X,StartY + *Y);
                    if (StartY == SysopLine)
                        BriteRed;
                    else
                        BriteGreen;
                    break;
                    }
                }
            break;
        case '\b' :
        case 0x7f:
        case 0xff:
            if (*X > 1)
                {
                send_string ("\b \b");
                *(cptr + (strlen(cptr) - 1)) = '\0';
				-- *X;
                }
            break;
		case '\x12':
			draw_screen();
			Locate(*X,StartY + *Y);
			if (StartY == SysopLine)
				BriteRed;
			else
				BriteGreen;
			break;
        case '\n':
        case '\r':
            *X = 1;
			++ *Y;
			if (*Y >= MAXLINE)
				{
				*Y = 4;
				for (count = 0; count < MAXLINE; count++)
					{
					Locate(1,StartY + count);
					send_string("\x1b[K");
					if (count < 4)
						{
						cptr = LineBuf + ((count + 5) * 80);
						send_string(cptr);
						}
					}
				memmove(LineBuf,LineBuf + (80 * 5),(80 * 4) * sizeof(char));
				memset(LineBuf + (80 * 4),0,(80 * 5) * sizeof(char));
				Locate(1,StartY + *Y);
				}
			else
				{
	            send_string("\r\n");
	            *(cptr + 80) = '\0';
				}
            break;
        case '\a':
            send_string("\a");
            break;
        default:
            if (BothHit > 31 && BothHit <= 0xff)
                {
				*(cptr + (strlen(cptr) + 1)) = '\0';
				*(cptr + strlen(cptr)) = (unsigned char)BothHit;
                sprintf(NewInt,"%c",BothHit);
                send_string(NewInt);
                ++*X;
                if (*X > MAXWIDTH)
                    {
					/* wrap line */
					cptr1 = cptr;
					while (*cptr1)
						++cptr1;
					--cptr1;
					while (*cptr1 > ' ' && cptr1 != cptr)
						--cptr1;
					++cptr1;
					len = (int)strlen(cptr1);
					if (len && len < 40)
						{
						for (count = 0; count < len; count++)
							send_string("\b");
						send_string("\x1b[K");		/* hack end of line */
						cptr = LineBuf + ((*Y + 1) * 80);
						if (*Y < (MAXLINE - 1))
							{
							strcpy(cptr,cptr1);
							Locate(1,StartY + *Y + 1);
							send_string(cptr1);
							}
						else
							{
							strcpy(buffer,cptr1);
							wrapflag = 1;
							}
						*X = (int)strlen(cptr1) + 1;
						*cptr1 = '\0';
						}
					else
						*X = 1;
					++ *Y;
					if (*Y >= MAXLINE)
						{
						*Y = 4;
						for (count = 0; count < MAXLINE; count++)
							{
							Locate(1,StartY + count);
							send_string("\x1b[K");
							if (count < 4)
								{
								cptr = LineBuf + ((count + 5) * 80);
								send_string(cptr);
								}
							}
						memmove(LineBuf,LineBuf + (80 * 5),(80 * 4) * sizeof(char));
						memset(LineBuf + (80 * 4),0,(80 * 5) * sizeof(char));
						if (wrapflag)
							{
							strcpy(LineBuf + (80 * 4),buffer);
							Locate(1,StartY + 4);
							send_string(buffer);
							}
						}
					else
			            *(cptr + 80) = '\0';
					Locate(*X,StartY + *Y);
                    }
                }
			break;
		}
	}

