/* s_import.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 21 May 1990
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_import.c_v  $
**                       $Date:   25 Oct 1992 14:18:22  $
**                       $Revision:   1.12  $
**
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include "smail.h"



#define BUFLEN			256


char **files = NULL;
int cur_files = 0;
int max_files = 0;

long pktoffset;			/* offsets used when/if we run out of disk space */
long headoffset;
long bodyoffset;
long linkoffset;
long dataoffset;




void remove_last_message(void)		/* if we run out of disk space */
	{
	struct mdata tmdata;
	struct mc tmc;

	fseek(linkfd,linkoffset,SEEK_SET);
	chsize(fileno(linkfd),linkoffset);		/* hack off changes */

	fseek(headfd,headoffset,SEEK_SET);
	chsize(fileno(headfd),headoffset);		/* hack off changes */

	fseek(bodyfd,bodyoffset,SEEK_SET);
	chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

	fseek(datafd,dataoffset,SEEK_SET);
	fread(&tmc,sizeof(struct mc),1,datafd);
	--tmc.mc_msgs;
	fseek(datafd,dataoffset,SEEK_SET);
	fwrite(&tmc,sizeof(struct mc),1,datafd);

	fseek(datafd,0L,SEEK_SET);
	fread(&tmdata,sizeof(struct mdata),1,datafd);
	--tmdata.mdata_msgs;
	fseek(datafd,0L,SEEK_SET);
	fwrite(&tmdata,sizeof(struct mdata),1,datafd);
	}



void pack_packet(FILE *fd)		/* if we run out of disk space, remove previously tossed messages */
	{
	char buffer[100];
	long from = pktoffset;
	long to = (long)sizeof(struct ph);
	int len;

	do
		{
		fseek(fd,from,SEEK_SET);
		if (len = (int)fread(buffer,sizeof(char),sizeof(buffer) * sizeof(char),fd))
			{
			from += (long)len;
			fseek(fd,to,SEEK_SET);
			fwrite(buffer,len * sizeof(char),1,fd);
			to += (long)len;
			}
		}
	while (len);
	fflush(fd);
	chsize(fileno(fd),to);		/* cut of remaining file length */
	}	



int pascal check_seenby(struct m *tm,int net,int node)
	{
	int count;
	int found = 0;

	for (count = 0; count < tm->m_actualseenby; count++)
		{
		if (tm->m_seenby[count]->sb_net == net && tm->m_seenby[count]->sb_node == node)
			{
			found = 1;
			break;
			}
		}
	return found;
	}



void pascal add_seenby(struct m *tm,int net,int node)
	{
	if (tm->m_actualseenby >= tm->m_curseenby)
		{
		if (tm->m_curseenby >= tm->m_maxseenby)
			{
			if (!(tm->m_seenby = realloc(tm->m_seenby,(tm->m_maxseenby += 100) * sizeof(struct sb *))))
				{
				fprintf(stderr,"Fatal Error: Out of memory to allocate message seenby.\n");
				exit(1);
				}
			}
		if (!(tm->m_seenby[tm->m_curseenby] = malloc(sizeof(struct sb))))
			{
			fprintf(stderr,"Fatal Error: Out of memory to allocate message seenby.\n");
			exit(1);
			}
		++tm->m_curseenby;
		}
	tm->m_seenby[tm->m_actualseenby]->sb_net = net;
	tm->m_seenby[tm->m_actualseenby]->sb_node = node;
	++tm->m_actualseenby;
	}



void pascal add_path(struct m *tm,int net,int node)
	{
	if (tm->m_actualpath >= tm->m_curpath)
		{
		if (tm->m_curpath >= tm->m_maxpath)
			{
			if (!(tm->m_path = realloc(tm->m_path,(tm->m_maxpath += 100) * sizeof(struct p *))))
				{
				fprintf(stderr,"Fatal Error: Out of memory to allocate message path.\n");
				exit(1);
				}
			}
		if (!(tm->m_path[tm->m_curpath] = malloc(sizeof(struct p))))
			{
			fprintf(stderr,"Fatal Error: Out of memory to allocate message path.\n");
			exit(1);
			}
		++tm->m_curpath;
		}
	tm->m_path[tm->m_actualpath]->p_net = net;
	tm->m_path[tm->m_actualpath]->p_node = node;
	++tm->m_actualpath;
	}



int sb_sort(struct sb **arg1,struct sb **arg2)
	{
	register int rtn;

	rtn = (*arg1)->sb_net - (*arg2)->sb_net;
	if (!rtn)
		rtn = (*arg1)->sb_node - (*arg2)->sb_node;
	return rtn;
	}



void pascal set_seenby(struct m *tm,int which,int net,int node)
	{
	int count;
	int kount;

	for (count = 0; count < areas[which]->area_cur_add; count++)
		{
		if (!check_seenby(tm,areas[which]->area_add[count].add_net,areas[which]->area_add[count].add_node))
			add_seenby(tm,areas[which]->area_add[count].add_net,areas[which]->area_add[count].add_node);
		for (kount = 0; kount < 5; kount++)		/* add our AKA addresses if passing to an AKA zone */
			{
			if (cfg.cfg_akanet[kount] && areas[which]->area_add[count].add_zone == cfg.cfg_akazone[kount])
				{
				if (!check_seenby(tm,cfg.cfg_akanet[kount],cfg.cfg_akanode[kount]))
					add_seenby(tm,cfg.cfg_akanet[kount],cfg.cfg_akanode[kount]);
				}
			}
		}

	if (!check_seenby(tm,net,node))
		add_seenby(tm,net,node);

	qsort(tm->m_seenby,tm->m_actualseenby,sizeof(struct sb *),sb_sort);
	}



