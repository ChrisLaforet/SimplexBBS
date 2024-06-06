/* sfac.h
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 11 June 1992
**
** Revision Information: $Logfile:   G:/simplex/sfac/vcs/sfac.h_v  $
**                       $Date:   25 Oct 1992 14:16:36  $
**                       $Revision:   1.0  $
**
*/

#define MAJOR_VERSION				1
#define MINOR_VERSION				1

#define P_CSEP						'\\'	/* character form of path separator */
#define P_SSEP						"\\"	/* string form of path separator */


typedef unsigned short DATE_T;
typedef unsigned short TIME_T;


struct file
	{
	short file_number;				/* file board number */
	char file_areaname[41];			/* file area name */
	char file_pathname[61];			/* path to file area */
	char file_descname[61];			/* if set, this is the path to files.bbs */
	unsigned char file_priv;		/* priv level to search this area */
	unsigned short file_flags;		/* 1.05.02 - flags for search access */
	char file_deleted;				/* true if deleted */
	};


struct fe		/* 384-byte file entry structure */
	{
	char fe_name[16];				/* due to speedups, do NOT change this length */
	short fe_location;				/* area number this file is located in */
	char fe_descrip[257];
	char fe_group[16];				/* grouping for this item - cut from 41 chars in 1.07.00*/
	char fe_res1[25];				/* 1.07 - reserved should be nulled out */
	char fe_uploader[41];
	DATE_T fe_uldate;
	DATE_T fe_dldate;				/* last date downloaded */
	unsigned short fe_dl;			/* number of downloads */
	unsigned short fe_priv;			/* min priv level to see/dl this file */
	short fe_flags;					/* flags to see/dl this file */
	short fe_eflags;				/* file entry flags - added in 1.07.00 */
	char fe_res2[15];				/* reserved should be nulled out */
	};


struct fi
	{
	char fi_name[256];
	DATE_T fi_date;					/* DOS style date YYYYYYY MMMM DDDDD where Y is YYYY - 1980 */
	TIME_T fi_time;					/* DOS style time HHHHH MMMMMM SSSSS where S is SSSS/2 */
	long fi_size;					/* Files size in bytes */
	};


extern int get_firstfile(struct fi *fi,char *mask);
extern void get_closefile(void);
extern DATE_T get_cdate(void);
