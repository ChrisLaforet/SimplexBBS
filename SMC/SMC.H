/* smc.h
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 December 1990
**
** Revision Information: $Logfile:   G:/simplex/smc/vcs/smc.h_v  $
**                       $Date:   25 Oct 1992 14:25:38  $
**                       $Revision:   1.7  $
**
*/



/* 1. Constants and Macros */


#define MAJOR_VERSION				1
#define MINOR_VERSION				7


#define P_CSEP						'\\'	/* character form of path separator */
#define P_SSEP						"\\"	/* string form of path separator */



#define NUM_MENUS					85


/* these macros access the menu type options */

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
#define MENU_CHGALIAS				0x4f		/* change aliases */

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
#define MENU_TIMELESS_YELL			0xf8		/* yell (unlimited time) for sysop */
#define MENU_PHONEDUP				0xf9		/* check for duplicate phone numbers - send message to sysop if match */
#define MENU_LOGOFF					0xff		/* hangup and log off */

#define M_SIGNATURE					0xa442		/* proves file is actual menu file */


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


/* color macros */

#define BLACK						0
#define BLUE						1
#define GREEN						2
#define CYAN						3
#define RED							4
#define MAGENTA						5
#define BROWN						6
#define WHITE						7
#define BRIGHT						8
#define BLINK						0x80
#define ON_BLACK					0x0
#define ON_BLUE						0x10
#define ON_GREEN					0x20
#define ON_CYAN						0x30
#define ON_RED						0x40
#define ON_MAGENTA					0x50
#define ON_BROWN					0x60
#define ON_WHITE					0x70


/* error types */

#define WARNING						0
#define FATAL						4


#define LINE_LEN					513


/* tokenizer returns */

#define IDLE						0			/* pseudo return for parser */
#define EOFILE						256
#define STRING						257
#define CONST						258
#define COMMENT						259
#define MENU						260
#define PROMPT						261
#define LINE						262
#define COLOR						263
#define HILITE						264
#define KEY							265
#define TYPE						266
#define AUTO						267
#define EXPERT						268
#define DATA						279
#define PRIV						270
#define FLAGS						271
#define IDENT						272
#define DEFINE						273



/* 2. Structures and Unions */

struct prompt
	{
	unsigned short prompt_sig;		/* signature for valid menu file 0xa442 */
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


struct m
	{
	unsigned char m_type;
	char *m_descrip;
	};


struct def
	{
	short def_type;
	char *def_name;
	char *def_define;
	};



/* 3. Global Variables */

extern char yyline[LINE_LEN];
extern char yytext[LINE_LEN];
extern FILE *yyfile;
extern int line;
extern struct m mtype[NUM_MENUS];


/* 4. Function Declarations */

extern void error(char *message,int strategy);
extern int yygettoken(void);
extern void parse_file(char *outpath);
extern void decompile(char *fname);
extern void add_define(int type,char *name,char *define);
extern struct def *get_define(char *name);
extern void free_define(void);
extern int xlat_flags(char *string);
