/* s_getmsg.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 18 November 1990
**
** Revision Information: $Logfile:   G:/simplex/smp/vcs/s_getmsg.c_v  $
**                       $Date:   25 Oct 1992 14:26:28  $
**                       $Revision:   1.10  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include "smp.h"




#define MAX_MLIST						20		/* number of mlist pointers to allocate at a chunk */
#define MAX_MSGS	 					50		/* number of message numbers to allocate at a chunk */



struct mlist **mlist = NULL;
int cur_mlist = 0;
int max_mlist = 0;
static char tbuffer[513];



char *read_msgline(char *buffer,int buflen,FILE *fd)
	{
	char *cptr;
	int len = 0;
	int inchar;

	cptr = buffer;
	while (len < buflen)
		{
		if ((inchar = fgetc(fd)) == EOF)
			{
			*cptr = (char)'\0';
			break;
			}
		if (inchar == '\r' || inchar == '\n')
			{
			*cptr = (char)'\0';
			break;
			}
		*cptr++ = (char)(unsigned char)inchar;
		++len;
		}
	if (inchar == EOF && cptr == buffer)
		return NULL;
	return buffer;
	}



int comp_mlist_msgs(int *arg1,int *arg2)
	{
	return *arg1 - *arg2;
	}



void add_mlist(int type,long msgnum,int szone,int snet,int snode,int tzone,int tnet,int tnode,int intl)
	{
	int found = 0;
	int count;
	int kount;
	int which;

	if (!tzone)
		tzone = zone;

	for (count = 0; count < cur_mlist; count++)
		{
		if (mlist[count]->mlist_type == type && mlist[count]->mlist_szone == szone && mlist[count]->mlist_snet == snet && mlist[count]->mlist_snode == snode &&
			mlist[count]->mlist_dzone == tzone && mlist[count]->mlist_dnet == tnet && mlist[count]->mlist_dnode == tnode)
			{
			found = 1;
			if (mlist[count]->mlist_curmsgs >= mlist[count]->mlist_maxmsgs)
				{
				if (!(mlist[count]->mlist_msgs = realloc(mlist[count]->mlist_msgs,(mlist[count]->mlist_maxmsgs += MAX_MSGS) * sizeof(int))))
					{
					for (kount = 0; kount < cur_actions; kount++)	/* free memory before crapping out */
						free(actions[kount]);
					free(actions);

					for (kount = 0; kount < cur_mlist; kount++)
						{
						if (mlist[kount]->mlist_maxmsgs)
							free(mlist[kount]->mlist_msgs);
						free(mlist[kount]);
						}
					free(mlist);
					printf("Fatal Error:  Out of memory to allocate message list structures.\n");
					exit(1);
					}
				}
			which = mlist[count]->mlist_curmsgs;
			mlist[count]->mlist_msgs[which] = (int)msgnum;
			++mlist[count]->mlist_curmsgs;
			break;
			}
		}

	if (!found)
		{
		if (cur_mlist >= max_mlist)
			{
			if (!(mlist = realloc(mlist,(max_mlist += MAX_MLIST) * sizeof(struct mlist *))))
				{
				for (count = 0; count < cur_actions; count++)	/* free memory before crapping out */
					free(actions[count]);
				free(actions);

				printf("Fatal Error:  Out of memory to allocate message list structures.\n");
				exit(1);
				}
			}
		if (!(mlist[cur_mlist] = (struct mlist *)malloc(sizeof(struct mlist))))
			{
			for (count = 0; count < cur_actions; count++)	/* free memory before crapping out */
				free(actions[count]);
			free(actions);

			for (count = 0; count < cur_mlist; count++)
				{
				if (mlist[count]->mlist_maxmsgs)
					free(mlist[count]->mlist_msgs);
				free(mlist[count]);
				}
			free(mlist);
			printf("Fatal Error:  Out of memory to allocate message list structures.\n");
			exit(1);
			}
		mlist[cur_mlist]->mlist_maxmsgs = MAX_MSGS;
		if (!(mlist[cur_mlist]->mlist_msgs = (int *)malloc(mlist[cur_mlist]->mlist_maxmsgs * sizeof(int))))
			{
			for (count = 0; count < cur_actions; count++)	/* free memory before crapping out */
				free(actions[count]);
			free(actions);

			for (count = 0; count < cur_mlist; count++)
				{
				if (mlist[count]->mlist_maxmsgs)
					free(mlist[count]->mlist_msgs);
				free(mlist[count]);
				}
			free(mlist[cur_mlist]);		/* free one we just allocated */
			free(mlist);
			printf("Fatal Error:  Out of memory to allocate message list structures.\n");
			exit(1);
			}
		mlist[cur_mlist]->mlist_msgs[0] = (int)(unsigned int)msgnum;
		mlist[cur_mlist]->mlist_curmsgs = 1;
		mlist[cur_mlist]->mlist_type = type;
		mlist[cur_mlist]->mlist_szone = szone;
		mlist[cur_mlist]->mlist_snet = snet;
		mlist[cur_mlist]->mlist_snode = snode;
		mlist[cur_mlist]->mlist_dzone = tzone;
		mlist[cur_mlist]->mlist_dnet = tnet;
		mlist[cur_mlist]->mlist_dnode = tnode;
		mlist[cur_mlist]->mlist_archived = 0;
		mlist[cur_mlist]->mlist_intl = intl;
		++cur_mlist;
		}
	}



