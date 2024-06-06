/* s_action.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 17 November 1990
**
** Revision Information: $Logfile:   G:/simplex/smp/vcs/s_action.c_v  $
**                       $Date:   25 Oct 1992 14:26:24  $
**                       $Revision:   1.10  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "smp.h"



struct act **actions = NULL;
int cur_actions = 0;
int max_actions = 0;



char *parse_string(char *string,char **next)
	{
	char *cptr;
	char *cptr1;

	if (!string)
		return NULL;

	cptr = string;
	if (*cptr == (char)'\0')
		return NULL;

	while (*cptr && *cptr <= (char)' ')
		++cptr;
	cptr1 = cptr;
	while (*cptr1 && *cptr1 > (char)' ')
		++cptr1;
	if (*cptr1)
		*next = cptr1 + 1;
	else
		*next = cptr1;
	*cptr1 = (char)'\0';
	return cptr;
	}



int parse_address(char *string,int *zone,int *net,int *node)
	{
	char buffer[10];
	char *cptr;
	char *cptr1;
	long tval;
	int rtn = 0;

	*zone = 0;
	*net = 0;
	*node = 0;
	if (isalnum(*string))
		{
		cptr = string;
		cptr1 = buffer;
		while (*cptr && isalnum(*cptr) && cptr1 < (buffer + 5))
			*cptr1++ = *cptr++;
		*cptr1 = (char)'\0';
		if (*cptr == (char)':')
			{
			if (!strnicmp(buffer,"all",3))
				tval = (long)(unsigned int)ALL;
			else if (isdigit(buffer[0]))
				tval = atol(buffer);
			else
				tval = 0x10000L;
			if (tval <= 0xffffL)
				{
				*zone = (unsigned int)tval;
				rtn = 3;
				++cptr;
				}
			}
		else if (*cptr == (char)'/')
			{
			if (!strnicmp(buffer,"all",3))
				tval = (long)(unsigned int)ALL;
			else if (isdigit(buffer[0]))
				tval = atol(buffer);
			else
				tval = 0x10000L;
			if (tval <= 0xffffL)
				{
				*net = (unsigned int)tval;
				rtn = 2;
				++cptr;
				}
			}
		else if (*cptr <= (char)' ')
			{
			if (!strnicmp(buffer,"all",3))
				tval = (long)(unsigned int)ALL;
			else if (isdigit(buffer[0]))
				tval = atol(buffer);
			else
				tval = 0x10000L;
			if (tval <= 0xffffL)
				{
				*node = (unsigned int)tval;
				rtn = 1;
				++cptr;
				}
			}
		if (rtn > 1)
			{
			cptr1 = buffer;
			while (*cptr && isalnum(*cptr) && cptr1 < (buffer + 5))
				*cptr1++ = *cptr++;
			*cptr1 = (char)'\0';
			if (*cptr == (char)'/' && rtn == 3)
				{
				if (!strnicmp(buffer,"all",3))
					tval = (long)(unsigned int)ALL;
				else if (isdigit(buffer[0]))
					tval = atol(buffer);
				else
					tval = 0x10000L;
				if (tval <= 0xffffL)
					{
					*net = (unsigned int)tval;
					rtn = 2;
					++cptr;
					}
				}
			else if (*cptr <= (char)' ' && rtn == 2)
				{
				if (!strnicmp(buffer,"all",3))
					tval = (long)(unsigned int)ALL;
				else if (isdigit(buffer[0]))
					tval = atol(buffer);
				else
					tval = 0x10000L;
				if (tval <= 0xffffL)
					{
					*node = (unsigned int)tval;
					rtn = 1;
					++cptr;
					}
				}
			if (rtn > 1)
				{
				cptr1 = buffer;
				while (*cptr && isalnum(*cptr) && cptr1 < (buffer + 5))
					*cptr1++ = *cptr++;
				*cptr1 = (char)'\0';
				if (*cptr <= (char)' ' && rtn == 2)
					{
					if (!strnicmp(buffer,"all",3))
						tval = (long)(unsigned int)ALL;
					else if (isdigit(buffer[0]))
						tval = atol(buffer);
					else
						tval = 0x10000L;
					if (tval <= 0xffffL)
						{
						*node = (unsigned int)tval;
						rtn = 1;
						++cptr;
						}
					}
				}
			}
		}
	return rtn;
	}



void add_action(int type,int tzone,int tnet,int tnode,int dzone,int dnet,int dnode)
	{
	int count;

	if (!tzone)
		tzone = zone;
	if (!tnet)
		tnet = net;
	if (!dzone)
		dzone = zone;
	if (!dnet)
		dnet = net;
	if (cur_actions >= max_actions)
		{
		if (!(actions = realloc(actions,(max_actions += 20) * sizeof(struct act *))))
			{
			printf("Fatal Error:  Out of memory to allocate action structures.\n");
			exit(1);
			}
		}
	if (!(actions[cur_actions] = (struct act *)malloc(sizeof(struct act))))
		{
		for (count = 0; count < cur_actions; count++)	/* free memory before crapping out */
			free(actions[count]);
		free(actions);
		printf("Fatal Error:  Out of memory to allocate action structures.\n");
		exit(1);
		}
	actions[cur_actions]->act_type = type;
	actions[cur_actions]->act_zone = tzone;
	actions[cur_actions]->act_net = tnet;
	actions[cur_actions]->act_node = tnode;
	actions[cur_actions]->act_dzone = dzone;	/* for some commands these are dummy values */
	actions[cur_actions]->act_dnet = dnet;
	actions[cur_actions]->act_dnode = dnode;
	++cur_actions;
	}



