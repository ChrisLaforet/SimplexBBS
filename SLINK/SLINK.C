/* slink.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 24 May 1990
**
** Revision Information: $Logfile:   G:/simplex/slink/vcs/slink.c_v  $
**                       $Date:   25 Oct 1992 14:17:22  $
**                       $Revision:   1.7  $
**
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <signal.h>
#include <io.h>
#include "slink.h"



FILE *areafd;
FILE *linkfd;
FILE *headfd;
char path[100];
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



int main(void)
	{
	struct mlink tmlink;
	struct msg tmsg;
	char buffer[120];
	char *cptr;
	int found;
	int count;
	int tint;
	FILE *fd;

	setbuf(stderr,NULL);		/* make sure that stderr is completely unbuffered */

#if defined(PROTECTED)
	fprintf(stderr,"SLINK/2 (v %d.%02d of %s): Simplex/2 BBS Echomail Reply Linker.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#else
	fprintf(stderr,"SLINK (v %d.%02d of %s): Simplex BBS Echomail Reply Linker.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#endif
	fprintf(stderr,"Copyright (c) 1990-92, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (cptr = getenv("SIMPLEX"))
		strcpy(path,cptr);
	else
		getcwd(path,sizeof(path));
	if (path[0] && path[strlen(path) - 1] != P_CSEP)
		strcat(path,P_SSEP);

	signal(SIGINT,signal_trap);				/* first, down with Ctrl-C */
	signal(SIGBREAK,signal_trap);			/* then down with Ctrl-Break */

	sprintf(buffer,"%smsgarea.bbs",path);
	if (!(areafd = fopen(buffer,"rb")))
		{
		fprintf(stderr,"Error: Unable to open msgarea.bbs.\n");
		exit(1);
		}

	sprintf(buffer,"%smsghead.bbs",path);
	if (!(headfd = fopen(buffer,"rb+")))
		{
		fprintf(stderr,"Error: Unable to open msghead.bbs.\n");
		exit(1);
		}

	sprintf(buffer,"%smsglink.bbs",path);
	if (!(linkfd = fopen(buffer,"rb+")))
		{
		fprintf(stderr,"Error: Unable to open msglink.bbs.\n");
		exit(1);
		}

	total_msgs = (int)(filelength(fileno(linkfd)) / (long)sizeof(struct mlink));
	if (total_msgs != (int)(filelength(fileno(headfd)) / (long)sizeof(struct msgh)))
		{
		fprintf(stderr,"Error: msghead.bbs and msglink.bbs are not synchronized.\n");
		exit(1);
		}
	if (!(msgs = (int *)malloc(total_msgs * sizeof(int))))
		{
		printf("Error: Unable to allocate memory for %d messages\n",total_msgs);
		exit(1);
		}

	fseek(linkfd,0L,SEEK_SET);
	for (count = 0; count < total_msgs; count++)
		{
		fread(&tmlink,sizeof(struct mlink),1,linkfd);
		if (tmlink.mlink_flags & MSGH_DELETED)
			msgs[count] = 0;
		else
			msgs[count] = tmlink.mlink_area;
		}

	sprintf(buffer,"%sslink.bbs",path);
	if (fd = fopen(buffer,"rb"))
		{
		while (fread(&tint,sizeof(int),1,fd))
			{
			found = 0;
			fseek(areafd,0L,SEEK_SET);
			while (fread(&tmsg,sizeof(struct msg),1,areafd))
				{
				if (tint == tmsg.msg_number)
					{
					found = 1;
					break;
					}
				}
			if (found)
				{
				fprintf(stderr,"Building reply links for area %d: \"%s\"...\n",tint,tmsg.msg_areaname);
				build_links(tint);
				}
			}
		fclose(fd);
		unlink(buffer);		/* delete slink.bbs file */
		}						
	else
		{
		while (fread(&tmsg,sizeof(struct msg),1,areafd))
			{
			fprintf(stderr,"Building reply links for area %d: \"%s\"...\n",tmsg.msg_number,tmsg.msg_areaname);
			build_links(tmsg.msg_number);
			}
		}

	fclose(headfd);
	fclose(linkfd);
	fclose(areafd);
	return 0;
	}

