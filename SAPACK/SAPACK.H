/* sapack.h
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 19 August 1992
**
** Revision Information: $Logfile:   G:/simplex/sapack/vcs/sapack.h_v  $
**                       $Date:   25 Oct 1992 14:13:40  $
**                       $Revision:   1.0  $
**
*/

#define MAJOR_VERSION				1
#define MINOR_VERSION				1


#define P_CSEP					(char)'\\'	/* character form of path separator */
#define P_SSEP					"\\"		/* string form of path separator */


/* these macros are for advertisement board header flags */

#define ADH_DELETED					0x1
#define ADH_REPLIES					0x2


typedef unsigned short DATE_T;
typedef unsigned short TIME_T;


struct adh		/* header for advertisements */
	{
	char adh_from[41];
	char adh_subject[71];
	DATE_T adh_date;
	TIME_T adh_time;
	DATE_T adh_killdate;
	short adh_flags;				/* ad flags */
	long adh_prev;					/* previous ad header */
	long adh_next;					/* next ad header */
	long adh_length;				/* ad length in bytes */
	};


