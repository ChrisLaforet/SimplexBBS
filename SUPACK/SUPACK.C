/* supack.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 October 1991
**
** Revision Information: $Logfile:   G:/simplex/supack/vcs/supack.c_v  $
**                       $Date:   25 Oct 1992 14:29:44  $
**                       $Revision:   1.1  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <signal.h>
#include <io.h>
#include "supack.h"



char path[100];
int *users = NULL;
int *lr_ptrs = NULL;
int lr_count = 0;
int max_users = 0;
int success = 0;
int deleted = 0;
FILE *sfd = NULL;
FILE *dfd = NULL;




void exit_list(void)
	{
	char buffer1[120];
	char buffer2[120];

	if (sfd)
		fclose(sfd);
	if (dfd)
		fclose(dfd);
	if (success)
		{
		sprintf(buffer1,"%suserlist.bbs",path);
		sprintf(buffer2,"%suserlist.bak",path);
		unlink(buffer1);
		if (rename(buffer2,buffer1))
			fprintf(stderr,"\a\a\aSerious Error:  Cannot rename userlist.bak to userlist.bbs!\n");
		else
			{
			sprintf(buffer1,"%smsgcomb.bbs",path);
			sprintf(buffer2,"%smsgcomb.bak",path);
			unlink(buffer1);
			if (rename(buffer2,buffer1))
				fprintf(stderr,"\a\a\aSerious Error:  Cannot rename msgcomb.bak to msgcomb.bbs!\n");
			sprintf(buffer1,"%smsgread.bbs",path);
			sprintf(buffer2,"%smsgread.bak",path);
			unlink(buffer1);
			if (rename(buffer2,buffer1))
				fprintf(stderr,"\a\a\aSerious Error:  Cannot rename msgread.bak to msgread.bbs!\n");
			}
		printf("\n\tThere were %d user%s on this system.\n",max_users,(char *)(max_users == 1 ? "" : "s"));
		printf("\tThere were %d deleted user%s.\n",deleted,(char *)(deleted == 1 ? "" : "s"));
		printf("\tThere are now %d user%s remaining.\n\n",max_users - deleted,(char *)((max_users - deleted) == 1 ? "" : "s"));
		}
	else
		{
		sprintf(buffer1,"%suserlist.bak",path);
		unlink(buffer1);
		sprintf(buffer1,"%smsgcomb.bak",path);
		unlink(buffer1);
		sprintf(buffer1,"%smsgread.bak",path);
		unlink(buffer1);
		}

	if (users)
		free(users);
	if (lr_ptrs)
		free(lr_ptrs);
	}



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
	struct user tuser;
	struct comb tcomb;
	char buffer[120];
	char *cptr;
	int current = 0;
	int newnum = 0;

#if defined(PROTECTED)
	fprintf(stderr,"SUPACK/2 (v %d.%02d%s of %s): Simplex/2 Deleted User Packing Utility.\n",MAJOR_VERSION,MINOR_VERSION,(char *)(BETA ? "á" : ""),__DATE__);
#else
	fprintf(stderr,"SUPACK (v %d.%02d%s of %s): Simplex Deleted User Packing Utility.\n",MAJOR_VERSION,MINOR_VERSION,(char *)(BETA ? "á" : ""),__DATE__);
#endif
	fprintf(stderr,"Copyright (c) 1991-92, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (cptr = getenv("SIMPLEX"))
		strcpy(path,cptr);
	else
		getcwd(path,sizeof(path));
	if (path[0] && path[strlen(path) - 1] != P_CSEP)
		strcat(path,P_SSEP);

	signal(SIGINT,signal_trap);				/* first, down with Ctrl-C */
	signal(SIGBREAK,signal_trap);			/* then down with Ctrl-Break */

	atexit(exit_list);

	sprintf(buffer,"%suserlist.bbs",path);
	if (!(sfd = fopen(buffer,"rb")))
		{
		fprintf(stderr,"Error: Unable to open userlist.bbs.\n");
		return 1;
		}
	sprintf(buffer,"%suserlist.bak",path);
	if (!(dfd = fopen(buffer,"wb")))
		{
		fprintf(stderr,"Error: Unable to create userlist.bak.\n");
		return 1;
		}

	max_users = (int)(filelength(fileno(sfd)) / (long)sizeof(struct user));
	if (!(users = (int *)calloc(max_users,sizeof(int))))
		{
		fprintf(stderr,"Error: Unable to allocate renumbering array!\n");
		return 1;
		}

	printf("1....Purging deleted user logins...\n");
	while (fread(&tuser,sizeof(struct user),1,sfd))
		{
		if (tuser.user_flags & USER_DELETED)
			{
			users[current] = -1;
			++deleted;
			}
		else
			{
			if (!fwrite(&tuser,sizeof(struct user),1,dfd))
				{
				fprintf(stderr,"Error: Unable to write to userlist.bak file!\n");
				return 1;
				}
			users[current] = newnum++;
			}
		++current;
		}
	fclose(sfd);
	fclose(dfd);
	sfd = NULL;
	dfd = NULL;

	if (deleted)
		{
		printf("2....Purging deleted users from combined area list and renumbering...\n");
		sprintf(buffer,"%smsgcomb.bbs",path);
		if (!(sfd = fopen(buffer,"rb")))
			fprintf(stderr,"Notice: Unable to open msgcomb.bbs.\n");
		else
			{
			sprintf(buffer,"%smsgcomb.bak",path);
			if (!(dfd = fopen(buffer,"wb")))
				{
				fprintf(stderr,"Error: Unable to create msgcomb.bak.\n");
				return 1;
				}

			while (fread(&tcomb,sizeof(struct comb),1,sfd))
				{
				if (tcomb.comb_user < max_users && users[tcomb.comb_user] != -1)		/* valid user number and user was not deleted */
					{
					tcomb.comb_user = users[tcomb.comb_user];		/* set up new number */
					if (!fwrite(&tcomb,sizeof(struct comb),1,dfd))
						{
						fprintf(stderr,"Error: Unable to write to msgcomb.bak file!\n");
						return 1;
						}
					}
				}
			fclose(sfd);
			fclose(dfd);
			sfd = NULL;
			dfd = NULL;
			}

		printf("3....Purging deleted users from lastread pointer list and renumbering...\n");
		sprintf(buffer,"%smsgread.bbs",path);
		if (!(sfd = fopen(buffer,"rb")))
			{
			fprintf(stderr,"Error: Unable to open msgread.bbs.\n");
			return 1;
			}
		sprintf(buffer,"%smsgread.bak",path);
		if (!(dfd = fopen(buffer,"wb")))
			{
			fprintf(stderr,"Error: Unable to create msgread.bak.\n");
			return 1;
			}

		if (!fread(&lr_count,sizeof(int),1,sfd))
			{
			fprintf(stderr,"Error: Unable to read number of areas in msgread.bbs.\n");
			return 1;
			}
		if (!(lr_ptrs = (int *)malloc(lr_count * sizeof(int))))
			{
			fprintf(stderr,"Error: Unable to allocate lastread pointer array!\n");
			return 1;
			}

		if (!fwrite(&lr_count,sizeof(int),1,dfd))
			{
			fprintf(stderr,"Error: Unable to write to msgread.bak file!\n");
			return 1;
			}

		if (!fread(lr_ptrs,lr_count,1,sfd))			/* read in area number array and write back out */
			{
			fprintf(stderr,"Error: Unable to read area pointers in msgread.bbs.\n");
			return 1;
			}
		if (!fwrite(lr_ptrs,lr_count,1,dfd))
			{
			fprintf(stderr,"Error: Unable to write to msgread.bak file!\n");
			return 1;
			}

		current = 0;
		while (fread(lr_ptrs,lr_count,1,sfd))
			{
			if (current < max_users && users[current] != -1)
				{
				if (!fwrite(lr_ptrs,lr_count,1,dfd))
					{
					fprintf(stderr,"Error: Unable to write to msgread.bak file!\n");
					return 1;
					}
				}
			++current;
			}
		fclose(sfd);
		fclose(dfd);
		sfd = NULL;
		dfd = NULL;

		success = 1;
		}
	else
		fprintf(stderr,"No users were marked as deleted.\n\n");

	return 0;
	}