int pascal check_security(struct ph *tph,int which)
	{
	int count;

	for (count = 0; count < areas[which]->area_cur_add; count++)
		{
		if (areas[which]->area_add[count].add_net == tph->ph_snet && areas[which]->area_add[count].add_node == tph->ph_snode)
			return 0;
		}
	return 1;
	}



int pascal check_password(char *passwd,int net,int node)
	{
	int count;
	int rtn = 1;

	for (count = 0; count < cur_passwords; count++)
		{
		if (passwords[count]->pp_net == net && passwords[count]->pp_node == node)
			{
			if (stricmp(passwords[count]->pp_password,passwd))
				rtn = 0;
			else
				{
				rtn = 1;
				break;			/* definite match? Ok to exit */
				}
			}
		}
	return rtn;
	}



void import_mail(void)
	{
	struct msgh tmsgh;
	struct sig *sigptr;
	struct pfm tpfm;
	struct ph tph;
	struct fi tfi;
	struct m tm;
	char buffer[120];
	char *cptr;
	unsigned int cur_netmsg;
	unsigned int cur_badmsg;
	long offset;
	long fsize;
	DATE_T curdate;
	TIME_T curtime;
	int imported;
	int count;
	int kount;
	int counter;
	int kounter;
	int delete;
	int inchar;
	int tval;
	int found;
	int cycle;
	int rtn;
	int resync;
	int ourmail;
	int scandate;
	int ok;
	FILE *fd;

	memset(&tm,0,sizeof(struct m));			/* initialize message structure */
	cur_netmsg = get_msg_number(cfg.cfg_netpath);
	if (cfg.cfg_badmsgpath[0])
		cur_badmsg = get_msg_number(cfg.cfg_badmsgpath);

	for (cycle = 0; cycle < 2; cycle++)
		{
		if (!cycle)
			{
			/* first to check the inbound path for Packed files */
			strcpy(buffer,cfg.cfg_inboundpath);
			if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
				strcat(buffer,P_SSEP);
			}
		else 
			{
			/* second to check the packet path for Packed files */
			strcpy(buffer,cfg.cfg_packetpath);
			if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
				strcat(buffer,P_SSEP);
			}
		strcat(buffer,"*.pkt");

		rtn = get_firstfile(&tfi,buffer);
		while (rtn)
			{
			if (cur_files >= max_files)
				{
				if (!(files = realloc(files,(max_files += 10) * sizeof(char *))))
					{
					fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for filenames.\n");
					exit(1);
					}
				}
			if (!(files[cur_files] = calloc(strlen(tfi.fi_name) + 1,sizeof(char))))
				{
				fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for filenames.\n");
				exit(1);
				}
			strcpy(files[cur_files],tfi.fi_name);
			++cur_files;
			rtn = get_nextfile(&tfi);
			}

		if (cur_files)
			{
			if (!cycle)
				fprintf(stderr,"\nImporting mail from inbound directory...\n");
			else 
				fprintf(stderr,"\nImporting mail from packet directory...\n");
			for (count = 0; count < cur_files; count++)
				{
				if (!cycle)
					{
					strcpy(buffer,cfg.cfg_inboundpath);
					if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
						strcat(buffer,P_SSEP);
					}
				else
					{
					strcpy(buffer,cfg.cfg_packetpath);
					if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
						strcat(buffer,P_SSEP);
					}
				strcat(buffer,files[count]);
				if (fd = fopen(buffer,"r+b"))
					{
					delete = 0;
					fsize = filelength(fileno(fd));
					if (fsize > (long)(sizeof(struct ph) + sizeof(struct pfm)))		/* less than a header + 1 packed message header */
						{
						fread(&tph,sizeof(struct ph),1,fd);
						if (tph.ph_sig1 == 2 && tph.ph_sig2 == 0)	/* file must have a packet signal word -- if it does, attempt to unpack it */
							{
							strncpy(buffer,tph.ph_password,8);
							buffer[8] = (char)'\0';
							cptr = buffer;
							while (*cptr)
								{
								if (isspace(*cptr))
									{
									*cptr = (char)'\0';
									break;
									}
								++cptr;
								}
							if (check_password(buffer,tph.ph_snet,tph.ph_snode))
								{
								fprintf(stderr,"\nUnpacking %s...\n",files[count]);
								kount = 0;
								resync = 0;
								pktoffset = ftell(fd);
								while (fread(&tpfm,sizeof(struct pfm),1,fd))
									{
									imported = 0;
									if (tpfm.pfm_sig1 != 2 || tpfm.pfm_sig2 != 0)	/* does message have correct signal word */
										{
										if (resync >= 50)
											{
											fprintf(stderr,"\r\aError: Resynchronizing failed after %d attempts.  Quitting this packet.\n",resync);
											break;
											}
										++resync;
										fprintf(stderr,"\rResynchronizing (attempt %d)....",resync);
										fseek(fd,-(long)sizeof(struct pfm),SEEK_CUR);		/* seek back to beginning of last "header" read */
										while ((inchar = fgetc(fd)) != '\0' && inchar != EOF)		/* look for a message NUL or EOF! */
											;		/* do nothing */
										}
									else
										{
										if (resync)
											fprintf(stderr,"\n");
										resync = 0;
										++kount;
										if (!quiet_flag)
											fprintf(stderr,"PktMsg %d ",kount);
										memset(&tmsgh,0,sizeof(struct msgh));
										tmsgh.msgh_snode = tpfm.pfm_snode;
										tmsgh.msgh_dnode = tpfm.pfm_dnode;
										tmsgh.msgh_snet = tpfm.pfm_snet;
										tmsgh.msgh_dnet = tpfm.pfm_dnet;
										tmsgh.msgh_szone = zone;
										tmsgh.msgh_dzone = zone;
										tmsgh.msgh_flags = 0;

										/* read (our supposedly) 20 byte date/time string */
										cptr = buffer;
										scandate = 0;
										counter = 0;
										kounter = 0;
										while ((tval = fgetc(fd)) != EOF)
											{
											if (!scandate)
												*cptr++ = (char)tval;
											++counter;

											if (!tval && !scandate)
												{
												if (counter >= 18)		/* we have found AREAFIX's format of DD MMM YY HH:MM:SS\0\0 */
													{
													tval = fgetc(fd);
													if (tval)
														ungetc(tval,fd);
													else 
														break;
													}
										  		if (counter >= 16)		/* we had a short time/date stamp - Frontdoor + Qecho violate the standard here! */
										  			break;
												scandate = 1;
												}
											if (scandate)
												++kounter;
											if (scandate && tval)
												{
												ungetc(tval,fd);		/* back up 1 character and get out! */
												if (counter <= 5 && counter == kounter)		/* found one of QM 1.0's screwed up message headers with 0 date, 0 to, 0 from, 0 subject!! */
													{
													while (--kounter)
														ungetc('\0',fd);
													}
												else if (counter <= 5 && (kounter > 1) && counter > kounter)	/* found a screwed up dBridge message with garbage date */
													{
													--kounter;
													while (--kounter)
														ungetc('\0',fd);
													}
												break;
												}
											else if (counter >= 20)
												break;
											}
										*cptr = '\0';
										if (tval == EOF)
											{
											if (!quiet_flag)
												fprintf(stderr,"- <Short Packet> - skipping\n");
											delete = 1;
											break;
											}

										/* get date and time stamp */
										load_datetime(&curdate,&curtime);		/* get current date/time */
										cptr = buffer;
										if (isdigit(*cptr))		/* fidonet standard date string */
											{
											tval = atoi(cptr);
											if (tval && tval < 32)
												tmsgh.msgh_date = tval;
											while (*cptr && !isalpha(*cptr))
												++cptr;
											if (*cptr)
												{
												for (counter = 0; counter < 12; counter++)
													{
													if (!strncmp(months[counter],cptr,3))
														{
														tmsgh.msgh_date |= (counter + 1) << 5;
														break;
														}
													}
												}
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr) % 100;
												if (tval < 80)
													tval += 20;
												else
													tval -= 80;
												tmsgh.msgh_date |= tval << 9;
												}
											while (*cptr && isdigit(*cptr))
												++cptr;
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr);
												if (tval < 24)
													tmsgh.msgh_time = tval << 11;		/* hours */
												}
											while (*cptr && isdigit(*cptr))
												++cptr;
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr);
												if (tval < 60)
													tmsgh.msgh_time |= tval << 5;		/* minutes */
												}
											while (*cptr && isdigit(*cptr))
												++cptr;
											while (*cptr && !isdigit(*cptr))
												++cptr;
											}
										else if (*cptr == ' ' && isdigit(*(cptr + 1)))			/* is perverted Fidonet date string with space padding NOT '0' padding (e.g. XFIDO CNET) */
											{
											++cptr;			/* skip padding */
											tval = atoi(cptr);
											if (tval && tval < 32)
												tmsgh.msgh_date = tval;
											while (*cptr && !isalpha(*cptr))
												++cptr;
											if (*cptr)
												{
												for (counter = 0; counter < 12; counter++)
													{
													if (!strncmp(months[counter],cptr,3))
														{
														tmsgh.msgh_date |= (counter + 1) << 5;
														break;
														}
													}
												}
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr) % 100;
												if (tval < 80)
													tval += 20;
												else
													tval -= 80;
												tmsgh.msgh_date |= tval << 9;
												}
											while (*cptr && isdigit(*cptr))
												++cptr;
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr);
												if (tval < 24)
													tmsgh.msgh_time = tval << 11;		/* hours */
												}
											while (*cptr && isdigit(*cptr))
												++cptr;
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr);
												if (tval < 60)
													tmsgh.msgh_time |= tval << 5;		/* minutes */
												}
											while (*cptr && isdigit(*cptr))
												++cptr;
											while (*cptr && !isdigit(*cptr))
												++cptr;
											}
										else if (*cptr)					/* Seadog format date string */
											{
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr);
												if (tval && tval < 32)
													tmsgh.msgh_date = tval;
												}
											while (*cptr && !isalpha(*cptr))
												++cptr;
											if (*cptr)
												{
												for (counter = 0; counter < 12; counter++)
													{
													if (!strncmp(months[counter],cptr,3))
														{
														tmsgh.msgh_date |= (counter + 1) << 5;
														break;
														}
													}
												}
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr) % 100;
												if (tval < 80)
													tval += 20;
												else
													tval -= 80;
												tmsgh.msgh_date |= tval << 9;
												}
											while (*cptr && isdigit(*cptr))
												++cptr;
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr);
												if (tval < 24)
													tmsgh.msgh_time = tval << 11;
												}
											while (*cptr && isdigit(*cptr))
												++cptr;
											while (*cptr && !isdigit(*cptr))
												++cptr;
											if (*cptr)
												{
												tval = atoi(cptr);
												if (tval < 60)
													tmsgh.msgh_time |= tval << 5;
												}
											}
										else
											{
											tmsgh.msgh_date = curdate;
											tmsgh.msgh_time = curtime;	/* some idiot can't read the specs and place the date/time in this field! */
											}

										if (curdate < tmsgh.msgh_date)
											{
											tmsgh.msgh_date = curdate;
											tmsgh.msgh_time = curtime;	/* some idiot can't read the specs and do the date/time in this field correctly! */

											if (!quiet_flag)
												fprintf(stderr,"Invalid time replaced with current - ");
											}


										cptr = buffer;
										while ((tval = fgetc(fd)) && tval != EOF)
											{
											if (cptr < (buffer + 40))
												*cptr++ = (char)tval;
											}
										if (tval == EOF)
											{
											if (!quiet_flag)
												fprintf(stderr,"- <Short Packet> - skipping\n");
											delete = 1;
											break;
											}
										*cptr = '\0';
										strcpy(tmsgh.msgh_to,buffer);

										cptr = buffer;
										while ((tval = fgetc(fd)) && tval != EOF)
											{
											if (cptr < (buffer + 40))
												*cptr++ = (char)tval;
											}
										if (tval == EOF)
											{
											if (!quiet_flag)
												fprintf(stderr,"- <Short Packet> - skipping\n");
											delete = 1;
											break;
											}
										*cptr = '\0';
										strcpy(tmsgh.msgh_from,buffer);

										cptr = buffer;
										while ((tval = fgetc(fd)) && tval != EOF)
											{
											if (cptr < (buffer + 70))
												*cptr++ = (char)tval;
											}
										if (tval == EOF)
											{
											if (!quiet_flag)
												fprintf(stderr,"- <Short Packet> - skipping\n");
											delete = 1;
											break;
											}
										*cptr = '\0';
										strcpy(tmsgh.msgh_subject,buffer);

										offset = ftell(fd);		/* keep our position for possible later use */
										cptr = buffer;
										counter = 0;
										while (counter < 5 && (tval = fgetc(fd)) && tval != EOF)
											{
											*cptr++ = (char)tval;
											++counter;
											}
										if (tval == EOF)
											{
											if (!quiet_flag)
												fprintf(stderr,"- <Short Packet> - skipping\n");
											delete = 1;
											break;
											}

										if ((rtn = check_toss(tmsgh.msgh_to)) != -1)
											{
											if (!quiet_flag)
												fprintf(stderr,"[Toss for %s] -> Sent to %s\n",tosslist[rtn]->toss_to,tosslist[rtn]->toss_path);
											toss_message(tosslist[rtn],&tmsgh,NULL,fd);
											}
										else if (!strnicmp(buffer,"AREA:",5))		/* is this ECHOMAIL? */
											{
											cptr = buffer;
											while ((tval = fgetc(fd)) != EOF && tval && tval != '\r' && tval != '\x8d') 
												*cptr++ = (char)tval;
											if (tval == EOF)
												{
												if (!quiet_flag)
													fprintf(stderr,"- <Short Packet> - skipping\n");
												delete = 1;
												break;
												}
											*cptr = '\0';
											while (cptr > buffer && !isalnum(*cptr))
												*cptr-- = '\0';

											cptr = buffer;
											while (*cptr && isspace(*cptr))
												++cptr;

											if (!quiet_flag)
												fprintf(stderr,"[%s] -> ",cptr);
										
											/* time to convert this into a binary search */
//	tarea = (struct area *)bsearch(&cptr,areas,cur_areas,sizeof(struct area *),comp_area);

											for (counter = 0, found = 0; counter < cur_areas; counter++)
												{
												if (!stricmp(areas[counter]->area_name,cptr))
													{
													found = 1;
													break;
													}
												}
											if (!found)
												{
												if (cfg.cfg_badmsgpath[0])
													{
													if (!quiet_flag)
														fprintf(stderr,"bad message in invalid area\n");
													toss_bad(&cur_badmsg,&tmsgh,cptr,fd);
													}
												else
													{
													if (!quiet_flag)
														fprintf(stderr,"skipping echo message from invalid area\n");
													while ((tval = fgetc(fd)) && tval != EOF)		/* bleed to EOF or to end of message */
														;
													}
												}
											else if (echosec_flag && check_security(&tph,counter))		/* echo security is on? */
												{
												if (cfg.cfg_badmsgpath[0])
													{
													if (!quiet_flag)
														fprintf(stderr,"bad message from unpermitted node\n");
													toss_bad(&cur_badmsg,&tmsgh,cptr,fd);
													}
												else
													{
													if (!quiet_flag)
														fprintf(stderr,"skipping echo message from unpermitted node\n");
													while ((tval = fgetc(fd)) && tval != EOF)		/* bleed to EOF or to end of message */
														;
													}
												}
											else
												{
												read_msg(&tm,&tmsgh,fd);

												set_seenby(&tm,counter,net,node);
												add_path(&tm,net,node);

												if (sigptr = check_signature(counter,tmsgh.msgh_to,tmsgh.msgh_from,tmsgh.msgh_subject,tm.m_body,tm.m_curbody))
													{
										 			tmsgh.msgh_flags |= MSGH_ECHO;
													if (tpfm.pfm_flags & 1)
														tmsgh.msgh_flags |= MSGH_PRIVATE;

										 			tmsgh.msgh_area = areas[counter]->area_msgarea;
													if (areas[counter]->area_msgarea != PASSTHRU)
														{
														ok = 1;
														if (twit_flag)
															{
															for (kounter = 0; kounter < cur_twits; kounter++)
																{
																if (twits[kounter]->tf_flags & TF_TO)
																	{
																	if (!stricmp(twits[kounter]->tf_name,tmsgh.msgh_to))
																		{
																		if (!quiet_flag)
																			fprintf(stderr,"\aTwitted msg to \"%s\"\n",tmsgh.msgh_to);
																		ok = 0;
																		break;
																		}
																	}
																if (ok && twits[kounter]->tf_flags & TF_FROM)
																	{
																	if (!stricmp(twits[kounter]->tf_name,tmsgh.msgh_from))
																		{
																		if (!quiet_flag)
																			fprintf(stderr,"\aTwitted msg from \"%s\"\n",tmsgh.msgh_from);
																		ok = 0;
																		break;
																		}
																	}
																}
															}
														if (ok)
															{
															if (import_message(&tmsgh,&tm))
																{
																fprintf(stderr,"\nNotice: Removing tossed packets from packet file \"%s\"\n",files[count]);
																pack_packet(fd);
																fclose(fd);
																exit(1);
																}
															++areas[counter]->area_tossed;
															}
														else
															twit_message(&tmsgh,&tm);

														imported = 1;
														}
													found = 0;

													for (kounter = 0; kounter < areas[counter]->area_cur_add; kounter++)
														{
														/* change sender to my address to prevent their echomail security from blowing! */
														tmsgh.msgh_szone = areas[counter]->area_source.add_zone;
														tmsgh.msgh_snet = areas[counter]->area_source.add_net;
														tmsgh.msgh_snode = areas[counter]->area_source.add_node;

														if (areas[counter]->area_add[kounter].add_net != tph.ph_snet || areas[counter]->area_add[kounter].add_node != tph.ph_snode)		/* do not send messages back to the source! */
															{
															if (!quiet_flag)
																fprintf(stderr,"%d:%d/%d ",areas[counter]->area_add[kounter].add_zone,areas[counter]->area_add[kounter].add_net,areas[counter]->area_add[kounter].add_node);

															/* get destination address! */
												   			tmsgh.msgh_dzone = areas[counter]->area_add[kounter].add_zone;
															tmsgh.msgh_dnet = areas[counter]->area_add[kounter].add_net;
															tmsgh.msgh_dnode = areas[counter]->area_add[kounter].add_node;
															if (forward_msg(&cur_netmsg,&tmsgh,cptr,&tm))
																{
																if (imported)
																	remove_last_message();
																fprintf(stderr,"\nNotice: Removing tossed packets from packet file \"%s\"\n",files[count]);
																pack_packet(fd);
																fclose(fd);
																exit(1);
																}
															++areas[counter]->area_forwarded;
															found = 1;
															}
														}
													if (!found && !imported)
														{
														if (!quiet_flag)
															fprintf(stderr,"area is neither imported nor forwarded");
														}
													else 
														add_signature(counter,sigptr);
													if (!quiet_flag)
														fprintf(stderr,"\n");
													}
												else
													{
													if (!quiet_flag)
														fprintf(stderr,"Skipping duplicate message\n");
													printf("  þ Duplicate message from %u:%u/%u in area \"%s\"\n",tmsgh.msgh_szone,tmsgh.msgh_snet,tmsgh.msgh_snode,cptr);
													++areas[counter]->area_dups;
													}
												}
											}
										else
											{
											fseek(fd,offset,SEEK_SET);
											tm.m_curbody = 0;
											tm.m_actualseenby = 0;
											tm.m_actualpath = 0;
											read_msg(&tm,&tmsgh,fd);

											for (kounter = 0, ourmail = 0; kounter < cur_netmail_areas; kounter++)
												{
												if (tmsgh.msgh_dnet == netmail_areas[kounter].nm_net && tmsgh.msgh_dnode == netmail_areas[kounter].nm_node && ((netmail_areas[kounter].nm_zone && tmsgh.msgh_dzone && tmsgh.msgh_dzone == netmail_areas[kounter].nm_zone) || !tmsgh.msgh_dzone || !netmail_areas[kounter].nm_zone))		/* is this *OUR* netmail to a special area? */
													{
													ourmail = 1;
													break;
													}
												}

											if (!ourmail && cur_netmail_areas)		/* otherwise it is *OUR* netmail to go to our primary netmail area */
												{
												if (tmsgh.msgh_dnet == net && tmsgh.msgh_dnode == node && ((zone && tmsgh.msgh_dzone && tmsgh.msgh_dzone == zone) || !zone || !tmsgh.msgh_dzone))	/* is it to our primary address */
													{
													ourmail = 1;
													kounter = 0;
													}
												else
													{
													for (kounter = 0; kounter < 5; kounter++)
														{
														if (tmsgh.msgh_dnet == cfg.cfg_akanet[kounter] && tmsgh.msgh_dnode == cfg.cfg_akanode[kounter] && ((cfg.cfg_akazone[kounter] && tmsgh.msgh_dzone && tmsgh.msgh_dzone == cfg.cfg_akazone[kounter]) || !tmsgh.msgh_dzone || !cfg.cfg_akazone[kounter]))		/* is this our AKA netmail? */
															{
															ourmail = 1;
															break;
															}
														}
													}
												}

											if (ourmail)
												{
												if (!quiet_flag)
													{
													fprintf(stderr,"[Net Mail from %u:%u/%u to our %u:%u/%u] -> ",tmsgh.msgh_szone,tmsgh.msgh_snet,tmsgh.msgh_snode,
														tmsgh.msgh_dzone,tmsgh.msgh_dnet,tmsgh.msgh_dnode);
													}

												tmsgh.msgh_area = netmail_areas[kounter].nm_area;
												tmsgh.msgh_flags |= MSGH_NET;
												if (tpfm.pfm_flags & 1)
													tmsgh.msgh_flags |= MSGH_PRIVATE;
												if (tpfm.pfm_flags & 2)
													tmsgh.msgh_flags |= NET_DIRECT;
												else if (tpfm.pfm_flags & 0x200)
													tmsgh.msgh_flags |= NET_HOLD;
												if (tpfm.pfm_flags & 16)
													tmsgh.msgh_flags |= NET_FILEATTACH;

												ok = 1;
												if (twit_flag)
													{
													for (kounter = 0; kounter < cur_twits; kounter++)
														{
														if (twits[kounter]->tf_flags & TF_TO)
															{
															if (!stricmp(twits[kounter]->tf_name,tmsgh.msgh_to))
																{
																if (!quiet_flag)
																	fprintf(stderr,"\aTwitted msg to \"%s\"\n",tmsgh.msgh_to);
																ok = 0;
																break;
																}
															}
														if (ok && twits[kounter]->tf_flags & TF_FROM)
															{
															if (!stricmp(twits[kounter]->tf_name,tmsgh.msgh_from))
																{
																if (!quiet_flag)
																	fprintf(stderr,"\aTwitted msg from \"%s\"\n",tmsgh.msgh_from);
																ok = 0;
																break;
																}
															}
														}
													}
												if (ok)
													{
													if (import_message(&tmsgh,&tm))
														{
														fprintf(stderr,"\nNotice: Removing tossed packets from packet file \"%s\"\n",files[count]);
														pack_packet(fd);
														fclose(fd);
														exit(1);
														}
													++netmail_areas[kounter].nm_tossed;
													netmail_tossed = 1;
													if (!quiet_flag)
														fprintf(stderr,"\n");
													}
												else
													twit_message(&tmsgh,&tm);
												imported = 1;
												}
											else
												{
												fseek(fd,offset,SEEK_SET);
												if (!quiet_flag)
													fprintf(stderr,"[Net Mail to %u:%u/%u from %u:%u/%u] -> Forwarding\n",tmsgh.msgh_dzone,tmsgh.msgh_dnet,tmsgh.msgh_dnode,tmsgh.msgh_szone,tmsgh.msgh_snet,tmsgh.msgh_snode);
												tmsgh.msgh_flags |= MSGH_NET;
												if (tpfm.pfm_flags & 0x1)
													tmsgh.msgh_flags |= MSGH_PRIVATE;
												if (tpfm.pfm_flags & 0x2)
													tmsgh.msgh_flags |= NET_DIRECT;
												else if (tpfm.pfm_flags & 0x200)
													tmsgh.msgh_flags |= NET_HOLD;
												if (tpfm.pfm_flags & 0x10)
													tmsgh.msgh_flags |= NET_FILEATTACH;

												if (forward_msg(&cur_netmsg,&tmsgh,NULL,&tm))
													{
													fprintf(stderr,"\nNotice: Removing tossed packets from packet file \"%s\"\n",files[count]);
													pack_packet(fd);
													fclose(fd);
													exit(1);
													}
												found = 1;
												}
											}
										delete = 1;		/* ok to delete packet -- it was valid and we got what we could! */
										}
									pktoffset = ftell(fd);
									}
								}
							else
								{
								fprintf(stderr,"\a\a");
								printf("NOTICE: Deleting packet \"%s\" - invalid password from %u/%u!\n",files[count],tph.ph_snet,tph.ph_snode);
								delete = 1;
								}
							}
						else
							printf("NOTICE: \"%s\" is not a valid packet...\n",files[count]);
						}
					else if (fsize == (long)sizeof(struct ph))
						{
						printf("NOTICE: \"%s\" is a NULL packet...deleting\n",files[count]);
						delete = 1;
						}
					else
						{
						printf("NOTICE: \"%s\" is too small to be a valid packet...deleting\n",files[count]);
						delete = 1;
						}
					fclose(fd);
					if (delete)
						{
						if (!cycle)
							strcpy(buffer,cfg.cfg_inboundpath);
						else 
							strcpy(buffer,cfg.cfg_packetpath);
						if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
							strcat(buffer,P_SSEP);
						strcat(buffer,files[count]);
						fprintf(stderr,"Deleting %s",buffer);
						if (unlink(buffer))
							fprintf(stderr," - Error\a\n");
						else
							fprintf(stderr," - Ok\n");
						}
					}
				else
					fprintf(stderr,"Error: Unable to open %s\n",buffer);
				}

			for (count = 0; count < cur_files; count++)
				free(files[count]);
			free(files);
			cur_files = 0;
			max_files = 0;
			files = NULL;
			}
		}
	if (tm.m_curbody)	 	/* instead of reallocing and freeing repeatedly! */
		free(tm.m_body);
	if (tm.m_curseenby)
		{
		for (count = 0; count < tm.m_curseenby; count++)
			free(tm.m_seenby[count]);
		free(tm.m_seenby);
		}
	if (tm.m_curpath)
		{
		for (count = 0; count < tm.m_curpath; count++)
			free(tm.m_path[count]);
		free(tm.m_path);
		}
	}



