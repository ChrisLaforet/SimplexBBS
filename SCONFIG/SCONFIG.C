/* sconfig.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 1 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/sconfig.c_v  $
**                       $Date:   25 Oct 1992 14:15:16  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <direct.h>
#include <ctype.h>
#include <io.h>
#include <conio.h>
#include <editor.h>
#include <screen.h>
#include <window.h>
#include "sconfig.h"



char cfgpath[257];
FILE *ccfgfd;
FILE *cfgfd;
FILE *evtfd;
FILE *msgfd;
FILE *filefd;
FILE *userfd;
int master_change = 0;
struct cfg cfg;
struct msg **msgs = NULL;
int cur_msgs = 0;
int max_msgs = 0;
struct file **files = NULL;
int cur_files = 0;
int max_files = 0;
struct evt **evts = NULL;
int cur_evts = 0;
int max_evts = 0;
struct usr **usrs = NULL;
int cur_usrs = 0;
int max_usrs = 0;
struct ut **uts = NULL;
int cur_uts = 0;
int max_uts = 0;


int color[7] =
		{
		LT_GRAY | ON_BLUE,
		YELLOW | ON_BLUE,
		WHITE | ON_RED,
		LT_GRAY | ON_RED,
		YELLOW | ON_RED,
		CYAN,
		YELLOW,
		};



void cleanup(void)
	{
	int count;

	fclose(filefd);
	fclose(msgfd);
	fclose(evtfd);
	fclose(cfgfd);
	fclose(userfd);

	if (max_uts)
		{
		for (count = 0; count < cur_uts; count++)
			free(uts[count]);
		free(uts);
		}

	if (max_msgs)
		{
		for (count = 0; count < cur_msgs; count++)
			free(msgs[count]);
		free(msgs);
		}

	if (max_files)
		{
		for (count = 0; count < cur_files; count++)
			free(files[count]);
		free(files);
		}

	if (max_evts)
		{
		for (count = 0; count < cur_evts; count++)
			free(evts[count]);
		free(evts);
		}

	if (max_usrs)
		{
		for (count = 0; count < cur_usrs; count++)
			free(usrs[count]);
		free(usrs);
		}

	_cls(NORMAL);
	clrblk(0x0,0x34f,ON_RED);

#if defined(PROTECTED)
	prntnomovf(0x0,80,YELLOW | ON_RED,"SCONFIG2 (v %u.%02u of %s): Simplex/2 BBS Configuration Editor.",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#else
	prntnomovf(0x0,80,YELLOW | ON_RED,"SCONFIG (v %u.%02u of %s): Simplex BBS Configuration Editor.",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#endif
	prntnomov(0x100,80,LT_CYAN | ON_RED,"Copyright (c) 1989-92, Christopher Laforet and Chris Laforet Software.");
	prntnomov(0x200,80,LT_GREEN | ON_RED,"All Rights Reserved.");
	set_cursor_type(LINE);
	_setcurpos(0x400);
	}



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
	struct file tfile;
	struct user tuser;
	struct evt tevt;
	struct msg tmsg;
	struct ut tut;
	char buffer[100];
	char *cptr;
	int count;
	int key;

	setbuf(stderr,NULL);		/* make sure that stderr is completely unbuffered */

