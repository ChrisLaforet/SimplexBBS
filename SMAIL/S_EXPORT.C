/* s_export.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 18 May 1990
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_export.c_v  $
**                       $Date:   25 Oct 1992 14:18:18  $
**                       $Revision:   1.12  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <io.h>
#include "smail.h"



char *months[12] = 
	{
		"Jan","Feb","Mar","Apr",
		"May","Jun","Jul","Aug",
		"Sep","Oct","Nov","Dec",
	};


char *body = NULL;
int cur_body = 0;
int max_body = 0;




unsigned int get_msg_number(char *path)
	{
	struct fi tfi;
	char buffer[120];
	unsigned int tval;
	unsigned int tmin = 0;
	int rtn;

	strcpy(buffer,path);
	if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
		strcat(buffer,P_SSEP);
	strcat(buffer,"*.MSG");

	rtn = get_firstfile(&tfi,buffer);
	while (rtn)
		{
		if (isdigit(tfi.fi_name[0]))
			{
			tval = atoi(tfi.fi_name);
			if (tval > tmin)
				tmin = tval;
			}
		rtn = get_nextfile(&tfi);
		}
	return tmin + 1;
	}



char *make_datetime(DATE_T date,TIME_T time)
	{
	static char buffer[20];

	memset(buffer,0,sizeof(buffer));
	sprintf(buffer,"%02d %s %02d  %02d:%02d:%02d",date & 0x1f,months[((date >> 5) & 0xf) - 1],((date >> 9) + 80) % 100,time >> 11,(time >> 5) & 0x3f,(time & 0x1f) << 1);
	return buffer;
	}



time_t get_actualtime(DATE_T date,TIME_T time)
	{
	struct tm ttm;

	memset(&ttm,0,sizeof(struct tm));
	ttm.tm_year = (date >> 9) + 80;					/* year since 1900 */
	ttm.tm_mon = ((date >> 5) & 0xf) - 1;			/* month with Jan = 0 */
	ttm.tm_mday = date & 0x1f;						/* day of month 1 - 31 */

	ttm.tm_hour = time >> 11;
	ttm.tm_min = (time >> 5) & 0x3f;
	ttm.tm_sec = time & 0x1f;

	return mktime(&ttm);
	}



unsigned long get_msgid(unsigned long suggestion)		/* this way no 2 MSGIDs are the same */
	{
	static unsigned long last_id = 0L;

	if (suggestion == last_id)
		++suggestion;
	last_id = suggestion;
	return suggestion;
	}



void kill_messages(unsigned int start,unsigned int stop)
	{
	char buffer[100];
	unsigned int count;

	for (count = start; count <= stop; count++)
		{
		strcpy(buffer,cfg.cfg_netpath);
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		sprintf(buffer + strlen(buffer),"%u.MSG",count);
		unlink(buffer);
		}
	}



