/* s_analys.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 May 1990
**
** Revision Information: $Logfile:   G:/simplex/spack/vcs/s_analys.c_v  $
**                       $Date:   25 Oct 1992 14:27:08  $
**                       $Revision:   1.4  $
**
*/


#include <stdio.h>
#include <ctype.h>
#if defined(PROTECTED)
	#define INCL_DOSFILEMGR
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include "spack.h"



#define KILL_SIZE				(unsigned long)100000




int analyse(void)
	{
	struct msgh tmsgh;
	struct mlink tmlink;
	unsigned int count = 0;
	unsigned int last = 0;
	long needed;
	long avail;
	unsigned int drive;
#if defined(PROTECTED)
	unsigned long tlong;
	FSALLOCATE fa;
#else
	struct diskfree_t df;
#endif

	fprintf(stderr,"Analyzing message base usage....\n");
	fseek(mheadfd,0L,SEEK_SET);
	while (fread(&tmsgh,sizeof(struct msgh),1,mheadfd))
		{
		if (!(tmsgh.msgh_flags & MSGH_DELETED) && (!tmsgh.msgh_area || (kill && tmsgh.msgh_length > KILL_SIZE)))		/* delete messages in area 0 */
			{
			if (!tmsgh.msgh_area)
				fprintf(stderr,"Deleting message %u: Invalid area #0 in header.\n",count + 1);
			else
				fprintf(stderr,"Deleting message %u: Invalid message size.\n",count + 1);

			tmsgh.msgh_flags |= MSGH_DELETED;
			fseek(mheadfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
			fwrite(&tmsgh,sizeof(struct msgh),1,mheadfd);
			fseek(mheadfd,0L,SEEK_CUR);		/* transition from write back to read mode! */
			fflush(mheadfd);

			fseek(mlinkfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
			fread(&tmlink,sizeof(struct mlink),1,mlinkfd);
			tmlink.mlink_flags = tmsgh.msgh_flags;
			fseek(mlinkfd,(long)count * (long)sizeof(struct msgh),SEEK_SET);
			fwrite(&tmlink,sizeof(struct mlink),1,mlinkfd);		/* write out data */
			fflush(mlinkfd);
			}

		if (!quiet_flag)
			fprintf(stderr,"\r  Message %u - %s ",count + 1,tmsgh.msgh_flags & MSGH_DELETED ? "Deleted" : "Active");

		if (tmsgh.msgh_flags & MSGH_DELETED)
			{
			++deleted_headers;
			deleted_bytes += (unsigned long)tmsgh.msgh_length;
			}
		else
			{
			++valid_headers;
			valid_bytes += (unsigned long)tmsgh.msgh_length;
			++last;
			}
		++count;
		}
	if (count)
		{
		fprintf(stderr,"\r   Message base should contain %lu bytes.\n",valid_bytes + deleted_bytes);
		fprintf(stderr,"   Message base contains %lu bytes in %u valid messages.\n",valid_bytes,valid_headers);
		fprintf(stderr,"   Message base contains %lu bytes in %u deleted messages.\n",deleted_bytes,deleted_headers);
		if (!deleted_headers)
			{
			fprintf(stderr,"   Message base does not need packing.  Exiting.\n\n");
			return 1;
			}
		if (!inplace)
			{
			needed = valid_bytes + ((long)valid_headers * (long)sizeof(struct msgh));
			fprintf(stderr,"   Message base pack needs %ld bytes of disk space.\n",needed);

			if (bbspath[1] == ':')
				drive = (toupper(bbspath[0]) - 'A') + 1;
			else
				{
#if defined(PROTECTED)
				DosQCurDisk((USHORT *)&drive,&tlong);
#else
				_dos_getdrive(&drive);
#endif
				}

#if defined(PROTECTED)
			DosQFSInfo(drive,1,(BYTE *)&fa,sizeof(FSALLOCATE));
			avail = (long)fa.cUnitAvail * (long)fa.cSectorUnit * (long)fa.cbSector;
#else
			_dos_getdiskfree(drive,&df);
			avail = (long)df.avail_clusters * (long)df.sectors_per_cluster * (long)df.bytes_per_sector;
#endif
			if (avail < needed)
				{
				fprintf(stderr,"   Disk has %ld bytes available, not enough for packing.\n",avail);
				return 1;
				}
			else
				fprintf(stderr,"   Disk has %ld bytes available, enough for packing.\n",avail);
			}
		}
	else
		{
		fprintf(stderr,"   Message base has no messages.  Exiting!\n\n");
		return 1;
		}
	return 0;
	}



