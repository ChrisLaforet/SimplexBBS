/* spack.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 May 1990
**
** Revision Information: $Logfile:   G:/simplex/spack/vcs/spack.c_v  $
**                       $Date:   25 Oct 1992 14:27:06  $
**                       $Revision:   1.4  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include "spack.h"



char bbspath[100];			/* BBS message files area */
FILE *mheadfd;
FILE *mbodyfd;
FILE *mdatafd;
FILE *mlinkfd;
FILE *bheadfd;
FILE *bbodyfd;
unsigned int valid_headers = 0;
unsigned int deleted_headers = 0;
unsigned long valid_bytes = 0L;
unsigned long deleted_bytes = 0L;
int inplace = 0;
int keep = 0;
int pack = 0;
int ana = 0;
int kill = 0;
int quiet_flag = 0;




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
	char buffer[120];
	char buffer1[120];
	char *cptr;
	int count;

	setbuf(stderr,NULL);		/* make sure that stderr is completely unbuffered */

#if defined(PROTECTED)
	fprintf(stderr,"SPACK/2 (v %d.%02d of %s): Simplex/2 Message Base Packing Utility.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#else
	fprintf(stderr,"SPACK (v %d.%02d of %s): Simplex Message Base Packing Utility.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#endif
	fprintf(stderr,"Copyright (c) 1990-92, Chris Laforet and Chris Laforet Software.\n");
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

		for (count = 1; count < argc; count++)
			{
			cptr = argv[count];
			if (*cptr == (char)'-' || *cptr == (char)'/')
				{
				++cptr;
				if (*cptr == (char)'W' || *cptr == (char)'w')
					{
					inplace = 1;
					printf("Writing message body and header files in place!\n");
					}
				else if (*cptr == (char)'P' || *cptr == (char)'p')
					pack = 1;
				else if (*cptr == (char)'A' || *cptr == (char)'a')
					ana = 1;
				else if (*cptr == (char)'Q' || *cptr == (char)'q')
					quiet_flag = 1;
				else if (*cptr == (char)'K' || *cptr == (char)'k')
					kill = 1;
				else
					printf("Error: Invalid command line argument \"%s\"\n",argv[count]);
				}
			else
				printf("Error: Invalid command line argument \"%s\"\n",argv[count]);
			}

		if (pack || ana)
			{
			sprintf(buffer,"%smsghead.bbs",bbspath); 		/* now, open and get message headers */
			if (!(mheadfd = fopen(buffer,"r+b")))
				{
				printf("Error: Unable to find/open msghead.bbs\n");
				return 1;
				}

			sprintf(buffer,"%smsgbody.bbs",bbspath); 		/* now, open and get message body */
			if (!(mbodyfd = fopen(buffer,"r+b")))
				{
				printf("Error: Unable to find/open msgbody.bbs\n");
				return 1;
				}

			sprintf(buffer,"%smsgdata.bbs",bbspath); 		/* now, open message data file */
			if (!(mdatafd = fopen(buffer,"r+b")))
				{
				printf("Error: Unable to find/open msgdata.bbs\n");
				return 1;
				}

			sprintf(buffer,"%smsglink.bbs",bbspath); 		/* now, open message link file */
			if (!(mlinkfd = fopen(buffer,"r+b")))
				{
				printf("Error: Unable to find/open msglink.bbs\n");
				return 1;
				}

			if (pack)
				{
				if (!analyse())
					{
					if (!inplace)
						{
						sprintf(buffer,"%smsghead.bak",bbspath); 		/* now, open new message header file */
						if (!(bheadfd = fopen(buffer,"w+b")))
							{
							printf("Error: Unable to find/open msghead.bak\n");
							return 1;
							}

						sprintf(buffer,"%smsgbody.bak",bbspath); 		/* now, open new message body file */
						if (!(bbodyfd = fopen(buffer,"w+b")))
							{
							printf("Error: Unable to find/open msgbody.bak\n");
							return 1;
							}
						}
					if (!do_pack())
						{
						printf("Packing operation was successful.\n");
						if (!inplace)
							{
							fclose(bheadfd);
							fclose(bbodyfd);
							fclose(mheadfd);
							fclose(mbodyfd);
						
							sprintf(buffer,"%smsghead.bbs",bbspath); 	 
							sprintf(buffer1,"%smsghead.bak",bbspath); 	 
							unlink(buffer);
							if (rename(buffer1,buffer))
								{
								printf("Error: Unable to rename msghead.bak to msghead.bbs!\n");
								printf("Error: Unable to rename msgbody.bak to msgbody.bbs!\n");
								return 1;
								}

							sprintf(buffer,"%smsgbody.bbs",bbspath); 	 
							sprintf(buffer1,"%smsgbody.bak",bbspath); 	 
							unlink(buffer);
							if (rename(buffer1,buffer))
								{
								printf("Error: Unable to rename msgbody.bak to msgbody.bbs!\n");
								return 1;
								}
							}
						}
					else if (!inplace)
						{
						printf("Packing operation was NOT successful.\n");
						fclose(bheadfd);
						fclose(bbodyfd);
						fclose(mheadfd);
						fclose(mbodyfd);
						sprintf(buffer,"%smsghead.bak",bbspath); 	 
						unlink(buffer);
						sprintf(buffer,"%smsgbody.bak",bbspath); 	 
						unlink(buffer);
						}
					}
				if (inplace)
					{
					fclose(mheadfd);
					fclose(mbodyfd);
					}
				}
			else if (ana)
				{
				analyse();
				fclose(mheadfd);
				fclose(mbodyfd);
				}
			fclose(mdatafd);
			fclose(mlinkfd);
			}
		else
			printf("Error: Nothing done neither -P and -A option wasn't on command line!\n");
		}
	else
		{
#if defined(PROTECTED)
		printf("Usage is: spack2 -options\n\n");
#else
		printf("Usage is: spack -options\n\n");
#endif
		printf("where options may be:\n");
		printf("\t-q   Quiet...do not show message activity.\n");
		printf("\t-k   Kill unreasonably sized messages (>100K each)\n");
		printf("\t-p   Pack message base\n");
		printf("\t-a   Analyse message base only\n");
		printf("\t-w   Write message base files in place\n\n");
		return 1;
		}
	
	return 0;
	}
