/* stop.c
**
** Copyright (c) 1991, Stardate Software/Robbie Foust
** All Rights Reserved
**
** Started: 19 August 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/



#include <stdio.h>
#include <ctype.h>
#include <io.h>
#include <direct.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stop.h"



#define MAX				10


char *months[12] =
	{
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December",
	};



struct callers *tcallers = NULL;
struct ul *tul = NULL;
struct dl *tdl = NULL;
struct reader *treader = NULL;
struct sender *tsender = NULL;
char stoppath[150];
char **unlisted = NULL;
int cur_unlisted = 0;
int max_unlisted = 0;



int comp_callers(struct callers *tcaller1,struct callers *tcaller2)
	{
	return (*tcaller2).user_calls - (*tcaller1).user_calls;
	}



int comp_ul(struct ul *tul1,struct ul *tul2)
	{
	if ((*tul2).user_uploadbytes > (*tul1).user_uploadbytes)
		return 1;
	else if ((*tul2).user_uploadbytes < (*tul1).user_uploadbytes)
		return -1;
	return 0;
	}



int comp_dl(struct dl *tdl1,struct dl *tdl2)
	{
	if ((*tdl2).user_dnloadbytes > (*tdl1).user_dnloadbytes)
		return 1;
	else if ((*tdl2).user_dnloadbytes < (*tdl1).user_dnloadbytes)
		return -1;
	return 0;
	}



int comp_readers(struct reader *treader1,struct reader *treader2)
	{
	if ((*treader2).user_msgread > (*treader1).user_msgread)
		return 1;
	else if ((*treader2).user_msgread < (*treader1).user_msgread)
		return -1;
	return 0;
	}



int comp_senders(struct sender *tsender1,struct sender *tsender2)
	{
	if ((*tsender2).user_msgsent > (*tsender1).user_msgsent)
		return 1;
	else if ((*tsender2).user_msgsent < (*tsender1).user_msgsent)
		return -1;
	return 0;
	}



int main(int argc,char *argv[])
	{
	struct exclude *exclude = NULL;
	struct tm *tmptr;
	struct user tuser;
	struct cfg tcfg;
	struct callers *clptr;
	struct ul *ulptr;
	struct dl *dlptr;
	struct reader *rdptr;
	struct sender *snptr;
	FILE *fd;
	FILE *user_fd;
	FILE *ansi_fd;
	FILE *ascii_fd;
	time_t curtime;
	char *env;
	char *cptr;
	char buffer[150];
	char fname[20];
	char excname[20];
	char date[30];
	int total = MAX;
	int tval;
	int count;
	int users = 1;
	int callers = 0;
	int ul = 0;
	int dl = 0;
	int reader = 0;
	int sender = 0;
	int ibm_ansi = 0;
	int esc2j = 0;
	int enter = 1;		/* default is add Press ENTER to continue onto end of file */
	int verbose = 0;

	fprintf(stderr,"\rSTOP (v%d.%02d%s of %s): Simplex Top Callers List!\n",MAJOR_VERSION,MINOR_VERSION,(char *)(BETA ? "á" : ""),__DATE__);
	fprintf(stderr,"Copyright (c) 1991-93, Stardate Software Inc.  All Rights Reserved.\n\n");

	if (argc < 2)
		{
		help();
		return 1;
		}

	fprintf(stderr,"Initializing ...\n");

	if ((env = getenv("SIMPLEX")) == NULL)
		env = getcwd(env,_MAX_PATH);
	strupr(env);
	strcpy(stoppath,env);
	if (stoppath[0] && stoppath[strlen(stoppath) - 1] != '\\')
		strcat(stoppath,"\\");

	excname[0] = (char)'\0';

	sprintf(buffer,"%sCONFIG.BBS",stoppath);
	if ((fd = fopen(buffer,"rb")) == NULL)
		{
		printf("Fatal Error: \aCan't open %s!\n\n",buffer);
		return 1;
		}
	fread(&tcfg,sizeof(struct cfg),1,fd);
	fclose(fd);

	sprintf(buffer,"%sUSERLIST.BBS",stoppath);

	if ((user_fd = fopen(buffer,"rb")) == NULL)
		{
		fprintf(stderr,"Fatal Error: \aCan't find %s!\n\n",buffer);
		return 1;
		}
	strcpy(fname,"stop");

	for (count = 1; count < argc; count++)
		{
		cptr = argv[count];
		if (*cptr == '-' || *cptr == '/')
			{
			++cptr;
			switch (*cptr)
				{
				case 'C':			/* top callers */
				case 'c':
					callers = 1;
					break;
				case 'U':			/* top uploaders */
				case 'u':
					ul = 1;
					break;
				case 'D':			/* top downloaders */
				case 'd':
					dl = 1;
					break;
				case 'R':			/* top msg readers */
				case 'r':
					reader = 1;
					break;
				case 'S':			/* top msg senders */
				case 's':
					sender = 1;
					break;
				case 'N':			/* number of entries */
				case 'n':
					++cptr;
					if (*cptr)
						{
						tval = atoi(cptr);
						if (tval && tval < 100)
							{
							total = tval;
							fprintf(stderr,"Writing %d users instead of the default of 10.\n",tval);
							}
						else
							fprintf(stderr,"Error: Number of entries \"%d\" is an illegal amount.\n",tval);
						}
					else
						fprintf(stderr,"Error: No number provided with -N option.\n");
					break;
				case 'I':			/* use IBM High-ansi (ascii, but ansi makes more sense) characters */
				case 'i':
					ibm_ansi = 1;
					break;
				case 'J':
				case 'j':
					esc2j = 1;
					break;
				case 'E':
				case 'e':
					enter = 0;
					break;
				case 'V':			/* verbose mode */
				case 'v':
					verbose = 1;
					break;
				case 'F':			/* root filename */
				case 'f':
					++cptr;
					if (*cptr)
						{
						strncpy(fname,cptr,sizeof(fname));
						cptr = fname;
						while (*cptr && *cptr != '.')
							++cptr;
						*cptr = (char)'\0';

						if (!fname[0])
							{
							fprintf(stderr,"Error: No valid filename provided with -F option. Using default name.\n");
							strcpy(fname,"stop");
							}
						else
							fprintf(stderr,"Output will go to file \"%s.a??\".\n",fname);
						}
					else
						{
						fprintf(stderr,"Error: No filename provided with -F option.\n");
						strcpy(fname,"stop");
						}
					break;
				case 'X':				/* exclude names file */
				case 'x':
					++cptr;
					if (*cptr)
						{
						strncpy(excname,cptr,sizeof(excname));
						cptr = excname;
						while (*cptr)
							++cptr;
						*cptr = (char)'\0';

						if (!excname[0])
							fprintf(stderr,"Error: No valid filename provided with -X option.  Not excluding any names ...\n");
						else
							fprintf(stderr,"Exluding names in file \"%s\".\n",excname);
						}
					else
						fprintf(stderr,"Error: No filename provided with -X option.\n");
					break;
				default:
					fprintf(stderr,"Error: Unknown option: \"%s\"\n",argv[count]);
					break;
				}
			}
		else
			fprintf(stderr,"Error: Unknown option: \"%s\"\n",argv[count]);
		}

	if (!callers && !ul && !dl && !reader && !sender)
		{
		fprintf(stderr,"\nNothing to do!\n\n");
		return 1;
		}

	/***** allocate memory for structures *****/
	if (callers)
		{
		if (!(tcallers = calloc(total,sizeof(struct callers))))
			{
			fprintf(stderr,"\n\nFatal Error:  \aUnable to allocate memory for structures.\n\n");
			return 1;
			}
		}

	if (ul)
		{
		if (!(tul = calloc(total,sizeof(struct ul))))
			{
			fprintf(stderr,"\n\nFatal Error:  \aUnable to allocate memory for structures.\n\n");
			return 1;
			}
		}

	if (dl)
		{
		if (!(tdl = calloc(total,sizeof(struct dl))))
			{
			fprintf(stderr,"\n\nFatal Error:  \aUnable to allocate memory for structures.\n\n");
			return 1;
			}
		}

	if (reader)
		{
		if (!(treader = calloc(total,sizeof(struct reader))))
			{
			fprintf(stderr,"\n\nFatal Error:  \aUnable to allocate memory for structures.\n\n");
			return 1;
			}
		}

	if (sender)
		{
		if (!(tsender = calloc(total,sizeof(struct sender))))
			{
			fprintf(stderr,"\n\nFatal Error:  \aUnable to allocate memory for structures.\n\n");
			return 1;
			}
		}

	if (excname[0])
		{
		sprintf(buffer,"%s%s",stoppath,excname);
		if (fd = fopen(buffer,"r+b"))
			{
			fprintf(stderr,"Excluding names ...\n");
			while (fgets(buffer,sizeof(buffer),fd))
				{
				cptr = buffer;
				while (*cptr && *cptr != '\r' && *cptr != '\n')
					++cptr;
				*cptr = '\0';
				if (cptr != buffer)
					{
					while (cptr != buffer && isspace(*cptr))
						--cptr;
					if (!isspace(*cptr))
						++cptr;
					*cptr = '\0';
					}
				if (strlen(buffer))
					{
					if (cur_unlisted >= max_unlisted)
						{
						if (!(unlisted = realloc(unlisted,(max_unlisted += 10) * sizeof(char *))))
							{
							fprintf(stderr,"Fatal Error: \aOut of memory....\n");
							return 1;
							}
						}
					if (!(unlisted[cur_unlisted] = calloc(strlen(buffer) + 1,sizeof(char))))
						{
						fprintf(stderr,"Fatal Error: \aOut of memory....\n");
						return 1;
						}
					strcpy(unlisted[cur_unlisted],buffer);
					++cur_unlisted;
					}
				}
			fclose(fd);
			}
		}

	fprintf(stderr,"Reading and Compiling USERLIST.BBS ...\n");

	while (fread(&tuser,sizeof(struct user),1,user_fd))
		{
		int add = 1;

		if (verbose)		/* fprintf slows us way down */
			fprintf(stderr,"\rCompiling User Record #%d        ",users);

		if (!(tuser.user_flags & USER_DELETED))
			{
			if (excname[0])			/* if we are excluding names */
				{
				if (!(tuser.user_flags & 0x8000))		/* if user is not deleted */
					{
					for (count = 0; count < cur_unlisted; count++)
						{
						if (!stricmp(unlisted[count],tuser.user_name))
							{
							add = 0;
							break;
							}
						}
					}
				}

			if (add)
				{
				if (callers)		/* lets find top callers */
					{
					int least = 0x7fff;
					int which = 0;

					for (count = 0; count < total; count++)
						{
						clptr = tcallers + count;
						if (clptr->user_calls < least)
							{
							least = clptr->user_calls;
							which = count;
							}
						}
					if (least < tuser.user_calls)
						{
						clptr = tcallers + which;
						strcpy(clptr->user_name,tuser.user_name);		/* name */
						strcpy(clptr->user_city,tuser.user_city);		/* city */
						clptr->user_calls = tuser.user_calls;			/* total calls */
						}
					}

				if (ul)				/* find top uploaders */
					{
					long least = 0x7fffffffL;
					int which = 0;

					for (count = 0; count < total; count++)
						{
						ulptr = tul + count;
						if (ulptr->user_uploadbytes < least)
							{
							least = ulptr->user_uploadbytes;
							which = count;
							}
						}
					if (least < tuser.user_uploadbytes)
						{
						ulptr = tul + which;
						strcpy(ulptr->user_name,tuser.user_name);
						strcpy(ulptr->user_city,tuser.user_city);
						ulptr->user_uploadbytes = tuser.user_uploadbytes;
						ulptr->user_upload = tuser.user_upload;
						}
					}

				if (dl)				/* find top downloaders */
					{
					long least = 0x7fffffffL;
					int which = 0;

					for (count = 0; count < total; count++)
						{
						dlptr = tdl + count;
						if (dlptr->user_dnloadbytes < least)
							{
							least = dlptr->user_dnloadbytes;
							which = count;
							}
						}
					if (least < tuser.user_dnloadbytes)
						{
						dlptr = tdl + which;
						strcpy(dlptr->user_name,tuser.user_name);
						strcpy(dlptr->user_city,tuser.user_city);
						dlptr->user_dnloadbytes = tuser.user_dnloadbytes;
						dlptr->user_dnload = tuser.user_dnload;
						}
					}

				if (reader)		/* lets find top message readers */
					{
					long least = 0x7fffffffL;
					int which = 0;

					for (count = 0; count < total; count++)
						{
						rdptr = treader + count;
						if (rdptr->user_msgread < least)
							{
							least = rdptr->user_msgread;
							which = count;
							}
						}
					if (least < tuser.user_msgread)
						{
						rdptr = treader + which;
						strcpy(rdptr->user_name,tuser.user_name);		/* name */
						strcpy(rdptr->user_city,tuser.user_city);		/* city */
						rdptr->user_msgread = tuser.user_msgread;		/* msgs read */
						}
					}

				if (sender)		/* lets find top message senders */
					{
					long least = 0x7fffffffL;
					int which = 0;

					for (count = 0; count < total; count++)
						{
						snptr = tsender + count;
						if (snptr->user_msgsent < least)
							{
							least = snptr->user_msgsent;
							which = count;
							}
						}
					if (least < tuser.user_msgsent)
						{
						snptr = tsender + which;
						strcpy(snptr->user_name,tuser.user_name);		/* name */
						strcpy(snptr->user_city,tuser.user_city);		/* city */
						snptr->user_msgsent = tuser.user_msgsent;		/* msgs sent */
						}
					}
				}				/* if add == 1; */
			}
		++users;
		}			/* while(fread()) */
				/* You shoulda seen it before I added those comments!! */

	if (verbose)
		fprintf(stderr,"\rCompiled %d Users!                     \r\n",users);

	printf("Creating ANSI and ASCII files ...\n");

	if (tcfg.cfg_screenpath[0] && tcfg.cfg_screenpath[strlen(tcfg.cfg_screenpath) - 1] != '\\')
		strcat(tcfg.cfg_screenpath,"\\");

	strcpy(buffer,tcfg.cfg_screenpath);
	strcat(buffer,fname);			/* open ANSI file */
	strcat(buffer,".ans");

	if ((ansi_fd = fopen(buffer,"wb")) == NULL)
		{
		printf("\n\n\aFatal Error!  Unable to create %s!\n\n",buffer);
		return 1;
		}

	strcpy(buffer,tcfg.cfg_screenpath);
	strcat(buffer,fname);			/* open ASCII file */
	strcat(buffer,".asc");

	if ((ascii_fd = fopen(buffer,"wb")) == NULL)
		{
		printf("\n\n\aFatal Error!  Unable to create %s!\n\n",buffer);
		return 1;
		}

	curtime = time(NULL);
	tmptr = localtime(&curtime);
	sprintf(date,"%s %d, %d",months[tmptr->tm_mon],tmptr->tm_mday,1900 + tmptr->tm_year);

	if (esc2j)
		fprintf(ansi_fd,"[2J");

	fprintf(ansi_fd,"\r[1;36mS-Top (v%d.%02d%s): [1;35mA Simplex Top Callers Program for DOS and OS/2!\r\n\r\n",MAJOR_VERSION,MINOR_VERSION,(char *)(BETA ? "á" : ""));
	fprintf(ascii_fd,"\rS-Top (v%d.%02d%s): A Simplex Top Callers Program for DOS and OS/2!\r\n\r\n",MAJOR_VERSION,MINOR_VERSION,(char *)(BETA ? "á" : ""));

	if (callers)
		{
		qsort(tcallers,users < total ? users : total,sizeof(struct callers),comp_callers);

		sprintf(buffer,"Top Callers as of %s",date);

		fprintf(ansi_fd,"\r\n\r\n\x1B[1;33m%s\r\n",buffer);
		fprintf(ansi_fd,"\x1B[1;35m");
		fprintf(ascii_fd,"%s\r\n",buffer);

		for (count = 0; (size_t)count < strlen(buffer); count++)
			{
			if (ibm_ansi)
				fputc('Ä',ansi_fd);
			else
				fputc('-',ansi_fd);
			fputc('-',ascii_fd);
			}
		fprintf(ansi_fd,"\r\n\r\n");
		fprintf(ascii_fd,"\r\n\r\n");

		fprintf(ansi_fd,"\x1B[1;36mName                      City                      Calls\r\n");
		fprintf(ascii_fd,"Name                      City                      Calls\r\n");
		if (ibm_ansi)
			fprintf(ansi_fd,"\x1b[1;31mÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄ\r\n");
		else
			fprintf(ansi_fd,"\x1b[1;31m------------------------  ------------------------  -----\r\n");
		fprintf(ascii_fd,"------------------------  ------------------------  -----\r\n");

		for (count = 0; count < (users < total ? users : total); count++)
			{
			clptr = tcallers + count;
			if (clptr->user_calls)
				{
				fprintf(ansi_fd,"\x1B[1;32m%-24.24s  \x1B[0;36m%-24.24s  \x1B[1;37m%-5d\r\n",clptr->user_name,clptr->user_city,clptr->user_calls);
				fprintf(ascii_fd,"%-24.24s  %-24.24s  %-5d\r\n",clptr->user_name,clptr->user_city,clptr->user_calls);
				}
			}

		fprintf(ansi_fd,"\r\n");
		fprintf(ascii_fd,"\r\n");
		}


	if (ul)
		{
		qsort(tul,users < total ? users : total,sizeof(struct ul),comp_ul);

		sprintf(buffer,"Top Uploaders as of %s",date);

		fprintf(ansi_fd,"\r\n\r\n\x1B[1;33m%s\r\n",buffer);
		fprintf(ansi_fd,"\x1B[1;35m");
		fprintf(ascii_fd,"%s\r\n",buffer);

		for (count = 0; (size_t)count < strlen(buffer); count++)
			{
			if (ibm_ansi)
				fputc('Ä',ansi_fd);
			else
				fputc('-',ansi_fd);
			fputc('-',ascii_fd);
			}
		fprintf(ansi_fd,"\r\n\r\n");
		fprintf(ascii_fd,"\r\n\r\n");

		fprintf(ansi_fd,"\x1B[1;36mName                      City                      UpLoads  Upload Bytes\r\n");
		fprintf(ascii_fd,"Name                      City                      UpLoads  Upload Bytes\r\n");
		if (ibm_ansi)
			fprintf(ansi_fd,"\x1b[1;31mÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄÄÄÄ\r\n");
		else
			fprintf(ansi_fd,"\x1b[1;31m------------------------  ------------------------  -------  ------------\r\n");
		fprintf(ascii_fd,"------------------------  ------------------------  -------  ------------\r\n");

		for (count = 0; count < (users < total ? users : total); count++)
			{
			ulptr = tul + count;
			if (ulptr->user_uploadbytes)
				{
				fprintf(ansi_fd,"\x1B[1;32m%-24.24s  \x1B[0;36m%-24.24s  \x1B[1;37m%-7d  %-12lu\r\n",ulptr->user_name,ulptr->user_city,ulptr->user_upload,ulptr->user_uploadbytes);
				fprintf(ascii_fd,"%-24.24s  %-24.24s  %-7d  %-12lu\r\n",ulptr->user_name,ulptr->user_city,ulptr->user_upload,ulptr->user_uploadbytes);
				}
			}

		fprintf(ansi_fd,"\r\n");
		fprintf(ascii_fd,"\r\n");
		}


	if (dl)
		{
		qsort(tdl,users < total ? users : total,sizeof(struct dl),comp_dl);

		sprintf(buffer,"Top Downloaders as of %s",date);

		fprintf(ansi_fd,"\r\n\r\n\x1B[1;33m%s\r\n",buffer);
		fprintf(ansi_fd,"\x1B[1;35m");
		fprintf(ascii_fd,"%s\r\n",buffer);

		for (count = 0; (size_t)count < strlen(buffer); count++)
			{
			if (ibm_ansi)
				fputc('Ä',ansi_fd);
			else
				fputc('-',ansi_fd);
			fputc('-',ascii_fd);
			}
		fprintf(ansi_fd,"\r\n\r\n");
		fprintf(ascii_fd,"\r\n\r\n");


		fprintf(ansi_fd,"\x1B[1;36mName                      City                      DnLoads  Dnload Bytes\r\n");
		fprintf(ascii_fd,"Name                      City                      DnLoads  Dnload Bytes\r\n");
		if (ibm_ansi)
			fprintf(ansi_fd,"\x1b[1;31mÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄÄÄÄ\r\n");
		else
			fprintf(ansi_fd,"\x1b[1;31m------------------------  ------------------------  -------  ------------\r\n");
		fprintf(ascii_fd,"------------------------  ------------------------  -------  ------------\r\n");

		for (count = 0; count < (users < total ? users : total); count++)
			{
			dlptr = tdl + count;
			if (dlptr->user_dnloadbytes)
				{
				fprintf(ansi_fd,"\x1B[1;32m%-24.24s  \x1B[0;36m%-24.24s  \x1B[1;37m%-7d  %-12lu\r\n",dlptr->user_name,dlptr->user_city,dlptr->user_dnload,dlptr->user_dnloadbytes);
				fprintf(ascii_fd,"%-24.24s  %-24.24s  %-7d  %-12lu\r\n",dlptr->user_name,dlptr->user_city,dlptr->user_dnload,dlptr->user_dnloadbytes);
				}
			}
		fprintf(ansi_fd,"\r\n");
		fprintf(ascii_fd,"\r\n");
		}

	if (reader)
		{
		qsort(treader,users < total ? users : total,sizeof(struct reader),comp_readers);

		sprintf(buffer,"Top Message Readers as of %s",date);

		fprintf(ansi_fd,"\r\n\r\n\x1B[1;33m%s\r\n",buffer);
		fprintf(ansi_fd,"\x1B[1;35m");
		fprintf(ascii_fd,"\r\n\r\n%s\r\n",buffer);
		for (count = 0; (size_t)count < strlen(buffer); count++)
			{
			if (ibm_ansi)
				fputc('Ä',ansi_fd);
			else
				fputc('-',ansi_fd);
			fputc('-',ascii_fd);
			}
		fprintf(ansi_fd,"\r\n\r\n");
		fprintf(ascii_fd,"\r\n\r\n");

		fprintf(ansi_fd,"\x1B[1;36mName                      City                      Msgs Read\r\n");
		fprintf(ascii_fd,"Name                      City                      Msgs Read\r\n");
		if (ibm_ansi)
			fprintf(ansi_fd,"\x1b[1;31mÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄ\r\n");
		else
			fprintf(ansi_fd,"\x1b[1;31m------------------------  ------------------------  ---------\r\n");
		fprintf(ascii_fd,"------------------------  ------------------------  ---------\r\n");

		for (count = 0; count < (users < total ? users : total); count++)
			{
			rdptr = treader + count;
			if (rdptr->user_msgread)
				{
				fprintf(ansi_fd,"\x1B[1;32m%-24.24s  \x1B[0;36m%-24.24s  \x1B[1;37m%-5lu\r\n",rdptr->user_name,rdptr->user_city,rdptr->user_msgread);
				fprintf(ascii_fd,"%-24.24s  %-24.24s  %-5lu\r\n",rdptr->user_name,rdptr->user_city,rdptr->user_msgread);
				}
			}

		fprintf(ansi_fd,"\r\n");
		fprintf(ascii_fd,"\r\n");
		}

	if (sender)
		{
		qsort(tsender,users < total ? users : total,sizeof(struct sender),comp_senders);

		sprintf(buffer,"Top Message Senders as of %s",date);

		fprintf(ansi_fd,"\r\n\r\n\x1B[1;33m%s\r\n",buffer);
		fprintf(ansi_fd,"\x1B[1;35m");
		fprintf(ascii_fd,"%s\r\n",buffer);
		for (count = 0; (size_t)count < strlen(buffer); count++)
			{
			if (ibm_ansi)
				fputc('Ä',ansi_fd);
			else
				fputc('-',ansi_fd);
			fputc('-',ascii_fd);
			}
		fprintf(ansi_fd,"\r\n\r\n");
		fprintf(ascii_fd,"\r\n\r\n");

		fprintf(ansi_fd,"\x1B[1;36mName                      City                      Msgs Sent\r\n");
		fprintf(ascii_fd,"Name                      City                      Msgs Sent\r\n");
		if (ibm_ansi)
			fprintf(ansi_fd,"\x1b[1;31mÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ  ÄÄÄÄÄÄÄÄÄ\r\n");
		else
			fprintf(ansi_fd,"\x1b[1;31m------------------------  ------------------------  ---------\r\n");
		fprintf(ascii_fd,"------------------------  ------------------------  ---------\r\n");

		for (count = 0; count < (users < total ? users : total); count++)
			{
			snptr = tsender + count;
			if (snptr->user_msgsent)
				{
				fprintf(ansi_fd,"\x1B[1;32m%-24.24s  \x1B[0;36m%-24.24s  \x1B[1;37m%-5lu\r\n",snptr->user_name,snptr->user_city,snptr->user_msgsent);
				fprintf(ascii_fd,"%-24.24s  %-24.24s  %-5lu\r\n",snptr->user_name,snptr->user_city,snptr->user_msgsent);
				}
			}

		fprintf(ansi_fd,"\r\n");
		fprintf(ascii_fd,"\r\n");
		}

	if (enter)
		{
		fprintf(ansi_fd,"\x1B[35;1mPress \x1B[33;1;5mENTER \x1B[0;35;1mto continue ...`4\r\n");
		fprintf(ascii_fd,"Press ENTER to continue ...`4\r\n");
		}

	fclose(ansi_fd);
	fclose(ascii_fd);

	printf("\n");
	}



void help(void)
	{
	fprintf(stderr,"Usage is: stop [-Nnumber] -options [-Ffilename] [-Xfilename.ext]\n\n");

	fprintf(stderr,"Where:\n");
	fprintf(stderr,"\t-N  Use \"number\" entries in files (Default is 10)\n");
	fprintf(stderr,"\t-C  Create top callers file\n");
	fprintf(stderr,"\t-U  Create top uploaders file\n");
	fprintf(stderr,"\t-D  Create top downloaders file\n");
	fprintf(stderr,"\t-R  Create top message reader file\n");
	fprintf(stderr,"\t-S  Create top message sender file\n");
	fprintf(stderr,"\t-I  Use IBM high-ASCII characters\n");
	fprintf(stderr,"\t-J  Add ESC[2J (clearscreen) to the ANSI file\n");
	fprintf(stderr,"\t-E  Leave off the \"Press ENTER to continue\" prompt\n");
	fprintf(stderr,"\t-X  Read \"filename.ext\" which contains names to exclude\n");
	fprintf(stderr,"\t-V  Verbose mode\n");
	fprintf(stderr,"\t-F  Use \"filename\" as the root name for output file\n\n");
	}
