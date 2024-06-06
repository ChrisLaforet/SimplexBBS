/* sconfig.h
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 1 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/sconfig.h_v  $
**                       $Date:   25 Oct 1992 14:15:42  $
**                       $Revision:   1.0  $
**
*/


/* 1. Constants and Macros */

#define MAJOR_VERSION				1
#define MINOR_VERSION				1


#define P_CSEP						'\\'	/* character form of path separator */
#define P_SSEP						"\\"	/* string form of path separator */


#define CLOCK_POS					0x32
#define EXIT_MENU					(void (*)(void))0xffffffffL



/* configuration flags */

#define CFG_SLOWMODEM				0x1			/* modem is slow */
#define CFG_PRIVATEBBS				0x2			/* allow only private-preregistered bbs operation */
#define CFG_DISABLEKB				0x4			/* disable local keyboard */
#define CFG_LOCKBAUD				0x8			/* lock baudrate */
#define CFG_DIRECT					0x10		/* use direct screen writes */
#define CFG_SNOW					0x20		/* do snow checking on color */
#define CFG_CROSSAREA_DL			0x40		/* allow cross-area file downloading */


/* color macros */

#define COLOR_WHITE					0x0
#define COLOR_GREEN					0x1
#define COLOR_MAGENTA				0x2
#define COLOR_YELLOW				0x3
#define COLOR_CYAN					0x4

#define COLOR_ONWHITE				0x0
#define COLOR_ONBLUE				0x1
#define COLOR_ONRED					0x2
#define COLOR_ONBROWN				0x3
#define COLOR_ONCYAN				0x4


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


/* these macros are for user flags */

#define UF_A						0x1
#define UF_B						0x2
#define UF_C						0x4
#define UF_D						0x8
#define UF_E						0x10
#define UF_F						0x20
#define UF_G						0x40
#define UF_H						0x80
#define UF_I						0x100
#define UF_J						0x200
#define UF_K						0x400
#define UF_L						0x800
#define UF_M						0x1000
#define UF_N						0x2000
#define UF_O						0x4000
#define UF_P						0x8000


/* these macros access the menu type options */

#define NUM_MENUS					85


#define MENU_SHOW					0x0			/* show menu line only */
#define MENU_CALL					0x1			/* call next menu */
#define MENU_GOTO					0x2			/* goto next menu - no return */
#define MENU_RET					0x3			/* return to previous menu */
#define MENU_REPLACE				0x4			/* goto next menu - replace current menu on stack */

#define MENU_ENTER					0x10		/* enter a message */
#define MENU_READ					0x11		/* read a message */
#define MENU_SCAN					0x12		/* scan messages */
#define MENU_QSCAN					0x13		/* quickscan messages */
#define MENU_USERLIST				0x14		/* search/display users list */
#define MENU_NETSEARCH				0x15		/* search for netmail address */
#define MENU_FREQ					0x16		/* generate File Request to netmail node */

#define MENU_LIST					0x20		/* list filelist.bbs listing */
#define MENU_UPLOAD					0x21		/* upload files to area */
#define MENU_DOWNLOAD				0x22		/* download files from area */
#define MENU_RAW					0x23		/* raw directory list in area */
#define MENU_CONTENTS				0x24		/* view archive contents */
#define MENU_NEW					0x25		/* show new files on system */
#define MENU_SRCHNAME				0x26		/* search for file name on system */
#define MENU_SRCHKEY				0x27		/* search for keyword on system */
#define MENU_KILL					0x28		/* kill/delete file in area */
#define MENU_READFILE				0x29		/* read a textfile in file area */
#define MENU_DOWNLOAD_FILE			0x2a		/* download 1 file from area */
#define MENU_MOVEFILES				0x2b		/* move files to another area */
#define MENU_REVLIST				0x2c		/* list filelist.bbs listing in reverse order */

#define MENU_SHOWFILE				0x30		/* show an ansi/ascii file */
#define MENU_SHOWHOT				0x31		/* show an ansi/ascii file with active keys */
#define MENU_SHOWWAIT				0x32		/* show an ansi/ascii file with wait for Enter */
#define MENU_QUESTION				0x33		/* show a questionaire file */
#define MENU_QUOTE					0x34		/* show the quote of the moment */
#define MENU_SHOWHOT_NOEXPERT		0x35		/* show an ansi/ascii file with active keys IF expert mode is off */

