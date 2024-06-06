/* s_date.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 May 1990
**
** Revision Information: $Logfile:   G:/simplex/sclean/vcs/s_date.c_v  $
**                       $Date:   25 Oct 1992 14:14:34  $
**                       $Revision:   1.10  $
**
*/



#include <stdio.h>
#include "sclean.h"



static int day_table[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


int get_absolute(DATE_T date)		/* returns days since 1-1-80 */
	{
	int tmp1;
	int tmp2;
	long tval;
	int leap = 0;
	int count;

	tmp1 = date >> 9;				/* year from 1980 */
	tmp2 = (date >> 5) & 0xf;		/* month */
	if (!(tmp1 % 4))
		leap = 1;
	tval = 0L;
	for (count = 0; count < tmp1; count++)
		{
		if (!(count % 4))
			tval += 366;
		else
			tval += 365;
		}
	for (count = 1; count < tmp2; count++)
		tval += (long)day_table[count - 1];
	if (tmp2 > 2 && leap)
		++tval;
	tval += (long)(date & 0x1f);
	return (int)tval;
	}



void kill_date(int area,int days)
	{
	struct mlink tmlink;
	struct mdata tmdata;
	struct msgh tmsgh;
	struct mc tmc;
	long mc_offset = -1L;
	int count = 0;
	int kount = 0;
	int killed = 0;
	int tdays = get_absolute(today);
	int mdays;
	int curnum = 1;
	int maxnum = 0;
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
	for (count = 0; count < max_renum; count++)
		renum[count] = 0;

	for (count = 0; count < total_msgs; count++)
		{
		if (msgs[count] == area)
			{
			fseek(mheadfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
			fread(&tmsgh,sizeof(struct msgh),1,mheadfd);
			mdays = get_absolute(tmsgh.msgh_date);
			if ((tdays - mdays) > days)
				{
				if (!quiet_flag)
					fprintf(stderr,"\r  Killing %u ",count + 1);
				if (tmsgh.msgh_flags & MSGH_LOCAL_FILEATTACH)
					check_delete(tmsgh.msgh_area,tmsgh.msgh_subject);
				tmsgh.msgh_flags |= MSGH_DELETED;

				maxnum = tmsgh.msgh_number - 1;
				if (maxnum > 0 && maxnum < max_renum)
					renum[maxnum] = curnum - 1;

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
				maxnum = tmsgh.msgh_number - 1;
				if (tmsgh.msgh_number != curnum)
					{
					tmsgh.msgh_number = curnum;		/* renumber message as we go by */
					fseek(mheadfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
					fwrite(&tmsgh,sizeof(struct msgh),1,mheadfd);
					fseek(mheadfd,0L,SEEK_CUR);
					fflush(mheadfd);
					}
				++curnum;
				if (maxnum > 0 && maxnum < max_renum)
					renum[maxnum] = curnum - 1;
				}
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
					else if (tint >= maxnum)
						tint1 = curnum - 1;
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
		fprintf(stderr,"\r  * Killed %u message%s in area %d.....\n",killed,killed == 1 ? "" : "s",area);
		}
	}
