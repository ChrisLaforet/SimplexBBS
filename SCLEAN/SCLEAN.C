/* sclean.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 20 May 1990
**
** Revision Information: $Logfile:   G:/simplex/sclean/vcs/sclean.c_v  $
**                       $Date:   25 Oct 1992 14:14:32  $
**                       $Revision:   1.10  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <signal.h>
#include <ctype.h>
#include <string.h>
#if !defined(PROTECTED)
	#include <dos.h>
#else
	#define INCL_DOSDATETIME
	#include <os2.h>
#endif
#include "sclean.h"



struct cfg cfg;
DATE_T today;
char bbspath[100];			/* BBS message files area */
FILE *mheadfd;
FILE *mdatafd;
FILE *mlinkfd;
FILE *mreadfd;
FILE *userfd;
int quiet_flag = 0;
int *renum = NULL;
int max_renum = 0;
int *areas = NULL;
int max_areas = 0;
int max_users = 0;
int *msgs = NULL;
int total_msgs = 0;




void signal_trap(int signo)
	{
	/* this function does nothing except receive
	** calls from SIGINT and SIGBREAK.  This is
	** done this way since not all compilers handle
	** SIG_IGN correctly.
	*/
	}



int main(int argc,char *argv[])
	{
	struct mlink tmlink;
	char buffer[120];
	char *cptr;
	int count;
	int kount;
	int max_days = 0;
	int max_num = 0;
	int tval;
#if !defined(PROTECTED)
	struct dosdate_t date;
#else
	DATETIME date;
#endif
	FILE *fd;

	setbuf(stderr,NULL);		/* make sure that stderr is completely unbuffered */

#if defined(PROTECTED)
	fprintf(stderr,"SCLEAN/2 (v %d.%02d of %s): Simplex/2 Message Base Cleaning Utility.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#else
	fprintf(stderr,"SCLEAN (v %d.%02d of %s): Simplex Message Base Cleaning Utility.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#endif
	fprintf(stderr,"Copyright (c) 1990-92, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");
	if (argc > 1)
		{
		if (cptr = getenv("SIMPLEX"))		/* now for our path */
			strcpy(bbspath,cptr);
		else
			getcwd(bbspath,sizeof(bbspath));
		if (bbspath[0] && bbspath[strlen(bbspath) - 1] != P_CSEP)
			strcat(bbspath,P_SSEP);

		signal(SIGINT,signal_trap);				/* first, down with Ctrl-C */
		signal(SIGBREAK,signal_trap);			/* then down with Ctrl-Break */

		sprintf(buffer,"%sconfig.bbs",bbspath); 		/* now, open and get configuration */
		if (!(fd = fopen(buffer,"r+b")))
			{
			printf("Error: Unable to find/open config.bbs\n",buffer);
			return 1;
			}
		if (!fread(&cfg,sizeof(struct cfg),1,fd))
			{
			printf("Error: Unable to read from config.bbs\n");
			return 1;
			}
		fclose(fd);

		sprintf(buffer,"%smsghead.bbs",bbspath); 		/* now, open and get message headers */
		if (!(mheadfd = fopen(buffer,"r+b")))
			{
			printf("Error: Unable to find/open msghead.bbs\n");
			return 1;
			}

		sprintf(buffer,"%smsgdata.bbs",bbspath); 		/* now, open message data file */
		if (!(mdatafd = fopen(buffer,"r+b")))
			{
			printf("Error: Unable to find/open msgdata.bbs\n");
			return 1;
			}

		sprintf(buffer,"%smsglink.bbs",bbspath); 		/* now, open message link file */
		if (!(mlinkfd = fopen(buffer,"r+b")))
			{
			printf("Error: Unable to find/open msglink.bbs\n");
			return 1;
			}
		total_msgs = (int)(filelength(fileno(mlinkfd)) / (long)sizeof(struct mlink));
		if (total_msgs != (int)(filelength(fileno(mheadfd)) / (long)sizeof(struct msgh)))
			{
			printf("Error: msglink.bbs and msghead.bbs are not synchronized\n");
			return 1;
			}
		if (!(msgs = (int *)malloc(total_msgs * sizeof(int))))
			{
			printf("Error: Unable to allocate memory for %d messages\n",total_msgs);
			return 1;
			}

		fprintf(stderr,"There are %u message%s in the message base.\n",total_msgs,total_msgs == 1 ? "" : "s");
		fseek(mlinkfd,0L,SEEK_SET);
		for (count = 0; count < total_msgs; count++)
			{
			fread(&tmlink,sizeof(struct mlink),1,mlinkfd);
			if (tmlink.mlink_flags & MSGH_DELETED)
				msgs[count] = 0;
			else
				msgs[count] = tmlink.mlink_area;
			}

		sprintf(buffer,"%suserlist.bbs",bbspath); 		/* now, open userlist file */
		if (!(userfd = fopen(buffer,"rb")))
			{
			printf("Error: Unable to find/open userlist.bbs\n");
			return 1;
			}
		sprintf(buffer,"%smsgread.bbs",bbspath); 		/* now, open message lastread file */
		if (!(mreadfd = fopen(buffer,"r+b")))
			{
			printf("Error: Unable to find/open msgread.bbs\n");
			return 1;
			}
		fread(&max_areas,sizeof(int),1,mreadfd);
		if (max_areas)
			{
			if (!(areas = (int *)calloc(max_areas,sizeof(int))))
				{
				printf("Error: Unable to allocate lastread areas array!\n");
				return 1;
				}
			for (kount = 0; kount < max_areas; kount++)
				fread(&areas[kount],sizeof(int),1,mreadfd);
			max_users = (int)(filelength(fileno(userfd)) / (long)sizeof(struct user));
			max_renum = (int)(filelength(fileno(mheadfd)) / (long)sizeof(struct msgh));
			if (!(renum = (int *)malloc(max_renum * sizeof(int))))
				{
				printf("Error: Unable to allocate renumbering array!\n");
				return 1;
				}
			}
		fclose(userfd);

#if defined(PROTECTED)
		DosGetDateTime(&date);
		today = ((date.year - 1980) << 9) | ((date.month & 15) << 5) | (date.day & 31);
#else
		_dos_getdate(&date);
		today = ((date.year - 1980) << 9) | ((date.month & 15) << 5) | (date.day & 31);
#endif

		for (count = 1; count < argc; count++)
			{
			cptr = argv[count];
			if (*cptr == (char)'-' || *cptr == (char)'/')
				{
				++cptr;
				if ((*cptr == (char)'d' || *cptr == (char)'D') && isdigit(*(cptr + 1)))
					{
					++cptr;
					tval = atoi(cptr);
					if (!tval)
						printf("Error: You cannot specify a kill period of 0 days\n");
					if (!max_days)
						{
						max_days = tval;
						printf("Setting message killing to messages older than %d day%s.\n",max_days,(char *)(max_days != 1 ? "s" : ""));
						}
					else
						printf("Error: You cannot specify multiple kill periods\n");
					}
				else if ((*cptr == (char)'n' || *cptr == (char)'N') && isdigit(*(cptr + 1)))
					{
					++cptr;
					tval = atoi(cptr);
					if (!tval)
						printf("Error: You cannot specify a maximum of 0 messages\n");
					if (!max_num)
						{
						max_num = tval;
						printf("Setting total messages to keep to %d message%s.\n",max_num,(char *)(max_num != 1 ? "s" : ""));
						}
					else
						printf("Error: You cannot specify multiple numbers of messages to keep\n");
					}
			  	else if (*cptr == (char)'q' || *cptr == (char)'Q')
			  		quiet_flag = 1;
				}
			else
				printf("Error: Invalid command line argument \"%s\"\n",argv[count]);
			}

		for (count = 1; count < argc; count++)
			{
			cptr = argv[count];
			if (*cptr == (char)'-' || *cptr == (char)'/')
				{
				++cptr;
				if (*cptr == (char)'k' || *cptr == (char)'K')
					{
					++cptr;
					if ((*cptr == (char)'a' || *cptr == (char)'A') && isdigit(*(cptr + 1)))
						{
						++cptr;
						tval = atoi(cptr);
						if (!tval || tval >= 10000)
							printf("Error: Message area %d cannot exist!\n",tval);
						else
							{
							printf("Killing ALL messages in message area %d\n",tval);
							kill_all(tval);
							}
						}
					else if ((*cptr == (char)'d' || *cptr == (char)'D') && isdigit(*(cptr + 1)))
						{
						++cptr;
						tval = atoi(cptr);
						if (!tval || tval >= 10000)
							printf("Error: Message area %d cannot exist!\n",tval);
						else
							{
							if (!max_days)
								printf("Error: A kill period has not been specified!\n",tval);
							else
								{
								printf("Killing messages older than %d days in message area %d\n",max_days,tval);
								kill_date(tval,max_days);
								}
							}
						}
					else if ((*cptr == (char)'n' || *cptr == (char)'N') && isdigit(*(cptr + 1)))
						{
						++cptr;
						tval = atoi(cptr);
						if (!tval || tval >= 10000)
							printf("Error: Message area %d cannot exist!\n",tval);
						else
							{
							if (!max_num)
								printf("Error: A kill period has not been specified!\n",tval);
							else
								{
								printf("Killing messages keeping %d messages in message area %d\n",max_num,tval);
								kill_num(tval,max_num);
								}
							}
						}
					else if ((*cptr == (char)'r' || *cptr == (char)'R') && isdigit(*(cptr + 1)))
						{
						++cptr;
						tval = atoi(cptr);
						if (!tval || tval >= 10000)
							printf("Error: Message area %d cannot exist!\n",tval);
						else
							{
							printf("Killing all received messages in message area %d\n",tval);
							kill_received(tval);
							}
						}
					else 
						printf("Error: Invalid command line argument \"%s\"\n",argv[count]);
					}
				else if (*cptr != (char)'D' && *cptr != (char)'d' && *cptr != (char)'N' && *cptr != (char)'n' && *cptr != (char)'Q' && *cptr != (char)'q')
					printf("Error: Invalid command line argument \"%s\"\n",argv[count]);
				}
			}
		fclose(mheadfd);
		fclose(mdatafd);
		fclose(mlinkfd);
		fclose(mreadfd);
		}
	else
		{
#if defined(PROTECTED)
		printf("Usage is: sclean2 -options\n\n");
#else
		printf("Usage is: sclean -options\n\n");
#endif
		printf("where options may be:\n");
		printf("\t-q      Quiet...do not show message by message activity.\n");
		printf("\t-dxx    Sets maximum days for message killing to xx\n");
		printf("\t-kdxx   Kill messages prior to maximum days on board #xx\n");
		printf("\t-nxx    Sets maximum number of messages to keep on boards to xx\n");
		printf("\t-knxx   Kill messages on board #xx keeping maximum number in area\n");
		printf("\t-kaxx   Kill ALL messages on board #xx\n");
		printf("\t-krxx   Kill all received messages on board #xx\n\n");
		return 1;
		}
	
	return 0;
	}

