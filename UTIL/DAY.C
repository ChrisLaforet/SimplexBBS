/* day.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 3 January 1992
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/

#define INCL_DOSDATETIME

#include <stdio.h>
#include <os2.h>



char  *weekday[7] =
	{
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
	};



int main(void)
	{
	DATETIME dt;

	fprintf(stderr,"DAY: Returns the day of the week!\n\n");
	DosGetDateTime(&dt);

	fprintf(stderr,"Today is %s\n",weekday[dt.weekday]);

	return dt.weekday;
	}
