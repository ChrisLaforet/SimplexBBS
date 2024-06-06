/* smail.h
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 9 December 1989
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/smail.h_v  $
**                       $Date:   25 Oct 1992 14:18:28  $
**                       $Revision:   1.12  $
**
*/


/* 1. Constants and Macros */


#define MAJOR_VERSION				1
#define MINOR_VERSION				9

#define P_CSEP						(char)'\\'	/* character form of path separator */
#define P_SSEP						"\\"		/* string form of path separator */

#define WRAP						76

#define PASSTHRU					0


/* these macros are for message board flags */

#define MSG_LOCAL					0x1
#define MSG_ECHO					0x2
#define MSG_NET						0x4
#define MSG_PRIVATE					0x8			/* messages are private */
#define MSG_PUBLIC					0x10		/* messages are public */
#define MSG_ALIAS					0x20		/* aliases are allowed */


/* these macros are for message header flags */
		
#define MSGH_DELETED   				0x1
#define MSGH_PRIVATE   				0x2
#define MSGH_RECEIVED				0x4
#define MSGH_NET_UNSENT				0x8		/* is unsent Netmail message */
#define MSGH_NET	   				0x10	/* is Netmail */
#define MSGH_ECHO_UNSENT			0x20	/* is unsent Echomail message */
#define MSGH_ECHO					0x40	/* is echomail */

#define NET_KILLSENT				0x100	/* kill/sent netmail */
#define NET_SENT					0x200	/* netmail sent */
#define NET_FILEATTACH				0x400	/* file(s) attached */
#define NET_DIRECT					0x800	/* Direct, i.e. crashmail (clo) */
#define NET_ROUTE					0x1000	/* Route this mail, i.e. normal mail (flo) */
#define NET_HOLD					0x2000	/* Hold mail for pickup (hlo) */


/* These macros are for twit-filter flags */

#define TF_TO						0x1		/* filter mail to name */
#define TF_FROM						0x2		/* filter mail from name */



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


