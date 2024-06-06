/* skill.h
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


/* 1. Constants and Macros */

#define MAJOR_VERSION				1
#define MINOR_VERSION				1

#ifdef UNIX
#define P_CSEP						'/'		/* character form of path separator */
#define P_SSEP						"/"		/* string form of path separator */
#else
#define P_CSEP						'\\'	/* character form of path separator */
#define P_SSEP						"\\"	/* string form of path separator */
#endif


typedef unsigned short DATE_T;
typedef unsigned short TIME_T;

#define INVALID						(DATE_T)0xffff

#define GET_MONTH(date)				(short)((date >> 5) & 0xf)
#define GET_DAY(date)				(short)(date & 0x1f)
#define GET_YEAR(date)				(short)((date >> 9) & 0x7f)



/* 2. Structures and Unions */

struct cfg
	{
	unsigned long cfg_version;		/* 1.07 current version of Simplex (Major << 16L | Minor << 8 | Sub) */
	unsigned short cfg_baud;		/* maximum baud rate */
	short cfg_port;					/* port number */
	char cfg_init[61];				/* modem initialization string */
	char cfg_resp[21];				/* normal modem response string */
	char cfg_dialpre[21];			/* dial prefix */
	char cfg_dialpost[21];			/* dial postfix */
	char cfg_resp3_1[21];			/* 300 baud response 1 */
	char cfg_resp3_2[21];			/* 300 baud response 2 */
	char cfg_resp12_1[21];			/* 1200 baud response 1 */
	char cfg_resp12_2[21];			/* 1200 baud response 2 */
	char cfg_resp24_1[21];			/* 2400 baud response 1 */
	char cfg_resp24_2[21];			/* 2400 baud response 2 */
	char cfg_resp48_1[21];			/* 4800 baud response 1 */
	char cfg_resp48_2[21];			/* 4800 baud response 2 */
	char cfg_resp96_1[21];			/* 9600 baud response 1 */
	char cfg_resp96_2[21];			/* 9600 baud response 2 */
	char cfg_resp192_1[21];			/* 19200 baud response 1 */
	char cfg_resp192_2[21];			/* 19200 baud response 2 */
	char cfg_resp384_1[21];			/* 38400 baud response 1 */
	char cfg_resp384_2[21];			/* 38400 baud response 2 */
	char cfg_ring[21];				/* ring response */
	char cfg_answer[21];			/* answer string */
	char cfg_hangup[21];			/* hangup string */
	char cfg_busy[21];				/* offhook string */
	short cfg_flags;				/* flags for setup */

	short cfg_zone;
	short cfg_net;
	short cfg_node;
	short cfg_point;				/* point for later use */
	short cfg_akazone[5];
	short cfg_akanet[5];
	short cfg_akanode[5];
	short cfg_akapoint[5];			/* points for later use */

	char cfg_sysopname[41];			/* sysop's name */
	char cfg_bbsname[61];			/* BBS name */
	char cfg_origin[66];			/* origin line */
	char cfg_qwkname[9];			/* name for QWK packet BBSID */
	char cfg_menupath[61];			/* path to menu files */
	char cfg_screenpath[61];		/* path to screen files */
	char cfg_nodepath[61];			/* path to nodelist */
	char cfg_fapath[61];			/* path to local fileattaches */

	char cfg_netpath[61];			/* path for net messages */
	char cfg_inboundpath[61];		/* path for inbound message pkts */
	char cfg_outboundpath[61];		/* path for outbound message pkts */
	char cfg_packetpath[61];		/* path for unpacking message pkts */
	char cfg_badmsgpath[61];		/* path for placing bad messages */
	char cfg_unarc[61];				/* command for unarcing ARCed files */
	char cfg_unzip[61];				/* command for unarcing ZIPped files */
	char cfg_unzoo[61];				/* command for unarcing ZOOed files */
	char cfg_unlzh[61];				/* command for unarcing LZHed files */
	char cfg_unarj[61];				/* command for unarcing ARJed files */
	char cfg_arc[61];				/* command for arcing ARCed files */
	char cfg_zip[61];				/* command for arcing ZIPped files */
	char cfg_zoo[61];				/* command for arcing ZOOed files */
	char cfg_lzh[61];				/* command for arcing LZHed files */
	char cfg_arj[61];				/* command for arcing ARJed files */

	unsigned char cfg_newpriv;		/* priv level of new user */
	unsigned short cfg_newflags;	/* v 1.02 new user's flags */
	short cfg_newcredit;			/* credit level of new user */
	short cfg_newtime;				/* logon time in mins for newuser */
	char cfg_askaddress;			/* ask for full address */
	char cfg_askhome;				/* ask for home phone */
	char cfg_askdata;				/* ask for data phone */

	unsigned short cfg_minbaud; 	/* minimum connect rate allowed on BBS */
	unsigned short cfg_ansibaud;	/* minimum baud for ansi */
	char cfg_status;				/* status line background color */
	char cfg_field;					/* entry field background color */
	char cfg_menu;					/* submenu color */
	char cfg_pwdtries;				/* number of attempts to get password */
	char cfg_inactive;				/* time before hanging up for inactivity */
	char cfg_yells;					/* number of yells per session */
	char cfg_yelltime;				/* number of seconds for yell bell */
	TIME_T cfg_yellstart;			/* start time for yell-bell */
	TIME_T cfg_yellstop;			/* stop time for yell-bell */
	short cfg_dlstart;				/* start time for downloads */
	short cfg_dlstop;				/* stop time for downloads */

	char cfg_welcome[61];			/* command line for welcome program */
	char cfg_viruschk[61];			/* command line for virus checker */

	short cfg_uts;					/* 1.07 number of user time structs following cfg */
	char cfg_reserved[120];			/* 1.07 */
	};


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
	char fe_name[16];			/* due to speedups, do NOT change this length */
	short fe_location;			/* area number this file is located in */
	char fe_descrip[257];
	char fe_group[16];			/* grouping for this item - cut from 41 chars in 1.07.00*/
	char fe_res1[25];			/* 1.07 - reserved should be nulled out */
	char fe_uploader[41];
	DATE_T fe_uldate;
	DATE_T fe_dldate;			/* last date downloaded */
	unsigned short fe_dl;		/* number of downloads */
	unsigned short fe_priv;		/* min priv level to see/dl this file */
	short fe_flags;				/* flags to see/dl this file */
	short fe_eflags;			/* file entry flags - added in 1.07.00 */
	char fe_res2[15];			/* reserved should be nulled out */
	};


struct fi
	{
	char fi_name[256];
	DATE_T fi_date;		/* DOS style date YYYYYYY MMMM DDDDD where Y is YYYY - 1980 */
	TIME_T fi_time;		/* DOS style time HHHHH MMMMMM SSSSS where S is SSSS/2 */
	long fi_size;		/* Files size in bytes */
	};



/* 3. Global Variables */

extern char bbspath[81];
extern short kill_flag;
extern short confirm_flag;
extern unsigned long freed;
extern DATE_T kill_date;

extern struct file **files;
extern short cur_files;
extern short max_files;



/* 4. Function Declarations */

extern DATE_T get_cdate(void);
extern DATE_T new_date(DATE_T start,unsigned short days);

extern void kill_files(struct file *fptr);

extern int pascal get_firstfile(struct fi *fi,char *mask);
extern int pascal get_nextfile(struct fi *fi);
extern void pascal get_closefile(void);
