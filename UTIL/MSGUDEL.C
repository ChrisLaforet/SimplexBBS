/* msgudel.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 10 October 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>


#define MSGH_DELETED   				0x1


typedef unsigned int DATE_T;
typedef unsigned int TIME_T;


struct msgh		/* systemwide message number is ((offset/sizeof(struct msgh)) + 1) -- 0 is invalid systemwide message */
	{
	int msgh_area;					/* area number of message */
	int msgh_number;				/* message number in that area! */
	char msgh_to[41];				/* addressee of message */
	char msgh_from[41];
	char msgh_subject[71];
	DATE_T msgh_date;
	TIME_T msgh_time;
	int msgh_flags;					/* net and message flags */
	int msgh_szone;
	int msgh_snet;
	int msgh_snode;
	int msgh_dzone;
	int msgh_dnet;
	int msgh_dnode;
	int msgh_cost;
	int msgh_prev;					/* previous message link - systemwide message number */
	int msgh_next;					/* next message link - systemwide message number */
	long msgh_offset;				/* start offset of message */
	long msgh_length;				/* message length in bytes */
	};



int main(void)
	{
	struct msgh tmsgh;
	int current = 0;
	FILE *fd;

	if (fd = fopen("msghead.bbs","r+b"))
		{
		while (fread(&tmsgh,sizeof(struct msgh),1,fd))
			{
			tmsgh.msgh_flags &= ~MSGH_DELETED;
			fseek(fd,(long)current * (long)sizeof(struct msgh),SEEK_SET);
			fwrite(&tmsgh,sizeof(struct msgh),1,fd);
			fseek(fd,0L,SEEK_CUR);
			++current;
			}
		fclose(fd);
		}
	else
		printf("Unable to open message header.\n");
	}
