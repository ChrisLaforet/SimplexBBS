/* s_arc.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 20 November 1990
**
** Revision Information: $Logfile:   G:/simplex/smp/vcs/s_arc.c_v  $
**                       $Date:   25 Oct 1992 14:26:26  $
**                       $Revision:   1.10  $
**
*/


#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>
#include <process.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "smp.h"




char *dayext[7] =
	{
	"su",
	"mo",
	"tu",
	"we",
	"th",
	"fr",
	"sa",
	};


char arc_name[512];
char file_name[512];
extern time_t curtime;
extern struct tm *tmptr;



unsigned long unique(void)
	{
	unsigned long pval = 0L;
	unsigned long tme;

	time(&tme);
	localtime(&tme);
	if (!pval || tme != pval)
		pval = tme;
	else
		pval = tme + 1L;
	return pval;
	}



void delete_truncated(void)
	{
	struct fi tfi;
	char buffer[100];
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
 	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char *outpath;
	char *cptr;
	int count;
	int kount;
	int tval;
	int rtn;
	int found = 0;

	printf("Checking for outbound directories....\n");

	outpath = get_outbound(cfg.cfg_zone);
	if (outpath[0] && outpath[strlen(outpath) - 1] == P_CSEP)
		outpath[strlen(outpath) - 1] = (char)'\0';

	_splitpath(outpath,drive,path,fname,ext);
	_makepath(buffer,drive,path,fname,"*");

	rtn = get_firstdir(&tfi,buffer);
	while (rtn)
		{
		_splitpath(tfi.fi_name,drive,path,fname,ext);

		cptr = ext;
		while (*cptr && !isxdigit(*cptr))
			++cptr;
		if (*cptr)
			sscanf(cptr,"%x",&tval);
		else
			tval = cfg.cfg_zone;
		if (tval >= 0 && tval < 256)
			{
			outbound[tval] = (char)1;
			if (tval == cfg.cfg_zone)
				outbound[0] = (char)1;
			}

		rtn = get_nextdir(&tfi);
		}
	get_closedir();		/* close directory handle if needed */

	printf("Deleting old truncated archives....\n");
	for (count = 0; count < 255; count++)
		{
		if (outbound[count])
			{
			outpath = get_outbound(count);
			for (kount = 0; kount < 7; kount++)
				{
				if (kount != tmptr->tm_wday)
					{
					sprintf(buffer,"%s*.%s?",outpath,dayext[kount]);

					rtn = get_firstfile(&tfi,buffer);
					while (rtn)
						{
						if (!tfi.fi_size)
							{
							printf("  Deleting old %s....    \r",tfi.fi_name);
							sprintf(buffer,"%s%s",outpath,tfi.fi_name);
							unlink(buffer);
							found = 1;
							}

						rtn = get_nextfile(&tfi);
						}
					get_closefile();		/* close directory handle if needed */
					}
				}
			}
		}
	printf("\n");
	if (found)
		printf("\n");
	}



