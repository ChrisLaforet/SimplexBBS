/* s_date.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 12 January 1991
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_date.c_v  $
**                       $Date:   25 Oct 1992 14:18:16  $
**                       $Revision:   1.9  $
**
*/


#include <stdio.h>
#ifdef PROTECTED
	#define INCL_DOSDATETIME
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include "smail.h"



int days_table[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


void load_datetime(DATE_T *dte,TIME_T *tme)		/* loads today's date when messages come in without timestamp */
	{
#ifdef PROTECTED		/* os/2 versions of get date and time functions */
	DATETIME dt;

	DosGetDateTime(&dt);
	*dte = ((dt.year - 1980) << 9) | ((dt.month & 15) << 5) | (dt.day & 31);
	*tme = (dt.hours << 11) | (dt.minutes << 5) | (dt.seconds >> 1);
#else
	struct dosdate_t date;
	struct dostime_t time;

	_dos_getdate(&date);
	_dos_gettime(&time);
	*dte = ((date.year - 1980) << 9) | ((date.month & 15) << 5) | (date.day & 31);
	*tme = (time.hour << 11) | (time.minute << 5) | (time.second >> 1);
#endif
	}



unsigned int date_to_julian(DATE_T dte) 
	{
	unsigned int work = 0;
	unsigned int days = 0;
	int count;
	int temp;
	int leap = 0;

	temp = dte >> 9;
	temp += 1980;
	if (!(temp % 4))
		leap = 1;
	temp -= 1980;				 /* 1980 is the base year for DOS, same for us! */
	work = (temp & 0x7f) << 9;	 /* fold year into top 7 bits! */

	temp = (dte >> 5) & 0xf;
	for (count = 1; count < temp; count++)
		days += days_table[count - 1];
	if ((temp > 2) && leap)
		++days;

	temp = dte & 0x1f;
	days += temp;
	work |= (days & 0x1ff);
	return work;
	}



DATE_T julian_to_date(unsigned int julian)
	{
	int year;
	int month = 1;
	int day;
	int leap = 0;

	year = julian >> 9;
	year += 1980;
	if (!(year % 4))
		leap = 1;
	year -= 1980;
	day = julian & 0x1ff;
	while (day > days_table[month - 1])
		{
		day -= days_table[month - 1];
		if (month == 2 && leap)
			{
			if (day > 1)
				--day;
			else
				day = 29;
				break;
			}
		++month;
		}
	return (year << 9) | (month << 5) | day;
	}


/* juliancmp() returns: -value if date1 < date2
**							 0 if date1 = date2
**						+value if date1 > date2
*/

int juliancmp(unsigned int julian1,unsigned int julian2)
	{
	int rtn;

	if (!(rtn = ((julian1 >> 9) - (julian2 >> 9))))
		rtn = (julian1 & 0x1ff) - (julian2 & 0x1ff);
	return rtn;
	}



unsigned int next_julian(unsigned int start)
	{
	int year;
	int max;
	int day;

	year = start >> 9;
	day = start & 0x1ff;
	if ((year + 1980) % 4)
		max = 365;	   
	else
		max = 366;
	if (day < max)
		++day;
	else
		{
		++year;
		day = 1;
		}
	return (unsigned int)(year << 9) | day;
	}



unsigned int prev_julian(unsigned int start)
	{
	int year;
	int day;

	year = start >> 9;
	day = start & 0x1ff;
	if (day > 1)
		--day;
	else
		{
		--year;
		if ((year + 1980) % 4)
			day = 365;	   
		else
			day = 366;
		}
	return (unsigned int)(year << 9) | day;
	}



#if defined(__WATCOMC__)


char *_strdate(char *dstr)
	{
#ifdef PROTECTED		/* os/2 versions of get date and time functions */
	DATETIME dt;

	DosGetDateTime(&dt);
	sprintf(dstr,"%02u/%02u/%02u",dt.month,dt.day,dt.year % 100);
#else
	struct dosdate_t date;

	_dos_getdate(&date);
	sprintf(dstr,"%02u/%02u/%02u",date.month,date.day,date.year % 100);
#endif
	return dstr;
	}



char *_strtime(char *tstr)
	{
#ifdef PROTECTED		/* os/2 versions of get date and time functions */
	DATETIME dt;

	DosGetDateTime(&dt);
	sprintf(tstr,"%02u:%02u:%02u",dt.hours,dt.minutes,dt.seconds);
#else
	struct dostime_t time;

	_dos_gettime(&time);
	sprintf(tstr,"%02u:%02u:%02u",time.hour,time.minute,time.second);
#endif

	return tstr;
	}


#endif
