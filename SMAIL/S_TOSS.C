/* s_toss.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 January 1992
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_toss.c_v  $
**                       $Date:   25 Oct 1992 14:18:10  $
**                       $Revision:   1.1  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <ctype.h>
#include <io.h>
#include "smail.h"



struct toss **tosslist = NULL;
int cur_tosslist = 0;
int max_tosslist = 0;




int check_toss(char *to)
	{
	int count;

	for (count = 0; count < cur_tosslist; count++)
		{
		if (!stricmp(to,tosslist[count]->toss_to))
			return count;
		}
	return -1;
	}



void toss_message(struct toss *ttoss,struct msgh *tmsgh,char *area,FILE *sfd)
	{
	struct fm tfm;
	char buffer[100];
	int tval;
	FILE *fd;

	if (access(ttoss->toss_path,0))		/* attempt to make the path if not exist */
		mkdir(ttoss->toss_path);
	strcpy(buffer,ttoss->toss_path);
	if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
		strcat(buffer,P_SSEP);
	sprintf(buffer + strlen(buffer),"%u.MSG",ttoss->toss_msgnum);
	++ttoss->toss_msgnum;
	if (fd = fopen(buffer,"w+b"))
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
		++ttoss->toss_total;
		}
	else
		{
		fprintf(stderr,"\aError: Unable to open toss message file to \"%s\"\n",ttoss->toss_to);
		while ((tval = fgetc(sfd)) && tval != EOF)		/* bleed to EOF or to end of message */
			;
		}
	}