void load_actions(FILE *fd,int sched)
	{
	char buffer[512];
	char *next;
	char *cptr;
	int cur_sched = 0;
	int tzone;
	int tnet;
	int tnode;
	int dzone;
	int dnet;
	int dnode;
	int skip;
	int type;

	printf("Loading actions from smp.ctl....\n");
	while (fgets(buffer,sizeof(buffer),fd))
		{
		if (buffer[0] != (char)';')		/* not a comment line */
			{
			skip = 0;
			cptr = parse_string(buffer,&next);
			if (cptr && *cptr)
				{
				if (!stricmp(cptr,"SCHED"))
					{
					if (cptr = parse_string(next,&next))
						if (isalpha(*cptr))
							cur_sched = (int)toupper(*cptr);
					++skip;
					}
				}
			if (cur_sched == sched && !skip)
				{
				if (cptr && *cptr)
					{
					type = - 1;

					/**** the LEAVE* commands ****/
					if (!stricmp(cptr,"LEAVEHOLD"))
						type = LEAVEHOLD;
					else if (!stricmp(cptr,"LEAVEROUTE"))
						type = LEAVEROUTE;
					else if (!stricmp(cptr,"LEAVEDIRECT"))
						type = LEAVEDIRECT;

					/**** the SEND* commands ****/
					else if (!stricmp(cptr,"SENDHOLD"))
						type = SENDHOLD;
					else if (!stricmp(cptr,"SENDROUTE"))
						type = SENDROUTE;
					else if (!stricmp(cptr,"SENDDIRECT"))
						type = SENDDIRECT;

					/**** the POLL command ****/
					else if (!stricmp(cptr,"POLL"))
						type = POLL;

					/**** the HOST* command ****/
					else if (!stricmp(cptr,"HOSTHOLD"))
						type = HOSTHOLD;
					else if (!stricmp(cptr,"HOSTROUTE"))
						type = HOSTROUTE;
					else if (!stricmp(cptr,"HOSTDIRECT"))
						type = HOSTDIRECT;

					/**** the HOLD* commands ****/
					else if (!stricmp(cptr,"HOLDROUTE"))
						type = HOLDROUTE;
					else if (!stricmp(cptr,"HOLDDIRECT"))
						type = HOLDDIRECT;

					/**** the ROUTE* commands ****/
					else if (!stricmp(cptr,"ROUTEHOLD"))
						type = ROUTEHOLD;
					else if (!stricmp(cptr,"ROUTEDIRECT"))
						type = ROUTEDIRECT;

					/**** the DIRECT* command ****/
					else if (!stricmp(cptr,"DIRECTHOLD"))
						type = DIRECTHOLD;
					else if (!stricmp(cptr,"DIRECTROUTE"))
						type = DIRECTROUTE;

					/**** the *TO commands */
					else if (!stricmp(cptr,"HOLDTO"))
						type = HOLDTO;
					else if (!stricmp(cptr,"ROUTETO"))
						type = ROUTETO;
					else if (!stricmp(cptr,"DIRECTTO"))
						type = DIRECTTO;

					/**** the ARC* commands ****/
					else if (!stricmp(cptr,"ARCHOLD"))
						type = ARCHOLD;
					else if (!stricmp(cptr,"ARCROUTE"))
						type = ARCROUTE;
					else if (!stricmp(cptr,"ARCDIRECT"))
						type = ARCDIRECT;

					/**** the ZIP* commands ****/
					else if (!stricmp(cptr,"ZIPHOLD"))
						type = ZIPHOLD;
					else if (!stricmp(cptr,"ZIPROUTE"))
						type = ZIPROUTE;
					else if (!stricmp(cptr,"ZIPDIRECT"))
						type = ZIPDIRECT;

					/**** the LZH* commands ****/
					else if (!stricmp(cptr,"LZHHOLD"))
						type = LZHHOLD;
					else if (!stricmp(cptr,"LZHROUTE"))
						type = LZHROUTE;
					else if (!stricmp(cptr,"LZHDIRECT"))
						type = LZHDIRECT;

					/**** the ARJ* commands ****/
					else if (!stricmp(cptr,"ARJHOLD"))
						type = ARJHOLD;
					else if (!stricmp(cptr,"ARJROUTE"))
						type = ARJROUTE;
					else if (!stricmp(cptr,"ARJDIRECT"))
						type = ARJDIRECT;

					/**** the ZOO* commands ****/
					else if (!stricmp(cptr,"ZOOHOLD"))
						type = ZOOHOLD;
					else if (!stricmp(cptr,"ZOOROUTE"))
						type = ZOOROUTE;
					else if (!stricmp(cptr,"ZOODIRECT"))
						type = ZOODIRECT;

					switch (type)
						{
						case LEAVEHOLD:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(LEAVEHOLD,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case LEAVEROUTE:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(LEAVEROUTE,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case LEAVEDIRECT:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(LEAVEDIRECT,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;

						case SENDHOLD:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(SENDHOLD,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case SENDROUTE:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(SENDROUTE,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case SENDDIRECT:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(SENDDIRECT,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;

						case POLL:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(POLL,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;

						case HOSTHOLD:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(HOSTHOLD,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case HOSTROUTE:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(HOSTROUTE,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case HOSTDIRECT:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(HOSTDIRECT,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;

						case HOLDROUTE:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(HOLDROUTE,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case HOLDDIRECT:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(HOLDDIRECT,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;

						case ROUTEHOLD:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(ROUTEHOLD,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case ROUTEDIRECT:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(ROUTEDIRECT,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;

						case DIRECTHOLD:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(DIRECTHOLD,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;
						case DIRECTROUTE:
							cptr = parse_string(next,&next);
							while (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									if (parse_address(cptr,&tzone,&tnet,&tnode))
										add_action(DIRECTROUTE,tzone,tnet,tnode,0,0,0);
								cptr = parse_string(next,&next);
								}
							break;

						case HOLDTO:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(HOLDTO,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ROUTETO:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ROUTETO,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case DIRECTTO:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(DIRECTTO,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;

						case ARCHOLD:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ARCHOLD,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ARCROUTE:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ARCROUTE,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ARCDIRECT:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ARCDIRECT,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;

						case ZIPHOLD:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ZIPHOLD,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ZIPROUTE:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ZIPROUTE,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ZIPDIRECT:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ZIPDIRECT,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;

						case LZHHOLD:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(LZHHOLD,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case LZHROUTE:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(LZHROUTE,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case LZHDIRECT:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(LZHDIRECT,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;

						case ARJHOLD:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ARJHOLD,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ARJROUTE:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ARJROUTE,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ARJDIRECT:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ARJDIRECT,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;

						case ZOOHOLD:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ZOOHOLD,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ZOOROUTE:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ZOOROUTE,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;
						case ZOODIRECT:
							cptr = parse_string(next,&next);
							if (cptr)
								{
								while (*cptr && !isalnum(*cptr))
									++cptr;
								if (*cptr)
									{
									if (parse_address(cptr,&dzone,&dnet,&dnode))
										{
						  				cptr = parse_string(next,&next);
										while (cptr)
											{
											while (*cptr && !isalnum(*cptr))
												++cptr;
											if (*cptr)
												if (parse_address(cptr,&tzone,&tnet,&tnode))
													add_action(ZOODIRECT,tzone,tnet,tnode,dzone,dnet,dnode);
											cptr = parse_string(next,&next);
											}
										}
									}
								}
							break;

						default:
							printf("Error: Unknown keyword \"%s\" in smp.ctl file\n",cptr);
						}
					}
				}
			}
		}
	}



static char *dump_address(int zone,int net,int node)
	{
	static char buffer[20];
	char *cptr;

	cptr = buffer;
	if ((unsigned int)zone == ALL)
		sprintf(cptr,"ALL:");
	else
		sprintf(cptr,"%u:",zone);
	cptr += strlen(cptr);
	if ((unsigned int)net == ALL)
		sprintf(cptr,"ALL/");
	else
		sprintf(cptr,"%u/",net);
	cptr += strlen(cptr);
	if ((unsigned int)node == ALL)
		sprintf(cptr,"ALL");
	else 
		sprintf(cptr,"%u",node);
	return buffer;
	}



void dump_actions(void)
	{
	int count;

	printf("The following is the action table as read from smp.ctl:\n\n");
	for (count = 0; count < cur_actions; count++)
		{
		switch (actions[count]->act_type)
			{
			case LEAVEHOLD:
				printf("\tLEAVEHOLD %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case LEAVEROUTE:
				printf("\tLEAVEROUTE %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case LEAVEDIRECT:
				printf("\tLEAVEDIRECT %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case SENDHOLD:
				printf("\tSENDHOLD %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case SENDROUTE:
				printf("\tSENDROUTE %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case SENDDIRECT:
				printf("\tSENDDIRECT %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case HOLDTO:
				printf("\tHOLDTO to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ROUTETO:
				printf("\tROUTETO to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case DIRECTTO:
				printf("\tDIRECTTO to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case POLL:
				printf("\tPOLL %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case HOSTHOLD:
				printf("\tHOSTHOLD to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case HOSTROUTE:
				printf("\tHOSTROUTE to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case HOSTDIRECT:
				printf("\tHOSTDIRECT to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case ARCHOLD:
				printf("\tARCHOLD to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ARCROUTE:
				printf("\tARCROUTE to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ARCDIRECT:
				printf("\tARCDIRECT to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case ZIPHOLD:
				printf("\tZIPHOLD to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ZIPROUTE:
				printf("\tZIPROUTE to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ZIPDIRECT:
				printf("\tZIPDIRECT to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case LZHHOLD:
				printf("\tLZHHOLD to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case LZHROUTE:
				printf("\tLZHROUTE to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case LZHDIRECT:
				printf("\tLZHDIRECT to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case ARJHOLD:
				printf("\tARJHOLD to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ARJROUTE:
				printf("\tARJROUTE to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ARJDIRECT:
				printf("\tARJDIRECT to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case ZOOHOLD:
				printf("\tZOOHOLD to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ZOOROUTE:
				printf("\tZOOROUTE to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ZOODIRECT:
				printf("\tZOODIRECT to %s",dump_address(actions[count]->act_dzone,actions[count]->act_dnet,actions[count]->act_dnode));
				printf(" mail for %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case HOLDROUTE:
				printf("\tHOLDROUTE %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case HOLDDIRECT:
				printf("\tHOLDDIRECT %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case ROUTEHOLD:
				printf("\tROUTEHOLD %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case ROUTEDIRECT:
				printf("\tROUTEDIRECT %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			case DIRECTHOLD:
				printf("\tDIRECTHOLD %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;
			case DIRECTROUTE:
				printf("\tDIRECTROUTE %s\n",dump_address(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node));
				break;

			}
		}
	printf("\n");
	}



int check_action(int tzone,int tnet,int tnode)
	{
	int type = ROUTE;
	int count;
	int found = 0;

	for (count = 0; count < cur_actions; count++)	/* first to see if there are direct actions for the node */
		{
		if (actions[count]->act_zone == tzone && actions[count]->act_net == tnet && actions[count]->act_node == tnode)
			{
			switch (actions[count]->act_type)
				{
				case ARCDIRECT:
				case ZIPDIRECT:
				case LZHDIRECT:
				case ARJDIRECT:
				case ZOODIRECT:
					type = DIRECT;
					found = 1;
					break;
				case ARCROUTE:
				case ZIPROUTE:
				case LZHROUTE:
				case ARJROUTE:
				case ZOOROUTE:
					type = ROUTE;
					found = 1;
					break;
				case ARCHOLD:
				case ZIPHOLD:
				case LZHHOLD:
				case ARJHOLD:
				case ZOOHOLD:
					type = HOLD;
					found = 1;
					break;
				}
			if (found)
				break;
			}
		}
	if (!found)
		{
		for (count = 0; count < cur_actions; count++)	/* first to see if there are direct actions for the whole */
			{
			if (actions[count]->act_zone == tzone || (unsigned int)actions[count]->act_zone == ALL)
				{
				if (actions[count]->act_net == tnet || (unsigned int)actions[count]->act_net == ALL)
					{
					if (actions[count]->act_node == tnode || (unsigned int)actions[count]->act_node == ALL)
						{
						switch (actions[count]->act_type)
							{
							case ARCDIRECT:
							case ZIPDIRECT:
							case LZHDIRECT:
							case ARJDIRECT:
							case ZOODIRECT:
								type = DIRECT;
								found = 1;
								break;
							case ARCROUTE:
							case ZIPROUTE:
							case LZHROUTE:
							case ARJROUTE:
							case ZOOROUTE:
								type = ROUTE;
								found = 1;
								break;
							case ARCHOLD:
							case ZIPHOLD:
							case LZHHOLD:
							case ARJHOLD:
							case ZOOHOLD:
								type = HOLD;
								found = 1;
								break;
							}
						if (found)
							break;
						}
					}
				}
			}
		}
	return type;
	}