void check_mlist(long msgnum)
	{
	struct fm msghead;
	char *cptr;
	char *next;
	int type = ROUTE;
	int fail = 0;
	int szone;
	int snet;
	int snode;
	int tzone;
	int tnet;
	int tnode;
	int izone;
	int inet;
	int inode;
	int intl = 0;
	FILE *fd;

	printf("%lu    \r",msgnum);
	strcpy(tbuffer,cfg.cfg_netpath);
	if (tbuffer[0] && tbuffer[strlen(tbuffer) - 1] != P_CSEP)
		strcat(tbuffer,P_SSEP);
	sprintf(tbuffer + strlen(tbuffer),"%lu.msg",msgnum);

	if (fd = fopen(tbuffer,"rb"))
		{
		if (fread(&msghead,sizeof(struct fm),1,fd))		/* read header */
			{
			szone = msghead.fm_szone;
			snet = msghead.fm_snet;
			snode = msghead.fm_snode;
			tzone = msghead.fm_dzone;
			tnet = msghead.fm_dnet;
			tnode = msghead.fm_dnode;
			if (msghead.fm_flags & 2)
				type = DIRECT;
			else if (msghead.fm_flags & 512)
				type = HOLD;
			while (read_msgline(tbuffer,sizeof(tbuffer),fd))
				{
				if (*tbuffer == (char)'\x01')		/* Got a Ctrl-A */
					{
					if (!strnicmp(tbuffer + 1,"INTL ",5))	/* is this an INTL message? */
						{
						cptr = parse_string(tbuffer + 6,&next);
						if (cptr && *cptr)
							{
							parse_address(cptr,&izone,&inet,&inode);
							if (inet == tnet && inode == tnode)		/* international address concurs with header destination!   Send, otherwise this might be gated so don't change destination! */
								{
								tzone = izone;
								tnet = inet;
								tnode = inode;
								}
							break;
							}
						intl = 1;
						}
					}
				}
			add_mlist(type,msgnum,szone,snet,snode,tzone,tnet,tnode,intl);
			}
		else
			fail = 1;
		fclose(fd);

		if (fail)
			unlink(tbuffer);		/* delete invalid MSG files */
		}
	}



