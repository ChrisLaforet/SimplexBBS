/* s_flist.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 11 June 1992
**
** Revision Information: $Logfile:   G:/simplex/sfac/vcs/s_flist.c_v  $
**                       $Date:   25 Oct 1992 14:16:34  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <string.h>
#if defined(PROTECTED)
	#define INCL_DOSFILEMGR
	#define INCL_DOSPROCESS
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include "sfac.h"




#if defined(PROTECTED)
	HDIR hdir;			/* OS/2 directory */
	USHORT search;
	FILEFINDBUF find;
#else
	struct find_t find;
#endif



#if defined(PROTECTED)

DATE_T conv_date(FDATE date)
	{
	DATE_T val;

	val = (DATE_T)date.year << 9;
	val |= (DATE_T)date.month << 5;
	val |= (DATE_T)date.day;
	return val;
	}



TIME_T pascal conv_time(FTIME time)
	{
	TIME_T val;

	val = (TIME_T)time.hours << 11;
	val |= (TIME_T)time.minutes << 5;
	return val;
	}

#endif




int get_firstfile(struct fi *fi,char *mask)
	{
	int rtn;
	int found = 0;

#if defined(PROTECTED)
	search = 1;
	hdir = HDIR_CREATE;
	rtn = (int)DosFindFirst(mask,&hdir,FILE_NORMAL | FILE_ARCHIVED | FILE_READONLY,&find,sizeof(FILEFINDBUF),&search,0L);
	if (!rtn)
		{
		strcpy(fi->fi_name,find.achName);
		strupr(fi->fi_name);
		fi->fi_size = find.cbFile;
		fi->fi_time = conv_time(find.ftimeLastWrite);
		fi->fi_date = conv_date(find.fdateLastWrite);
		found = 1;
		}
#else
	rtn = (int)_dos_findfirst(mask,_A_ARCH | _A_RDONLY,&find);
	if (!rtn)
		{
		strcpy(fi->fi_name,find.name);
		strupr(fi->fi_name);
		fi->fi_size = find.size;
		fi->fi_time = (TIME_T)find.wr_time;
		fi->fi_date = (DATE_T)find.wr_date;
		found = 1;
		}
#endif
	return found;
	}



void get_closefile(void)
	{
#if defined(PROTECTED)
	DosFindClose(hdir);
#endif
	}
