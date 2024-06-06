/* supack.h
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 October 1991
**
** Revision Information: $Logfile:   G:/simplex/supack/vcs/supack.h_v  $
**                       $Date:   25 Oct 1992 14:29:46  $
**                       $Revision:   1.1  $
**
*/


#define MAJOR_VERSION				1
#define MINOR_VERSION				2

#define BETA						0



/* these macros access the user_flags bit-mapped options */

#define USER_CLS					0x1			/* send cls codes */
#define USER_MORE					0x2			/* use more? paging */
#define USER_ANSI					0x4			/* use ansi */
#define USER_EDITOR					0x8			/* use full screen editor */
#define USER_EXPERT					0x10		/* expert (abbreviated) menus */
#define USER_FILEATTACH				0x20		/* allow user to fileattach in netmail areas? */
#define USER_GUEST					0x40		/* this login is guest - recycle login time for day */
#define USER_ANSWERED				0x4000		/* user has answered a newuser.qf questionaire */
#define USER_DELETED				0x8000		/* user is deleted pending purge */


#define MSG_AREAS					1250


#ifdef UNIX
#define P_CSEP						'/'		/* character form of path separator */
#define P_SSEP						"/"		/* string form of path separator */
#else
#define P_CSEP						'\\'	/* character form of path separator */
#define P_SSEP						"\\"	/* string form of path separator */
#endif


typedef unsigned short DATE_T;
typedef unsigned short TIME_T;


struct user
	{
	char user_name[41];
	char user_address1[31];
	char user_address2[31];
	char user_city[31];
	char user_state[16];
	char user_zip[16];
	char user_password[16];
	char user_home[15];				/* users home number */
	char user_data[15];				/* users data/business number */
	char user_alias[4][41];			/* registered aliases added in v 1.07.00 */

	unsigned char user_priv;		/* users default privilege level */
	unsigned short user_uflags;		/* 16 default definable flags */
	unsigned char user_spriv;		/* users special privilege level */
	unsigned short user_sflags;		/* 16 special definable flags */
	DATE_T user_sdate;				/* expiration date of special levels */

	char user_screenlen;			/* length of users screen in lines */
	short user_credit;				/* credit in cents for fidomail */
	unsigned short user_flags;		/* bit mapped flags for options */

	DATE_T user_firstdate; 			/* first date user called */
	DATE_T user_lastdate; 			/* last date called */
	TIME_T user_lasttime; 			/* last time called */
	short user_calls; 				/* number of calls to the BBS */
	short user_timeused;			/* amount of time used for the day (minutes) */

	short user_upload;				/* number of uploads */
	long user_uploadbytes;			/* number of bytes uploaded */
	short user_dnload;				/* number of downloads */
	long user_dnloadbytes;			/* number of bytes downloaded */
	long user_msgsent;				/* number of messages sent */
	long user_msgread;				/* number of messages read */
	};


struct comb				/* combined message board structure */
	{
	short comb_user;				/* user number */
	unsigned short comb_flags;
	unsigned char comb_areas[MSG_AREAS];
	};


