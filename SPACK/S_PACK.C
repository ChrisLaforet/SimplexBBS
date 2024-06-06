/* s_pack.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 May 1990
**
** Revision Information: $Logfile:   G:/simplex/spack/vcs/s_pack.c_v  $
**                       $Date:   25 Oct 1992 14:27:10  $
**                       $Revision:   1.4  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include "spack.h"





int do_pack(void)
	{
	struct msgh tmsgh;
	struct msgh tmsgh1;
	struct mdata tmdata;
	struct mlink tmlink;
	struct mc tmc;
	char buffer[100];
	char *cptr;
	int current = 1;
	int count = 0;
	int kount;
	long body_offset = 0L;
	long total;
	long diff;
	long boffset;
	long moffset;
	int len;
	struct mc *msgcount = NULL;
	int cur_msgcount = 0;
	int max_msgcount = 0;

	fprintf(stderr,"\nPacking message base....\n");

	max_msgcount = (int)((filelength(fileno(mdatafd)) - (long)sizeof(struct mdata)) / (long)sizeof(struct mc));
	if (max_msgcount)
		{
		if (!(msgcount = (struct mc *)calloc(max_msgcount,sizeof(struct mc))))
			{
			printf("Error: Unable to allocate message count array!\n");
			exit(1);
			}
		fseek(mdatafd,(long)sizeof(struct mdata),SEEK_SET);
		while (fread(&tmc,sizeof(struct mc),1,mdatafd))
			{
			memcpy(&msgcount[cur_msgcount],&tmc,sizeof(struct mc));
			msgcount[cur_msgcount].mc_msgs = 0;
			++cur_msgcount;
			}
		}

	fseek(mheadfd,0L,SEEK_SET);
	if (inplace)		/* write records inplace */
		{
		while (fread(&tmsgh,sizeof(struct msgh),1,mheadfd))
			{
			if (!quiet_flag)
				fprintf(stderr,"\r   Packing %u -> %u",count + 1,current + 1);
			if (!(tmsgh.msgh_flags & MSGH_DELETED))
				{
				if (count != current)		/* make sure that we are not synchronized! */
					{
					total = 0L;
					while (total < tmsgh.msgh_length)
						{
						diff = tmsgh.msgh_length - total;
						if (diff >= (long)sizeof(buffer))
							len = sizeof(buffer);
						else
							len = (int)diff;

						fseek(mbodyfd,tmsgh.msgh_offset + total,SEEK_SET);
						fread(buffer,len,1,mbodyfd);
						fseek(mbodyfd,body_offset + total,SEEK_SET);
						fwrite(buffer,len,1,mbodyfd);

						total += (long)len;
						}
					tmsgh.msgh_offset = body_offset;
					body_offset += tmsgh.msgh_length;

					if (tmsgh.msgh_prev)		/* patch the prev header's next to point at us */
						{
						fseek(mheadfd,(long)(tmsgh.msgh_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
						fread(&tmsgh1,sizeof(struct msgh),1,mheadfd);
						tmsgh1.msgh_next = current;
						fseek(mheadfd,(long)(tmsgh.msgh_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
						fwrite(&tmsgh1,sizeof(struct msgh),1,mheadfd);
						}
					if (tmsgh.msgh_next)		/* point the next header's previous to us -- we'll get our next patched later */
						{
						fseek(mheadfd,(long)(tmsgh.msgh_next - 1) * (long)sizeof(struct msgh),SEEK_SET);
						fread(&tmsgh1,sizeof(struct msgh),1,mheadfd);
						tmsgh1.msgh_prev = current;
						fseek(mheadfd,(long)(tmsgh.msgh_next - 1) * (long)sizeof(struct msgh),SEEK_SET);
						fwrite(&tmsgh1,sizeof(struct msgh),1,mheadfd);
						}

					fseek(mheadfd,(long)(current - 1) * (long)sizeof(struct msgh),SEEK_SET);
					++current;
					fwrite(&tmsgh,sizeof(struct msgh),1,mheadfd);
					fseek(mheadfd,(long)(count + 1) * (long)sizeof(struct msgh),SEEK_SET);

					for (kount = 0; kount < cur_msgcount; kount++)
						{
						if (msgcount[kount].mc_area == tmsgh.msgh_area)
							{
							++msgcount[kount].mc_msgs;
							break;
							}
						}
					}
				}
			++count;
			}
		fflush(mheadfd);
		fflush(mbodyfd);
		chsize(fileno(mheadfd),(long)(current - 1) * (long)sizeof(struct msgh));
		chsize(fileno(mbodyfd),body_offset);
		}
	else
		{
		while (fread(&tmsgh,sizeof(struct msgh),1,mheadfd))
			{
			if (!quiet_flag)
				fprintf(stderr,"\r   Packing %u -> %u....",count + 1,current + 1);
			if (!(tmsgh.msgh_flags & MSGH_DELETED))
				{
				fseek(mbodyfd,tmsgh.msgh_offset,SEEK_SET);
				tmsgh.msgh_offset = ftell(bbodyfd);

				total = 0L;
				while (total < tmsgh.msgh_length)
					{
					diff = tmsgh.msgh_length - total;
					if (diff >= (long)sizeof(buffer))
						len = sizeof(buffer);
					else
						len = (int)diff;

					fread(buffer,len,1,mbodyfd);
					if (fwrite(buffer,len,1,bbodyfd) != 1)
						return 1;

					total += (long)len;
					}

				boffset = ftell(bheadfd);
				moffset = ftell(mheadfd);

				if (tmsgh.msgh_prev)		/* patch the prev header's next to point at us */
					{
					fseek(bheadfd,(long)(tmsgh.msgh_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
					fread(&tmsgh1,sizeof(struct msgh),1,bheadfd);
					tmsgh1.msgh_next = current;
					fseek(bheadfd,(long)(tmsgh.msgh_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
					if (fwrite(&tmsgh1,sizeof(struct msgh),1,bheadfd) != 1)
						return 1;
					fseek(bheadfd,boffset,SEEK_SET);
					}
				if (tmsgh.msgh_next)		/* point the next header's previous to us -- we'll get our next patched later */
					{
					fseek(mheadfd,(long)(tmsgh.msgh_next - 1) * (long)sizeof(struct msgh),SEEK_SET);
					fread(&tmsgh1,sizeof(struct msgh),1,mheadfd);
					tmsgh1.msgh_prev = current;
					fseek(mheadfd,(long)(tmsgh.msgh_next - 1) * (long)sizeof(struct msgh),SEEK_SET);
					if (fwrite(&tmsgh1,sizeof(struct msgh),1,mheadfd) != 1)
						return 1;
					fseek(mheadfd,moffset,SEEK_SET);
					}

				++current;

				if (fwrite(&tmsgh,sizeof(struct msgh),1,bheadfd) != 1)
					return 1;

				for (kount = 0; kount < cur_msgcount; kount++)
					{
					if (msgcount[kount].mc_area == tmsgh.msgh_area)
						{
						++msgcount[kount].mc_msgs;
						break;
						}
					}
				}
			++count;
			}
		fflush(bheadfd);
		fflush(bbodyfd);
		}
	if (count && !quiet_flag)
		fprintf(stderr,"\r                            \r");
	if (count)
		{
		fprintf(stderr,"Updating message data file....\n");
		fseek(mdatafd,0L,SEEK_SET);
		chsize(fileno(mdatafd),0L);
		tmdata.mdata_msgs = current - 1;
		tmdata.mdata_del = 0;
		fwrite(&tmdata,sizeof(struct mdata),1,mdatafd);
		for (kount = 0; kount < cur_msgcount; kount++)
			fwrite(&msgcount[kount],sizeof(struct mc),1,mdatafd);
		fflush(mdatafd);

		fprintf(stderr,"Updating message links file....\n");
		fseek(mlinkfd,0L,SEEK_SET);
		chsize(fileno(mlinkfd),0L);
		if (inplace)		/* write records inplace */
			{
			fseek(mheadfd,0L,SEEK_SET);
			while (fread(&tmsgh,sizeof(struct msgh),1,mheadfd))
				{
				tmlink.mlink_area = tmsgh.msgh_area;
				tmlink.mlink_number = tmsgh.msgh_number;
				tmlink.mlink_flags = tmsgh.msgh_flags;
				tmlink.mlink_cksum = 0;
				cptr = tmsgh.msgh_to;
				while (*cptr)
					{
					tmlink.mlink_cksum += toupper(*cptr);
					++cptr;
					}
				fwrite(&tmlink,sizeof(struct mlink),1,mlinkfd);
				}
			}
		else
			{
			fseek(bheadfd,0L,SEEK_SET);
			while (fread(&tmsgh,sizeof(struct msgh),1,bheadfd))
				{
				tmlink.mlink_area = tmsgh.msgh_area;
				tmlink.mlink_number = tmsgh.msgh_number;
				tmlink.mlink_flags = tmsgh.msgh_flags;
				tmlink.mlink_cksum = 0;
				cptr = tmsgh.msgh_to;
				while (*cptr)
					{
					tmlink.mlink_cksum += toupper(*cptr);
					++cptr;
					}
				fwrite(&tmlink,sizeof(struct mlink),1,mlinkfd);
				}
			}
		fflush(mlinkfd);
		}
	if (max_msgcount)
		free(msgcount);
	return 0;
	}