int archive(int type,char *filename,char *newname,int dzone,int dnet,int dnode)
	{
	struct fi tfi;
	char **args = NULL;
	int cur_args = 0;
	int max_args = 0;
	char buffer[100];
	char buffer1[100];
	char buffer2[100];
	char arcname[15];
 	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char *cptr;
	char *cptr1;
	char *cptr2;
	char *outpath;
	unsigned long tlong;
	long smallest_size = -1L;
	int smallest;
	int count;
	int ctype;
	int rtn = 0;
	int ok = 0;
	int found = 0;
	int gotarc = 0;
	int gotfile = 0;
	FILE *fd;

	_splitpath(filename,buffer,buffer,fname,ext);
	outpath = get_outbound(dzone);

	for (count = 0; count < 10; count++)
		{
		sprintf(buffer,"%s%04x%04x.%s%d",outpath,dnet - net,dnode - node,dayext[tmptr->tm_wday],count);

		rtn = get_firstfile(&tfi,buffer);
		get_closefile();		/* close directory handle if needed */

		if (!rtn || !tfi.fi_size)
			{
			found = 1;
			if (!tfi.fi_size)
				unlink(buffer);
			break;
			}
		else
			{
			if (tfi.fi_size < smallest_size)
				{
				smallest = count;
				smallest_size = tfi.fi_size;
				}
			}
		}

	if (!found)
		{
		sprintf(arcname,"%04x%04x.%s%d",dnet - net,dnode - node,dayext[tmptr->tm_wday],smallest);
		printf("   Appending to archive %s\n",arcname);
		}
	else
		{
		sprintf(arcname,"%04x%04x.%s%d",dnet - net,dnode - node,dayext[tmptr->tm_wday],count);
		printf("   Creating new archive %s\n",arcname);
		}

	switch (type)
		{
		case ARC:
			strcpy(buffer2,cfg.cfg_arc);
			break;
		case ZIP:
			strcpy(buffer2,cfg.cfg_zip);
			break;
		case LZH:
			strcpy(buffer2,cfg.cfg_lzh);
			break;
		case ARJ:
			strcpy(buffer2,cfg.cfg_arj);
			break;
		case ZOO:
			strcpy(buffer2,cfg.cfg_zoo);
			break;
		}

	if (buffer2[0])
		{
		sprintf(buffer,"%s%s",outpath,filename);					/* now to rename to a unique packet name */
		tlong = unique();
		sprintf(buffer1,"%s%08.8lx.pkt",outpath,tlong);
		if (!rename(buffer,buffer1))
			{
			sprintf(buffer,"%s%s",outpath,arcname);		/* buffer contains %A and buffer1 contains %F */

			cptr = strtok(buffer2," \t\0");
			while (cptr)	/* strtok returns NULL or pointer */
				{
				ctype = 0;
				cptr1 = cptr;
				while (*cptr1 && *(cptr1 + 1))
					{
					if (*cptr1 == (char)'%')
						{
						if (*(cptr1 + 1) == (char)'a' || *(cptr1 + 1) == (char)'A')
							{
							ctype = 1;
							break;
							}
						if (*(cptr1 + 1) == (char)'f' || *(cptr1 + 1) == (char)'F')
							{
							ctype = 2;
							break;
							}
						}
					++cptr1;
					}

				if ((cur_args + 1) >= max_args)
					{
					if (!(args = realloc(args,(max_args += 5) * sizeof(char *))))
						{
						printf("Error: Out of memory to allocate archive args\n");
						return -1;
						}
					}

				cptr1 = cptr;
				while (isspace(*cptr1))		/* walk off leading spaces */
					++cptr1;
				if (!ctype)
					args[cur_args++] = cptr1;
				else if (ctype == 1)
					{
					cptr2 = arc_name;
					while (*cptr1)
						{
						if (*cptr1 == (char)'%' && (*(cptr1 + 1) == (char)'a' || *(cptr1 + 1) == (char)'A'))
							{
							strcpy(cptr2,buffer);
							cptr2 += strlen(buffer);
							cptr1 += 2;
							}
						else 
							*cptr2++ = *cptr1++;
						}
					gotarc = 1;
					args[cur_args++] = arc_name;
					}
				else
					{
					cptr2 = file_name;
					while (*cptr1)
						{
						if (*cptr1 == (char)'%' && (*(cptr1 + 1) == (char)'f' || *(cptr1 + 1) == (char)'F'))
							{
							strcpy(cptr2,buffer1);
							cptr2 += strlen(buffer1);
							cptr1 += 2;
							}
						else 
							*cptr2++ = *cptr1++;
						}
					gotfile = 1;
					args[cur_args++] = file_name;
					}
				args[cur_args] = NULL;
				cptr = strtok(NULL," \t\0");
				}
	
			if (gotarc && gotfile)
				{
				if ((rtn = spawnvp(P_WAIT,args[0],args)) == -1)
					{
					switch (errno)
						{
						case ENOENT:
							printf("Error: Unable to find %s!",args[0]);
							break;
						case ENOEXEC:
							printf("Error: Unable to run %s!",args[0]);
							break;
						case ENOMEM:
							printf("Error: Out of memory [%s %u]!",__FILE__,__LINE__);
							break;
						default:
							break;
						}
					}
				else if (!rtn)
					ok = 1;
				free(args);
				}
			else
				printf("Error: No %%A or %%F options were found on archive command line.\n");

			if (!ok)
				{
				sprintf(buffer,"%s%s",outpath,filename);					/* now to rename to a unique packet name */
				tlong = unique();
				sprintf(buffer1,"%s%08.8lx.pkt",outpath,tlong);
				if (rename(buffer1,buffer))
					printf("Error: Unable to rename %08.8lx.pkt back to %s\n",tlong,filename);
				}
			else
				{
				unlink(buffer1);		/* delete the PKT file if archiver did not */
				cptr = ext;
				if (*cptr == (char)'.')
					++cptr;
				sprintf(buffer,"%s%s.%clo",outpath,fname,tolower(*cptr));
				if (!(fd = fopen(buffer,"r+b")))
					{
					if (!(fd = fopen(buffer,"w+b")))
						printf("Error: Unable to open/create routing file %s.%clo",fname,tolower(*cptr));
					printf("    Routing file %s.%clo has been created.\n",fname,tolower(*cptr));
					}

				newname[0] = '\0';
				if (fd)
					{
					sprintf(buffer,"%s%s",outpath,arcname);		/* archive name */
					strlwr(buffer);		/* lowercase archive name */
					sprintf(newname,"%s.%clo",fname,tolower(*cptr));
					found = 0;
					while (fgets(buffer2,sizeof(buffer2),fd))
						{
						strlwr(buffer2);
						if (strstr(buffer2,buffer))
							{
							printf("    Archive file %s already listed for transmission.\n",arcname);
							found = 1;
							break;
							}
						}

					if (!found)
						{
						printf("    Listing archive file %s for transmission.\n",arcname);
						fseek(fd,0L,SEEK_END);
						if (ftell(fd))
							{
							fseek(fd,-1L,SEEK_END);
							rtn = fgetc(fd);
							fseek(fd,0L,SEEK_END);			/* must seek between read and write */
							if (rtn != '\r' && rtn != '\n')
								fprintf(fd,"\r\n");
							}
						fprintf(fd,"#%s\r\n",buffer);
						}

					fclose(fd);
					}
				}
			}
		else
			printf("Error: Unable to rename %s\n",filename);
		}
	else
		printf("Error: No archiver provided for %s in config.bbs\n",type == ARC ? "ARCing" : (type == ZIP ? "ZIPping" : (type == LZH ? "LZHing" : (type == ARJ ? "ARJing" : "ZOOing"))));

	if (ok)
		return 1;
	return 0;
	}



