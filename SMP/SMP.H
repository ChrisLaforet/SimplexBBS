/* smp.h
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 17 November 1990
**
** Revision Information: $Logfile:   G:/simplex/smp/vcs/smp.h_v  $
**                       $Date:   25 Oct 1992 14:26:38  $
**                       $Revision:   1.10  $
**
*/


/* 1. Constants and Macros */

#define MAJOR_VERSION					1
#define MINOR_VERSION					9



#define P_CSEP							(char)'\\'	/* character form of path separator */
#define P_SSEP							"\\"		/* string form of path separator */


#define ALL								-1


#define LEFT							0		/* do not change these constants */
#define HOLD							1
#define ROUTE							2
#define DIRECT							3
#define DELETED							100


#define ARC								1		/* archive type */
#define ZIP								2
#define LZH								3
#define ZOO								4
#define ARJ								5


#define LEAVEHOLD						1		/* action constants */
#define LEAVEROUTE						2
#define LEAVEDIRECT						3
#define SENDHOLD						4
#define SENDROUTE						5
#define SENDDIRECT						6
#define HOSTHOLD						7
#define HOSTROUTE						8
#define HOSTDIRECT						9
#define POLL							10
#define HOLDTO							11
#define ROUTETO							12
#define DIRECTTO 						13

#define ARCHOLD							20		/* this makes held hlo files */
#define ZIPHOLD							21
#define LZHHOLD							22
#define ZOOHOLD							23
#define ARJHOLD							24
#define HOLDROUTE						25
#define HOLDDIRECT						26

#define ARCROUTE		  				30		/* this makes Normal flo files */
#define ZIPROUTE		  				31
#define LZHROUTE		  				32
#define ZOOROUTE		  				33
#define ARJROUTE		  				34
#define ROUTEHOLD						35
#define ROUTEDIRECT		  				36

#define ARCDIRECT						40		/* this make CM (Crash) files */
#define ZIPDIRECT						41
#define LZHDIRECT						42
#define ZOODIRECT						43
#define ARJDIRECT						44
#define DIRECTHOLD						45
#define DIRECTROUTE						46


typedef unsigned short DATE_T;
typedef unsigned short TIME_T;



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


struct act
	{
	short act_type;
	short act_zone;
	short act_net;
	short act_node;
	short act_dzone;					/* destination zone */
	short act_dnet;					/* destination net */
	short act_dnode;					/* destination node */
	};


struct mlist
	{
	short mlist_type;
	short mlist_archived;				/* true if destined for archive */
	short mlist_szone;
	short mlist_snet;
	short mlist_snode;
	short mlist_dzone;
	short mlist_dnet;
	short mlist_dnode;
	short mlist_intl;					/* true if ^aINTL line is in message */
	short *mlist_msgs;
	short mlist_curmsgs;
	short mlist_maxmsgs;
	};


struct olist
	{
	short olist_type;
	short olist_szone;
	short olist_snet;
	short olist_snode;
	short olist_dzone;
	short olist_dnet;
	short olist_dnode;
	char *olist_file;
	};


struct fm				/* fidonet message format as of 1990 */
	{
	char fm_from[36];
	char fm_to[36];
	char fm_subject[72];
	char fm_datetime[20];
	short fm_read;					/* times read */
	short fm_dnode;					/* destination node */
	short fm_snode;					/* source node */
	short fm_cost;
	short fm_snet;					/* source net */
	short fm_dnet;					/* destination net */
	short fm_dzone;					/* destination zone */
	short fm_szone;					/* source node */
	short fm_dpoint;				/* destination poshort */
	short fm_spoint;				/* source node */
	short fm_prev;					/* reply to... */
	short fm_flags;
	short fm_next;
	};



struct pfm				/* packed fidonet message format */
	{
	char pfm_sig1;				/* must be 2 */
	char pfm_sig2;				/* must be 0 */
	short pfm_snode;
	short pfm_dnode;
	short pfm_snet;
	short pfm_dnet;
	short pfm_flags;
	short pfm_cost;
	};


struct ph				/* packet header */
	{
	short ph_snode;
	short ph_dnode;
	short ph_year;
	short ph_month;
	short ph_day;
	short ph_hour;
	short ph_minute;
	short ph_second;
	short ph_baud;
	char ph_sig1;				/* must be 2 */
	char ph_sig2;				/* must be 0 */
	short ph_snet;
	short ph_dnet;
	char ph_code;				/* product code */
	char ph_fill1;
	char ph_password[8];
	char ph_fill2[24];
	};


struct fi
	{
	char fi_name[256];
	DATE_T fi_date;				/* DOS style date YYYYYYY MMMM DDDDD where Y is YYYY - 1980 */
	TIME_T fi_time;				/* DOS style time HHHHH MMMMMM SSSSS where S is SSSS/2 */
	long fi_size;				/* Files size in bytes */
	};


struct pp				/* packet passwords */
	{
	short pp_zone;
	short pp_net;
	short pp_node;
	char *pp_password;
	};



/* 3. Global Variables */

extern struct cfg cfg;
extern char bbspath[100];
extern char outbound[256];
extern int zone;
extern int net;
extern int node;
extern int debug;
extern int echosec_flag;
extern struct pp **passwords;
extern int cur_passwords;
extern int max_passwords;
extern struct act **actions;
extern int cur_actions;
extern int max_actions;
extern struct mlist **mlist;
extern int cur_mlist;
extern int max_mlist;
extern struct olist **olist;
extern int cur_olist;
extern int max_olist;


/* 4. Function Declarations */

extern char *parse_string(char *string,char **next);
extern int parse_address(char *string,int *zone,int *net,int *node);
extern void load_actions(FILE *fd,int sched);
extern void dump_actions(void);
extern int check_action(int tzone,int tnet,int tnode);

extern void load_mlist(void);
extern void dump_mlist(void);
extern void route_mlist(void);

extern char *get_outbound(int tzone);
extern void packet_mail(void);
extern int append_mail(FILE *srcfd,FILE *destfd);
extern int append_outfile(FILE *srcfd,FILE *destfd);

extern void load_outbound(void);
extern void dump_outbound(void);
extern void process_outbound(void);

extern void delete_truncated(void);
extern int archive(int type,char *filename,char *newname,int tzone,int tnet,int tnode);
extern void generate_poll(int dzone,int dnet,int dnode);

extern int pascal get_firstfile(struct fi *fi,char *mask);
extern int pascal get_nextfile(struct fi *fi);
extern void pascal get_closefile(void);

extern int pascal get_firstdir(struct fi *fi,char *mask);
extern int pascal get_nextdir(struct fi *fi);
extern void pascal get_closedir(void);

extern void read_passwords(FILE *fd);
