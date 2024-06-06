/* f_config.c
**
** Copyright (c) 1989, Christopher Laforet
** All Rights Reserved
**
** Started: 10 July 1989
**
** Revision Information:
**
** $Logfile:   E:/quickbbs/programs/fforward/vcs/f_config.c_v  $
** $Date:   10 Aug 1989 14:51:58  $
** $Revision:   0.5  $
**
*/


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "fforward.h"



/* Configuration file:
**
** MYNET
** MYNODE
** LOGFILE
** INBOUND
** OUTBOUND
** HOLDPATH
** MOVEPATH
**
** CLEAN	- remove all holding files not in .HLO .FLO or .CLO files
**			  before forwarding new files.
**
** FORWARD	- forward to node
** CRASH	- forward to node with crashmail
** HOLD		- hold for pickup by node
**
** MOVE
** KILL
*/


int mynet = 0xffff;
int mynode = 0xffff;
char logfile[100] = "";
char inbound[100] = "";
char outbound[100] = "";
char holdpath[100] = "";
char movepath[100] = "";
struct act **actions = NULL;
int cur_actions = 0;
int max_actions = 0;
int clean_flag = 0;


void read_config(char *config)
	{
	FILE *cfd;
	char buffer[257];
	char *cptr;
	char *cptr1;
	int holdflag = 0;
	int moveflag = 0;
	int net;
	int node;

	if (cfd = fopen(config,"rb"))
		{
		while (fgets(buffer,sizeof(buffer),cfd))
			{
			if (*buffer != ';' && *buffer != '\r' && *buffer != '\n') 	/* comment character */
				{
				cptr = strtok(buffer," \n\r");
				if (!stricmp(cptr,"mynet"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					net = atoi(cptr);
					}
				else if (!stricmp(cptr,"mynode"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					node = atoi(cptr);
					}
				else if (!stricmp(cptr,"logfile"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					strcpy(logfile,cptr);
					}
				else if (!stricmp(cptr,"inbound"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					strcpy(inbound,cptr);
					}
				else if (!stricmp(cptr,"outbound"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					strcpy(outbound,cptr);
					}
				else if (!stricmp(cptr,"holdpath"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					strcpy(holdpath,cptr);
					}
				else if (!stricmp(cptr,"movepath"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					strcpy(movepath,cptr);
					}
				else if (!stricmp(cptr,"clean"))
					{
					clean_flag = 1;
					}
				else if (!stricmp(cptr,"forward"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					if (!(cptr1 = calloc(strlen(cptr) + 1,sizeof(char))))
						{
						fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
						exit(-1);
						}
					strcpy(cptr1,cptr);
					cptr = strtok(NULL,"\n\r");
					while (cptr && *cptr)
						{
						net = 0xffff;
						node = 0xffff;
						while (*cptr && *cptr == ' ' && *cptr == '\t')
							{
							++cptr;
							}
						net = atoi(cptr);
						while (*cptr && *cptr != '/')
							{
							++cptr;
							}
						if (*cptr == '/')
							{
							++cptr;
							}
						node = atoi(cptr);
						while (*cptr && isdigit(*cptr))
							{
							++cptr;
							}
						if (net != 0xffff && node != 0xffff)
							{
							if (cur_actions <= max_actions)
								{
								if (!(actions = realloc(actions,(max_actions += 10) * sizeof(struct act *))))
									{
									fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
									exit(-1);
									}
								}
							if (!(actions[cur_actions] = calloc(1,sizeof(struct act))))
								{
								fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
								exit(-1);
								}
							actions[cur_actions]->act_type = FORWARD;
							actions[cur_actions]->act_name = cptr1;
							actions[cur_actions]->act_net = net;
							actions[cur_actions]->act_node = node;
							holdflag = 1;
							++cur_actions;
							}
						}	
					}
				else if (!stricmp(cptr,"crash"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					if (!(cptr1 = calloc(strlen(cptr) + 1,sizeof(char))))
						{
						fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
						exit(-1);
						}
					strcpy(cptr1,cptr);
					cptr = strtok(NULL,"\n\r");
					while (cptr && *cptr)
						{
						net = 0xffff;
						node = 0xffff;
						while (*cptr && *cptr == ' ' && *cptr == '\t')
							{
							++cptr;
							}
						net = atoi(cptr);
						while (*cptr && *cptr != '/')
							{
							++cptr;
							}
						if (*cptr == '/')
							{
							++cptr;
							}
						node = atoi(cptr);
						while (*cptr && isdigit(*cptr))
							{
							++cptr;
							}
						if (net != 0xffff && node != 0xffff)
							{
							if (cur_actions <= max_actions)
								{
								if (!(actions = realloc(actions,(max_actions += 10) * sizeof(struct act *))))
									{
									fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
									exit(-1);
									}
								}
							if (!(actions[cur_actions] = calloc(1,sizeof(struct act))))
								{
								fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
								exit(-1);
								}
							actions[cur_actions]->act_type = CRASH;
							actions[cur_actions]->act_name = cptr1;
							actions[cur_actions]->act_net = net;
							actions[cur_actions]->act_node = node;
							holdflag = 1;
							++cur_actions;
							}
						}	
					}
				else if (!stricmp(cptr,"hold"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					if (!(cptr1 = calloc(strlen(cptr) + 1,sizeof(char))))
						{
						fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
						exit(-1);
						}
					strcpy(cptr1,cptr);
					cptr = strtok(NULL,"\n\r");
					while (cptr && *cptr)
						{
						net = 0xffff;
						node = 0xffff;
						while (*cptr && *cptr == ' ' && *cptr == '\t')
							{
							++cptr;
							}
						net = atoi(cptr);
						while (*cptr && *cptr != '/')
							{
							++cptr;
							}
						if (*cptr == '/')
							{
							++cptr;
							}
						node = atoi(cptr);
						while (*cptr && isdigit(*cptr))
							{
							++cptr;
							}
						if (net != 0xffff && node != 0xffff)
							{
							if (cur_actions <= max_actions)
								{
								if (!(actions = realloc(actions,(max_actions += 10) * sizeof(struct act *))))
									{
									fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
									exit(-1);
									}
								}
							if (!(actions[cur_actions] = calloc(1,sizeof(struct act))))
								{
								fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
								exit(-1);
								}
							actions[cur_actions]->act_type = HOLD;
							actions[cur_actions]->act_name = cptr1;
							actions[cur_actions]->act_net = net;
							actions[cur_actions]->act_node = node;
							holdflag = 1;
							++cur_actions;
							}
						}	
					}
				else if (!stricmp(cptr,"move"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ' && *cptr == '\t')
						{
						++cptr;
						}
					if (!(cptr1 = calloc(strlen(cptr) + 1,sizeof(char))))
						{
						fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
						exit(-1);
						}
					strcpy(cptr1,cptr);
					if (cur_actions <= max_actions)
						{
						if (!(actions = realloc(actions,(max_actions += 10) * sizeof(struct act *))))
							{
							fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
							exit(-1);
							}
						}
					if (!(actions[cur_actions] = calloc(1,sizeof(struct act))))
						{
						fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
						exit(-1);
						}
					actions[cur_actions]->act_type = MOVE;
					actions[cur_actions]->act_name = cptr1;
					++cur_actions;
					moveflag = 1;
					}
				else if (!stricmp(cptr,"kill"))
					{
					cptr = strtok(NULL," \t\n\r");
					while (*cptr && *cptr == ' ')
						{
						++cptr;
						}
					if (!(cptr1 = calloc(strlen(cptr) + 1,sizeof(char))))
						{
						fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
						exit(-1);
						}
					strcpy(cptr1,cptr);
					if (cur_actions <= max_actions)
						{
						if (!(actions = realloc(actions,(max_actions += 10) * sizeof(struct act *))))
							{
							fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
							exit(-1);
							}
						}
					if (!(actions[cur_actions] = calloc(1,sizeof(struct act))))
						{
						fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
						exit(-1);
						}
					actions[cur_actions]->act_type = KILL;
					actions[cur_actions]->act_name = cptr1;
					++cur_actions;
					}
				else
					{
					fprintf(stderr,"\aError: Ignoring unknown action \"%s\"!\n",cptr);
					}
				}
			}
		fclose(cfd);
		}
	else
		{
		fprintf(stderr,"\a\a\aFatal Error: Unable to open config file!\n\n");
		exit(-1);
		}
	if (net == 0xffff || node == 0xffff)
		{
		fprintf(stderr,"\a\a\aFatal Error: Missing Net/Node number!\n\n");
		exit(-1);
		}
	if (!strlen(logfile))
		{
		fprintf(stderr,"\a\a\aFatal Error: No log file provided!\n\n");
		exit(-1);
		}
	if (!strlen(outbound))
		{
		fprintf(stderr,"\a\a\aFatal Error: No outbound path provided!\n\n");
		exit(-1);
		}
    if (!strlen(inbound))
		{
		fprintf(stderr,"\a\a\aFatal Error: No inbound path provided!\n\n");
		exit(-1);
		}
    if (!strlen(holdpath) && holdflag)
		{
		fprintf(stderr,"\a\a\aFatal Error: Forward instructions and no hold path provided!\n\n");
		exit(-1);
		}
    if (!strlen(movepath) && moveflag)
		{
		fprintf(stderr,"\a\a\aFatal Error: Move instructions and no move path provided!\n\n");
		exit(-1);
		}
    }