void export_echo(void)
	{
	struct sig *sigptr;
	struct msgh tmsgh;
	struct fm tfm;
	char buffer[120];
	char buffer1[20];
	time_t actual_time;
	unsigned int cur_msg;
	unsigned int this_msg;
	unsigned int start_msg;
	long total;
	long diff;
	int count;
	int kount;
	int counter;
	int width;
	int found;
	int tlen;
	int len;
	int marked;
	FILE *fd;

	fprintf(stderr,"Exporting Echomail......\n");
	cur_msg = get_msg_number(cfg.cfg_netpath);
	start_msg = cur_msg;
	fseek(headfd,0L,SEEK_SET);
	while (fread(&tmsgh,1,sizeof(struct msgh),headfd))
		{
		if (tmsgh.msgh_flags & MSGH_ECHO && tmsgh.msgh_flags & MSGH_ECHO_UNSENT && !(tmsgh.msgh_flags & MSGH_DELETED))
			{
			found = 0;
			marked = 0;
			for (count = 0; count < cur_areas; count++)
				{
				if (areas[count]->area_msgarea != PASSTHRU && areas[count]->area_msgarea == tmsgh.msgh_area)
					{
					found = 1;
					break;
					}
				}

			if (found)
				{
				if (message_limit && cur_msg >= (unsigned int)message_limit)
					{
					limit_flag = 1;
					break;
					}

				if (tmsgh.msgh_length > 0x7fffL)
					found = 0;

				if (found && (unsigned int)tmsgh.msgh_length > max_body)
					{
					max_body = (int)tmsgh.msgh_length;
					if (!(body = realloc(body,max_body * sizeof(char))))
						{
						fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for message body.\n");
						exit(1);
						}
					}
				}

			if (found)
				{
				total = 0L;
				cur_body = 0;

				actual_time = get_actualtime(tmsgh.msgh_date,tmsgh.msgh_time);

				fseek(bodyfd,tmsgh.msgh_offset,SEEK_SET);
				while (total < tmsgh.msgh_length)
					{
					diff = tmsgh.msgh_length - total;
					if (diff >= (long)sizeof(buffer))
						len = sizeof(buffer);
					else
						len = (int)diff;
					fread(buffer,len,1,bodyfd);
					memcpy(body + cur_body,buffer,len);
					cur_body += len;
					total += (long)len;
					}

				if (sigptr = check_signature(count,tmsgh.msgh_to,tmsgh.msgh_from,tmsgh.msgh_subject,body,cur_body))
					{
					fprintf(stderr,"Exporting msg %d from area %s to ",tmsgh.msgh_number,areas[count]->area_name);
					for (kount = 0; kount < areas[count]->area_cur_add; kount++)
						{
						this_msg = cur_msg;
						strcpy(buffer,cfg.cfg_netpath);
						if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
							strcat(buffer,P_SSEP);
						sprintf(buffer + strlen(buffer),"%u.MSG",cur_msg++);
						if (fd = fopen(buffer,"wb+"))
							{
							fprintf(stderr,"%d:%d/%d ",areas[count]->area_add[kount].add_zone,areas[count]->area_add[kount].add_net,areas[count]->area_add[kount].add_node);
							memset(&tfm,0,sizeof(struct fm));
							strncpy(tfm.fm_from,tmsgh.msgh_from,35);
							strncpy(tfm.fm_to,tmsgh.msgh_to,35);
							strcpy(tfm.fm_subject,tmsgh.msgh_subject);
							strcpy(tfm.fm_datetime,make_datetime(tmsgh.msgh_date,tmsgh.msgh_time));
							tfm.fm_szone = areas[count]->area_source.add_zone;
							tfm.fm_snet = areas[count]->area_source.add_net;
							tfm.fm_snode = areas[count]->area_source.add_node;
							tfm.fm_dzone = areas[count]->area_add[kount].add_zone;
							tfm.fm_dnet = areas[count]->area_add[kount].add_net;
							tfm.fm_dnode = areas[count]->area_add[kount].add_node;
							if (!export_routed)
								tfm.fm_flags = 0x82;			/* tell it it is CRASH and Kill/Sent */
							else 
								tfm.fm_flags = 0x80;			/* tell it it is ROUTED and Kill/Sent */

							if (tmsgh.msgh_flags & MSGH_PRIVATE)
								tfm.fm_flags |= 0x1;  		/* set private */

							if (!fwrite(&tfm,sizeof(struct fm),1,fd))
								{
								fclose(fd);
								kill_messages(start_msg,this_msg);
								exit(1);
								}

							sprintf(buffer,"AREA:%s\r",areas[count]->area_name);
							if (!fwrite(buffer,strlen(buffer) * sizeof(char),1,fd))
								{
								fclose(fd);
								kill_messages(start_msg,this_msg);
								exit(1);
								}

							sprintf(buffer,"\x01MSGID: %u:%u/%u %08lx\r",areas[count]->area_source.add_zone,areas[count]->area_source.add_net,areas[count]->area_source.add_node,get_msgid((unsigned long)actual_time));
							if (!fwrite(buffer,strlen(buffer) * sizeof(char),1,fd))
								{
								fclose(fd);
								kill_messages(start_msg,this_msg);
								exit(1);
								}

//							if (!fwrite(body,cur_body * sizeof(char),1,fd))
							if (write_body(body,cur_body,fd))
								{
								fclose(fd);
								kill_messages(start_msg,this_msg);
								exit(1);
								}
							sprintf(buffer,"SEEN-BY: %d/%d",areas[count]->area_source.add_net,areas[count]->area_source.add_node);
							width = strlen(buffer);
							if (!fwrite(buffer,width * sizeof(char),1,fd))
								{
								fclose(fd);
								kill_messages(start_msg,this_msg);
								exit(1);
								}
							for (counter = 0; counter < areas[count]->area_cur_add; counter++)
								{
								sprintf(buffer," %d/%d",areas[count]->area_add[counter].add_net,areas[count]->area_add[counter].add_node);
								tlen = strlen(buffer);
								if ((width + tlen) >= 78)
									{
									if (fputc('\r',fd) == EOF)
										{
										fclose(fd);
										kill_messages(start_msg,this_msg);
										exit(1);
										}
									strcpy(buffer1,"SEEN-BY: ");
									width = strlen(buffer1);
									if (!fwrite(buffer1,width * sizeof(char),1,fd))
										{
										fclose(fd);
										kill_messages(start_msg,this_msg);
										exit(1);
										}
									}

								width += tlen;
								if (!fwrite(buffer,tlen * sizeof(char),1,fd))
									{
									fclose(fd);
									kill_messages(start_msg,this_msg);
									exit(1);
									}
								}
							sprintf(buffer,"\r\x01PATH: %d/%d",areas[count]->area_source.add_net,areas[count]->area_source.add_node);			/* ^APATH: kludge */
							if (!fwrite(buffer,strlen(buffer) * sizeof(char),1,fd))
								{
								fclose(fd);
								kill_messages(start_msg,this_msg);
								exit(1);
								}
							if (fputc('\0',fd) == EOF)		/* NUL terminate the message */
								{
								fclose(fd);
								kill_messages(start_msg,this_msg);
								exit(1);
								}
							++areas[count]->area_forwarded;
							fclose(fd);
							}
						else
							{
							fprintf(stderr,"\n\aError: Unable to open %s\n",buffer);
							exit(1);
							}
						}
					}
				else		/* mark duplicate as sent! */
					{
					fprintf(stderr,"Duplicate msg %d from area %s cancelled.",tmsgh.msgh_number,areas[count]->area_name);
					tmsgh.msgh_flags &= ~MSGH_ECHO_UNSENT;
					fseek(headfd,ftell(headfd) - (long)sizeof(struct msgh),SEEK_SET);
					fwrite(&tmsgh,1,sizeof(struct msgh),headfd);
					fflush(headfd);
					fseek(headfd,0L,SEEK_CUR);
					}
				if (!marked)
					{
					tmsgh.msgh_flags &= ~MSGH_ECHO_UNSENT;
					fseek(headfd,ftell(headfd) - (long)sizeof(struct msgh),SEEK_SET);
					fwrite(&tmsgh,1,sizeof(struct msgh),headfd);
					fflush(headfd);
					fseek(headfd,0L,SEEK_CUR);
					add_signature(count,sigptr);
					marked = 1;
					}
				fprintf(stderr,"\n");
				}
			else
				fprintf(stderr,"Notice: Unable to find an Echomail area for area %d\n",tmsgh.msgh_area);
			}
		}
	}



