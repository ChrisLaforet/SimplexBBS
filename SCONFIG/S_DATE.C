/* s_date.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 1 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_date.c_v  $
**                       $Date:   25 Oct 1992 14:15:38  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef PROTECTED
	#define INCL_DOSDATETIME
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include "sconfig.h"



int legit[12] =                // Days Per Month
	{
	31,28,31,
	30,31,30,
	31,31,30,
	31,30,31
	};


#ifdef PROTECTED		/* os/2 versions of get date and time functions */

DATE_T get_cdate(void)
	{
	DATETIME dt;

	DosGetDateTime(&dt);
	return (DATE_T)((dt.year - 1980) << 9) | ((dt.month & 15) << 5) | (dt.day & 31);
	}

#else			/* MS-C DOS versions of get date and time functions */

DATE_T get_cdate(void)
	{
	struct dosdate_t date;

	_dos_getdate(&date);
	return (DATE_T)((date.year - 1980) << 9) | ((date.month & 15) << 5) | (date.day & 31);
	}

#endif



int check_date(char *date)		/* check date formatted MMDDYY for validity: Returns TRUE if valid */
	{
	char buffer[5];
	int ok = 0;
	int day;
	int month;
	int year;

	if (strlen(date) == 6)
		{
		strncpy(buffer,date,2);
		buffer[2] = '\0';
		month = atoi(buffer);
		strncpy(buffer,date + 2,2);
		buffer[2] = '\0';
		day = atoi(buffer);
		strncpy(buffer,date + 4,2);
		buffer[2] = '\0';
		year = atoi(buffer);
		if (year < 80)
			year += 2000;
		else 
			year += 1900;

		if (month && month < 13 && day)
			{
			switch (month)
				{
				case 4:
				case 6:
				case 9:
				case 11:
					if (day <= 30)
						ok = 1;
					break;
				case 2:
					if (!(year % 4))
						{
						if (day <= 29)
							ok = 1;
						}
					else if (day <= 28)
						ok = 1;
					break;
				default:
					if (day <= 31)
						ok = 1;
					break;
				}
			}
		}
	return ok;
	}



DATE_T atodate(char *date)		/* convert date formatted MMDDYY to DATE type */
	{
	char buffer[5];
	int day;
	int month;
	int year;
	DATE_T rtn_date = 0;
	int ok = 0;

	if (strlen(date) == 6)
		{
		strncpy(buffer,date,2);
		buffer[2] = '\0';
		month = atoi(buffer);
		strncpy(buffer,date + 2,2);
		buffer[2] = '\0';
		day = atoi(buffer);
		strncpy(buffer,date + 4,2);
		buffer[2] = '\0';
		year = atoi(buffer);
		if (year < 80)
			year += 2000;
		else 
			year += 1900;

		if (month && month < 13 && day)
			{
			switch (month)
				{
				case 4:
				case 6:
				case 9:
				case 11:
					if (day <= 30)
						ok = 1;
					break;
				case 2:
					if (!(year % 4))
						{
						if (day <= 29)
							ok = 1;
						}
					else if (day <= 28)
						ok = 1;
					break;
				default:
					if (day <= 31)
						ok = 1;
					break;
				}

			if (ok)
				{
				year -= 1980;
				rtn_date = (DATE_T)((year << 9) | (month << 5) | day);
				}
			}
		}
	return rtn_date;
	}



char *datetoa(DATE_T date)		/* returns MMDDYY in ascii string */
	{
	static char dbuf[10];

	sprintf(dbuf,"%02u%02u%02u",(date >> 5) & 0xf,date & 0x1f,((date >> 9) + 80) % 100);
	return dbuf;
	}