void generate_poll(int dzone,int dnet,int dnode)
	{
	char buffer[100];
	char *outpath;
	char *cptr;
	int count;
	int polled = 0;
	FILE *fd;

	printf("POLL %u:%u/%u ",dzone,dnet,dnode);

	outpath = get_outbound(dzone);
	strcpy(buffer,outpath);		/* let's create directory if it doesn't exist! */
	if (buffer[0])
		{
		cptr = buffer + (strlen(buffer) - 1);
		if (*cptr == P_CSEP)
			*cptr = (char)'\0';
		if (access(buffer,0))
			{
			printf("Notice: Attempting to create outbound directory for zone %u\n",dzone);
			if (!mkdir(buffer))
				outbound[dzone] = (char)1;
			}
		}

//	for (count = 0; count < 3; count++)
//		{
//		sprintf(buffer,"%s%04x%04x.%s",outpath,dnet,dnode,count == 2 ? "flo" : (count == 1 ? "clo" : "dlo"));
//		if (!access(buffer,0))
//			polled = 1;
//		}


	for (count = 0; count < 2; count++)
		{
		sprintf(buffer,"%s%04x%04x.%s",outpath,dnet,dnode,count == 1 ? "cut" : "clo");
		if (!access(buffer,0))
			polled = 1;
		}

	if (polled)
		printf("-- POLL is not needed.\n");
	else
		{
		sprintf(buffer,"%s%04x%04x.%s",outpath,dnet,dnode,"clo");
		if (!(fd = fopen(buffer,"w")))
			printf("\nError: Unable to generate POLL file.\n");
		else
			{
			printf("-- POLL generated.\n");
			fclose(fd);
			}
		}
	}