struct msg
	{
	short msg_number;			   	/* message board number */
	char msg_areaname[41];			/* message area name */
	char msg_flags;					/* message area flags */
	unsigned char msg_readpriv;
	unsigned short msg_readflags; 	/* 1.05.02 */
	unsigned char msg_writepriv;
	unsigned short msg_writeflags;	/* 1.05.02 */
	unsigned char msg_sysoppriv;
	char msg_origin[66];			/* special origin line for this if echomail area */
	char msg_source;				/* source net/node for this netmail/echomail area */
	char msg_deleted;				/* true if deleted */
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


struct add
	{
	short add_zone;
	short add_net;
	short add_node;
	};


struct area
	{
	char *area_name;		/* name of echo area */
	short area_msgarea;		/* message area number or 0 if passthru */
	long area_tossed;		/* number of messages tossed into area */
	long area_forwarded; 	/* number of messages forwarded */
	long area_dups;			/* number of dups stopped */
	long area_sigs;			/* number of signatures */
	struct add area_source;	/* source address for this area for SEEN_BY and PATH lines! */
	struct add *area_add;	/* addresses of linked nodes */
	int area_cur_add;
	int area_max_add;
	};


struct nm
	{
	short nm_area;					/* area for netmail */
	short nm_zone;
	short nm_net;
	short nm_node;
	short nm_tossed;
	short nm_forwarded;
	};


struct fm				/* fidonet message format as of 1990 */
	{
	char fm_from[36];
	char fm_to[36];
	char fm_subject[72];
	char fm_datetime[20];
	short fm_read;			/* times read */
	short fm_dnode;			/* destination node */
	short fm_snode;			/* source node */
	short fm_cost;
	short fm_snet;			/* source net */
	short fm_dnet;			/* destination net */
	short fm_dzone;			/* destination zone */
	short fm_szone;			/* source node */
	short fm_dpoint;		/* destination point */
	short fm_spoint;		/* source node */
	short fm_prev;			/* reply to... */
	short fm_flags;
	short fm_next;
	};


struct pfm				/* packed fidonet message format */
	{
	char pfm_sig1;			/* must be 2 */
	char pfm_sig2;			/* must be 0 */
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
	char ph_sig1;			/* must be 2 */
	char ph_sig2;			/* must be 0 */
	short ph_snet;
	short ph_dnet;
	char ph_code;			/* product code */
	char ph_fill1;
	char ph_password[8];
	char ph_fill2[24];
	};


struct sb		/* seenby line structure */
	{
	short sb_net;
	short sb_node;
	};


struct p		/* path line structure */
	{
	short p_net;
	short p_node;
	};

	
struct m
	{
	char *m_body;
	short m_curbody;
	short m_maxbody;
	struct sb **m_seenby;
	short m_curseenby;
	short m_actualseenby;
	short m_maxseenby;
	struct p **m_path;
	short m_curpath;
	short m_actualpath;
	short m_maxpath;
	};


struct sh		/* signiture file header */
	{
	char sh_signature[21];	/* "SMAIL SIGNATURE FILE" */
	short sh_areas;			/* number of areas */
	};


struct sf
	{
	char *sf_file;			/* filename of signiture file */
	long sf_start;			/* offset in file of first signiture len/offset pair */
	long sf_prev;			/* next to last offset in file of last signiture len/offset */
	long sf_end;			/* last offset in file of last signiture len/offset */
	};


struct sigh		/* signature header */
	{
	short sigh_area;
	char sigh_mask[1024];			/* CRC mask */
	struct sf **sigh_files;
	short sigh_curfiles;
	short sigh_maxfiles;
	};


struct sig
	{
	unsigned short sig_crc1;		/* CRC of message body non-space characters */
	unsigned short sig_crc2;		/* CRC of to, from, subject fields */
	};


struct fi
	{
	char fi_name[256];
	DATE_T fi_date;					/* DOS style date YYYYYYY MMMM DDDDD where Y is YYYY - 1980 */
	TIME_T fi_time;					/* DOS style time HHHHH MMMMMM SSSSS where S is SSSS/2 */
	long fi_size;					/* Files size in bytes */
	};


struct toss
	{
	char *toss_to;
	char *toss_path;
	short toss_msgnum;
	short toss_total;
	};


struct pp				/* packet passwords */
	{
	short pp_zone;
	short pp_net;
	short pp_node;
	char *pp_password;
	};


struct tf				/* twit filter */
	{
	char *tf_name;
	short tf_flags;
	};




/* 3. Global Variables */

extern struct area **areas;
extern int cur_areas;
extern int max_areas;
extern int zone;
extern int net;
extern int node;
extern int keep_path;
extern int export_routed;
extern struct nm *netmail_areas;
extern int cur_netmail_areas;
extern int max_netmail_areas;
extern int netmail_tossed;
extern int netmail_forwarded;
extern int sysop_convert;
extern int message_limit;
extern int limit_flag;
extern int echosec_flag;
extern int quiet_flag;
extern int twit_flag;
extern FILE *headfd;
extern FILE *bodyfd;
extern FILE *linkfd;
extern FILE *datafd;
extern FILE *msgfd;
extern char *months[12];
extern char *body;
extern int cur_body;
extern int max_body;
extern struct cfg cfg;
extern char bbspath[100];
extern struct toss **tosslist;
extern int cur_tosslist;
extern int max_tosslist;

extern long pktoffset;
extern long headoffset;
extern long bodyoffset;
extern long linkoffset;
extern long dataoffset;

extern struct pp **passwords;
extern int cur_passwords;
extern int max_passwords;

extern struct tf **twits;
extern int cur_twits;
extern int max_twits;


/* 4. Function Declarations */

extern int comp_area(struct area **arg1,struct area **arg2);
extern int parse_address(char *string,int *zone,int *net,int *node);
extern void read_areas(FILE *fd);
extern void free_areas(void);
extern void read_config(FILE *fd);
extern void export_echo(void);
extern void export_net(void);
extern int write_body(char *text,int len,FILE *fd);
extern void import_mail(void);
extern void import_bad(void);
extern void unarc_packets(void);
extern unsigned int get_msg_number(char *path);
extern char *make_datetime(DATE_T date,TIME_T time);
extern void toss_bad(unsigned int *msgnum,struct msgh *tmsgh,char *area,FILE *fd);
extern int forward_msg(unsigned int *msgnum,struct msgh *tmsgh,char *area,struct m *tm);
extern void read_msg(struct m *tm,struct msgh *tmsgh,FILE *fd);
extern int import_message(struct msgh *tmsgh,struct m *tm);

extern void load_signatures(void);
extern void clean_signatures(int days);
extern void add_signature(int area,struct sig *sigptr);
extern struct sig *check_signature(int area,char *to,char *from,char *subject,char *body,int bodylen);
extern void free_signatures(void);
extern void report_traffic(void);

extern void load_datetime(DATE_T *dte,TIME_T *tme);
extern unsigned int date_to_julian(DATE_T dte);
extern DATE_T julian_to_date(unsigned int julian);
extern int juliancmp(unsigned int julian1,unsigned int julian2);
extern unsigned int next_julian(unsigned int start);
extern unsigned int prev_julian(unsigned int start);
#if defined(__WATCOMC__)
	extern char *_strdate(char *dstr);
	extern char *_strtime(char *tstr);
#endif

extern int pascal get_firstfile(struct fi *fi,char *mask);
extern int pascal get_nextfile(struct fi *fi);
extern void pascal get_closefile(void);

extern int check_toss(char *to);
extern void toss_message(struct toss *ttoss,struct msgh *tmsgh,char *area,FILE *sfd);

extern void read_passwords(FILE *fd);

extern void read_twits(FILE *fd);
extern void twit_message(struct msgh *tmsgh,struct m *tm);
