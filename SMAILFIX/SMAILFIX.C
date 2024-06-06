/* smailfix.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 20 September 1990
**
** Revision Information: $Logfile:   G:/simplex/smailfix/vcs/smailfix.c_v  $
**                       $Date:   25 Oct 1992 14:24:48  $
**                       $Revision:   1.8  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <direct.h>
#include <conio.h>
#include <io.h>
#if defined(BOUND)
	#define INCL_DOSDATETIME
	#include <os2.h>
#endif
#include "smailfix.h"



struct mc **msgcount = NULL;
int max_msgcount = 0;
int cur_msgcount = 0;

struct mdata msgdata;

char bbspath[256];



DATE_T get_date(void)
	{
#if defined(BOUND)
	DATETIME dt;

	DosGetDateTime(&dt);
	return ((dt.year - 1980) << 9) | (dt.month << 5) | dt.day;
#endif
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
	struct msg tmsg;
	struct msgh tmsgh;
	struct mlink tmlink;
	long msgnum = 0L;
	long maxlen;
	char buffer[256];
	char *cptr;
	DATE_T today;
	int count;
	int kount;
	int found;
	int key;
	int chkdate = 0;
	int chksize = 0;
	int chkhdr = 0;
	int ok = 0;
	FILE *fd;
	FILE *fd1;

	setbuf(stderr,NULL);	/* make sure that stderr is completely unbuffered */

	fprintf(stderr,"SMAILFIX (v %u.%02u of %s): Simplex BBS Mail Header Repair Utility\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1989-91, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (argc > 1)
		{
		for (count = 0; count < argc; count++)
			{
			if (*argv[count] == (char)'-' || *argv[count] == (char)'/')
				{
				if (argv[count][1] == 'd' || argv[count][1] == 'D')
					{
					chkdate = 1;
					today = get_date();
					fprintf(stderr,"Notice: Killing messages dated AFTER today!\n");
					}
				else if ((argv[count][1] == 's' || argv[count][1] == 'S') && isdigit(argv[count][2]))
					{
					maxlen = atol(argv[count] + 2);
					if (maxlen)
						{
						chksize = 1;
						fprintf(stderr,"Notice: Killing messages longer than %lu bytes.\n",maxlen);
						}
					}
				else if (argv[count][1] == 'h' || argv[count][1] == 'H')
					{
					chkhdr = 1;
					fprintf(stderr,"Notice: Killing messages with high-ascii characters in headers!\n");
					}
				else if (argv[count][1] == 'y' || argv[count][1] == 'Y')
					ok = 1;
				}
			}

		if (ok)
			{
			fprintf(stderr,"\nNOTICE!  NOTICE!\n");
			fprintf(stderr,"Prior to running this utility, Simplex must be offline.  This\n");
			fprintf(stderr,"utility will fix the numbering of message headers in the headers\n");
			fprintf(stderr,"file.  It is imperative that you do not stop this program until\n");
			fprintf(stderr,"it has finished its work.  Since the message files can be\n");
			fprintf(stderr,"corrupted, it is strongly advised that you back up MSG*.BBS files.\n\n");
			fprintf(stderr,"\aAre you SURE that you want to run this program now? ");
			fflush(stderr);

			do
				{
				key = getch();
				key = toupper(key);
				}
			while (key != 'Y' && key != 'N');

			fprintf(stderr,"%s\n\n",key == 'Y' ? "Yes" : "No");

			if (key == 'Y')
				{
				fprintf(stderr,"Remember that YOU asked for it!!  Well let's get to work....\n\n");

				if (cptr = getenv("SIMPLEX"))		/* now for our path */
					strcpy(buffer,cptr);
				else
					getcwd(buffer,sizeof(buffer));
				if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
					strcat(buffer,P_SSEP);
				sprintf(bbspath,buffer);

				signal(SIGINT,signal_trap);				/* first, down with Ctrl-C */
				signal(SIGBREAK,signal_trap);			/* then down with Ctrl-Break */

				fprintf(stderr,"\n1....Opening MSGAREA.BBS and getting numbers of message boards...\n");
				sprintf(buffer,"%smsgarea.bbs",bbspath);
				if (!(fd = fopen(buffer,"rb")))
					{
					fprintf(stderr,"Fatal Error: Unable to find/open MSGAREA.BBS!\n");
					exit(1);
					}
				while (fread(&tmsg,sizeof(struct msg),1,fd))
					{
					if (!tmsg.msg_deleted)
						{
						fprintf(stderr,"    Found area %d: \"%s\".\n",tmsg.msg_number,tmsg.msg_areaname);

						if (cur_msgcount >= max_msgcount)
							{
							if (!(msgcount = (struct mc **)realloc(msgcount,(max_msgcount += 20) * sizeof(struct mc *))))
								{
								fprintf(stderr,"Fatal Error: Out of memory....\n");
								exit(1);
								}
							}
						if (!(msgcount[cur_msgcount] = (struct mc *)malloc(sizeof(struct mc))))
							{
							fprintf(stderr,"Fatal Error: Out of memory....\n");
							exit(1);
							}
						msgcount[cur_msgcount]->mc_area = tmsg.msg_number;
						msgcount[cur_msgcount]->mc_msgs = 0;
						++cur_msgcount;
						}
					}
				fclose(fd);
				if (!cur_msgcount)
					{
					fprintf(stderr,"Sorry, but I cannot continue.  There are NO message areas!\n");
					exit(1);
					}

				msgdata.mdata_msgs = 0;
				msgdata.mdata_del = 0;

				fprintf(stderr,"\n2....Opening MSGHEAD.BBS and renumbering messages...\n");
				sprintf(buffer,"%smsghead.bbs",bbspath);
				if (!(fd = fopen(buffer,"r+b")))
					{
					fprintf(stderr,"Fatal Error: Unable to find/open MSGHEAD.BBS!\n");
					exit(1);
					}
				while (fread(&tmsgh,sizeof(struct msgh),1,fd))
					{
					fprintf(stderr,"    System message #%ld: Area %d.               \r",msgnum,tmsgh.msgh_area);
					if (!(tmsgh.msgh_flags & MSGH_DELETED))
						{
						for (count = 0, found = 0; count < cur_msgcount; count++)
							{
							if (msgcount[count]->mc_area == tmsgh.msgh_area)
								{
								found = 1;
								break;
								}
							}
						if (!found)
							{
							fprintf(stderr,"    Deleting system message #%ld: Area %d unknown.\n",msgnum,tmsgh.msgh_area);
							tmsgh.msgh_flags |= MSGH_DELETED;
							tmsgh.msgh_number = 0;
							}
						else
							{
							if (chkdate)
								{
								if (tmsgh.msgh_date > today)
									{
									fprintf(stderr,"    Deleting system message #%ld: Area %d.  Invalid date.\n",msgnum,tmsgh.msgh_area);
									tmsgh.msgh_flags |= MSGH_DELETED;
									tmsgh.msgh_number = 0;
									found = 0;
									}
								}
							if (found && chksize)
								{
								if ((unsigned long)tmsgh.msgh_length > (unsigned long)maxlen)
									{
									fprintf(stderr,"    Deleting system message #%ld: Area %d.  Invalid size (%lu).\n",msgnum,tmsgh.msgh_area,tmsgh.msgh_length);
									tmsgh.msgh_flags |= MSGH_DELETED;
									tmsgh.msgh_number = 0;
									found = 0;
									}
								}
							if (found && chkhdr)
								{
								for (kount = 0; kount < 41; kount++)
									{
									if (!tmsgh.msgh_to[kount])
										break;
									if (tmsgh.msgh_to[kount] < ' ' || (unsigned char)tmsgh.msgh_to[kount] >= (unsigned char)'\x7f')
										{
										found = 0;
										break;
										}
									}
								if (found)
									{
									for (kount = 0; kount < 41; kount++)
										{
										if (!tmsgh.msgh_from[kount])
											break;
										if (tmsgh.msgh_from[kount] < ' ' || (unsigned char)tmsgh.msgh_from[kount] >= (unsigned char)'\x7f')
											{
											found = 0;
											break;
											}
										}
									}
								if (found)
									{
									for (kount = 0; kount < 71; kount++)
										{
										if (!tmsgh.msgh_subject[kount])
											break;
										if (tmsgh.msgh_subject[kount] < ' ' || (unsigned char)tmsgh.msgh_subject[kount] >= (unsigned char)'\x7f')
											{
											found = 0;
											break;
											}
										}
									}
								if (!found)
									{
									fprintf(stderr,"    Deleting system message #%ld: Area %d.  Invalid characters in header.\n",msgnum,tmsgh.msgh_area);
									tmsgh.msgh_flags |= MSGH_DELETED;
									tmsgh.msgh_number = 0;
									found = 0;
									}
								}
							}
						if (found)
							{
							++msgcount[count]->mc_msgs;
							tmsgh.msgh_number = msgcount[count]->mc_msgs;
							fprintf(stderr,"    System message #%ld: Area %d.  Local #%d\r",msgnum,tmsgh.msgh_area,msgcount[count]->mc_msgs);
							}

						tmsgh.msgh_to[40] = (char)'\0';
						tmsgh.msgh_from[40] = (char)'\0';
						tmsgh.msgh_subject[70] = (char)'\0';

						fseek(fd,msgnum * (long)sizeof(struct msgh),SEEK_SET);
						fwrite(&tmsgh,sizeof(struct msgh),1,fd);
						fseek(fd,0L,SEEK_CUR);		/* transition from write back to read mode! */
						}
					++msgnum;
					++msgdata.mdata_msgs;
					if (tmsgh.msgh_flags & MSGH_DELETED)
						++msgdata.mdata_del;
					}
				fflush(fd);

				fprintf(stderr,"\n3....Opening MSGDATA.BBS and updating information...\n");
				sprintf(buffer,"%sMSGDATA.BBS",bbspath);
				if (!(fd1 = fopen(buffer,"wb")))
					{
					fprintf(stderr,"Fatal Error: Unable to open/create MSGDATA.BBS!\n");
					exit(1);
					}

				fwrite(&msgdata,sizeof(struct mdata),1,fd1);		/* write out data */
				for (count = 0; count < cur_msgcount; count++)
					fwrite(msgcount[count],sizeof(struct mc),1,fd1);
				fclose(fd1);

				fprintf(stderr,"\n4....Opening MSGLINK.BBS and rebuilding header link information...\n");
				sprintf(buffer,"%sMSGLINK.BBS",bbspath);
				if (!(fd1 = fopen(buffer,"wb")))
					{
					fprintf(stderr,"Fatal Error: Unable to open/create MSGLINK.BBS!\n");
					exit(1);
					}

				fseek(fd,0L,SEEK_SET);
				while (fread(&tmsgh,sizeof(struct msgh),1,fd))
					{
					tmlink.mlink_area = tmsgh.msgh_area;
					tmlink.mlink_number = tmsgh.msgh_number;
					tmlink.mlink_flags = tmsgh.msgh_flags;

					tmlink.mlink_cksum = 0;
					strcpy(buffer,tmsgh.msgh_to);			/* strip out any "on x:x/x" or "at x:x/x" messages from the checksum */
					cptr = buffer;
					while (*cptr)
						{
						if (!strnicmp(cptr," on ",4) || !strnicmp(cptr," at ",4))
							{
							while (*cptr && cptr != buffer)
								{
								if (!isspace(*cptr))
									{
									++cptr;
									break;
									}
								--cptr;
								}
							*cptr = (char)'\0';
							break;
							}
						++cptr;
						}

					cptr = buffer;
					while (*cptr)
						{
						tmlink.mlink_cksum += (int)toupper(*cptr);
						++cptr;
						}
					fwrite(&tmlink,sizeof(struct mlink),1,fd1);		/* write out data */
					}
				fclose(fd1);
				fclose(fd);

				fprintf(stderr,"\n5....Mail statistics gathered during this process...\n");
				fprintf(stderr,"    Total Message Areas:        %d\n",cur_msgcount);
				fprintf(stderr,"    Total Messages in Database: %ld\n",msgnum);
				fprintf(stderr,"    Total Active Messages:      %ld\n",msgnum - (long)msgdata.mdata_del);
				fprintf(stderr,"    Total Deleted Messages:     %d  %s\n\n",msgdata.mdata_del,msgdata.mdata_del ? "(Run SPACK to compress database!)" : "");

				for (count = 0; count < cur_msgcount; count++)
					free(msgcount[count]);
				if (msgcount)
					free(msgcount);
				fprintf(stderr,"\aFinished....Please run SLINK to rebuild message links!\n");
				}
			}
		}

	if (!ok)
		{
		fprintf(stderr,"Usage is: SMAILFIX -y [-d] [-h] [-sxxxx]\n");
		fprintf(stderr,"where:\n");
		fprintf(stderr,"\t-y       Yes, run SMailFix (Must be there)\n");
		fprintf(stderr,"\t-d       Kill messages if date is after today.\n");
		fprintf(stderr,"\t-h       Kill messages if high-ascii is in header.\n");
		fprintf(stderr,"\t-sxxx    Kill messages if size is larger than xxx bytes.\n\n");
		}
	return 0;
	}