void route_mlist(void)
	{
	int count;
	int kount;
	int stype;
	int dtype;
	int ok;

	printf("Performing preliminary routing....\n");
	for (count = 0; count < cur_mlist; count++)
		{
		for (kount = 0; kount < cur_actions; kount++)
			{
			ok = 0;
			if (actions[kount]->act_type == HOLDROUTE)
				{
				stype = HOLD;
				dtype = ROUTE;
				ok = 1;
				}
			else if (actions[kount]->act_type == HOLDDIRECT)
				{
				stype = HOLD;
				dtype = DIRECT;
				ok = 1;
				}
			else if (actions[kount]->act_type == ROUTEHOLD)
				{
				stype = ROUTE;
				dtype = HOLD;
				ok = 1;
				}
			else if (actions[kount]->act_type == ROUTEDIRECT)
				{
				stype = ROUTE;
				dtype = DIRECT;
				ok = 1;
				}
			else if (actions[kount]->act_type == DIRECTHOLD)
				{
				stype = DIRECT;
				dtype = HOLD;
				ok = 1;
				}
			else if (actions[kount]->act_type == DIRECTROUTE)
				{
				stype = DIRECT;
				dtype = ROUTE;
				ok = 1;
				}

			if (ok)
				{
				if (stype == mlist[count]->mlist_type)
					{
					if ((actions[kount]->act_zone == mlist[count]->mlist_dzone || actions[kount]->act_zone == ALL) && (actions[kount]->act_net == mlist[count]->mlist_dnet || actions[kount]->act_net == ALL) && (actions[kount]->act_node == mlist[count]->mlist_dnode || actions[kount]->act_node == ALL) &&
						(actions[kount]->act_dzone != mlist[count]->mlist_dzone || actions[kount]->act_dnet != mlist[count]->mlist_dnet || actions[kount]->act_dnode != mlist[count]->mlist_dnode))
						{
						mlist[count]->mlist_type = dtype;
						if (actions[kount]->act_dzone != mlist[count]->mlist_dzone || actions[kount]->act_net != mlist[count]->mlist_dnet || actions[kount]->act_node != mlist[count]->mlist_dnode)
							{
							printf("  Routing %u:%u/%u to %u:%u/%u\n",mlist[count]->mlist_dzone,mlist[count]->mlist_dnet,mlist[count]->mlist_dnode,actions[kount]->act_dzone,actions[kount]->act_dnet,actions[kount]->act_dnode);
							mlist[count]->mlist_dzone = actions[kount]->act_dzone;
							mlist[count]->mlist_dnet = actions[kount]->act_dnet;
							mlist[count]->mlist_dnode = actions[kount]->act_dnode;
							}
						}
					}
				}
			else
				{
				switch (actions[kount]->act_type)
					{
					case ARCHOLD:
					case ZIPHOLD:
					case LZHHOLD:
					case ARJHOLD:
					case ZOOHOLD:
						stype = HOLD;
						ok = 1;
						break;
					case ARCROUTE:
					case ZIPROUTE:
					case LZHROUTE:
					case ARJROUTE:
					case ZOOROUTE:
						stype = ROUTE;
						ok = 1;
						break;
					case ARCDIRECT:
					case ZIPDIRECT:
					case LZHDIRECT:
					case ARJDIRECT:
					case ZOODIRECT:
						stype = DIRECT;
						ok = 1;
						break;
					}
				if (ok)
					{
					if (!mlist[count]->mlist_archived && stype == mlist[count]->mlist_type)
						{
						if ((actions[kount]->act_zone == mlist[count]->mlist_dzone || actions[kount]->act_zone == ALL) && (actions[kount]->act_net == mlist[count]->mlist_dnet || actions[kount]->act_net == ALL) && (actions[kount]->act_node == mlist[count]->mlist_dnode || actions[kount]->act_node == ALL))
							{
							printf("  Routing %u:%u/%u to %u:%u/%u\n",mlist[count]->mlist_dzone,mlist[count]->mlist_dnet,mlist[count]->mlist_dnode,actions[kount]->act_dzone,actions[kount]->act_dnet,actions[kount]->act_dnode);
							mlist[count]->mlist_dzone = actions[kount]->act_dzone;
							mlist[count]->mlist_dnet = actions[kount]->act_dnet;
							mlist[count]->mlist_dnode = actions[kount]->act_dnode;
							mlist[count]->mlist_archived = 1;
							}
						}
					}
				else
					{
					switch (actions[kount]->act_type)
						{
						case HOSTROUTE:
							stype = ROUTE;
							ok = 1;
							break;
						case HOSTHOLD:
							stype = HOLD;
							ok = 1;
							break;
						case HOSTDIRECT:
							stype = DIRECT;
							ok = 1;
							break;
						}
					if (ok)
						{
						if (!mlist[count]->mlist_archived && stype == mlist[count]->mlist_type)
							{
							if ((actions[kount]->act_zone == mlist[count]->mlist_dzone || actions[kount]->act_zone == ALL) && (actions[kount]->act_net == mlist[count]->mlist_dnet || actions[kount]->act_net == ALL) && (actions[kount]->act_node == mlist[count]->mlist_dnode || actions[kount]->act_node == ALL))
								{
								printf("  Routing %u:%u/%u to %u:%u/0\n",mlist[count]->mlist_dzone,mlist[count]->mlist_dnet,mlist[count]->mlist_dnode,actions[kount]->act_dzone,actions[kount]->act_dnet);
								mlist[count]->mlist_dzone = actions[kount]->act_dzone;
								mlist[count]->mlist_dnet = actions[kount]->act_dnet;
								mlist[count]->mlist_dnode = 0;
								}
							}
						}
					else
						{
						switch (actions[kount]->act_type)
							{
							case ROUTETO:
								stype = ROUTE;
								ok = 1;
								break;
							case HOLDTO:
								stype = HOLD;
								ok = 1;
								break;
							case DIRECTTO:
								stype = DIRECT;
								ok = 1;
								break;
							}
						if (ok)
							{
							if (!mlist[count]->mlist_archived && stype == mlist[count]->mlist_type)
								{
								if ((actions[kount]->act_zone == mlist[count]->mlist_dzone || actions[kount]->act_zone == ALL) && (actions[kount]->act_net == mlist[count]->mlist_dnet || actions[kount]->act_net == ALL) && (actions[kount]->act_node == mlist[count]->mlist_dnode || actions[kount]->act_node == ALL))
									{
									printf("  Routing %u:%u/%u to %u:%u/%u\n",mlist[count]->mlist_dzone,mlist[count]->mlist_dnet,mlist[count]->mlist_dnode,actions[kount]->act_dzone,actions[kount]->act_dnet,actions[kount]->act_dnode);
									mlist[count]->mlist_dzone = actions[kount]->act_dzone;
									mlist[count]->mlist_dnet = actions[kount]->act_dnet;
									mlist[count]->mlist_dnode = actions[kount]->act_dnode;
									}
								}
							}
						}
					}
				}
			}
		}
	}



