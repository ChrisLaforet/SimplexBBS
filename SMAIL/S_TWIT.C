/* s_twit.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 29 September 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "smail.h"



struct tf **twits = NULL;	/* twit filtering introduced in v 1.09 */
int cur_twits = 0;
int max_twits = 0;



void read_twits(FILE *fd)
	{
	char buffer[100];
	char *cptr;
	char *cptr1;
	int tflags;

	fseek(fd,0L,SEEK_SET);
	while (fgets(buffer,sizeof(buffer),fd))
		{
		cptr = buffer;
		if (*cptr != ';')
			{
			while (*cptr && isspace(*cptr))		/* trim off leading whitespace */
				++cptr;

			tflags = 0;

			if (*cptr)
				{
				cptr1 = cptr;
				while (*cptr && !isspace(*cptr))
					++cptr;
				if (*cptr)
					{
					*cptr++ = '\0';
					if (strlen(cptr1) && strlen(cptr1) <= 2)
						{
						if (toupper(*cptr1) == 'T')
							tflags |= TF_TO;
						else if (toupper(*cptr1) == 'F')
							tflags |= TF_FROM;
						if (*++cptr1)
							{
							if (toupper(*cptr1) == 'T')
								tflags |= TF_TO;
							else if (toupper(*cptr1) == 'F')
								tflags |= TF_FROM;
							}

						if (tflags)			/* now for the twit's name */
							{
							while (*cptr && isspace(*cptr))		/* trim off leading whitespace */
								++cptr;
							if (isalnum(*cptr))
								{
								cptr1 = cptr;
								while (*cptr && *cptr != '\r' && *cptr != '\n')
									++cptr;

								while (isspace(*(cptr - 1)))
									--cptr;
								*cptr = (char)'\0';

								if (cur_twits >= max_twits)
									{
									if (!(twits = realloc(twits,(max_twits += 5) * sizeof(struct tf *))))
										{
										fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
										exit(2);
										}
									}
								if (!(twits[cur_twits] = malloc(sizeof(struct tf))))
									{
									fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
									exit(2);
									}
								if (!(twits[cur_twits]->tf_name = malloc(strlen(cptr1) + 1)))
									{
									fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
									exit(2);
									}
								strcpy(twits[cur_twits]->tf_name,cptr1);
								twits[cur_twits]->tf_flags = tflags;
								++cur_twits;
								}
							}
						}
					}
				}
			}
		}
	}



void twit_message(struct msgh *tmsgh,struct m *tm)
	{
	char buffer[100];
	char curdate[10];
	char curtime[10];
	char *cptr;
	char *cptr1;
	int offset;
	int count;
	int len;
	FILE *twitfd;

	sprintf(buffer,"%stwits.log",bbspath);
	if (!(twitfd = fopen(buffer,"r+b")))
		twitfd = fopen(buffer,"wb");
	if (twitfd)
		{
		fseek(twitfd,0L,SEEK_END);
		_strdate(curdate);
		_strtime(curtime);

		fprintf(twitfd,"Twitted message stopped by SMAIL on %s at %s\r\n\r\n.",curdate,curtime);
		if (tmsgh->msgh_flags & MSGH_ECHO)
			{
			cptr = "Unknown echo";
			for (count = 0; count < cur_areas; count++)
				{
				if (areas[count]->area_msgarea == tmsgh->msgh_area)
					{
					sprintf(buffer,"[%s] from %u:%u/%u",areas[count]->area_name,tmsgh->msgh_szone,tmsgh->msgh_snet,tmsgh->msgh_snode);
					cptr = buffer;
					break;
					}
				}
			}
		else
			{
			sprintf(buffer,"from %u:%u/%u",tmsgh->msgh_szone,tmsgh->msgh_snet,tmsgh->msgh_snode);
			cptr = buffer;
			}

		fprintf(twitfd,"  Source: %s %s\r\n",(char *)(tmsgh->msgh_flags & MSGH_ECHO ? "Echomail" : "Netmail"),cptr);
		fprintf(twitfd,"    Date: %u/%02u/%02u at %u:%u\r\n",(tmsgh->msgh_date >> 5) & 0xf,tmsgh->msgh_date & 0x1f,((tmsgh->msgh_date >> 9) + 80) % 100,tmsgh->msgh_time >> 11,(tmsgh->msgh_date >> 5) & 0x3f);
		fprintf(twitfd,"      To: %s\r\n",tmsgh->msgh_to);
		fprintf(twitfd,"    From: %s\r\n",tmsgh->msgh_from);
		fprintf(twitfd," Subject: %s\r\n\r\n",tmsgh->msgh_subject);

		/* ok now to write out and word-wrap message */
		offset = 0;
		cptr = tm->m_body;
		while (cptr < (tm->m_body + tm->m_curbody))
			{
			if (*cptr != '\x8d' && *cptr != '\n' && *cptr != '\r')
				{
				buffer[offset++] = *cptr;

				if (offset >= WRAP)			/* word wrap at 76 */
					{
					cptr1 = buffer + (offset - 1);
					if (isspace(*cptr1))
						{
						*cptr1 = '\r';
						fwrite(buffer,offset,1,twitfd);
						offset = 0;
						}
					else
						{
						while (cptr1 > buffer && !isspace(*cptr1))
							--cptr1;
						++cptr1;

						len = offset - (cptr1 - buffer);
						if (len <= 35)				/* 35 character wrap limit */
							{
							fwrite(buffer,offset - len,1,twitfd);
							fputc('\r',twitfd);
							memmove(buffer,cptr1,len);		/* move the data to the new line */
							offset = len;
							}
						else
							{
							buffer[offset++] = '\r';
							fwrite(buffer,offset,1,twitfd);
							offset = 0;
							}
						}
					}
				}
			else if (*cptr == '\r')
				{
				buffer[offset++] = *cptr;
				fwrite(buffer,offset,1,twitfd);
				offset = 0;
				}
			++cptr;
			}

		if (offset)			/* do we need to flush buffer */
			fwrite(buffer,offset,1,twitfd);

		fprintf(twitfd,"\r\n----------------------------------------\r\n\r\n");

		fclose(twitfd);
		}
	}