#define MENU_CITY					0x40		/* change users city/state */
#define MENU_PASSWORD				0x41		/* change users password */
#define MENU_CLS					0x42		/* toggle users cls state */
#define MENU_MORE					0x43		/* toggle users more state */
#define MENU_SCREENLEN				0x44		/* change users screen len */
#define MENU_ANSI					0x45		/* toggle ansi on/off */
#define MENU_EDITOR					0x46		/* toggle between line and full editor */
#define MENU_EXPERT					0x47		/* toggle between novice and expert mode */
#define MENU_HOMEPHONE				0x48		/* change home phone number */
#define MENU_DATAPHONE				0x49		/* change data phone number */
#define MENU_CHGADD1				0x4a		/* change full address line 1 */
#define MENU_CHGADD2				0x4b		/* change full address line 2 */
#define MENU_CHGCITY				0x4c		/* change full address city */
#define MENU_CHGSTATE				0x4d		/* change full address state */
#define MENU_CHGZIP					0x4e		/* change full address zip */
#define MENU_CHGALIAS				0x4f		/* change registered aliases (1.07.00) */

#define MENU_ADENTER				0x50		/* enter an advertisement */
#define MENU_ADREAD					0x51		/* read advertisements */
#define MENU_ADSCAN					0x52		/* scan advertisments */

#define MENU_NEWPRIV				0x60		/* set a new privilege */
#define MENU_UPTIME					0x61		/* raise the time of the user x minutes */
#define MENU_DOWNTIME				0x62		/* drop the time of the user x minutes */
#define MENU_ADDFLAGS				0x63		/* add flags to user's flags */
#define MENU_DELFLAGS				0x64		/* delete flags to user's flags */
#define MENU_SETTIME				0x65		/* set the time of the user to x minutes */
#define MENU_LOGENTRY				0x66		/* log data line to sysop log */
#define MENU_SYSOPALERT				0x67		/* beep tones to alert sysop of entry to menu */

#define MENU_SETCOMB				0x70		/* set up combined boards */
#define MENU_READCOMB				0x71		/* read a message in comb boards */
#define MENU_SCANCOMB 				0x72		/* scan messages in comb boards */
#define MENU_QSCANCOMB				0x73		/* quickscan messages in comb boards */
#define MENU_DLCOMB					0x74		/* download messages in comb boards */
#define MENU_CFGCOMB				0x75		/* configure combined boards */
#define MENU_ULCOMB					0x76		/* upload messages in .QWK offline reader format */

#define MENU_XMITXMODEM				0x80		/* send files using Xmodem */
#define MENU_XMITXMODEM1K			0x81		/* send files using Xmodem-1K */
#define MENU_XMITYMODEM				0x82		/* send files using Ymodem */
#define MENU_XMITYMODEMG			0x83		/* send files using Ymodem-G */
#define MENU_XMITZMODEM				0x84		/* send files using Zmodem */

#define MENU_RECVXMODEM				0x90		/* recv files using Xmodem */
#define MENU_RECVXMODEM1K			0x91		/* recv files using Xmodem-1K */
#define MENU_RECVYMODEM				0x92		/* recv files using Ymodem */
#define MENU_RECVYMODEMG			0x93		/* recv files using Ymodem-G */
#define MENU_RECVZMODEM				0x94		/* recv files using Zmodem */

#define MENU_RUN					0xf0		/* run a program from BBS */
#define MENU_VERS					0xf2		/* show BBS version info */
#define MENU_USAGE					0xf3		/* show users time usage and dl/ul info */
#define MENU_YELL					0xf4		/* yell for sysop */
#define MENU_CLOCK					0xf5		/* show clock for time synchronizing */
#define MENU_CHECKMAIL				0xf6		/* check for mail addressed to user */
#define MENU_USERUPGRADE			0xf7		/* sysop's user-upgrade option */
#define MENU_TIMELESS_YELL 			0xf8		/* yell at any time for sysop */
#define MENU_PHONEDUP				0xf9		/* check for duplicate phone numbers - send message to sysop if match */
#define MENU_LOGOFF					0xff		/* hangup and log off */


/* these macros are for message board flags */

#define MSG_LOCAL_FILEATTACH		0x0
#define MSG_LOCAL					0x1
#define MSG_ECHO					0x2
#define MSG_NET						0x4
#define MSG_PRIVATE					0x8			/* messages are private */
#define MSG_PUBLIC					0x10		/* messages are public */
#define MSG_ALIAS					0x20		/* aliases are allowed */


#define M_SIGNATURE					0xa442		/* proves file is actual menu file */


typedef unsigned int DATE_T;
typedef unsigned int TIME_T;



/* 2. Structures and Unions */

struct prompt
	{
	short prompt_sig; 				/* signiture for valid menu file 0xa442 */
	char prompt_color;
	char prompt_hilite;
	char prompt_string[56];			/* string to show */
	};


