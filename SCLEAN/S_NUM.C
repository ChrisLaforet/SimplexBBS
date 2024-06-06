/* s_num.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 8 October 1991
**
** Revision Information: $Logfile:   G:/simplex/sclean/vcs/s_num.c_v  $
**                       $Date:   25 Oct 1992 14:14:28  $
**                       $Revision:   1.3  $
**
*/


#include <stdio.h>
#include "sclean.h"




void kill_num(int area,int keep)
	{
	struct mlink tmlink;
	struct mdata tmdata;
	struct msgh tmsgh;
	struct mc tmc;
	long mc_offset = -1L;
	int count = 0;
	int kount = 0;
	int counter = 0;
	int killed = 0;
	int tint;
	int tint1;

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

	for (count = 0, kount = 0; count < total_msgs; count++)		/* count number of messages and see if we have to do something */
		{
		if (msgs[count] == area)
			++kount;
		}

	if (kount > keep)
		{
		for (count = total_msgs, counter = kount, kount = keep; count > 0; count--)		/* mark which ones get deleted in the renum array */
			{
			if (msgs[count] == area)
				{
				if (kount)
					renum[counter] = kount--;
				else 
					renum[counter] = 0;
				--counter;
				}
			}

		/* renum array now has the ones to delete marked as 0 */

		for (count = 0, kount = 0; count < total_msgs; count++)
			{
			if (msgs[count] == area)
				{
				fseek(mheadfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
				fread(&tmsgh,sizeof(struct msgh),1,mheadfd);
				if (!renum[kount])
					{
					if (!quiet_flag)
						fprintf(stderr,"\r  Killing %u ",count + 1);
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
				else
					{
					if (!quiet_flag)
						fprintf(stderr,"\r  Keeping %u ",count + 1);
					if (tmsgh.msgh_number != renum[kount])
						{
						tmsgh.msgh_number = renum[kount];		/* renumber message as we go by */
						fseek(mheadfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
						fwrite(&tmsgh,sizeof(struct msgh),1,mheadfd);
						fseek(mheadfd,0L,SEEK_CUR);
						fflush(mheadfd);
						}
					}
				++kount;
				}
			else if (!quiet_flag)
				fprintf(stderr,"\r  Skipping %u",count + 1);
			}

		if (count)
			{
			if (killed)
				{
				for (kount = 0, tint = renum[0]; kount < max_renum; kount++)		/* normalize our pointers */
					{
					tint1 = renum[kount];
					if (tint1 < tint)
						{
						tint1 = tint;
						renum[kount] = tint;
						}
					else if (tint1 > tint)
						tint = tint1;
					}

				for (kount = 0; kount < max_areas; kount++)
					if (areas[kount] == area)
						break;

				if (kount < max_areas)			/* update last read pointers */
					{
					for (count = 0; count < max_users; count++)
						{
						/* ((user + 1) * #areas * sizeof(int)) + sizeof(int) + (area_offset * sizeof(int)) */
						fseek(mreadfd,((long)(count + 1) * (long)max_areas * (long)sizeof(int)) + (long)sizeof(int) + ((long)kount * (long)sizeof(int)),SEEK_SET);
						fread(&tint,sizeof(int),1,mreadfd);

						if (tint >= max_renum || !tint)
							tint1 = 0;
						else if (tint >= keep)
							tint1 = keep - 1;
						else 
							tint1 = renum[tint - 1];

						if (tint1 != tint)
							{
							fseek(mreadfd,((long)(count + 1) * (long)max_areas * (long)sizeof(int)) + (long)sizeof(int) + ((long)kount * (long)sizeof(int)),SEEK_SET);
							fwrite(&tint1,sizeof(int),1,mreadfd);
							}
						else 
							fseek(mreadfd,0L,SEEK_CUR);
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
			}
		fprintf(stderr,"\r  * Killed %u message%s in area %d.....\n",killed,killed == 1 ? "" : "s",area);
		}
	}