/* Function parses seen-by and path lines.  Returns 0 if end of string
** and no match.  Returns 1 if net/node found and not EOL.  Returns
** 2 if net/node found and EOL.  Returns 3 if EOL found and no net/node
** is found.  Returns 4 if end of string hit while matching numbers.
** If return is 1, offset has the number of characters read and 
** which need to be stripped before the next invocation of this function.
*/

int pascal parse_path(char *buffer,int *offset,int *tnet,int *tnode,int lastnet)
	{
	char tbuf[30];
	char *cptr;
	char *cptr1;
	int quit = 0;
	int ok = 0;

	cptr = buffer;
	while (*cptr && !quit)
		{
		while (*cptr && !isdigit(*cptr))
			{
			if (*cptr == '\r' || *cptr == '\x8d')
				{
				*offset = (int)(cptr - buffer);
				return 3;
				}
			++cptr;
			}

		cptr1 = tbuf;
		while (isdigit(*cptr))
			*cptr1++ = *cptr++;

		*cptr1 = '\0';
		if (!*cptr)
			{
			*offset = (int)(cptr - buffer);
			ok = 4;
			quit = 1;
			}

		if (*cptr == '/')		/* got net */
			{
			*tnet = atoi(tbuf);

			while (*cptr && !isdigit(*cptr))
				{
				if (*cptr == '\r' || *cptr == '\x8d')
					{
					*offset = (int)(cptr - buffer);
					return 3;
					}
				++cptr;
				}

			cptr1 = tbuf;
			while (isdigit(*cptr))
				*cptr1++ = *cptr++;
			*cptr1 = '\0';

			if (!*cptr)
				{
				ok = 4;
				quit = 1;
				}

			if (cptr != tbuf)		/* got node */
				{
				*tnode = atoi(tbuf);
				*offset = (int)(cptr - buffer);
				if (*cptr == '\r' || *cptr == '\x8d')
					ok = 2;
				else 
					ok = 1;
				quit = 1;
				}
			else
				*offset = (int)(cptr - buffer);

			}
		else if (cptr1 != tbuf)			/* got node only */
			{
			*tnet = lastnet;
			*tnode = atoi(tbuf);
			*offset = (int)(cptr - buffer);
			if (*cptr == '\r' || *cptr == '\x8d')
				ok = 2;
			else 
				ok = 1;
			quit = 1;
			}
		}
	return ok;
	}



