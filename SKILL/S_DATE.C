/* s_date.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 16 March 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#ifdef PROTECTED
	#define INCL_DOSDATETIME
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include "skill.h"



#ifdef PROTECTED		/* os/2 versions of get date and time functions */

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
	return (DATE_T)(((date.year - 1980) << 9) | ((date.month & 15) << 5) | (date.day & 31));
	}

#endif



DATE_T new_date(DATE_T start,unsigned short days)
	{
	unsigned short total = 0;
	short tval;
	short month;
	short year;
	short day;

	month = GET_MONTH(start);
	year = GET_YEAR(start) + 80;
	day = GET_DAY(start);

	while (total < days)
		{
		tval = days - total;
		if (tval > day)
			tval = day;
		day -= tval;
		total += tval;

		if (!day)
			{
			switch (month)
				{
				case 1:
					if (year == 80)
						return (DATE_T)((1 << 5) | 1);		/* Jan 1, 80 */
					--year;
					month = 12;
					day = 31;
					break;

				case 3:
					--month;
					if (year % 4)
						day = 28;
					else
						day = 29;
					break;

				case 5:
				case 7:
				case 10:
				case 12:
					--month;
					day = 30;
					break;

				default:
					--month;
					day = 31;
					break;
				}
			}
		}
	year -= 80;
	return (DATE_T)((year << 9) | (month << 5) | day);
	}






