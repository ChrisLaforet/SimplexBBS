/* s_badmsg.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 23 May 1990
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_badmsg.c_v  $
**                       $Date:   25 Oct 1992 14:18:14  $
**                       $Revision:   1.12  $
**
*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <stdlib.h>
#include "smail.h"




void toss_bad(unsigned int *msgnum,struct msgh *tmsgh,char *area,FILE *sfd)
	{
	struct fm tfm;
	char buffer[120];
	FILE *fd;
	int tval;

	strcpy(buffer,cfg.cfg_badmsgpath);
	if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
		strcat(buffer,P_SSEP);
	sprintf(buffer + strlen(buffer),"%u.MSG",*msgnum);
	*msgnum += 1;
	if (fd = fopen(buffer,"wb+"))
		{
		memset(&tfm,0,sizeof(struct fm));
		strncpy(tfm.fm_from,tmsgh->msgh_from,35);
		strncpy(tfm.fm_to,tmsgh->msgh_to,35);
		strncpy(tfm.fm_subject,tmsgh->msgh_subject,72);
		strncpy(tfm.fm_datetime,make_datetime(tmsgh->msgh_date,tmsgh->msgh_time),20);
		tfm.fm_snet = tmsgh->msgh_snet;
		tfm.fm_snode = tmsgh->msgh_snode;
		tfm.fm_dnet = tmsgh->msgh_dnet;
		tfm.fm_dnode = tmsgh->msgh_dnode;
		tfm.fm_flags = 0x2;			/* tell it is CRASH */
		if (tmsgh->msgh_flags & MSGH_PRIVATE)
			tfm.fm_flags |= 1;

		fwrite(&tfm,sizeof(struct fm),1,fd);
		if (area)
			fprintf(fd,"AREA:%s\r",area);

		while ((tval = fgetc(sfd)) && tval != EOF)
			fputc(tval,fd);
		fputc('\0',fd);			/* NUL terminate the message */

		fclose(fd);
		}
	else
		{
		fprintf(stderr,"Error: Unable to open bad message file\n");
		while ((tval = fgetc(sfd)) && tval != EOF)		/* bleed to EOF or to end of message */
			;
		}
	}



void import_bad(void)
	{

	}
