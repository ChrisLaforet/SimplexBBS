/* s_unarc.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 May 1990
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_unarc.c_v  $
**                       $Date:   25 Oct 1992 14:18:12  $
**                       $Revision:   1.12  $
**
*/


#include <stdio.h>
#include <errno.h>
#include <process.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <stdlib.h>
#include "smail.h"




char source[512];
char destination[512];
extern char **files;
extern int cur_files;
extern int max_files;


int unarchive(char *command,char *filename)
	{
	char **args = NULL;
	int cur_args = 0;
	int max_args = 0;
	char buffer[300];
	char buffer1[120];
	char buffer2[120];
	char *cptr;
	char *cptr1;
	char *cptr2;
	int gotsrc = 0;
	int gotdest = 0;
	int rtn = -1;
	int type;		/* if 0, normal arg.  if 1, %S found.  if 2, %D found */

	cptr = command;
	while (*cptr && isspace(*cptr))
		++cptr;
	if (*cptr)
		{
		strcpy(buffer,cptr);

		strcpy(buffer1,cfg.cfg_inboundpath);
		if (buffer1[0] && buffer1[strlen(buffer1) - 1] != P_CSEP)
			strcat(buffer1,P_SSEP);
		strcat(buffer1,filename);		/* buffer now holds %S string (source file) */

		strcpy(buffer2,cfg.cfg_packetpath);
		if (buffer2[0] && buffer2[strlen(buffer2) - 1] != P_CSEP)
			strcat(buffer2,P_SSEP);		/* buffer now holds %D or %P string (destination path) */

		cptr = strtok(buffer," \t\0");
		while (cptr)	/* strtok returns NULL or pointer */
			{
			type = 0;
			cptr1 = cptr;
			while (*cptr1 && *(cptr1 + 1))
				{
				if (*cptr1 == '%')
					{
					if (*(cptr1 + 1) == 's' || *(cptr1 + 1) == 'S')
						{
						type = 1;
						break;
						}
					if (*(cptr1 + 1) == 'd' || *(cptr1 + 1) == 'D')
						{
						type = 2;
						break;
						}
					if (*(cptr1 + 1) == 'p' || *(cptr1 + 1) == 'P')
						{
						type = 2;
						break;
						}
					}
				++cptr1;
				}

			if ((cur_args + 1) >= max_args)
				{
				if (!(args = realloc(args,(max_args += 5) * sizeof(char *))))
					{
					fprintf(stderr,"Fatal Error: Out of memory to allocate archive args\n");
					return -1;
					}
				}
			cptr1 = cptr;
			while (isspace(*cptr1))		/* walk off leading spaces */
				++cptr1;
			if (!type)
				args[cur_args++] = cptr1;
			else if (type == 1)
				{
				cptr2 = source;
				while (*cptr1)
					{
					if (*cptr1 == '%' && (*(cptr1 + 1) == 's' || *(cptr1 + 1) == 'S'))
						{
						strcpy(cptr2,buffer1);
						cptr2 += strlen(buffer1);
						cptr1 += 2;
						}
					else 
						*cptr2++ = *cptr1++;
					}
				gotsrc = 1;
				args[cur_args++] = source;
				}
			else
				{
				cptr2 = destination;
				while (*cptr1)
					{
					if (*cptr1 == '%' && (*(cptr1 + 1) == 'd' || *(cptr1 + 1) == 'D'))
						{
						strcpy(cptr2,buffer2);
						cptr2 += strlen(buffer2);
						cptr1 += 2;
						}
					else if (*cptr1 == '%' && (*(cptr1 + 1) == 'p' || *(cptr1 + 1) == 'P'))
						{
						strcpy(cptr2,buffer2);
						cptr2 += strlen(buffer2) - 1;		/* strip off trailing backslash */
						*cptr2 = (char)'\0';
						cptr1 += 2;
						}
					else 
						*cptr2++ = *cptr1++;
					}
				gotdest = 1;
				args[cur_args++] = destination;
				}
			args[cur_args] = NULL;
			cptr = strtok(NULL," \t\0");
			}
		if (gotsrc && gotdest)
			{
			if ((rtn = spawnvp(P_WAIT,args[0],args)) == -1)
				{
				switch (errno)
					{
					case ENOENT:
						fprintf(stderr,"Error: Unable to find %s!",args[0]);
						break;
					case ENOEXEC:
						fprintf(stderr,"Error: Unable to run %s!",args[0]);
						break;
					case ENOMEM:
						fprintf(stderr,"Error: Out of memory [%s %u]!",__FILE__,__LINE__);
						break;
					default:
						break;
					}
				}
			free(args);
			}
		else
			fprintf(stderr,"Error: No %%S, %%D or %%P options were found on archive command line.\n");
		}
	return rtn;
	}



