/* sclean.h
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 20 May 1990
**
** Revision Information: $Logfile:   G:/simplex/sclean/vcs/sclean.h_v  $
**                       $Date:   25 Oct 1992 14:14:36  $
**                       $Revision:   1.10  $
**
*/


#define MAJOR_VERSION				1
#define MINOR_VERSION				8


#define P_CSEP					(char)'\\'	/* character form of path separator */
#define P_SSEP					"\\"		/* string form of path separator */


/* these macros are for message header flags */
		
#define MSGH_DELETED   				0x1
#define MSGH_PRIVATE   				0x2
#define MSGH_RECEIVED				0x4
#define MSGH_NET_UNSENT				0x8		/* is unsent Netmail message */
#define MSGH_NET	   				0x10	/* is Netmail */
#define MSGH_ECHO_UNSENT			0x20	/* is unsent Echomail message */
#define MSGH_ECHO					0x40	/* is echomail */
#define MSGH_LOCAL_FILEATTACH		0x80	/* is local fileattach */

#define NET_KILLSENT				0x100	/* kill/sent netmail */
#define NET_SENT					0x200	/* netmail sent */
#define NET_FILEATTACH				0x400	/* file(s) attached */
#define NET_CRASH					0x800	/* crashmail */
#define NET_RECEIPT_REQ				0x1000	/* receipt requested */
#define NET_AUDIT_REQ				0x2000	/* audit request */
#define NET_RECEIPT					0x4000	/* return receipt */


typedef unsigned short DATE_T;
typedef unsigned short TIME_T;


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



struct msgh		/* systemwide message number is ((offset/sizeof(struct msgh)) + 1) -- 0 is invalid systemwide message */
	{
	short msgh_area;				/* area number of message */
	short msgh_number;				/* message number in that area! */
	char msgh_to[41];				/* addressee of message */
	char msgh_from[41];
	char msgh_subject[71];
	DATE_T msgh_date;
	TIME_T msgh_time;
	unsigned short msgh_flags;		/* net and message flags */
	short msgh_szone;
	short msgh_snet;
	short msgh_snode;
	short msgh_dzone;
	short msgh_dnet;
	short msgh_dnode;
	short msgh_cost;
	short msgh_prev; 				/* previous message link - systemwide message number */
	short msgh_next; 				/* next message link - systemwide message number */
	long msgh_offset;				/* start offset of message */
	long msgh_length;				/* message length in bytes */
	};


struct mdata
	{
	short mdata_msgs;				/* total number of messages */
	short mdata_del;				/* total number of deleted messages */
	};


struct mc		/* message counts by area */
	{
	short mc_area;
	short mc_msgs;
	};


struct mlink
	{
	short mlink_area;
	short mlink_number;				/* number of the message in that area! */
	short mlink_flags;				/* a copy of the message flags */
	unsigned short mlink_cksum;		/* a checksum on the uppercased TO: person's name */
	};



struct mth			/* message threading utility */
	{
	short mth_deleted;
	unsigned short mth_prev;
	unsigned short mth_next;
	};


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



extern struct cfg cfg;
extern DATE_T today;
extern char bbspath[100];
extern int max_days;
extern FILE *mheadfd;
extern FILE *mdatafd;
extern FILE *mlinkfd;
extern FILE *mreadfd;
extern int *renum;
extern int max_renum;
extern int *areas;
extern int max_areas;
extern int max_users;
extern int *msgs;
extern int total_msgs;
extern int quiet_flag;



extern void check_delete(int area,char *fname);
extern void kill_all(int area);
extern void kill_date(int area,int days);
extern void kill_num(int area,int keep);
extern void kill_received(int area);
extern void update_chain(struct msgh *tmsgh,int total_msgs);