#if defined(PROTECTED)
	fprintf(stderr,"SCONFIG2 (v %u.%02u of %s): Simplex/2 BBS Configuration Editor.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#else
	fprintf(stderr,"SCONFIG (v %u.%02u of %s): Simplex BBS Configuration Editor.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#endif
	fprintf(stderr,"Copyright (c) 1989-92, Christopher Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	scr_init();

#ifndef PROTECTED
	set_critical_error();
#endif

	if (argc > 1)
		{
		if (argv[1][0] == '/' || argv[1][0] == '-')
			{
#ifndef PROTECTED
			if (argv[1][1] == 'S' || argv[1][1] == 's')
				snow_flag = 1;
			else if (argv[1][1] == 'M' || argv[1][1] == 'm')	/* monochrome color set */
#else
			if (argv[1][1] == 'M' || argv[1][1] == 'm')			/* monochrome color set */
#endif
				{
				color[0] = LT_GRAY;
				color[1] = WHITE;
				color[2] = ON_WHITE;
				color[3] = ON_WHITE;
				color[4] = ON_WHITE;
				color[5] = ON_WHITE;
				color[6] = ON_WHITE;
				}
			}
		}

	if (cptr = getenv("SIMPLEX"))
		strcpy(cfgpath,cptr);
	else
		getcwd(cfgpath,sizeof(cfgpath));
	if (cfgpath[0] && cfgpath[strlen(cfgpath) - 1] != P_CSEP)
		strcat(cfgpath,P_SSEP);

	signal(SIGINT,signal_trap);				/* first, down with Ctrl-C */
	signal(SIGBREAK,signal_trap);			/* then down with Ctrl-Break */

	fprintf(stderr,"Opening configuration file....\n");
	sprintf(buffer,"%ssconfig.cfg",cfgpath);
	if (!(ccfgfd = fopen(buffer,"r+b")))
		{
		if (!(ccfgfd = fopen(buffer,"w+b")))
			{
			fprintf(stderr,"Fatal Error: Unable to open/create configuration file.\nFile name: \"%s\"",buffer);
			return 1;
			}
		else
			{
			memset(&ccfg,0,sizeof(struct ccfg));
			for (count = 0; count < 7; count++)
				ccfg.ccfg_color[count] = color[count];
#if !defined(PROTECTED)
			ccfg.ccfg_snow = snow_flag;
#endif
			fwrite(&ccfg,sizeof(struct ccfg),1,ccfgfd);
			fflush(ccfgfd);
			}
		}
	else
		fread(&ccfg,sizeof(struct ccfg),1,ccfgfd);

	/* assign default values */
	for (count = 0; count < 7; count++)
		color[count] = ccfg.ccfg_color[count];
#if !defined(PROTECTED)
	snow_flag = ccfg.ccfg_snow;
#endif

	fprintf(stderr,"Opening help file....\n");
	sprintf(buffer,"%ssconfig.hlp",cfgpath);
	if (open_help(buffer,color[6],color[5]))
		{
		strcpy(buffer,argv[0]);				/* try the startup directory */
		if (buffer[0])
			{
			cptr = buffer + strlen(buffer);
			while (cptr != buffer && *cptr != P_CSEP)
				--cptr;

			if (*cptr == P_CSEP)
				{
				++cptr;
				strcpy(cptr,"sconfig.hlp");

				if (open_help(buffer,color[6],color[5]))
					fprintf(stderr,"Notice:  No help file available\n");
				}
			else 
				fprintf(stderr,"Notice:  No help file available\n");
			}
		else 
			fprintf(stderr,"Notice:  No help file available\n");
		}

	sprintf(buffer,"%sconfig.bbs",cfgpath);
	if (!(cfgfd = fopen(buffer,"r+b")))
		{
		fprintf(stderr,"\aUnable to open \"%s\".\nMay I create it? ",buffer);
		do
			{
			key = getch();
			key = toupper(key);
			}
		while (key != 'Y' && key != 'N');
		fprintf(stderr,"%s\n",key == 'Y' ? "Yes" : "No");
		if (key == 'N')
			{
			fprintf(stderr,"\nError: Unable to continue without open file!\n\n");
			return 1;
			}
		if (!(cfgfd = fopen(buffer,"w+b")))
			{
			fprintf(stderr,"\nError: Unable to create %s!\n\n",buffer);
			return 1;
			}
		}

	sprintf(buffer,"%sevents.bbs",cfgpath);
	if (!(evtfd = fopen(buffer,"r+b")))
		{
		fprintf(stderr,"\aUnable to open \"%s\".\nMay I create it? ",buffer);
		do
			{
			key = getch();
			key = toupper(key);
			}
		while (key != 'Y' && key != 'N');
		fprintf(stderr,"%s\n",key == 'Y' ? "Yes" : "No");
		if (key == 'N')
			{
			fprintf(stderr,"\nError: Unable to continue without open file!\n\n");
			return 1;
			}
		if (!(evtfd = fopen(buffer,"w+b")))
			{
			fprintf(stderr,"\nError: Unable to create %s!\n\n",buffer);
			return 1;
			}
		}

	sprintf(buffer,"%suserlist.bbs",cfgpath);
	if (!(userfd = fopen(buffer,"r+b")))
		{
		fprintf(stderr,"\aUnable to open \"%s\".\nMay I create it? ",buffer);
		do
			{
			key = getch();
			key = toupper(key);
			}
		while (key != 'Y' && key != 'N');
		fprintf(stderr,"%s\n",key == 'Y' ? "Yes" : "No");
		if (key == 'N')
			{
			fprintf(stderr,"\nError: Unable to continue without open file!\n\n");
			return 1;
			}
		if (!(userfd = fopen(buffer,"w+b")))
			{
			fprintf(stderr,"\nError: Unable to create %s!\n\n",buffer);
			return 1;
			}
		}

	sprintf(buffer,"%smsgarea.bbs",cfgpath);
	if (!(msgfd = fopen(buffer,"r+b")))
		{
		fprintf(stderr,"\aUnable to open \"%s\".\nMay I create it? ",buffer);
		do
			{
			key = getch();
			key = toupper(key);
			}
		while (key != 'Y' && key != 'N');
		fprintf(stderr,"%s\n",key == 'Y' ? "Yes" : "No");
		if (key == 'N')
			{
			fprintf(stderr,"\nError: Unable to continue without open file!\n\n");
			return 1;
			}
		if (!(msgfd = fopen(buffer,"w+b")))
			{
			fprintf(stderr,"\nError: Unable to create %s!\n\n",buffer);
			return 1;
			}
		}

	sprintf(buffer,"%sfilearea.bbs",cfgpath);
	if (!(filefd = fopen(buffer,"r+b")))
		{
		fprintf(stderr,"\aUnable to open \"%s\".\nMay I create it? ",buffer);
		do
			{
			key = getch();
			key = toupper(key);
			}
		while (key != 'Y' && key != 'N');
		fprintf(stderr,"%s\n",key == 'Y' ? "Yes" : "No");
		if (key == 'N')
			{
			fprintf(stderr,"\nError: Unable to continue without open file!\n\n");
			return 1;
			}
		if (!(filefd = fopen(buffer,"w+b")))
			{
			fprintf(stderr,"\nError: Unable to create %s!\n\n",buffer);
			return 1;
			}
		}

	atexit(cleanup);

	fprintf(stderr,"Reading configuration information....");
	fflush(stderr);
	memset(&cfg,0,sizeof(struct cfg));
	if (!fread(&cfg,1,sizeof(struct cfg),cfgfd))
		{
		cfg.cfg_baud = 2400;
		strcpy(cfg.cfg_init,"ATZ|~~~AT&D3V1X4E0F1Q0C1H0M0S0=0S7=30|");
		strcpy(cfg.cfg_resp,"OK");
		strcpy(cfg.cfg_resp3_1,"CONNECT");
		strcpy(cfg.cfg_resp12_1,"CONNECT 1200");
		strcpy(cfg.cfg_resp24_1,"CONNECT 2400");
		strcpy(cfg.cfg_resp48_1,"CONNECT 4800");
		strcpy(cfg.cfg_resp96_1,"CONNECT 9600");
		strcpy(cfg.cfg_resp96_2,"CONNECT 7200");
		strcpy(cfg.cfg_resp192_1,"CONNECT 14400");
		strcpy(cfg.cfg_resp192_2,"CONNECT 12000");
		strcpy(cfg.cfg_ring,"RING");
		strcpy(cfg.cfg_answer,"ATA|");
		strcpy(cfg.cfg_dialpre,"ATDT");
		strcpy(cfg.cfg_dialpost,"|");
		strcpy(cfg.cfg_hangup,"~~~~+++~~~~ATH0|");
		strcpy(cfg.cfg_busy,"ATM0H1|");
#ifdef PROTECTED
		strcpy(cfg.cfg_arc,"ARC2 a %A %F");
		strcpy(cfg.cfg_unarc,"ARC2 xo %S %D*.*");
		strcpy(cfg.cfg_zip,"PKZIP2 -a %A %F");
		strcpy(cfg.cfg_unzip,"PKUNZIP2 -x -o %S %D");
		strcpy(cfg.cfg_lzh,"LH2 a %A %F");
		strcpy(cfg.cfg_unlzh,"LH2 x %S %D");
#else
		strcpy(cfg.cfg_arc,"ARC a %A %F");
		strcpy(cfg.cfg_unarc,"ARC xo %S %D*.*");
		strcpy(cfg.cfg_zip,"PKZIP -a %A %F");
		strcpy(cfg.cfg_unzip,"PKUNZIP -x -o %S %D");
		strcpy(cfg.cfg_lzh,"LHARC m %A %F");
		strcpy(cfg.cfg_unlzh,"LHARC x %S %D");
#endif

		cfg.cfg_minbaud = 300;
		cfg.cfg_ansibaud = 1200;
		cfg.cfg_newpriv = (char)1;
		cfg.cfg_newtime = 20;
		cfg.cfg_askhome = (char)1;
		cfg.cfg_askdata = (char)1;
		cfg.cfg_pwdtries = (char)3;
		cfg.cfg_yells = (char)2;
		cfg.cfg_yelltime = (char)10;
		cfg.cfg_yellstop = (int)0xbf60;		/* 23:59 */
		cfg.cfg_dlstop = (int)0xbf60;		/* 23:59 */
		cfg.cfg_inactive = (char)5;			/* 5 minutes */

		cfg.cfg_status = COLOR_ONWHITE;
		cfg.cfg_field = COLOR_ONBLUE;
		cfg.cfg_menu = COLOR_WHITE;
		}
	else
		{
		/* read in the times information */

		while (fread(&tut,sizeof(struct ut),1,cfgfd))
			{
			if (cur_uts >= max_uts)
				{
				if (!(uts = realloc(uts,(max_uts += 5) * sizeof(struct ut *))))
					{
					fprintf(stderr,"Fatal Error: Unable to allocate memory for times structure.\n");
					exit(1);
					}
				}
			if (!(uts[cur_uts] = malloc(sizeof(struct ut))))
				{
				fprintf(stderr,"Fatal Error: Unable to allocate memory for times structure.\n");
				exit(1);
				}
			memcpy(uts[cur_uts],&tut,sizeof(struct ut));
			++cur_uts;
			}
		}

	while (fread(&tevt,1,sizeof(struct evt),evtfd))
		{
		if (cur_evts >= max_evts)
			{
			if (!(evts = realloc(evts,(max_evts += 10) * sizeof(struct evt *))))
				{
				fprintf(stderr,"Error: Out of memory to allocate structures!\n");
				exit(1);
				}
			}
		if (!(evts[cur_evts] = malloc(sizeof(struct evt))))
			{
			fprintf(stderr,"Error: Out of memory to allocate structures!\n");
			exit(1);
			}
		memcpy(evts[cur_evts],&tevt,sizeof(struct evt));
		++cur_evts;
		}

	while (fread(&tuser,1,sizeof(struct user),userfd))
		{
		if (cur_usrs >= max_usrs)
			{
			if (!(usrs = realloc(usrs,(max_usrs += 100) * sizeof(struct usr *))))
				{
				fprintf(stderr,"Error: Out of memory to allocate structures!\n");
				exit(1);
				}
			}
		if (!(usrs[cur_usrs] = malloc(sizeof(struct usr))))
			{
			fprintf(stderr,"Error: Out of memory to allocate structures!\n");
			exit(1);
			}

		if (!(usrs[cur_usrs]->usr_name = malloc(strlen(tuser.user_name) + 1)))
			{
			fprintf(stderr,"Error: Out of memory to allocate structures!\n");
			exit(1);
			}
		if (!(usrs[cur_usrs]->usr_city = malloc(strlen(tuser.user_city) + 1)))
			{
			fprintf(stderr,"Error: Out of memory to allocate structures!\n");
			exit(1);
			}

		strcpy(usrs[cur_usrs]->usr_name,tuser.user_name);
		strcpy(usrs[cur_usrs]->usr_city,tuser.user_city);
		usrs[cur_usrs]->usr_deleted = (char)((tuser.user_flags & USER_DELETED) ? 1 : 0);
		usrs[cur_usrs]->usr_priv = tuser.user_priv;
		usrs[cur_usrs]->usr_flags = tuser.user_uflags;

		++cur_usrs;
		}

	while (fread(&tmsg,1,sizeof(struct msg),msgfd))
		{
		if (cur_msgs >= max_msgs)
			{
			if (!(msgs = realloc(msgs,(max_msgs += 10) * sizeof(struct msg *))))
				{
				fprintf(stderr,"Error: Out of memory to allocate structures!\n");
				exit(1);
				}
			}
		if (!(msgs[cur_msgs] = malloc(sizeof(struct msg))))
			{
			fprintf(stderr,"Error: Out of memory to allocate structures!\n");
			exit(1);
			}
		memcpy(msgs[cur_msgs],&tmsg,sizeof(struct msg));
		++cur_msgs;
		}
	if (cur_msgs)
		qsort(msgs,cur_msgs,sizeof(struct msg *),msg_comp);

	while (fread(&tfile,1,sizeof(struct file),filefd))
		{
		if (cur_files >= max_files)
			{
			if (!(files = realloc(files,(max_files += 10) * sizeof(struct file *))))
				{
				fprintf(stderr,"Error: Out of memory to allocate structures!\n");
				exit(1);
				}
			}
		if (!(files[cur_files] = malloc(sizeof(struct file))))
			{
			fprintf(stderr,"Error: Out of memory to allocate structures!\n");
			exit(1);
			}
		memcpy(files[cur_files],&tfile,sizeof(struct file));
		++cur_files;
		}
	if (cur_files)
		qsort(files,cur_files,sizeof(struct file *),file_comp);

	main_menu();

	if (master_change)
		{
		if (askyn("Save Configuration Changes?","Do you want to save the changes made during this session? (Y/N) ",color[4],color[3],0,0,0))
			{
			fseek(cfgfd,0L,SEEK_SET);
			chsize(fileno(cfgfd),0L);
			cfg.cfg_uts = cur_uts;
			fwrite(&cfg,1,sizeof(struct cfg),cfgfd);
			for (count = 0; count < cur_uts; count++)			/* write out times */
				fwrite(uts[count],sizeof(struct ut),1,cfgfd);
			fflush(cfgfd);

			fseek(msgfd,0L,SEEK_SET);
			chsize(fileno(msgfd),0L);
			for (count = 0; count < cur_msgs; count++)
				{
				if (!msgs[count]->msg_deleted)
					fwrite(msgs[count],1,sizeof(struct msg),msgfd);
				}
			fflush(msgfd);

			fseek(filefd,0L,SEEK_SET);
			chsize(fileno(filefd),0L);
			for (count = 0; count < cur_files; count++)
				{
				if (!files[count]->file_deleted)
					fwrite(files[count],1,sizeof(struct file),filefd);
				}
			fflush(filefd);

			fseek(evtfd,0L,SEEK_SET);
			chsize(fileno(evtfd),0L);
			for (count = 0; count < cur_evts; count++)
				{
				if (!evts[count]->evt_deleted)
					fwrite(evts[count],1,sizeof(struct evt),evtfd);
				}
			fflush(evtfd);
			}
		}

	return 0;
	}
