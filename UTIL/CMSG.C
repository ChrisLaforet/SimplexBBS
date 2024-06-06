/* cmsg.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 31 January 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h>
#include <io.h>


struct omsg
	{
	int msg_number;					/* message board number */
	char msg_areaname[41];			/* message area name */
	char msg_flags;					/* message area flags */
	unsigned char msg_readpriv;
	unsigned char msg_writepriv;
	unsigned char msg_sysoppriv;
	char msg_deleted;				/* true if deleted */
	};


struct nmsg
	{
	int msg_number;					/* message board number */
	char msg_areaname[41];			/* message area name */
	char msg_flags;					/* message area flags */
	unsigned char msg_readpriv;
	unsigned char msg_writepriv;
	unsigned char msg_sysoppriv;
	char msg_origin[66];			/* special origin line if Echomail */
	char msg_deleted;				/* true if deleted */
	};



char oname[129];
char nname[129];
char path[129];


int main(void)
	{
	struct omsg omsg;
	struct nmsg nmsg;
	char *cptr;
	FILE *ofd;
	FILE *nfd;

	fprintf(stderr,"CMSG: Converts MSGAREA.BBS to new format.\n");
	fprintf(stderr,"Copyright (c) 1991, Chris Laforet and Chris Laforet Software.\n\n");

	if (cptr = getenv("SIMPLEX"))
		strcpy(path,cptr);
	else
		getcwd(path,sizeof(path));

	if (path[0] && path[strlen(path) - 1] != '\\')
		strcat(path,"\\");

	sprintf(oname,"%smsgarea.sav",path);
	sprintf(nname,"%smsgarea.bbs",path);

	unlink(oname);
	if (!rename(nname,oname))
		{
		if (ofd = fopen(oname,"rb"))
			{
			if (nfd = fopen(nname,"wb"))
				{
				while (fread(&omsg,sizeof(struct omsg),1,ofd))
					{
					printf("Converting area %d: \"%s\".\n",omsg.msg_number,omsg.msg_areaname);
					memset(&nmsg,0,sizeof(struct nmsg));
					nmsg.msg_number = omsg.msg_number;
					strcpy(nmsg.msg_areaname,omsg.msg_areaname);
					nmsg.msg_flags = omsg.msg_flags;
					nmsg.msg_readpriv = omsg.msg_readpriv;
					nmsg.msg_writepriv = omsg.msg_writepriv;
					nmsg.msg_sysoppriv = omsg.msg_sysoppriv;
					if (!fwrite(&nmsg,sizeof(struct nmsg),1,nfd))
						{
						printf("Error: Unable to write new record.  Aborting.\n");
						fclose(nfd);
						nfd = NULL;
						fclose(ofd);
						ofd = NULL;
						rename(oname,nname);
						break;
						}
					}

				if (nfd)
					fclose(nfd);
				}
			else
				{
				printf("Error: Unable to open new msgarea.bbs file.\n");
				rename(oname,nname);
				fclose(ofd);
				ofd = NULL;
				}
			if (ofd)
				fclose(ofd);
			}
		else
			{
			printf("Error: Unable to open old msgarea.bbs file.\n");
			rename(oname,nname);
			}

		}
	else
		printf("Error: Unable to rename msgarea.bbs\n");

	return 0;
	}

