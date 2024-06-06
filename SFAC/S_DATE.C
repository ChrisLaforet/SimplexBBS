/* s_date.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 16 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sfac/vcs/s_date.c_v  $
**                       $Date:   25 Oct 1992 14:16:32  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef PROTECTED
	#define INCL_DOSDATETIME
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include "sfac.h"




#ifdef PROTECTED 

DATE_T get_cdate(void)
	{
	DATETIME dt;

	DosGetDateTime(&dt);
	return (DATE_T)(((dt.year - 1980) << 9) | ((dt.month & 15) << 5) | (dt.day & 31));
	}


#else 

DATE_T get_cdate(void)
	{
	struct dosdate_t date;

	_dos_getdate(&date);
	return (DATE_T)((date.year - 1980) << 9) | ((date.month & 15) << 5) | (date.day & 31);
	}

#endif
