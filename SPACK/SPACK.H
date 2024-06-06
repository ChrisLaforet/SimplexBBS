/* spack.h
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 May 1990
**
** Revision Information: $Logfile:   G:/simplex/spack/vcs/spack.h_v  $
**                       $Date:   25 Oct 1992 14:27:12  $
**                       $Revision:   1.4  $
**
*/


/* 1. Constants and Macros */

#define MAJOR_VERSION				1
#define MINOR_VERSION				3


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

#define NET_KILLSENT				0x100	/* kill/sent netmail */
#define NET_SENT					0x200	/* netmail sent */
#define NET_FILEATTACH				0x400	/* file(s) attached */
#define NET_CRASH					0x800	/* crashmail */
#define NET_RECEIPT_REQ				0x1000	/* receipt requested */
#define NET_AUDIT_REQ				0x2000	/* audit request */
#define NET_RECEIPT					0x4000	/* return receipt */


typedef unsigned short DATE_T;
typedef unsigned short TIME_T;



/* 2. Structures and Unions */

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



/* 3. Global Variables */

extern char bbspath[100];			/* BBS message files area */
extern FILE *mheadfd;
extern FILE *mbodyfd;
extern FILE *mdatafd;
extern FILE *mreadfd;
extern FILE *mlinkfd;
extern FILE *bheadfd;
extern FILE *bbodyfd;
extern unsigned int valid_headers;
extern unsigned int deleted_headers;
extern unsigned long valid_bytes;
extern unsigned long deleted_bytes;
extern int inplace;
extern int keep;
extern int kill;
extern int quiet_flag;


/* 4. Function Declarations */

extern int analyse(void);
extern int do_pack(void);
