/* multchat.h
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 17 February 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/



/* 1. Constants and Macros */


#define MAJOR_VERSION				0
#define MINOR_VERSION				1

#define BETA						1		/* if TRUE, then version is in Beta! */


/* these macros are for MultiChat messages */

#define MC_NOTHING				0x0
#define MC_ERROR				0x1

#define MC_REQUEST				0x10
#define MC_PIPENAME				0x11
#define MC_CONNECT				0x12
#define MC_DISCONNECT			0x13

#define MC_LOGIN				0x20
#define MC_LOGOUT				0x21
#define MC_YELL					0x22
#define MC_ANSWER				0x23
#define MC_NOANSWER				0x24
#define MC_INACCESSIBLE			0x25
#define MC_NOTIME				0x26

#define MC_QUERY				0x30
#define MC_USERNAME				0x31

#define MC_TYPED				0x40
#define MC_SHOW					0x41
#define MC_ENDCHAT				0x42


#define BLACK						0
#define BLUE						1
#define GREEN						2
#define CYAN						3
#define RED							4
#define MAGENTA						5
#define BROWN						6
#define WHITE						7
#define BRIGHT						8


/* 2. Structures and Unions */

struct mcmsg
	{
	int mcmsg_type;			/* type of message */
	int mcmsg_pid;			/* PID of sender */
	int mcmsg_len;			/* length of message */
	char mcmsg_msg[100];	/* actual message */
	};


struct node
	{
	int node_pid;
	HPIPE node_pipe;
	char node_name[61];
	char node_user[41];
	char node_date[15];
	char node_time[6];
	int node_ansi;
	int node_chat;			/* pid of chatting node */
	int node_color;
	int node_last;
	};


/* 3. Global Variables */

extern HPIPE mainpipe;
extern volatile int quit;


/* 4. Function Declarations */

extern void wait(void);
