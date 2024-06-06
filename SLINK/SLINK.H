/* slink.h
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 24 May 1990
**
** Revision Information: $Logfile:   G:/simplex/slink/vcs/slink.h_v  $
**                       $Date:   25 Oct 1992 14:17:26  $
**                       $Revision:   1.7  $
**
*/



#define MAJOR_VERSION				1
#define MINOR_VERSION				5


#define P_CSEP					(char)'\\'	/* character form of path separator */
#define P_SSEP					"\\"		/* string form of path separator */



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


struct mlink
	{
	short mlink_area;
	short mlink_number;				/* number of the message in that area! */
	short mlink_flags;				/* a copy of the message flags */
	unsigned short mlink_cksum;		/* a checksum on the uppercased TO: person's name */
	};


struct l
	{
	char *l_subject;
	short l_prev;
	};


extern FILE *areafd;
extern FILE *linkfd;
extern FILE *headfd;
extern int *msgs;
extern int total_msgs;



extern void build_links(int area);