void load_mlist(void)
	{
	struct fi tfi;
	long msgnum;
	int count;
	int rtn;

	printf("Scanning matrix message area....\n");
	strcpy(tbuffer,cfg.cfg_netpath);
	if (tbuffer[0] && tbuffer[strlen(tbuffer) - 1] != P_CSEP)
		strcat(tbuffer,P_SSEP);
	strcat(tbuffer,"*.msg");

	rtn = get_firstfile(&tfi,tbuffer);
	while (rtn)
		{
		msgnum = atol(tfi.fi_name);
		if (msgnum && msgnum <= 0xffffL)
			check_mlist(msgnum);
		rtn = get_nextfile(&tfi);
		}
	get_closefile();		/* close directory handle if needed */

	if (cur_mlist)
		{
		printf("Finished scanning.  Sorting entries....");
		for (count = 0; count < cur_mlist; count++)
			qsort(mlist[count]->mlist_msgs,mlist[count]->mlist_curmsgs,sizeof(int),comp_mlist_msgs);
		printf("Done!\n");
		}
	else
		printf("No messages found.\n");
	}



void dump_mlist(void)
	{
	int count;
	int kount;
	char *cptr;

	printf("The following is the message list:\n");
	if (cur_mlist)
		{
		for (count = 0; count < cur_mlist; count++)
			{
			if (mlist[count]->mlist_type == DIRECT)
				cptr = "DIRECT";
			else if (mlist[count]->mlist_type == HOLD)
				cptr = "HOLD";
			else if (mlist[count]->mlist_type == ROUTE)
				cptr = "ROUTE";
			printf("\n\tMessages bound for %u:%u/%u as %s:",mlist[count]->mlist_dzone,mlist[count]->mlist_dnet,mlist[count]->mlist_dnode,cptr);
			for (kount = 0; kount < mlist[count]->mlist_curmsgs; kount++)
				{
				if (!(kount % 12))
					printf("\n\t\t");
				printf("%u%s",mlist[count]->mlist_msgs[kount],(kount < mlist[count]->mlist_curmsgs - 1) ? "+" : "");
				}
			printf("\n");
			}
		}
	else
		printf("\n\tNo messages were found in the matrix area.\n");
	printf("\n");
	}
