/* sapack.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 19 August 1992
**
** Revision Information: $Logfile:   G:/simplex/sapack/vcs/sapack.c_v  $
**                       $Date:   25 Oct 1992 14:13:38  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>
#include <signal.h>
#include <conio.h>
#include <ctype.h>
#if defined(BOUND)
	#define INCL_DOSDATETIME
	#include <os2.h>
#endif
#include "sapack.h"



#define COPYBUFLEN			500

char copybuffer[COPYBUFLEN];
char bbspath[256];



int get_date(void)
	{
	DATETIME dt;

	DosGetDateTime(&dt);
	return ((dt.year - 1980) << 9) | (dt.month << 5) | dt.day;
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
	struct adh tadh;
	struct adh tadh1;
	char buffer[100];
	char buffer1[100];
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char *cptr;
	long offset;
	long prev = -1L;
	long length;
	int today;
	int total = 0;
	int deleted = 0;
	int date_deleted = 0;
	int len;
	FILE *fd;
	FILE *fd1;

	fprintf(stderr,"SAPACK (v %u.%02u of %s): Simplex Advertising File Packing Utility.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1992, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (argc == 2)
		{
		if (cptr = getenv("SIMPLEX"))		/* now for our path */
			strcpy(buffer,cptr);
		else
			getcwd(buffer,sizeof(buffer));
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		sprintf(bbspath,buffer);

		signal(SIGINT,signal_trap);				/* first, down with Ctrl-C */
		signal(SIGBREAK,signal_trap);			/* then down with Ctrl-Break */

		_splitpath(argv[1],drive,path,fname,ext);

		sprintf(buffer,"%s%s.ad",bbspath,fname);
		sprintf(buffer1,"%s%s.bak",bbspath,fname);

		if (fd = fopen(buffer,"rb"))
			{
			if (fd1 = fopen(buffer1,"w+b"))
				{
				today = get_date();

				/* now walk the chain of ads to the end */
				while (fread(&tadh,sizeof(struct adh),1,fd))
					{
					++total;

					if (!(tadh.adh_flags & ADH_DELETED) && tadh.adh_killdate <= today)		/* check date of deletion and delete if need be */
						{
						tadh.adh_flags |= ADH_DELETED;
						++date_deleted;
						}

					if (!(tadh.adh_flags & ADH_DELETED))
						{
						offset = ftell(fd1);
						if (prev != -1L)
							{
							fseek(fd1,prev,SEEK_SET);
							if (fread(&tadh1,1,sizeof(struct adh),fd1))
								{
								tadh1.adh_next = offset;

								fseek(fd1,prev,SEEK_SET);
								fwrite(&tadh1,1,sizeof(struct adh),fd1);
								}

							fseek(fd1,offset,SEEK_SET);
							}

						memcpy(&tadh1,&tadh,sizeof(struct adh));

						tadh1.adh_prev = prev;
						tadh1.adh_next = -1L;
						prev = offset;

						fwrite(&tadh1,1,sizeof(struct adh),fd1);

						length = tadh1.adh_length;
						while (length > 0)
							{
							if (length >= (long)COPYBUFLEN)
								len = COPYBUFLEN;
							else
								len = (int)length;
							length -= (long)len;

							len = fread(copybuffer,1,len,fd);
							if (len)
								fwrite(copybuffer,len,1,fd1);
							}
						}
					else 
						++deleted;

					if (tadh.adh_next == -1L)
						break;
					fseek(fd,tadh.adh_next,SEEK_SET);
					}
				fclose(fd1);
				}
			else
				{
				fprintf(stderr,"Fatal Error: Unable to create \"%s\"...\n",buffer);
				fclose(fd);
				return 1;
				}
			fclose(fd);

			unlink(buffer);
			rename(buffer1,buffer);

			printf("Total entries originally in file: %u\n",total);
			printf("   Total entries deleted by date: %u\n",date_deleted);
			printf("            Total entries purged: %u\n",deleted);
			printf("         Total entries remaining: %u\n\n",total - deleted);
			}
		else
			{
			fprintf(stderr,"Fatal Error: Unable to open \"%s\"...\n",buffer);
			return 1;
			}

		}
	else
		{
		printf("Usage is: SAPACK filename[.ad]\n\n");
		return 1;
		}

	return 0;
	}