void export_net(void)
	{
	struct msgh tmsgh;
	struct msgh pmsgh;
	struct msgh nmsgh;
	struct mlink tmlink;
	struct mdata tmdata;
	struct fm tfm;
	struct mc tmc;
	char pathname[120];
	char buffer[120];
	unsigned int cur_msg;
	long offset;
	long total;
	long diff;
	int current = 0;
	int ourmail;
	int renum;
	int count;
	int kount;
	int len;
	int ok;
	FILE *fd;

	fprintf(stderr,"Exporting Netmail......\n");
	cur_msg = get_msg_number(cfg.cfg_netpath);
	fseek(headfd,0L,SEEK_SET);
	while (fread(&tmsgh,1,sizeof(struct msgh),headfd))
		{
		ourmail = 0;
		for (count = 0, ok = 0; count < cur_netmail_areas; count++)
			{
			if (netmail_areas[count].nm_area == tmsgh.msgh_area)
				{
				ok = 1;
				break;
				}
			}
		if (ok)
			{
			if (tmsgh.msgh_flags & MSGH_NET && tmsgh.msgh_flags & MSGH_NET_UNSENT && !(tmsgh.msgh_flags & MSGH_DELETED))
				{
				if (tmsgh.msgh_dnet == net && tmsgh.msgh_dnode == node && ((zone && tmsgh.msgh_dzone && tmsgh.msgh_dzone == zone) || !zone || !tmsgh.msgh_dzone))		/* is this our netmail? */
					ourmail = 1;
				for (kount = 0; kount < 5; kount++)
					{
					if (tmsgh.msgh_dnet == cfg.cfg_akanet[kount] && tmsgh.msgh_dnode == cfg.cfg_akanode[kount] && ((tmsgh.msgh_dzone && cfg.cfg_akazone[kount] && tmsgh.msgh_dzone == cfg.cfg_akazone[kount]) || !tmsgh.msgh_dzone || !cfg.cfg_akazone[kount]))		/* is this our AKA netmail? */
						{
						ourmail = 1;
						break;
						}
					}
				if (!ourmail)
					{
					if (message_limit && cur_msg >= (unsigned int)message_limit)
						{
						limit_flag = 1;
						break;
						}
					strcpy(pathname,cfg.cfg_netpath);
					if (pathname[0] && pathname[strlen(pathname) - 1] != P_CSEP)
						strcat(pathname,P_SSEP);
					sprintf(pathname + strlen(pathname),"%u.MSG",cur_msg++);
					if (fd = fopen(pathname,"wb+"))
						{
						fprintf(stderr,"Exporting Netmail msg %d to %d:%d/%d\n",tmsgh.msgh_number,tmsgh.msgh_dzone,tmsgh.msgh_dnet,tmsgh.msgh_dnode);
						memset(&tfm,0,sizeof(struct fm));
						strncpy(tfm.fm_from,tmsgh.msgh_from,35);
						strncpy(tfm.fm_to,tmsgh.msgh_to,35);
						strcpy(tfm.fm_subject,tmsgh.msgh_subject);
						strcpy(tfm.fm_datetime,make_datetime(tmsgh.msgh_date,tmsgh.msgh_time));
						tfm.fm_szone = netmail_areas[count].nm_zone;
						tfm.fm_snet = netmail_areas[count].nm_net;
						tfm.fm_snode = netmail_areas[count].nm_node;
						tfm.fm_dzone = tmsgh.msgh_dzone;
						tfm.fm_dnet = tmsgh.msgh_dnet;
						tfm.fm_dnode = tmsgh.msgh_dnode;
						tfm.fm_flags = 0x80;		/* set Kill/Sent flag */
						if (tmsgh.msgh_flags & MSGH_PRIVATE)
							tfm.fm_flags |= 0x1;  		/* set private */
						if (tmsgh.msgh_flags & NET_DIRECT)
							tfm.fm_flags |= 0x2;  		/* set crash */
						else if (tmsgh.msgh_flags & NET_HOLD)
							tfm.fm_flags |= 0x200; 		/* set hold-for-pickup */
						if (tmsgh.msgh_flags & NET_FILEATTACH)
							tfm.fm_flags |= 0x10;  		/* set fileattach - Simplex has already placed file path in route file */
						tfm.fm_flags |= 0x100;		/* set local - shows other mail handlers this originated here */

						if (!fwrite(&tfm,sizeof(struct fm),1,fd))
							{
							fclose(fd);
							unlink(pathname);
							exit(1);
							}


						if (zone)			/* put international statement regardless */
							{
							sprintf(buffer,"\x01INTL %u:%u/%u %u:%u/%u\r",tmsgh.msgh_dzone,tmsgh.msgh_dnet,tmsgh.msgh_dnode,tmsgh.msgh_szone,tmsgh.msgh_snet,tmsgh.msgh_snode);		/* ^AINTL: information */
							if (!fwrite(buffer,strlen(buffer) * sizeof(char),1,fd))
								{
								fclose(fd);
								unlink(pathname);
								exit(1);
								}
							}

						sprintf(buffer,"\x01MSGID: %u:%u/%u %08lx\r",tmsgh.msgh_szone,tmsgh.msgh_snet,tmsgh.msgh_snode,get_msgid((unsigned long)get_actualtime(tmsgh.msgh_date,tmsgh.msgh_time)));

						if (!fwrite(buffer,strlen(buffer) * sizeof(char),1,fd))
							{
							fclose(fd);
							unlink(pathname);
							exit(1);
							}

						cur_body = 0;
						if ((int)tmsgh.msgh_length > max_body)
							{
							max_body = (int)tmsgh.msgh_length;
							if (!(body = realloc(body,max_body * sizeof(char))))
								{
								fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for message body.\n");
								fclose(fd);
								unlink(pathname);
								exit(1);
								}
							}

						total = 0L;
						fseek(bodyfd,tmsgh.msgh_offset,SEEK_SET);
						while (total < tmsgh.msgh_length)
							{
							diff = tmsgh.msgh_length - total;
							if (diff >= (long)sizeof(buffer))
								len = sizeof(buffer);
							else
								len = (int)diff;
							fread(buffer,len,1,bodyfd);
							memcpy(body + cur_body,buffer,len);
							cur_body += len;
							total += (long)len;
							}

						if (write_body(body,cur_body,fd))
							{
							fclose(fd);
							unlink(pathname);
							exit(1);
							}

						if (fputc('\0',fd) == EOF)			/* NUL terminate the message */
							{
							fclose(fd);
							unlink(pathname);
							exit(1);
							}

						tmsgh.msgh_flags &= ~MSGH_NET_UNSENT;
						offset = ftell(headfd) - (long)sizeof(struct msgh);
						if (tmsgh.msgh_flags & NET_KILLSENT)
							{
							fseek(datafd,0L,SEEK_SET);
							fread(&tmdata,sizeof(struct mdata),1,datafd);

							/* forge message links first */
							if (tmsgh.msgh_prev)
								{
								if (tmsgh.msgh_prev > 0 && tmsgh.msgh_prev <= tmdata.mdata_msgs)
									{
									fseek(headfd,(long)(tmsgh.msgh_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
									fread(&pmsgh,1,sizeof(struct msgh),headfd);
									pmsgh.msgh_next = tmsgh.msgh_next;		/* may be valid or 0 */
									fseek(headfd,(long)(tmsgh.msgh_prev - 1) * (long)sizeof(struct msgh),SEEK_SET);
									fwrite(&pmsgh,1,sizeof(struct msgh),headfd);
									}
								}
							if (tmsgh.msgh_next)
								{
								if (tmsgh.msgh_next > 0 && tmsgh.msgh_next <= tmdata.mdata_msgs)
									{
									fseek(headfd,(long)(tmsgh.msgh_next - 1) * (long)sizeof(struct msgh),SEEK_SET);
									fread(&nmsgh,1,sizeof(struct msgh),headfd);
									nmsgh.msgh_prev = tmsgh.msgh_prev;		/* may be valid of 0 */
									fseek(headfd,(long)(tmsgh.msgh_next - 1) * (long)sizeof(struct msgh),SEEK_SET);
									fwrite(&nmsgh,1,sizeof(struct msgh),headfd);
									}
								}
							renum = tmsgh.msgh_number;		/* save for later use */

							tmsgh.msgh_flags |= MSGH_DELETED;
							tmsgh.msgh_number = 0;
							tmsgh.msgh_prev = 0;
							tmsgh.msgh_next = 0;
							}
						fseek(headfd,offset,SEEK_SET);
						fwrite(&tmsgh,1,sizeof(struct msgh),headfd);
						fflush(headfd);
						fseek(headfd,0L,SEEK_CUR);		/* ready for next read */

						if (tmsgh.msgh_flags & NET_KILLSENT)
							{
							offset = ftell(headfd);

							fseek(linkfd,(long)current * (long)sizeof(struct mlink),SEEK_SET);
							fread(&tmlink,sizeof(struct mlink),1,linkfd);
							tmlink.mlink_flags |= MSGH_DELETED;
							fseek(linkfd,(long)current * (long)sizeof(struct mlink),SEEK_SET);
							fwrite(&tmlink,sizeof(struct mlink),1,linkfd);
							fflush(linkfd);

							if (tmdata.mdata_del)
								{
								--tmdata.mdata_del;
								fseek(datafd,0L,SEEK_SET);
								fwrite(&tmdata,sizeof(struct mdata),1,datafd);
								}

							while (fread(&tmc,sizeof(struct mc),1,datafd))
								{
								if (tmc.mc_area == tmsgh.msgh_area)
									{
									--tmc.mc_msgs;
									fseek(datafd,ftell(datafd) - (long)sizeof(struct mc),SEEK_SET);
									fwrite(&tmc,sizeof(struct mc),1,datafd);
									break;
									}
								}
							fflush(datafd);

							kount = (int)(offset / (long)sizeof(struct msgh));
							fseek(linkfd,(long)(kount - 1) * (long)sizeof(struct mlink),SEEK_SET);
							while (fread(&tmlink,sizeof(struct mlink),1,linkfd))
								{
								if (tmlink.mlink_area == (int)cur_msg && !(tmlink.mlink_flags & MSGH_DELETED))
									{
									fseek(headfd,(long)(kount - 1) * (long)sizeof(struct msgh),SEEK_SET);
									fread(&nmsgh,sizeof(struct msgh),1,headfd);
									fseek(headfd,(long)(kount - 1) * (long)sizeof(struct msgh),SEEK_SET);
									nmsgh.msgh_number = renum;
									fwrite(&nmsgh,sizeof(struct msgh),1,headfd);

									fseek(linkfd,(long)(kount - 1) * (long)sizeof(struct mlink),SEEK_SET);
									tmlink.mlink_number = renum;
									fwrite(&tmlink,sizeof(struct mlink),1,linkfd);
									fseek(linkfd,0L,SEEK_CUR);		/* seek between write and read -- required */
									++renum;
									}
								++kount;
								}
							fflush(headfd);
							fflush(linkfd);
							}
						++netmail_areas[count].nm_forwarded;
						netmail_forwarded = 1;
						fclose(fd);
						}
					else
						{
						fprintf(stderr,"\n\aError: Unable to open %s\n",pathname);
						exit(1);
						}
					fseek(headfd,0L,SEEK_CUR);
					}
				else
					{
					tmsgh.msgh_flags &= ~MSGH_NET_UNSENT;			/* mark it sent -- it is to us anyway! */
					fseek(headfd,ftell(headfd) - (long)sizeof(struct msgh),SEEK_SET);
					fwrite(&tmsgh,1,sizeof(struct msgh),headfd);
					fseek(headfd,0L,SEEK_CUR);
					}
				}
			}
		++current;
		}
	}



int forward_msg(unsigned int *msgnum,struct msgh *tmsgh,char *area,struct m *tm)
	{
	struct fm tfm;
	char pathname[120];
	char buffer[100];
	char buffer1[20];
	char tchar;
	DATE_T dte;
	TIME_T tme;
	int lastnet = 0;
	int count;
	int kount;
	int tzone;
	int tnet;
	int tnode;
	int ok;
	FILE *fd;

	if (message_limit && *msgnum >= (unsigned int)message_limit)
		{
		limit_flag = 1;
		return 1;
		}
	strcpy(pathname,cfg.cfg_netpath);
	if (pathname[0] && pathname[strlen(pathname) - 1] != P_CSEP)
		strcat(pathname,P_SSEP);
	sprintf(pathname + strlen(pathname),"%u.MSG",*msgnum);
	*msgnum += 1;
	if (fd = fopen(pathname,"wb+"))
		{
		memset(&tfm,0,sizeof(struct fm));
		strncpy(tfm.fm_from,tmsgh->msgh_from,35);
		strncpy(tfm.fm_to,tmsgh->msgh_to,35);
		strcpy(tfm.fm_subject,tmsgh->msgh_subject);
		strcpy(tfm.fm_datetime,make_datetime(tmsgh->msgh_date,tmsgh->msgh_time));
		tfm.fm_szone = tmsgh->msgh_szone;
		tfm.fm_snet = tmsgh->msgh_snet;
		tfm.fm_snode = tmsgh->msgh_snode;
		tfm.fm_dzone = tmsgh->msgh_dzone;
		tfm.fm_dnet = tmsgh->msgh_dnet;
		tfm.fm_dnode = tmsgh->msgh_dnode;

		if (tmsgh->msgh_flags & MSGH_ECHO)
			{
			if (!export_routed)
				tfm.fm_flags = 0x82;			/* tell it it is CRASH and Kill/Sent */
			else 
				tfm.fm_flags = 0x80;			/* tell it it is ROUTED and Kill/Sent */

			if (tmsgh->msgh_flags & MSGH_PRIVATE)
				tfm.fm_flags |= 0x1;
			}
		else
			{
			for (count = 0, ok = 0; count < cur_netmail_areas; count++)		/* figure which AKA to put on there */
				{
				if (netmail_areas[count].nm_zone == tmsgh->msgh_dzone)
					{
					tzone = netmail_areas[count].nm_zone;
					tnet = netmail_areas[count].nm_net;
					tnode = netmail_areas[count].nm_node;
					ok = 1;
					break;
					}
				}
			if (!ok)
				{
				tzone = zone;
				tnet = net;
				tnode = node;
				}

			tfm.fm_flags = 0x80;			/* tell Kill/Sent */
			if (tmsgh->msgh_flags & MSGH_PRIVATE)
				tfm.fm_flags |= 0x1;
			if (tmsgh->msgh_flags & NET_DIRECT)
				tfm.fm_flags |= 0x2;		/* set crash */
			else if (tmsgh->msgh_flags & NET_HOLD)
				tfm.fm_flags |= 0x200;		/* set hold-for-pickup */
			if (tmsgh->msgh_flags & NET_FILEATTACH)
				tfm.fm_flags |= 0x10;
			}

		if (!fwrite(&tfm,sizeof(struct fm),1,fd))
			{
			fclose(fd);
			unlink(pathname);
			return 1;
			}

		if (tmsgh->msgh_flags & MSGH_ECHO)
			{
			sprintf(buffer,"AREA:%s\r",area);
			if (!fwrite(buffer,strlen(buffer) * sizeof(char),1,fd))
				{
				fclose(fd);
				unlink(pathname);
				return 1;
				}
			}

		if (zone && !(tmsgh->msgh_flags & MSGH_ECHO))		/* put international statement regardless */
			{
			sprintf(buffer,"\x01INTL %u:%u/%u %u:%u/%u\r",tmsgh->msgh_dzone,tmsgh->msgh_dnet,tmsgh->msgh_dnode,tmsgh->msgh_szone,tmsgh->msgh_snet,tmsgh->msgh_snode);		/* ^AINTL: information */
			if (!fwrite(buffer,strlen(buffer) * sizeof(char),1,fd))
				{
				fclose(fd);
				unlink(pathname);
				return 1;
				}
			}

		if (tm->m_curbody)
			{
//			if (!fwrite(tm->m_body,tm->m_curbody,1,fd))
			if (write_body(tm->m_body,tm->m_curbody,fd))
				{
				fclose(fd);
				unlink(pathname);
				return 1;
				}
			}

		if (tmsgh->msgh_flags & MSGH_ECHO)		/* if not netmail */
			{
			if (tm->m_actualseenby)
				{
				buffer[0] = '\0';
				for (kount = 0; kount < tm->m_actualseenby; kount++)
					{
					if (!kount)
						strcpy(buffer,"SEEN-BY:");
					if (lastnet != tm->m_seenby[kount]->sb_net)
						sprintf(buffer1," %u/%u",tm->m_seenby[kount]->sb_net,tm->m_seenby[kount]->sb_node);
					else 
						sprintf(buffer1," %u",tm->m_seenby[kount]->sb_node);
					lastnet = tm->m_seenby[kount]->sb_net;
					if (strlen(buffer) + strlen(buffer1) >= 78)
						{
						strcat(buffer,"\r");
						if (!fwrite(buffer,strlen(buffer),1,fd))
							{
							fclose(fd);
							unlink(pathname);
							return 1;
							}
						sprintf(buffer,"SEEN-BY: %u/%u",tm->m_seenby[kount]->sb_net,tm->m_seenby[kount]->sb_node);
						}
					else
						strcat(buffer,buffer1);
					}
				if (buffer[0])
					{
					strcat(buffer,"\r");
					if (!fwrite(buffer,strlen(buffer),1,fd))
						{
						fclose(fd);
						unlink(pathname);
						return 1;
						}
					}
				}
		
			if (tm->m_actualpath)
				{
				buffer[0] = '\0';
				for (kount = 0; kount < tm->m_actualpath; kount++)
					{
					if (!kount)
						strcpy(buffer,"\r\x01PATH:");
					sprintf(buffer1," %u/%u",tm->m_path[kount]->p_net,tm->m_path[kount]->p_node);
					if (strlen(buffer) + strlen(buffer1) >= 80)
						{
						strcat(buffer,"\r");
						if (!fwrite(buffer,strlen(buffer),1,fd))
							{
							fclose(fd);
							unlink(pathname);
							return 1;
							}
						sprintf(buffer,"\x01PATH:%s",buffer1);
						}
					else
						strcat(buffer,buffer1);
					}
				if (buffer[0])
					{
					strcat(buffer,"\r");
					if (!fwrite(buffer,strlen(buffer),1,fd))
						{
						fclose(fd);
						unlink(pathname);
						return 1;
						}
					}
				}
			}
		else
			{
			fseek(fd,-1L,SEEK_END);
			fread(&tchar,sizeof(char),1,fd);
			fseek(fd,0L,SEEK_END);
			if (tchar != '\r')
				{
				if (fputc('\r',fd) == EOF)		/* we need a CR before the Ctrl-A */
					{
					fclose(fd);
					unlink(pathname);
					return 1;
					}
				}

			load_datetime(&dte,&tme);
			sprintf(buffer,"\x01Via Simplex SMAIL (v %d.%02d) on %u:%u/%u at %s\r",MAJOR_VERSION,MINOR_VERSION,tzone,tnet,tnode,make_datetime(dte,tme));
			if (!fwrite(buffer,strlen(buffer),1,fd))
				{
				fclose(fd);
				unlink(pathname);
				return 1;
				}
			}

		if (fputc('\0',fd) == EOF)		/* NUL terminate the message */
			{
			fclose(fd);
			unlink(pathname);
			return 1;
			}
		fclose(fd);
		}
	else
		{
		fprintf(stderr,"\n\aError: Unable to open %s\n",pathname);
		return 1;
		}

	return 0;
	}



int write_body(char *text,int len,FILE *fd)
	{
	char *cptr = text;
	int rtn = 0;
	int cr = 1;
	int skip = 0;

	while (cptr < (text + len))
		{
		if (*cptr == '\r')
			{
			cr = 1;
			skip = 0;
			}
		else if (cr)
			{
			if ((text + len) >= (cptr + 6))
				{
				if (!strncmp(cptr,"\x01INTL ",6))	/* strip out INTL kludges */
					skip = 1;
				}
			cr = 0;
			}

		if (!skip)
			{
			if (fputc((int)(unsigned char)*cptr,fd) == EOF)
				{
				rtn = 1;
				break;
				}
			}

		++cptr;
		}

	return rtn;
	}