void read_msg(struct m *tm,struct msgh *tmsgh,FILE *fd)
	{
	char buffer[BUFLEN + 1];
	char *cptr;
	char *cptr1;
	int cr = 1;
	int tszone;
	int tsnet;
	int tsnode;
	int tdzone;
	int tdnet;
	int tdnode;
	int offset;
	int lastnet = 0;
	int tnet;
	int tnode;
	int type = 0;		/* 0 is msgbody, 1 is SEEN-BY:, 2 is ^aPATH: */
	int len;
	int tval;
	int rtn;

	memset(buffer,0,BUFLEN);
	len = 0;
	tm->m_actualseenby = 0;
	tm->m_actualpath = 0;
	tm->m_curbody = 0;

	while ((tval = fgetc(fd)) && tval != EOF)
		{
		if (len >= BUFLEN)
			{
			if (!type)
				{
				if ((tm->m_curbody + len) >= tm->m_maxbody)
					{
					if (!(tm->m_body = realloc(tm->m_body,(tm->m_maxbody += 1000) * sizeof(char))))
						{
						fprintf(stderr,"Fatal Error: Out of memory to allocate message body.\n");
						exit(1);
						}
					}
				memcpy(tm->m_body + tm->m_curbody,buffer,len);
				tm->m_curbody += len;
				len = 0;
				}
			else if (type == 1)
				{
				cptr = buffer;
				buffer[len] = '\0';
				do
					{
					rtn = parse_path(cptr,&offset,&tnet,&tnode,lastnet);
					switch (rtn)
						{
						case 0:
							len = 0;
							break;
						case 1:
						case 2:
							lastnet = tnet;
							if (!check_seenby(tm,tnet,tnode))
								add_seenby(tm,tnet,tnode);
							if (rtn == 2)
								len = 0;
							else
								cptr += offset;
							break;
						case 3:
							len = 0;
							break;
						case 4:
							memmove(buffer,cptr,(strlen(cptr) + 1) * sizeof(char));
							len = (int)strlen(buffer);
							break;
						}
					}
				while (rtn != 0 && rtn != 2 && rtn != 3 && rtn != 4);
				}
			else
				{
				cptr = buffer;
				buffer[len] = '\0';
				do
					{
					rtn = parse_path(cptr,&offset,&tnet,&tnode,lastnet);
					switch (rtn)
						{
						case 0:
							len = 0;
							break;
						case 1:
						case 2:
							lastnet = tnet;
							add_path(tm,tnet,tnode);
							if (rtn == 2)
								len = 0;
							else
								cptr += offset;
							break;
						case 3:
							len = 0;
							break;
						case 4:
							memmove(buffer,cptr,(strlen(cptr) + 1) * sizeof(char));
							len = (int)strlen(buffer);
							break;
						}
					}
				while (rtn != 0 && rtn != 2 && rtn != 3 && rtn != 4);
				}
			cr = 0;

			}

		if (!type)		/* strip invalid control chars from text */
			{
			if (!iscntrl(tval) || (iscntrl(tval) && (tval == '\r' || tval == '\n' || tval == '\x01')))
				{
				buffer[len] = (char)tval;
				++len;
				}
			}
		else
			{
			buffer[len] = (char)tval;
			++len;
			}

		if (cr)
			{
			if (!strnicmp(buffer,"SEEN-BY:",8))
				{
				if (type != 1)
					{
					type = 1;
					lastnet = 0;
					}
				}
			else if (!strnicmp(buffer,"\x01PATH:",6))		/* ^aPATH: */
				{
				if (type != 2)
					{
					type = 2;
					lastnet = 0;
					}
				}
			}
		if (cr && (tval == '\r' || tval == '\n' || tval == '\x8d'))
			{
			if (!strnicmp(buffer,"\x01INTL",5))		/* check the ^aINTL line */
				{
				cptr = buffer;
				while (!isdigit(*cptr))
					++cptr;
				cptr1 = cptr;
				while (isdigit(*cptr1))
					++cptr1;
				if (*cptr1 == ':')
					{
					tdzone = atoi(cptr);
					cptr = cptr1;
					while (!isdigit(*cptr))
						++cptr;
					cptr1 = cptr;
					while (isdigit(*cptr1))
						++cptr1;
					if (*cptr1 == '/')
						{
						tdnet = atoi(cptr);
						cptr = cptr1;
						while (!isdigit(*cptr))
							++cptr;
						cptr1 = cptr;
						while (isdigit(*cptr1))
							++cptr1;
						if (*cptr1 == ' ')
							{
							tdnode = atoi(cptr);
							cptr = cptr1;
							while (!isdigit(*cptr))
								++cptr;
							cptr1 = cptr;
							while (isdigit(*cptr1))
								++cptr1;
							if (*cptr1 == ':')
								{
								tszone = atoi(cptr);
								cptr = cptr1;
								while (!isdigit(*cptr))
									++cptr;
								cptr1 = cptr;
								while (isdigit(*cptr1))
									++cptr1;
								if (*cptr1 == '/')
									{
									tsnet = atoi(cptr);
									cptr = cptr1;
									while (!isdigit(*cptr))
										++cptr;
									cptr1 = cptr;
									if (isdigit(*cptr))
										{
										tsnode = atoi(cptr);

										/* change our src and dest address to the ^aINTL line! */
										tmsgh->msgh_szone = tszone;
										tmsgh->msgh_snet = tsnet;
										tmsgh->msgh_snode = tsnode;
										tmsgh->msgh_dzone = tdzone;
										tmsgh->msgh_dnet = tdnet;
										tmsgh->msgh_dnode = tdnode;
										}	
									}
								}
							}
						}
					}
				}
			}

		if (tval == '\r' || tval == '\n' || tval == '\x8d')
			{
			if (len)
				{
				if (!type)
					{
					if ((tm->m_curbody + len) >= tm->m_maxbody)
						{
						if (!(tm->m_body = realloc(tm->m_body,(tm->m_maxbody += 1000) * sizeof(char))))
							{
							fprintf(stderr,"Fatal Error: Out of memory to allocate message body.\n");
							exit(1);
							}
						}
					memcpy(tm->m_body + tm->m_curbody,buffer,len);
					tm->m_curbody += len;
					len = 0;
					}
				else if (type == 1)
					{
					cptr = buffer;
					buffer[len] = '\0';
					do
						{
						rtn = parse_path(cptr,&offset,&tnet,&tnode,lastnet);
						switch (rtn)
							{
							case 0:
								len = 0;
								break;
							case 1:
							case 2:
								lastnet = tnet;
								if (!check_seenby(tm,tnet,tnode))
									add_seenby(tm,tnet,tnode);
								if (rtn == 2)
									len = 0;
								else
									cptr += offset;
								break;
							case 3:
								len = 0;
								break;
							case 4:
								memmove(buffer,cptr,(strlen(cptr) + 1) * sizeof(char));
								len = (int)strlen(buffer);
								break;
							}
						}
					while (rtn != 0 && rtn != 2 && rtn != 3 && rtn != 4);
					}
				else
					{
					cptr = buffer;
					buffer[len] = '\0';
					do
						{
						rtn = parse_path(cptr,&offset,&tnet,&tnode,lastnet);
						switch (rtn)
							{
							case 0:
								len = 0;
								break;
							case 1:
							case 2:
								lastnet = tnet;
								add_path(tm,tnet,tnode);
								if (rtn == 2)
									len = 0;
								else
									cptr += offset;
								break;
							case 3:
								len = 0;
								break;
							case 4:
								memmove(buffer,cptr,(strlen(cptr) + 1) * sizeof(char));
								len = (int)strlen(buffer);
								break;
							}
						}
					while (rtn != 0 && rtn != 2 && rtn != 3 && rtn != 4);
					}
				}
			cr = 1;
			}
		}

	if (len)
		{
		if (!type)
			{
			if ((tm->m_curbody + len) >= tm->m_maxbody)
				{
				if (!(tm->m_body = realloc(tm->m_body,(tm->m_maxbody += 1000) * sizeof(char))))
					{
					fprintf(stderr,"Fatal Error: Out of memory to allocate message body.\n");
					exit(1);
					}
				}
			memcpy(tm->m_body + tm->m_curbody,buffer,len);
			tm->m_curbody += len;
			}
		else if (type == 1)
			{
			cptr = buffer;
			buffer[len] = '\0';
			do
				{
				rtn = parse_path(cptr,&offset,&tnet,&tnode,lastnet);
				switch (rtn)
					{
					case 0:
						len = 0;
						break;
					case 1:
					case 2:
						lastnet = tnet;
						if (!check_seenby(tm,tnet,tnode))
							add_seenby(tm,tnet,tnode);
						if (rtn == 2)
							len = 0;
						else
							cptr += offset;
						break;
					case 3:
					case 4:
						break;
					}
				}
			while (rtn != 0 && rtn != 2 && rtn != 3 && rtn != 4);
			}
		else
			{
			cptr = buffer;
			buffer[len] = '\0';
			do
				{
				rtn = parse_path(cptr,&offset,&tnet,&tnode,lastnet);
				switch (rtn)
					{
					case 0:
						len = 0;
						break;
					case 1:
					case 2:
						lastnet = tnet;
						add_path(tm,tnet,tnode);
						if (rtn == 2)
							len = 0;
						else
							cptr += offset;
						break;
					case 3:
					case 4:
						break;
					}
				}
			while (rtn != 0 && rtn != 2 && rtn != 3 && rtn != 4);
			}
		}
	}