void unarc_packets(void)
	{
	struct fi tfi;
	struct ph tph;
	char buffer[120];
	char tbuf[4];
	char *cptr;
	int rtn;
	int count;
	int found;
	int unarced;
	FILE *fd;

	fprintf(stderr,"\nSearching for unarchived mail packets...\n");
	if (!cfg.cfg_packetpath[0])
		{
		fprintf(stderr,"Error: An exclusive un-archiving packet directory MUST be provided.\n");
		return;
		}

	/* first to check the inbound path for Archived mail files */
	strcpy(buffer,cfg.cfg_inboundpath);
	if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
		strcat(buffer,P_SSEP);
	strcat(buffer,"*.*");

	rtn = get_firstfile(&tfi,buffer);
	while (rtn)
		{
		found = 1;		/* arcmail is always HHHHHHHH.* like 09af0912.mo1 */
		cptr = tfi.fi_name;
		count = 0;
		while (count < 8)
			{
			if (!isxdigit(*cptr))
				{
				found = 0;
				break;
				}
			++cptr;
			++count;
			}
		if (found && *cptr++ != '.')
			found = 0;
		if (found)
			{
			if (cur_files >= max_files)
				{
				if (!(files = realloc(files,(max_files += 10) * sizeof(char *))))
					{
					fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for filenames.\n");
					exit(1);
					}
				}
			if (!(files[cur_files] = calloc(strlen(tfi.fi_name) + 1,sizeof(char))))
				{
				fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for filenames.\n");
				exit(1);
				}
			strcpy(files[cur_files],tfi.fi_name);
			++cur_files;
			}
		rtn = get_nextfile(&tfi);
		}

	if (cur_files)
		{
		for (count = 0; count < cur_files; count++)
			{
			strcpy(buffer,cfg.cfg_inboundpath);
			if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
				strcat(buffer,P_SSEP);
			strcat(buffer,files[count]);
			if (fd = fopen(buffer,"rb"))
				{
				tbuf[0] = '\0';
				fread(tbuf,4,1,fd);
				fclose(fd);
				/* first to unarchive any ARCmail as long as we can do it! */
				if (tbuf[0] == '\x1a')
					{
					if (cfg.cfg_unarc[0])
						{
						fprintf(stderr,"Attempting to unARC %s \n",files[count]);
						if (!unarchive(cfg.cfg_unarc,files[count]))
							{
							fprintf(stderr,"Successful - deleting %s.\n",files[count]);
							unlink(buffer);
							unarced = 1;
							}
						else
							fprintf(stderr,"Failed - keeping %s.\n",files[count]);
						}
					else
						fprintf(stderr,"Error: No unarchiver to unARC %s\n",files[count]);
					}
				else if (tbuf[0] == 'P' && tbuf[1] == 'K')
					{
					if (cfg.cfg_unzip[0])
						{
						fprintf(stderr,"Attempting to unZIP %s \n",files[count]);
						if (!unarchive(cfg.cfg_unzip,files[count]))
							{
							fprintf(stderr,"Successful - deleting %s.\n",files[count]);
							unlink(buffer);
							unarced = 1;
							}
						else
							fprintf(stderr,"Failed - keeping %s.\n",files[count]);
						}
					else
						fprintf(stderr,"Error: No unarchiver to unZIP %s\n",files[count]);
					}
				else if (tbuf[0] == 0x60 && tbuf[1] == 0xea)
					{
					if (cfg.cfg_unarj[0])
						{
						fprintf(stderr,"Attempting to unARJ %s \n",files[count]);
						if (!unarchive(cfg.cfg_unarj,files[count]))
							{
							fprintf(stderr,"Successful - deleting %s.\n",files[count]);
							unlink(buffer);
							unarced = 1;
							}
						else
							fprintf(stderr,"Failed - keeping %s.\n",files[count]);
						}
					else
						fprintf(stderr,"Error: No unarchiver to unZIP %s\n",files[count]);
					}
				else if (tbuf[0] == 'Z' && tbuf[1] == 'O')
					{
					if (cfg.cfg_unzoo[0])
						{
						fprintf(stderr,"Attempting to unZOO %s \n",files[count]);
						if (!unarchive(cfg.cfg_unzoo,files[count]))
							{
							fprintf(stderr,"Successful - deleting %s.\n",files[count]);
							unlink(buffer);
							unarced = 1;
							}
						else
							fprintf(stderr,"Failed - keeping %s.\n",files[count]);
						}
					else
						fprintf(stderr,"Error: No unarchiver to unZOO %s\n",files[count]);
					}
				else if (tbuf[2] == '-' && tbuf[3] == 'l')		/* bytes 2 and 3 are "-l" for LZH */
					{
					if (cfg.cfg_unlzh[0])
						{
						fprintf(stderr,"Attempting to unLZH %s \n",files[count]);
						if (!unarchive(cfg.cfg_unlzh,files[count]))
							{
							fprintf(stderr,"Successful - deleting %s.\n",files[count]);
							unlink(buffer);
							unarced = 1;
							}
						else
							fprintf(stderr,"Failed - keeping %s.\n",files[count]);
						}
					else
						fprintf(stderr,"Error: No unarchiver to unLZH %s\n",files[count]);
					}
				}
			}
		for (count = 0; count < cur_files; count++)
			free(files[count]);
		free(files);
		files = NULL;
		cur_files = 0;
		max_files = 0;
		}

	/* now to delete non-possible packet files! */
	if (unarced)
		{
		strcpy(buffer,cfg.cfg_packetpath);
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		strcat(buffer,"*.*");


		rtn = get_firstfile(&tfi,buffer);
		while (rtn)
			{
			if (cur_files >= max_files)
				{
				if (!(files = realloc(files,(max_files += 10) * sizeof(char *))))
					{
					fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for filenames.\n");
					exit(1);
					}
				}
			if (!(files[cur_files] = calloc(strlen(tfi.fi_name) + 1,sizeof(char))))
				{
				fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for filenames.\n");
				exit(1);
				}
			strcpy(files[cur_files],tfi.fi_name);
			++cur_files;
			rtn = get_nextfile(&tfi);
			}

		if (cur_files)
			{
			for (count = 0; count < cur_files; count++)
				{
				strcpy(buffer,cfg.cfg_packetpath);
				if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
					strcat(buffer,P_SSEP);
				strcat(buffer,files[count]);
				found = 0;
				if (fd = fopen(buffer,"rb"))
					{
					if (filelength(fileno(fd)) >= (long)sizeof(struct ph))
						{
						fread(&tph,sizeof(struct ph),1,fd);
						if (tph.ph_sig1 == 2 && tph.ph_sig2 == 0)		/* valid packet! */
							found = 1;
						}
					fclose(fd);
					}
				if (!found)
					printf("Leaving \"%s\" because it is not a packet file.\n",files[count]);
				}
			for (count = 0; count < cur_files; count++)
				free(files[count]);
			free(files);
			cur_files = 0;
			max_files = 0;
			files = NULL;
			}
		}
	}



