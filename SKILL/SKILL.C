/* skill.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 16 March 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <direct.h>
#include "skill.h"



#define MAX_AREA			9999


char bbspath[81];
short kill_flag = 0;
short confirm_flag = 1;
unsigned long freed = 0L;
DATE_T kill_date = INVALID;
struct cfg cfg;

short *areas  = NULL;
short cur_areas = 0;
short max_areas = 0;

struct file **files = NULL;
short cur_files = 0;
short max_files = 0;



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
	char buffer[100];
	char *cptr;
	int count;
	int kount;
	int list = 0;
	int temp;
	FILE *fd;

	setbuf(stderr,NULL);		/* make sure that stderr is completely unbuffered */
	setbuf(stdout,NULL);		/* make sure that stderr is completely unbuffered */

#if defined(PROTECTED)
	fprintf(stderr,"SKILL/2 (v %d.%02d of %s): Simplex/2 File Area Cleaning Utility.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#else
	fprintf(stderr,"SKILL (v %d.%02d of %s): Simplex File Area Cleaning Utility.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#endif
	fprintf(stderr,"Copyright (c) 1993, Chris Laforet and Chris Laforet Software.\n");
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

		sprintf(buffer,"%sfilearea.bbs",bbspath); 		/* now, open file area file and load it */
		if (!(fd = fopen(buffer,"r+b")))
			{
			printf("Error: Unable to find/open filearea.bbs\n",buffer);
			return 1;
			}
		while (fread(&tfile,sizeof(struct file),1,fd))
			{
			if (cur_files >= max_files)
				{
				if (!(files = realloc(files,(max_files += 25) * sizeof(struct file *))))
					{
					fprintf(stderr,"Fatal Error: Out of memory.\n");
					return 1;
					}
				}

			if (!(files[cur_files] = malloc(sizeof(struct file))))
				{
				fprintf(stderr,"Fatal Error: Out of memory.\n");
				return 1;
				}
			memcpy(files[cur_files],&tfile,sizeof(struct file));
			++cur_files;
			}
		fclose(fd);

		for (count = 1; count < argc; count++)
			{
			cptr = argv[count];
			if (*cptr == '-' || *cptr == '/')
				{
				++cptr;
				switch (*cptr)
					{
					case 'L':
					case 'l':
						printf("Listing of All File Areas:\n");
						printf("-------------------------\n\n");
						for (kount = 0; kount < cur_files; kount++)
							printf("   %4d -> %s\n",files[kount]->file_number,files[kount]->file_areaname);
						list = 1;
						break;

					case 'K':
					case 'k':
						kill_flag = 1;
						break;

					case 'D':
					case 'd':
						temp = atoi(cptr + 1);
						if (temp > 0)
							kill_date = new_date(get_cdate(),temp);
						else
							fprintf(stderr,"Invalid number of days (%d) provided.\n",temp);
						break;

					case 'A':
					case 'a':
						temp = atoi(cptr + 1);
						if (temp > 0 && temp <= MAX_AREA)
							{
							if (cur_areas >= max_areas)
								{
								if (!(areas = realloc(areas,(max_areas += 10) * sizeof(short))))
									{
									fprintf(stderr,"Fatal Error: Out of memory.\n");
									return 1;
									}
								}
							areas[cur_areas++] = temp;
							}
						else
							fprintf(stderr,"Invalid area \"%u\" provided.\n",temp);
						break;

					case 'Y':
					case 'y':
						confirm_flag = 0;
						break;

					default:
						fprintf(stderr,"Bad flag \"%s\"\n",argv[count]);
						break;
					}
				}
			else
				fprintf(stderr,"Bad argument \"%s\"\n",argv[count]);
			}

		if (kill_date != INVALID)
			{
			fprintf(stderr,"Kill date has been set to %02u/%02u/%02u.\n",GET_MONTH(kill_date),GET_DAY(kill_date),(GET_YEAR(kill_date) + 80) % 100);
			for (count = 0; count < cur_areas; count++)
				{
				for (kount = 0; kount < cur_files; kount++)
					{
					if (files[kount]->file_number == areas[count])
						{
						fprintf(stderr,"Processing area %u (%s)...\n",areas[count],files[kount]->file_areaname);
						kill_files(files[kount]);
						break;
						}
					}
				}
			fprintf(stderr,"\n%s %lu bytes.\n",(char *)(kill_flag ? "Deleted" : "Would delete"),freed);
			}
		else if (!list)
			{
			fprintf(stderr,"Error: No kill date/kill days provided.\n");
			return 1;
			}
		}
	else
		{
#if defined(PROTECTED)
		printf("Usage is: skill2 -options\n\n");
#else
		printf("Usage is: skill -options\n\n");
#endif
		printf("where options may be:\n");
		printf("\t-k      Kill files instead of merely showing filenames.\n");
		printf("\t-dxx    Sets maximum days for file killing to xx\n");
		printf("\t-axxx   Sets area number for file killing.\n");
		printf("\t-y      Do not ask for confirmation when killing each file.\n");
		printf("\t-l      List all file areas to stdout.\n");
		return 1;
		}
	return 0;
	}



