/* s_areas.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 18 May 1990
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_areas.c_v  $
**                       $Date:   25 Oct 1992 14:18:08  $
**                       $Revision:   1.12  $
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "smail.h"




int comp_area(struct area **arg1,struct area **arg2)
	{
	return stricmp((*arg1)->area_name,(*arg2)->area_name);
	}



int parse_address(char *string,int *zone,int *net,int *node)
	{
	char buffer[10];
	char *cptr;
	char *cptr1;
	long tval;
	int rtn = 0;

	if (isdigit(string[0]))
		{
		cptr = string;
		cptr1 = buffer;
		while (*cptr && isdigit(*cptr) && cptr1 < (buffer + 5))
			*cptr1++ = *cptr++;
		*cptr1 = '\0';
		if (*cptr == ':')
			{
			tval = atol(buffer);
			if (tval <= 0xffffL)
				{
				*zone = (int)(unsigned int)tval;
				rtn = 3;
				++cptr;
				}
			}
		else if (*cptr == '/')
			{
			tval = atol(buffer);
			if (tval <= 0xffffL)
				{
				*net = (int)(unsigned int)tval;
				rtn = 2;
				++cptr;
				}
			}
		else if (*cptr <= ' ')
			{
			tval = atol(buffer);
			if (tval <= 0xffffL)
				{
				*node = (int)(unsigned int)tval;
				rtn = 1;
				++cptr;
				}
			}
		if (rtn > 1)
			{
			cptr1 = buffer;
			while (*cptr && isdigit(*cptr) && cptr1 < (buffer + 5))
				*cptr1++ = *cptr++;
			*cptr1 = '\0';
			if (*cptr == '/' && rtn == 3)
				{
				tval = atol(buffer);
				if (tval <= 0xffffL)
					{
					*net = (int)(unsigned int)tval;
					rtn = 2;
					++cptr;
					}
				}
			else if (*cptr <= ' ' && rtn == 2)
				{
				tval = atol(buffer);
				if (tval <= 0xffffL)
					{
					*node = (int)(unsigned int)tval;
					rtn = 1;
					++cptr;
					}
				}
			if (rtn > 1)
				{
				cptr1 = buffer;
				while (*cptr && isdigit(*cptr) && cptr1 < (buffer + 5))
					*cptr1++ = *cptr++;
				*cptr1 = '\0';
				if (*cptr <= ' ' && rtn == 2)
					{
					tval = atol(buffer);
					if (tval <= 0xffffL)
						{
						*node = (int)(unsigned int)tval;
						rtn = 1;
						++cptr;
						}
					}
				}
			}
		}
	return rtn;
	}



int pascal check_address(struct add *addptr,int max_add,int zone,int net,int node)
	{
	int count;
	int found = 0;

	for (count = 0; count < max_add; count++)
		{
		if (addptr[count].add_node == node && addptr[count].add_net == net && addptr[count].add_zone == zone)
			{
			found = 1;
			break;
			}
		}
	return found;
	}



void read_areas(FILE *fd)
	{
	struct msg tmsg;
	struct area *tarea;
	char buffer[515];
	char buffer1[6];
	char *cptr;
	char *cptr1;
	char *to;
	char *path;
	long tval;
	int count;
	int tval1;
	int match;
	int area;
	int tzone;
	int tnet;
	int tnode;
	int szone;
	int snet;
	int snode;
	int ok;

	fseek(fd,0L,SEEK_SET);
	fgets(buffer,sizeof(buffer),fd);		/* waste first line! */
	while (fgets(buffer,sizeof(buffer),fd))
		{
		cptr = buffer;
		if (*cptr == ';')
			{
			++cptr;
			while (*cptr && isspace(*cptr))
				++cptr;
			if ((*cptr == 'T' || *cptr == 't') && isspace(*(cptr + 1)))
				{
				++cptr;
				while (*cptr && isspace(*cptr))
					++cptr;
				if (*cptr)
					{
					if (*cptr == '"')			/* we have a quoted name */
						{
						++cptr;
						to = cptr;
						while (*cptr && *cptr != '"')
							++cptr;
						}
					else
						{
						to = cptr;
						while (*cptr && !isspace(*cptr))
							++cptr;
						}
					*cptr = (char)'\0';		/* nul-terminate string */
					cptr1 = cptr;
					++cptr1;			/* increment past nul */

					while (*cptr1 && isspace(*cptr1))
						++cptr1;
					if (*cptr1)
						{
						if (*cptr1 == '"')			/* we have a quoted path */
							{
							++cptr1;
							path = cptr1;
							while (*cptr1 && *cptr1 != '"')
								++cptr1;
							}
						else
							{
							path = cptr1;
							while (*cptr1 && !isspace(*cptr1))
								++cptr1;
							}
						if (*cptr1 == P_CSEP)
							--cptr1;
						*cptr1 = (char)'\0';		/* nul-terminate string */

						if (strlen(to) && strlen(path))
							{
							if (!(tosslist = realloc(tosslist,(max_tosslist += 5) * sizeof(struct toss *))))
								{
								fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
								exit(2);
								}

							if (!(tosslist[cur_tosslist] = malloc(sizeof(struct toss))))
								{
								fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
								exit(2);
								}
							if (!(tosslist[cur_tosslist]->toss_to = malloc(strlen(to) + 1)))
								{
								fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
								exit(2);
								}
							strcpy(tosslist[cur_tosslist]->toss_to,to);

							if (!(tosslist[cur_tosslist]->toss_path = malloc(strlen(path) + 1)))
								{
								fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
								exit(2);
								}
							strcpy(tosslist[cur_tosslist]->toss_path,path);
							tosslist[cur_tosslist]->toss_total = 0;
							tosslist[cur_tosslist]->toss_msgnum = get_msg_number(path);

							++cur_tosslist;
							}
						}
					}
				}
			}
		else 
			{
			ok = 0;
			while (*cptr && isspace(*cptr))		/* trim off leading whitespace */
				++cptr;

			if (isdigit(*cptr))
				{
				cptr1 = buffer1;
				while (*cptr && isdigit(*cptr) && (cptr1 < (buffer1 + 5)))
					*cptr1++ = *cptr++;
				*cptr1 = '\0';
				tval = atol(buffer1);
				if (tval > -1L)
					{
					area = (int)tval;
					ok = 1;
					}
				if (ok)
					{
					ok = 0;
					fseek(msgfd,0L,SEEK_SET);		/* now lets check to see if we have such an area! */
					while (fread(&tmsg,sizeof(struct msg),1,msgfd))
						{
						if (area == tmsg.msg_number && (tmsg.msg_flags & MSG_ECHO))		/* area exists and is echomail! */
							{
							tval1 = tmsg.msg_source;

							if (tval1 < 0 || tval1 > 5)
								tval1 = 0;
							if (tval1)
								{
								if (cfg.cfg_akanet[tval1 - 1])
									{
									szone = cfg.cfg_akazone[tval1 - 1];
									snet = cfg.cfg_akanet[tval1 - 1];
									snode = cfg.cfg_akanode[tval1 - 1];
									}
								else
									tval1 = 0;
								}
							if (!tval1)
								{
								szone = zone;
								snet = net;
								snode = node;
								}
							ok = 1;
							break;
							}
						}
					if (!ok)
						fprintf(stderr,"Error: There is no echomail area #%u as indicated in \"areas.bbs\"\n",(int)tval);
					}
				}
			else if ((*cptr == 'P' || *cptr == 'p') && isspace(*(cptr + 1)))
				{
			 	area = PASSTHRU;
				cptr += 2;
				ok = 1;
				szone = zone;
				snet = net;
				snode = node;
				}
			if (ok)
				{
				while (*cptr && isspace(*cptr))
					++cptr;
				if (*cptr)
					{
					cptr1 = cptr;
					while (*cptr && !isspace(*cptr))
						++cptr;
					if (*cptr)
						{
						*cptr++ = '\0';
						match = 0;
						for (count = 0; count < cur_areas; count++)
							{
							if (!stricmp(cptr1,areas[count]->area_name))
								{
								fprintf(stderr,"Error: Alternate reference to area \"%s\" in areas.bbs.  Adding to previous.\n",cptr1);
								tarea = areas[count];
								match = 1;
								break;
								}
							}

						if (!match)
							{
							if (cur_areas >= max_areas)
								{
								if (!(areas = realloc(areas,(max_areas += 50) * sizeof(struct area *))))
									{
									fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
									exit(2);
									}
								}
							if (!(areas[cur_areas] = (struct area *)calloc(1,sizeof(struct area))))
								{
								fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
								exit(2);
								}
							if (!(areas[cur_areas]->area_name = calloc(strlen(cptr1) + 1,sizeof(char))))
								{
								fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
								exit(2);
								}
							strcpy(areas[cur_areas]->area_name,cptr1);
							strupr(areas[cur_areas]->area_name);
							areas[cur_areas]->area_msgarea = area;
							areas[cur_areas]->area_tossed = 0L;
							areas[cur_areas]->area_forwarded = 0L;
							areas[cur_areas]->area_dups = 0L;
							areas[cur_areas]->area_sigs = 0L;

							areas[cur_areas]->area_source.add_zone = szone;
							areas[cur_areas]->area_source.add_net = snet;
							areas[cur_areas]->area_source.add_node = snode;

							tarea = areas[cur_areas];
							}

						while (*cptr)
							{
							while (*cptr && isspace(*cptr))
								++cptr;
							if (*cptr)
								{
								cptr1 = cptr;
								while (*cptr && !isspace(*cptr))
									++cptr;
								if (*cptr)
									{
									*cptr++ = '\0';
									tzone = zone;
									tnet = net;
									tnode = node;
									if (parse_address(cptr1,&tzone,&tnet,&tnode))
										{
										if (!tarea->area_cur_add || !check_address(tarea->area_add,tarea->area_cur_add,tzone,tnet,tnode))
											{
											if (tarea->area_cur_add >= tarea->area_max_add)
												{
												if (!(tarea->area_add = realloc(tarea->area_add,(tarea->area_max_add += 10) * sizeof(struct add))))
													{
													fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
													exit(2);
													}
												}
											tarea->area_add[tarea->area_cur_add].add_zone = tzone;
											tarea->area_add[tarea->area_cur_add].add_net = tnet;
											tarea->area_add[tarea->area_cur_add].add_node = tnode;
											++tarea->area_cur_add;
											}
										else
											fprintf(stderr,"Error: Duplicate address %u:%u/%u set for \"%s\" in areas.bbs.\n",tzone,tnet,tnode,tarea->area_name);
										}
									}
								}
							}
						if (!match)
							++cur_areas;
						}
					}
				}
			}
		}
	if (cur_areas)
		qsort(areas,cur_areas,sizeof(struct area *),comp_area);
	}



void free_areas(void)
	{
	int count;

	if (max_areas)
		{
		for (count = 0; count < cur_areas; count++)
			{
			free(areas[count]->area_name);
			free(areas[count]->area_add);
			free(areas[count]);
			}
		free(areas);
		areas = NULL;
		cur_areas = 0;
		max_areas = 0;
		}

	if (max_tosslist)
		{
		for (count = 0; count < cur_tosslist; count++)
			{
			free(tosslist[count]->toss_to);
			free(tosslist[count]->toss_path);
			free(tosslist[count]);
			}
		free(tosslist);
		tosslist = NULL;
		cur_tosslist = 0;
		max_tosslist = 0;
		}
	fprintf(stderr,"\nFinished!\n\n");
	}
