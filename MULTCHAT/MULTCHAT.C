/* multchat.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 17 February 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#define INCL_DOSNMPIPES
#define INCL_DOSSEMAPHORES

#include <stdio.h>
#include <stdlib.h>
#include <os2.h>
#include <signal.h>
#include "multchat.h"


HSEM loaded;
HPIPE mainpipe;



void ctrl_c(void)
	{
	quit = 1;
	}



int main(void)
	{
	PIDINFO pidinfo;
	int err;

	fprintf(stderr,"MULTCHAT (v %d.%02d%s of %s): MultiChat, A Simplex BBS Cross-Chat Utility.\n",MAJOR_VERSION,MINOR_VERSION,BETA ? "á" : "",__DATE__);
	fprintf(stderr,"Copyright (c) 1991, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	fprintf(stderr,"Attempting to secure main MultiChat semaphore.\n");
	if (err = DosCreateSem(CSEM_PRIVATE,&loaded,"\\sem\\multchat.sem"))
		{
		if (err == ERROR_ALREADY_EXISTS)
			fprintf(stderr,"Error: MultiChat already loaded.\n");
		else 
			fprintf(stderr,"Error: Unable to create MultiChat semaphore.\n");
		return 1;
		}
	if (DosSemSet(loaded))
		{
		fprintf(stderr,"Error: MultiChat already loaded.\n");
		DosCloseSem(loaded);
		return 1;
		}

	fprintf(stderr,"Attempting to create main MultiChat connection line.\n");
	if (DosMakeNmPipe("\\pipe\\multchat",&mainpipe,NP_ACCESS_DUPLEX | NP_NOINHERIT | NP_NOWRITEBEHIND,NP_NOWAIT | NP_READMODE_MESSAGE | NP_TYPE_MESSAGE | 1,20 * sizeof(struct mcmsg),20 * sizeof(struct mcmsg),0x1000L))
		{
		fprintf(stderr,"Error: Unable to create pipe!\n");
		return 1;
		}
	signal(SIGINT,ctrl_c);
	signal(SIGBREAK,ctrl_c);

	DosGetPID(&pidinfo);
	if (_osmajor == 10 && _osminor < 20)		/* if OS/2 version less than 1.20 make it timecritical */
		DosSetPrty(PRTYS_PROCESS,PRTYC_TIMECRITICAL,PRTYD_MINIMUM,pidinfo.pid);
	else 
		DosSetPrty(PRTYS_PROCESS,PRTYC_FOREGROUNDSERVER,PRTYD_MINIMUM,pidinfo.pid);

	wait();
	DosClose(mainpipe);
	DosCloseSem(loaded);
	return 0;
	}
