/* fforward.h
**
** Copyright (c) 1989, Christopher Laforet
** All Rights Reserved
**
** Started: 10 July 1989
**
** Revision Information:
**
** $Logfile:   E:/quickbbs/programs/fforward/vcs/fforward.h_v  $
** $Date:   10 Aug 1989 14:52:12  $
** $Revision:   0.5  $
**
*/



#define MAJOR_VERSION				1
#define MINOR_VERSION				0

#define FORWARD						256
#define CRASH						257
#define HOLD						258
#define MOVE						259
#define KILL						260


struct act
	{
	int act_type;
	char *act_name;	/* filename */
	int act_net;
	int act_node;
	};


struct clean
	{
	char *clean_name;
	int clean_flag;
	};


extern int mynet;
extern int mynode;
extern char logfile[100];
extern char inbound[100];
extern char outbound[100];
extern char holdpath[100];
extern char movepath[100];
extern struct act **actions;
extern int cur_actions;
extern int max_actions;
extern int clean_flag;


extern void read_config(char *config);
extern int copy(char *file,char *from,char *to);
extern void log(char *string);
extern void clean(void);
