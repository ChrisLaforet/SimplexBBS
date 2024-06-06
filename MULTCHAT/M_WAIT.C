/* m_wait.c
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


#define INCL_DOSERRORS
#define INCL_DOSNMPIPES
#define INCL_DOSPROCESS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <os2.h>
#include "multchat.h"


#define MAX_NODES			8


struct node node[MAX_NODES];
volatile int quit = 0;




char *get_color(struct node *tnode)
	{
	static char ansi[20];
	int count;

	if (!tnode->node_ansi || tnode->node_last)
		return "";
	strcpy(ansi,"[");
	if (!(tnode->node_color & BRIGHT))
		strcat(ansi,"0;");
	switch (tnode->node_color & 7)
		{
		case BLACK:
			strcat(ansi,"30");
			break;
		case RED:
			strcat(ansi,"31");
			break;
		case GREEN:
			strcat(ansi,"32");
			break;
		case BROWN:
			strcat(ansi,"33");
			break;
		case BLUE:
			strcat(ansi,"34");
			break;
		case MAGENTA:
			strcat(ansi,"35");
			break;
		case CYAN:
			strcat(ansi,"36");
			break;
		case WHITE:
			strcat(ansi,"37");
			break;
		}
	if (tnode->node_color & BRIGHT)
		strcat(ansi,";1");
	strcat(ansi,"m");
	if (!tnode->node_last)
		{
		for (count = 0; count < MAX_NODES; count++)
			{
			if (node[count].node_chat == tnode->node_pid)
				node[count].node_last = 0;
			}
		tnode->node_last = 1;
		}
	return ansi;
	}



void wait(void)
	{
	struct mcmsg tmsg;
	char buffer[100];
	char *cptr;
	char *cptr1;
	USHORT err;
	USHORT inbytes;
	USHORT outbytes;
	int count;
	int counter;
	int kount;
	int found;
	int which;
	int instring;
	int connected = 0;
	int pid;
int timer = 0;

	for (count = 0; count < MAX_NODES; count++)
		{
		node[count].node_pipe = (HFILE)-1;
		node[count].node_chat = 0;
		}
	node[0].node_color = CYAN | BRIGHT;
	node[1].node_color = GREEN | BRIGHT;
	node[2].node_color = BROWN | BRIGHT;
	node[3].node_color = WHITE;
	node[4].node_color = RED | BRIGHT;
	node[5].node_color = MAGENTA | BRIGHT;
	node[6].node_color = BLUE | BRIGHT;
	node[7].node_color = WHITE | BRIGHT;

	do
		{
		if (!connected)
			{
			if (!(err = DosConnectNmPipe(mainpipe)))
				connected = 1;
			else if (err == ERROR_BROKEN_PIPE)
				{
				fprintf(stderr,"Error: MultiChat main pipe is broken -- Exiting!\n");
				exit(1);
				}
			}
		else 
			{
			if (err = DosRead(mainpipe,&tmsg,sizeof(struct mcmsg),&inbytes))
				{
		 		if (err == ERROR_BROKEN_PIPE || err == ERROR_INVALID_HANDLE)
					{
					fprintf(stderr,"Error: MultiChat main pipe is broken -- Exiting!\n");
					exit(1);
					}
				}
			else if (inbytes && tmsg.mcmsg_type == MC_REQUEST)
				{
				fprintf(stderr,"Notice: Pipename request from PID %u\n",tmsg.mcmsg_pid);
				for (count = 0, found = 0; count < MAX_NODES; count++)
					{
					if (node[count].node_pipe == (HPIPE)-1)
						{
						found = 1;
						break;
						}
					}
				if (found)
					{
					node[count].node_pid = tmsg.mcmsg_pid;
					sprintf(buffer,"\\pipe\\mcht%04x",tmsg.mcmsg_pid);
					memset(&tmsg,0,sizeof(struct mcmsg));
					tmsg.mcmsg_type = MC_PIPENAME;
					tmsg.mcmsg_pid = 0;
					tmsg.mcmsg_len = strlen(buffer);
					strcpy(tmsg.mcmsg_msg,buffer);
					}
				else
					{
					memset(&tmsg,0,sizeof(struct mcmsg));
					tmsg.mcmsg_type = MC_ERROR;
					}
				DosWrite(mainpipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);		/* send back message */
				DosDisConnectNmPipe(mainpipe);
				connected = 0;
				if (found)
					{
					fprintf(stderr,"Connecting to pipe \"%s\"...\n",buffer);
					if (DosMakeNmPipe(buffer,&node[count].node_pipe,NP_ACCESS_DUPLEX | NP_NOINHERIT | NP_NOWRITEBEHIND,NP_NOWAIT | NP_READMODE_MESSAGE | NP_TYPE_MESSAGE | 1,20 * sizeof(struct mcmsg),20 * sizeof(struct mcmsg),0x1000L))
						{
						fprintf(stderr,"Error: Unable to create pipe \"%s\"!\n",buffer);
						node[count].node_pipe = (HFILE)-1;
						}
					else 
						DosConnectNmPipe(node[count].node_pipe);
					}
				}
			}
		for (count = 0; count < MAX_NODES; count++)
			{
			if (node[count].node_pipe != (HPIPE)-1)
				{
				if (err = DosRead(node[count].node_pipe,&tmsg,sizeof(struct mcmsg),&inbytes))
					{
		 			if (err == ERROR_BROKEN_PIPE || err == ERROR_INVALID_HANDLE)
						{
						fprintf(stderr,"Error: A MultiChat pipe is broken -- Disconnecting process!\n");
						DosDisConnectNmPipe(node[count].node_pipe);
						node[count].node_pipe = (HFILE)-1;
						}
					}
				else if (inbytes)
					{
					switch (tmsg.mcmsg_type)
						{
						case MC_ERROR:
							break;

						case MC_CONNECT:
							if (tmsg.mcmsg_len >= 100)
								tmsg.mcmsg_len = 100;
							tmsg.mcmsg_msg[tmsg.mcmsg_len] = '\0';		/* just in case */
							strncpy(node[count].node_name,tmsg.mcmsg_msg,61);
							node[count].node_user[0] = '\0';
							break;

						case MC_DISCONNECT:
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == node[count].node_chat)
									{
					  				memset(&tmsg,0,sizeof(struct mcmsg));		/* blank user name signs end of list */
					  				tmsg.mcmsg_type = MC_ENDCHAT;
			  						tmsg.mcmsg_pid = 0;
			  						tmsg.mcmsg_len = 0;
				  					DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									break;
									}
								}
							node[count].node_chat = 0;
							fprintf(stderr,"Disconnecting from pipe \"\\pipe\\mcht%04x\"...\n",node[count].node_pid);
							DosDisConnectNmPipe(node[count].node_pipe);
							DosClose(node[count].node_pipe);
							node[count].node_pipe = (HFILE)-1;
							break;

						case MC_LOGIN:
							if (tmsg.mcmsg_len >= 100)
								tmsg.mcmsg_len = 100;
							tmsg.mcmsg_msg[tmsg.mcmsg_len] = '\0';		/* just in case */
							cptr = tmsg.mcmsg_msg;
							cptr1 = buffer;
							which = 0;
							instring = 0;
							while (*cptr)
								{
								if (*cptr == '"')
									{
									if (instring)
										{
										++which;
										if (which != 1)
											{
											*cptr1++ = '\0';
											cptr1 = buffer;
											}
										else 
											*cptr1++ = ' ';
										switch (which)
											{
											case 2:
												strncpy(node[count].node_user,buffer,41);
												break;
											case 3:
												strncpy(node[count].node_date,buffer,15);
												break;
											case 4:
												strncpy(node[count].node_time,buffer,6);
												break;
											}
										}
									instring = instring ? 0 : 1;
									}
								else if (instring)
									*cptr1++ = *cptr;

								++cptr;
								}
							fprintf(stderr,"%s @ %s  %s logged in\n",node[count].node_date,node[count].node_time,node[count].node_user);
							fprintf(stderr,"                   on \"%s\" (PID 0x%04x).\n",node[count].node_name,node[count].node_pid);
							break;

						case MC_LOGOUT:
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == node[count].node_chat)
									{
					  				memset(&tmsg,0,sizeof(struct mcmsg));		/* blank user name signs end of list */
					  				tmsg.mcmsg_type = MC_ENDCHAT;
			  						tmsg.mcmsg_pid = 0;
			  						tmsg.mcmsg_len = 0;
				  					DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									break;
									}
								}
							node[count].node_chat = 0;
							node[count].node_user[0] = '\0';
							break;

						case MC_YELL:
							cptr = tmsg.mcmsg_msg;
							pid = atoi(cptr);
							while (*cptr && !isspace(*cptr))
								++cptr;
							while (*cptr && isspace(*cptr))
								++cptr;
							node[count].node_ansi = atoi(cptr);
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == pid)
									{
									memset(&tmsg,0,sizeof(struct mcmsg));
									tmsg.mcmsg_type = MC_YELL;
					  				tmsg.mcmsg_pid = node[count].node_pid;		/* send yeller's pid */
									strcpy(tmsg.mcmsg_msg,node[count].node_user);
									tmsg.mcmsg_len = strlen(tmsg.mcmsg_msg);
									DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									break;
									}
								}
							break;

						case MC_ANSWER:
							pid = atoi(tmsg.mcmsg_msg);
							found = 0;
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == pid)
									{
									node[kount].node_chat = tmsg.mcmsg_pid;
									node[kount].node_last = 0;
									node[count].node_chat = pid;
									node[count].node_last = 0;

									memset(&tmsg,0,sizeof(struct mcmsg));
									tmsg.mcmsg_type = MC_ANSWER;
					  				tmsg.mcmsg_pid = 0;
									tmsg.mcmsg_len = 0;
									DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									
									memset(&tmsg,0,sizeof(struct mcmsg));
									tmsg.mcmsg_type = MC_SHOW;
					  				tmsg.mcmsg_pid = 0;
									sprintf(tmsg.mcmsg_msg,"%sHello, %s is now online.\r\n",get_color(&node[count]),node[kount].node_user);
									tmsg.mcmsg_len = strlen(tmsg.mcmsg_msg);
									DosWrite(node[count].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);

									memset(&tmsg,0,sizeof(struct mcmsg));
									tmsg.mcmsg_type = MC_SHOW;
					  				tmsg.mcmsg_pid = 0;
									sprintf(tmsg.mcmsg_msg,"%sHello, %s is now online.\r\n",get_color(&node[kount]),node[count].node_user);
									tmsg.mcmsg_len = strlen(tmsg.mcmsg_msg);
									DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);


									found = 1;
									break;
									}
								}
							if (!found)
								{
								memset(&tmsg,0,sizeof(struct mcmsg));
								tmsg.mcmsg_type = MC_INACCESSIBLE;
				  				tmsg.mcmsg_pid = 0;
								strcpy(tmsg.mcmsg_msg,"Logout");
								tmsg.mcmsg_len = strlen(tmsg.mcmsg_msg);
								DosWrite(node[count].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
								}
							break;

						case MC_NOANSWER:
							pid = atoi(tmsg.mcmsg_msg);
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == pid)
									{
									memset(&tmsg,0,sizeof(struct mcmsg));
									tmsg.mcmsg_type = MC_NOANSWER;
					  				tmsg.mcmsg_pid = 0;
									tmsg.mcmsg_len = 0;
									DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									break;
									}
								}
							break;

						case MC_INACCESSIBLE:
							cptr = tmsg.mcmsg_msg;
							pid = atoi(cptr);
							while (*cptr && !isspace(*cptr))
								++cptr;
							while (*cptr && isspace(*cptr))
								++cptr;
							strcpy(buffer,cptr);
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == pid)
									{
									memset(&tmsg,0,sizeof(struct mcmsg));
									tmsg.mcmsg_type = MC_INACCESSIBLE;
					  				tmsg.mcmsg_pid = 0;
									strcpy(tmsg.mcmsg_msg,buffer);
									tmsg.mcmsg_len = strlen(tmsg.mcmsg_msg);
									DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									break;
									}
								}
							break;

						case MC_NOTIME:
							pid = atoi(tmsg.mcmsg_msg);
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == pid)
									{
									memset(&tmsg,0,sizeof(struct mcmsg));
									tmsg.mcmsg_type = MC_NOTIME;
					  				tmsg.mcmsg_pid = 0;
									tmsg.mcmsg_len = 0;
									DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									break;
									}
								}
							break;

						case MC_QUERY:
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (kount != count && node[kount].node_pipe != (HPIPE)-1)
									{
									memset(&tmsg,0,sizeof(struct mcmsg));
									tmsg.mcmsg_type = MC_USERNAME;
					  				tmsg.mcmsg_pid = node[kount].node_pid;	/* send destination PID */
									strcpy(tmsg.mcmsg_msg,node[kount].node_user);
									tmsg.mcmsg_len = strlen(tmsg.mcmsg_msg);
									DosWrite(node[count].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									}
								}
			  				memset(&tmsg,0,sizeof(struct mcmsg));		/* blank user name signs end of list */
			  				tmsg.mcmsg_type = MC_USERNAME;
			  				tmsg.mcmsg_pid = 0;
			  				tmsg.mcmsg_len = 0;
			  				DosWrite(node[count].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
							break;

						case MC_TYPED:
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == node[count].node_chat)
									{
									if (node[count].node_last)
										{
						  				tmsg.mcmsg_type = MC_SHOW;
				  						tmsg.mcmsg_pid = 0;
					  					DosWrite(node[count].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);

										tmsg.mcmsg_msg[tmsg.mcmsg_len] = '\0';
										sprintf(buffer,"%s%s",get_color(&node[kount]),tmsg.mcmsg_msg);
						  				memset(&tmsg,0,sizeof(struct mcmsg));		/* blank user name signs end of list */
						  				tmsg.mcmsg_type = MC_SHOW;
			  							tmsg.mcmsg_pid = 0;
										strcpy(tmsg.mcmsg_msg,buffer);
			  							tmsg.mcmsg_len = strlen(tmsg.mcmsg_msg);
				  						DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
										}
									else
										{
						  				tmsg.mcmsg_type = MC_SHOW;
				  						tmsg.mcmsg_pid = 0;
					  					DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);

										tmsg.mcmsg_msg[tmsg.mcmsg_len] = '\0';
										sprintf(buffer,"%s%s",get_color(&node[count]),tmsg.mcmsg_msg);
					  					memset(&tmsg,0,sizeof(struct mcmsg));		/* blank user name signs end of list */
					  					tmsg.mcmsg_type = MC_SHOW;
			  							tmsg.mcmsg_pid = 0;
										strcpy(tmsg.mcmsg_msg,buffer);
			  							tmsg.mcmsg_len = strlen(tmsg.mcmsg_msg);
				  						DosWrite(node[count].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
										}
									break;
									}
								}
							break;

						case MC_ENDCHAT:
							for (kount = 0; kount < MAX_NODES; kount++)
								{
								if (node[kount].node_pipe != (HPIPE)-1 && node[kount].node_pid == node[count].node_chat)
									{
					  				memset(&tmsg,0,sizeof(struct mcmsg));		/* blank user name signs end of list */
					  				tmsg.mcmsg_type = MC_ENDCHAT;
			  						tmsg.mcmsg_pid = 0;
			  						tmsg.mcmsg_len = 0;
				  					DosWrite(node[kount].node_pipe,(void *)&tmsg,sizeof(struct mcmsg),&outbytes);
									break;
									}
								}
							node[count].node_chat = 0;
							break;
						}
					}
				}
			}
		DosSleep(100L);
		}
	while (!quit);
	fprintf(stderr,"Exiting...\n");
	for (count = 0; count < MAX_NODES; count++)
		{
		if (node[count].node_pipe != (HPIPE)-1)
			{
			DosDisConnectNmPipe(node[count].node_pipe);
			DosClose(node[count].node_pipe);
			}
		}
	}