struct menu
	{
	short menu_number;				/* line number */
	unsigned char menu_type;		/* type of choice */
	char menu_key;					/* key to initiate choice */
	unsigned char menu_priv;		/* user priv to see/use choice */
	unsigned short menu_flags;		/* v 1.02 added flags to access option */
	char menu_auto;					/* automatic execution if true */
	char menu_color;				/* color of choice */
	char menu_hilite;				/* color of hilight */
	char menu_expert;				/* if true show when expert mode is on */
	char menu_line[71];				/* line to show on screen */
	char menu_data[71]; 			/* data line for menu option */
	char menu_deleted;
	};


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


struct ut				/* 1.07 - users time structure */
	{
	unsigned char ut_priv;
	short ut_time;
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


struct evt
	{
	char evt_enabled;				/* true if enabled */
	char evt_forced;				/* true if forced event */
	char evt_days;					/* 0-S-M-T-W-T-F-S */
	unsigned short evt_time;		/* encoded time to run */
	char evt_errorlevel;
	char evt_title[21];				/* optional name for event */
	char evt_deleted;				/* true if deleted */
	};


struct mtype
	{
	char mtype_type;
	char *mtype_name;
	char *mtype_help;
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


struct usr
	{
	char *usr_name;
	char *usr_city;
	char usr_deleted;
	unsigned char usr_priv;
	int usr_flags;
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


struct ccfg				/* color configuration for SCONFIG */
	{
	short ccfg_color[7];
	short ccfg_snow;
	};


struct mnu
	{
	char *mnu_string; 				/* string for menu item */
	char mnu_shortcut;				/* shortcut character */
	char *mnu_help;					/* a simple help string */
	int (*mnu_function)(void); 		/* function returns 1 if needs to exit */
	};




/* 3. Global Variables */

extern char cfgpath[257];
extern FILE *ccfgfd;
extern FILE *cfgfd;
extern FILE *evtfd;
extern FILE *msgfd;
extern FILE *filefd;
extern FILE *userfd;
extern int color[7];
extern int master_change;
extern struct cfg cfg;
extern struct msg **msgs;
extern int cur_msgs;
extern int max_msgs;
extern struct file **files;
extern int cur_files;
extern int max_files;
extern struct evt **evts;
extern int cur_evts;
extern int max_evts;
extern struct usr **usrs;
extern int cur_usrs;
extern int max_usrs;
extern struct ut **uts;
extern int cur_uts;
extern int max_uts;

extern int color_change_flag;

extern struct ccfg ccfg;


/* 4. Function Declarations */

extern DATE_T get_cdate(void);
extern int check_date(char *date);
extern DATE_T atodate(char *date);
extern char *datetoa(DATE_T date);

extern void main_menu(void);

extern void push_menu(int tlc,int hicolor,int locolor,char *title,struct mnu *choices,int max_choices);

extern void fill_flags(int cursor,int flags,int color);
extern int edit_flags(int cursor,int *flags,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color);
extern void fill_date(int cursor,DATE_T date,int color);
extern int edit_date(int cursor,char *buffer,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color);
extern int edit_number(int cursor,int hicolor,int locolor,char *buffer,int minimum,int maximum,int clock_flag,int clock_pos,int clock_color);
extern int file_comp(struct file **arg1,struct file **arg2);
extern int msg_comp(struct msg **arg1,struct msg **arg2);
extern int get_anychar_first(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action);
extern int get_filename(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action);
extern int get_fileext(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action);
extern int get_pathname(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action);
extern int get_anychar(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action);
extern char *parse_color(int color);
extern int select_color(char *title,int *pcolor);

extern void edit_general_info(void);

extern void edit_miscellaneous_info(void);

extern void edit_modem_info(void);

extern void edit_general_paths(void);

extern void edit_network_paths(void);

extern void edit_archiver_paths(void);

extern void edit_unarchiver_paths(void);

extern void edit_msg_boards(void);

extern void fedit_list(struct file *tfile);

extern void draw_file_list(void);
extern int find_file_list(void);
extern int next_file_list(int start);
extern void edit_file_boards(void);

extern void edit_file_lists(void);

extern void edit_user_list(void);

extern void edit_events(void);

extern int edit_menu(char *filename);

extern void edit_menufile(void);
extern void load_menu(FILE *fd,char *filename);
extern void show_screen(struct prompt *tprompt,char *filename,int level,int flags);

extern int cfg_color(char *title,int *pcolor);
extern void draw_config(void);
extern void fill_config(struct ccfg *tccfg);
extern void configure_colors(void);

extern void edit_times(void);
