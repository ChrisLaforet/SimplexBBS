/* squote.h
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 7 May 1991
**
** Revision Information: $Logfile:   G:/simplex/squote/vcs/squote.h_v  $
**                       $Date:   25 Oct 1992 14:28:26  $
**                       $Revision:   1.1  $
**
*/


/* 1. Constants and Macros */


#define MAJOR_VERSION				1
#define MINOR_VERSION				2

#define P_CSEP						'\\'	/* character form of path separator */
#define P_SSEP						"\\"	/* string form of path separator */


/* 2. Structures and Unions */

struct line
	{
	short line_len;
	long line_offset;
	};


struct qh		/* quote file header! */
	{
	char qh_sig[14];		/* must be "Simplex Quote" */
	short qh_lines;			/* total number of lines */
	};
