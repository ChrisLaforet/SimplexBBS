/* s_build.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 24 May 1990
**
** Revision Information: $Logfile:   G:/simplex/slink/vcs/s_build.c_v  $
**                       $Date:   25 Oct 1992 14:17:24  $
**                       $Revision:   1.7  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <ctype.h>
#include <string.h>
#include "slink.h"



struct l **links = NULL;
int cur_links = 0;
int max_links = 0;



void build_links(int area)
	{
	struct msgh tmsgh;
	struct msgh tmsgh1;
	char buffer[150];
	char *cptr;
	int count;
	int kount;
	int found;
	int total_found = 0;
	int write_flag;

	fseek(linkfd,0L,SEEK_SET);
	fprintf(stderr,"  * Message base has %d messages.\n",total_msgs);

	for (count = 0; count < total_msgs; count++)
		{
		if (msgs[count] == area)
			{
			fseek(headfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
			fread(&tmsgh,sizeof(struct msgh),1,headfd);
			++total_found;

			strcpy(buffer,tmsgh.msgh_subject);

			/* strip off trailing spaces */
			if (buffer[0])
				{
				cptr = buffer + (strlen(buffer) - 1);
				while (cptr != buffer && isspace(*cptr))
					*cptr-- = (char)'\0';
				}

			/* strip off leading spaces and RE:'s */
			cptr = buffer;
			while (*cptr && (isspace(*cptr) || !strnicmp(cptr,"RE:",3)))
				{
				if (isspace(*cptr))
					++cptr;
				else
					cptr += 3;
				}

			strupr(cptr);

			found = 0;
			write_flag = 0;
			for (kount = 0; kount < cur_links; kount++)
				{
				if (!strcmp(links[kount]->l_subject,cptr))
					{
					found = 1;
					break;
					}
				}
			if (found)
				{
				tmsgh.msgh_prev = links[kount]->l_prev;
				tmsgh.msgh_next = 0;

				fseek(headfd,(long)(links[kount]->l_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
				fread(&tmsgh1,sizeof(struct msgh),1,headfd);
				tmsgh1.msgh_next = count + 1;
				fseek(headfd,(long)(links[kount]->l_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
				fwrite(&tmsgh1,sizeof(struct msgh),1,headfd);

				links[kount]->l_prev = count + 1;

 				write_flag = 1;		/* have to write record */
				}
			else
				{
				if (cur_links >= max_links)
					{
					if (!(links = (struct l **)realloc(links,(max_links += 100) * sizeof(struct l *))))
						{
						printf("Error: Out of memory.\n");
						exit(1);
						}
					}
				if (!(links[cur_links] = (struct l *)calloc(1,sizeof(struct l))))
					{
					printf("Error: Out of memory.\n");
					exit(1);
					}
				if (!(links[cur_links]->l_subject = (char *)calloc(strlen(cptr) + 1,sizeof(char))))
					{
					printf("Error: Out of memory.\n");
					exit(1);
					}
				strcpy(links[cur_links]->l_subject,cptr);
				links[cur_links]->l_prev = count + 1;
				++cur_links;

				if (tmsgh.msgh_next || tmsgh.msgh_prev)
					{
					tmsgh.msgh_next = 0;
					tmsgh.msgh_prev = 0;
					write_flag = 1;			/* have to write record */
					}
				}
			if (write_flag)		/* only write if you HAVE to! */
				{
				fseek(headfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
				fwrite(&tmsgh,sizeof(struct msgh),1,headfd);
				fseek(headfd,0L,SEEK_CUR);		
				}
			}
		}

	fflush(headfd);

	fprintf(stderr,"  * Found %d message%s and %d topic%s.\n\n",total_found,total_found == 1 ? "" : "s",cur_links,cur_links == 1 ? "" : "s");

	if (max_links)
		{
		for (kount = 0; kount < cur_links; kount++)
			{
			free(links[kount]->l_subject);
			free(links[kount]);
			}
		free(links);
		links = NULL;
		cur_links = 0;
		max_links = 0;
		}
	}
