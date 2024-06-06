/* listgen.h
**
** Copyright (c) 1989, Christopher Laforet
** All Rights Reserved
**
** Started: 14 July 1989
**
** Revision Information:
**
** $Logfile:   E:/quickbbs/programs/listgen/vcs/listgen.h_v  $
** $Date:   26 Jul 1989 13:44:56  $
** $Revision:   1.1  $
**
*/


#define MAJOR_VERSION		3
#define MINOR_VERSION		0


#define P_CSEP						'\\'	/* character form of path separator */
#define P_SSEP						"\\"	/* string form of path separator */


struct farea
	{
	int farea_number;				/* file board number */
	char farea_areaname[41];  		/* file area name */
	char farea_pathname[61];  		/* path to file area */
	char farea_descname[61]; 		/* if set, this is the path to files.bbs */
	unsigned char farea_priv;		/* priv level to search this area */
	int farea_flags;					/* 1.05.02 - flags for search access */
	char farea_deleted;				/* true if deleted */
	};


struct file
	{
	unsigned char file_name[13];
	unsigned short file_area;
	unsigned long file_size;
	unsigned short file_date;
	};


struct fe		/* 384-byte file entry structure */
	{
	char fe_name[16];
	int fe_location;			/* area number this file is located in */
	char fe_descrip[257];
	char fe_group[41];			/* grouping for this item */
	char fe_uploader[41];
	unsigned int fe_uldate;
	unsigned int fe_dldate;		/* last date downloaded */
	unsigned int fe_dl;			/* number of downloads */
	unsigned int fe_priv;		/* min priv level to see/dl this file */
	int fe_flags;				/* flags to see/dl this file */
	char fe_reserved[17];
	};

	
extern void get_paths(void);
extern void list_files(void);
extern void cross_reference(void);

