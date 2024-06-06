/* smailfix.h
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 20 September 1990
**
** Revision Information: $Logfile:   G:/simplex/smailfix/vcs/smailfix.h_v  $
**                       $Date:   25 Oct 1992 14:24:48  $
**                       $Revision:   1.8  $
**
*/


#define MAJOR_VERSION				1
#define MINOR_VERSION				9


#define P_CSEP						(char)'\\'	/* character form of path separator */
#define P_SSEP						"\\"		/* string form of path separator */



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


