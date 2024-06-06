/* s_all.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 May 1990
**
** Revision Information: $Logfile:   G:/simplex/sclean/vcs/s_all.c_v  $
**                       $Date:   25 Oct 1992 14:14:28  $
**                       $Revision:   1.10  $
**
*/


#include <stdio.h>
#include <string.h>
#include <io.h>
#include "sclean.h"




void check_delete(int area,char *fname)
	{
	char buffer[100];
	struct msgh tmsgh;
	long offset;
	int total = 0;
	int count;

	offset = ftell(mheadfd);		/* save current header offset */
	for (count = 0; count < total_msgs; count++)
		{
		if (msgs[count] == area)
			{
			fseek(mheadfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
			fread(&tmsgh,sizeof(struct msgh),1,mheadfd);
			if (tmsgh.msgh_flags & MSGH_LOCAL_FILEATTACH)
				if (!stricmp(tmsgh.msgh_subject,fname))
					++total;
			}
		}
	if (total < 2)
		{
		strcpy(buffer,cfg.cfg_fapath);
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		strcat(buffer,fname);
		if (!access(buffer,0))
			{
			if (!quiet_flag)
				fprintf(stderr,"and file-attach file %s ",fname);
			if (unlink(buffer))				/* delete file */
				{
				if (!quiet_flag)
					fprintf(stderr,"- Error!\n");
				}
			else
				{
				if (!quiet_flag)
					fprintf(stderr,"- Ok!\n");
				}
			}
		}
	fseek(mheadfd,offset,SEEK_SET);		/* reset header position */
	}



void kill_all(int area)
	{
	struct mlink tmlink;
	struct mdata tmdata;
	struct msgh tmsgh;
	struct mc tmc;
	long mc_offset = -1L;
	int count = 0;
	int kount = 0;
	int killed = 0;
	int tint;

	fseek(mdatafd,0L,SEEK_SET);
	fread(&tmdata,sizeof(struct mdata),1,mdatafd);
	while (fread(&tmc,sizeof(struct mc),1,mdatafd))
		{
		if (tmc.mc_area == area)
			{
			mc_offset = ftell(mdatafd) - (long)sizeof(struct mc);
			break;
			}
		}

	for (count = 0; count < total_msgs; count++)
		{
		if (msgs[count] == area)
			{
			if (!quiet_flag)
				fprintf(stderr,"\r  Killing %u ",count + 1);
			fseek(mheadfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
			fread(&tmsgh,sizeof(struct msgh),1,mheadfd);
			if (tmsgh.msgh_flags & MSGH_LOCAL_FILEATTACH)
				check_delete(tmsgh.msgh_area,tmsgh.msgh_subject);
			tmsgh.msgh_flags |= MSGH_DELETED;
			update_chain(&tmsgh,tmdata.mdata_msgs);
			tmsgh.msgh_number = 0;
			tmsgh.msgh_next = 0;
			tmsgh.msgh_prev = 0;
			fseek(mheadfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
			fwrite(&tmsgh,sizeof(struct msgh),1,mheadfd);
			fseek(mheadfd,0L,SEEK_CUR);
			fflush(mheadfd);

			fseek(mlinkfd,(long)count * (long)sizeof(struct mlink),SEEK_SET);
			fread(&tmlink,sizeof(struct mlink),1,mlinkfd);
			tmlink.mlink_flags |= MSGH_DELETED;		/* mark link as deleted */
			fseek(mlinkfd,(long)count * (long)sizeof(struct mlink),SEEK_SET);
			fwrite(&tmlink,sizeof(struct mlink),1,mlinkfd);
			fflush(mlinkfd);

			++tmdata.mdata_del;
			fseek(mdatafd,0L,SEEK_SET);
			fwrite(&tmdata,sizeof(struct mdata),1,mdatafd);
			fflush(mdatafd);

			msgs[count] = 0;

			if (tmc.mc_msgs)
				--tmc.mc_msgs;
			++killed;
			}
		else if (!quiet_flag)
			fprintf(stderr,"\r  Skipping %u",count + 1);
		}

	if (count)
		{
		if (killed)
			{
			for (kount = 0; kount < max_areas; kount++)
				if (areas[kount] == area)
					break;

			if (kount < max_areas)			/* update last read pointers */
				{
				for (count = 0; count < max_users; count++)
					{
					fseek(mreadfd,((long)(count + 1) * (long)max_areas * (long)sizeof(int)) + (long)sizeof(int) + ((long)kount * (long)sizeof(int)),SEEK_SET);
					fread(&tint,sizeof(int),1,mreadfd);
					tint = 0;
					fseek(mreadfd,((long)(count + 1) * (long)max_areas * (long)sizeof(int)) + (long)sizeof(int) + ((long)kount * (long)sizeof(int)),SEEK_SET);
					fwrite(&tint,sizeof(int),1,mreadfd);
					}
				}
			fflush(mreadfd);
			}
		if (killed && mc_offset != -1L)
			{
			fseek(mdatafd,mc_offset,SEEK_SET);
			fwrite(&tmc,sizeof(struct mc),1,mdatafd);
			fflush(mdatafd);
			}
		fprintf(stderr,"\r  * Killed %u message%s in area %d.....\n",killed,killed == 1 ? "" : "s",area);
		}
	}



void update_chain(struct msgh *tmsgh,int total_msgs)
	{
	struct msgh smsgh;

	if (tmsgh->msgh_prev)
		{
		/**** Check that our message number is existant ****/
		if (tmsgh->msgh_prev < 0 || tmsgh->msgh_prev > total_msgs)
			printf("\n  * Warning: Invalid prev message %d during delete.",tmsgh->msgh_prev);
		else
			{
			fseek(mheadfd,(long)(tmsgh->msgh_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
			fread(&smsgh,sizeof(struct msgh),1,mheadfd);
			smsgh.msgh_next = tmsgh->msgh_next;
			fseek(mheadfd,(long)(tmsgh->msgh_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
			fwrite(&smsgh,sizeof(struct msgh),1,mheadfd);
			}
		}
	if (tmsgh->msgh_next)
		{
		/**** Check that our message number is existant ****/
		if (tmsgh->msgh_next < 0 || tmsgh->msgh_next > total_msgs)
			printf("\n  * Warning: Invalid next message %d during delete.",tmsgh->msgh_next);
		else
			{
			fseek(mheadfd,(long)(tmsgh->msgh_next - 1) * (long)sizeof(struct msgh),SEEK_SET);
			fread(&smsgh,sizeof(struct msgh),1,mheadfd);
			smsgh.msgh_prev = tmsgh->msgh_prev;
			fseek(mheadfd,(long)(tmsgh->msgh_next - 1) * (long)sizeof(struct msgh),SEEK_SET);
			fwrite(&smsgh,sizeof(struct msgh),1,mheadfd);
			}
		}
	fflush(mheadfd);
	}
