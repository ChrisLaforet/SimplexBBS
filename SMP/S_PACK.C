/* s_pack.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 18 November 1990
**
** Revision Information: $Logfile:   G:/simplex/smp/vcs/s_pack.c_v  $
**                       $Date:   25 Oct 1992 14:26:34  $
**                       $Revision:   1.10  $
**
*/



#include <stdio.h>
#include <direct.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "smp.h"


#define BUFLEN					2048
#define EOF_MARKER				0x1a		/* ctrl-Z */


extern struct tm *tmptr;


char *get_outbound(int tzone)
	{
	static char outpath[100];
	char buffer[5];
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
 	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	strcpy(outpath,cfg.cfg_outboundpath);
	if (outpath[0])
		{
		if (outpath[strlen(outpath) - 1] == P_CSEP)
			outpath[strlen(outpath) - 1] = (char)'\0';
		}
	if (tzone && tzone != cfg.cfg_zone)
		{
		_splitpath(outpath,drive,path,fname,ext);
		sprintf(buffer,"%03x",tzone);
		_makepath(outpath,drive,path,fname,buffer);
		}
	if (outpath[0] && outpath[strlen(outpath) - 1] != P_CSEP)
		strcat(outpath,P_SSEP);
	return outpath;
	}



void packet_mail(void)				/* packetizes *.msg mail */
	{
	struct fm msg;
	struct pfm pmsg;
	struct ph pheader;
	char buffer[100];
	char buffer1[100];
	char buffer2[100];
	char *cptr;
	char *cptr1;
	long lastoffset;
	size_t len;
	int diskerror;
	int count;
	int kount;
	int append;
	int type;
	FILE *pfd;
	FILE *fd;

	for (count = 0; count < cur_mlist; count++)
		{
/*		type = check_action(mlist[count]->mlist_dzone,mlist[count]->mlist_dnet,mlist[count]->mlist_dnode); */
		type = mlist[count]->mlist_type;

		cptr = get_outbound(mlist[count]->mlist_dzone);
		strcpy(buffer,cptr);		/* let's create directory if it doesn't exist! */
		if (buffer[0])
			{
			cptr1 = buffer + (strlen(buffer) - 1);
			if (*cptr1 == P_CSEP)
				*cptr1 = (char)'\0';
			if (access(buffer,0))
				{
				printf("Notice: Attempting to create outbound directory for zone %u\n",mlist[count]->mlist_dzone);
				if (!mkdir(buffer))
					outbound[mlist[count]->mlist_dzone] = (char)1;
				}
			}
		strcpy(buffer,cptr);
		sprintf(buffer + strlen(buffer),"%04x%04x.%s",mlist[count]->mlist_dnet,mlist[count]->mlist_dnode,type == HOLD ? "hut" : (type == DIRECT ? "cut" : "out"));

		append = 0;
		if (pfd = fopen(buffer,"r+b"))
			{
			if (!fread(&pheader,sizeof(struct ph),1,pfd))
				{
				printf("Error: \"%s\" is an invalid packet file...deleting.\n",buffer);
				fclose(pfd);
				pfd = NULL;
				}
			else if (pheader.ph_sig1 != (char)2 || pheader.ph_sig2 != (char)0)
				{
				printf("Error: \"%s\" is an invalid packet file...deleting.\n",buffer);
				fclose(pfd);
				pfd = NULL;
				}
			else
				{
				if (filelength(fileno(pfd)) > sizeof(struct ph))
					{
					fseek(pfd,0L,SEEK_END);
					fseek(pfd,-1L,SEEK_CUR);
					if (!fgetc(pfd))	/* got one nul */
						{
						fseek(pfd,-2L,SEEK_CUR);
						if (!fgetc(pfd))	/* got another nul -- else we are pointing at last nul anyway */
							fseek(pfd,-1L,SEEK_CUR);
						}				
					}
				append = 1;
				}
			}
		if (!append)
			{
			if (!(pfd = fopen(buffer,"w+b")))
				printf("Error: Unable to open \"%s\"...Skipping.\n",buffer);
			else
				{
				memset(&pheader,0,sizeof(struct ph));
				pheader.ph_snode = mlist[count]->mlist_snode;
				pheader.ph_snet = mlist[count]->mlist_snet;
				pheader.ph_dnode = mlist[count]->mlist_dnode;
				pheader.ph_dnet = mlist[count]->mlist_dnet;

				pheader.ph_year = tmptr->tm_year + 1900;
				pheader.ph_month = tmptr->tm_mon;
				pheader.ph_day = tmptr->tm_mday;
				pheader.ph_hour = tmptr->tm_hour;
				pheader.ph_minute = tmptr->tm_min;
				pheader.ph_second = tmptr->tm_sec;

				pheader.ph_sig1 = (char)2;		/* signiture is 02 00 */
				pheader.ph_sig2 = (char)0;

				pheader.ph_baud = 0;
				pheader.ph_code = (char)0x85;		/* product code is hex 85 from Rick Moore on 2/5/91 */

				for (kount = 0; kount < cur_passwords; kount++)
					{
					if (passwords[kount]->pp_net == mlist[count]->mlist_dnet && passwords[kount]->pp_node == mlist[count]->mlist_dnode)
						{
						strncpy(pheader.ph_password,passwords[kount]->pp_password,strlen(passwords[kount]->pp_password));
						break;
						}
					}

				if (!fwrite(&pheader,sizeof(struct ph),1,pfd))
					{
					printf("Error: Unable to write \"%s\"...Skipping.\n",buffer);
					fclose(pfd);
					unlink(buffer);
					pfd = NULL;
					}
				}
			}

		if (pfd)
			{
			diskerror = 0;
			for (kount = 0; kount < mlist[count]->mlist_curmsgs && !diskerror; kount++)
				{
				lastoffset = ftell(pfd);
				switch (type)
					{
					case HOLD:
						cptr = "Hold";
						break;
					case ROUTE:
						cptr = "Route";
						break;
					case DIRECT:
						cptr = "Direct";
						break;
					}
				printf("Pack to %u:%u/%u from %u:%u/%u  Msg %u (%s) %s\r",mlist[count]->mlist_dzone,mlist[count]->mlist_dnet,mlist[count]->mlist_dnode,
					mlist[count]->mlist_szone,mlist[count]->mlist_snet,mlist[count]->mlist_snode,
					mlist[count]->mlist_msgs[kount],cptr,append ? "(Append)" : "");

				strcpy(buffer1,cfg.cfg_netpath);
				if (buffer1[0] && buffer1[strlen(buffer1) - 1] != P_CSEP)
					strcat(buffer1,P_SSEP);
				sprintf(buffer1 + strlen(buffer1),"%u.msg",mlist[count]->mlist_msgs[kount]);

				if (fd = fopen(buffer1,"rb"))
					{
					if (!fread(&msg,sizeof(struct fm),1,fd))
						{
						printf("Error: Unable to read header from \"%u.msg\"...Skipping.\n",mlist[count]->mlist_msgs[kount]);
						fclose(fd);
						}
					else
						{
						memset(&pmsg,0,sizeof(struct pfm));

						pmsg.pfm_sig1 = (char)2;		/* signature is 02 00 */
						pmsg.pfm_sig2 = (char)0;
						pmsg.pfm_flags = msg.fm_flags;
						pmsg.pfm_cost = msg.fm_cost;

						pmsg.pfm_snode = msg.fm_snode;
						pmsg.pfm_snet = msg.fm_snet;

						if (zone && msg.fm_dzone && msg.fm_dzone != zone && msg.fm_dzone != mlist[count]->mlist_dzone && mlist[count]->mlist_intl)
							{
							pmsg.pfm_dnet = mlist[count]->mlist_dnet;
							pmsg.pfm_dnode = mlist[count]->mlist_dnode;
							}
						else
							{
							pmsg.pfm_dnet = msg.fm_dnet;
							pmsg.pfm_dnode = msg.fm_dnode;
							}

						if (!fwrite(&pmsg,sizeof(struct pfm),1,pfd))
							diskerror = 1;

						if (!diskerror)
							{
							if (!fwrite(msg.fm_datetime,20,1,pfd))
								diskerror = 1;
							}

						if (!diskerror)
							{
							strcpy(buffer2,msg.fm_to);
							if (!fwrite(buffer2,strlen(buffer2) + 1,1,pfd))
								diskerror = 1;
							}

						if (!diskerror)
							{
							strcpy(buffer2,msg.fm_from);
							if (!fwrite(buffer2,strlen(buffer2) + 1,1,pfd))
								diskerror = 1;
							}

						if (!diskerror)
							{
							strcpy(buffer2,msg.fm_subject);
							if (!fwrite(buffer2,strlen(buffer2) + 1,1,pfd))
								diskerror = 1;
							}

						while (!diskerror)
							{
							while (len = fread(buffer2,sizeof(char),(size_t)sizeof(buffer2),fd))
								{
								if (fwrite(buffer2,sizeof(char),len,pfd) != len)
									{
									diskerror = 1;
									break;
									}
								}

							if (!len)
								{
								fseek(fd,-1L,SEEK_CUR);
								if (fgetc(fd))			/* was last byte a nul? */
									{
									if (fputc('\x0',pfd) == EOF)			/* output final nul */
										diskerror = 1;
									}
								break;
								}
							}
						fclose(fd);

						if (!diskerror)
							{
							if (unlink(buffer1))		/* delete msg file! */
								printf("Error: Unable to delete \"%u.msg\".\n",mlist[count]->mlist_msgs[kount]);
							}
						else
							{
							printf("Error: Unable to include \"%u.msg\" in packet...Skipping.\n",mlist[count]->mlist_msgs[kount]);
							fseek(pfd,lastoffset,SEEK_SET);
							}
						}
					}
				else
					printf("Error: Unable to open \"%u.msg\"...Skipping.\n",mlist[count]->mlist_msgs[kount]);
				}
			printf("\n");

			fputc('\x0',pfd);			/* output final packet nul-nul */
			fputc('\x0',pfd);
			chsize(fileno(pfd),ftell(pfd));		/* make sure this is the EOF! */
			fclose(pfd);
			}
		}
	printf("\n");
	}



int append_mail(FILE *srcfd,FILE *destfd)		/* copies one packet's mail to another */
	{
	struct ph tph;
	char buffer[BUFLEN];
	long start_offset;
	long offset;
	size_t len;
	int diskerror = 0;

	fseek(srcfd,0L,SEEK_SET);		/* check that the src is a valid packet */
	if (!fread(&tph,1,sizeof(struct ph),srcfd))
		return 1;
	if (tph.ph_sig1 != 2 || tph.ph_sig2 != 0)
		return 1;

	fseek(destfd,0L,SEEK_SET);		/* check that the dest is a valid packet */
	if (!fread(&tph,1,sizeof(struct ph),destfd))
		return 1;
	if (tph.ph_sig1 != 2 || tph.ph_sig2 != 0)
		return 1;

	if (filelength(fileno(destfd)) > (long)sizeof(struct ph))	/* check that dest packet is longer than a header */
		{
		offset = -1L;
		fseek(destfd,offset,SEEK_END);
		while (fgetc(destfd) == '\0')		/* tear off NULs in case */
			{
			--offset;
			fseek(destfd,offset,SEEK_END);
			}

		offset = ftell(destfd);
		chsize(fileno(destfd),offset);

		if (offset > (long)sizeof(struct ph))	/* check that dest packet is longer than a header */
			{
			fseek(destfd,0L,SEEK_END);
			fputc('\x0',destfd);			/* output packed message nul */
			}
		}

	start_offset = ftell(destfd);

	fseek(srcfd,(long)sizeof(struct ph),SEEK_SET);		/* get beyond header and copy packed messages */

	while (!diskerror && (len = fread(buffer,BUFLEN,1,srcfd)))
		{
		if (fwrite(buffer,len,1,destfd) != len)
			diskerror = 1;
		}

	if (diskerror)
		{
		fflush(destfd);
		fseek(destfd,0L,SEEK_SET);
		chsize(fileno(destfd),start_offset);
		fseek(destfd,start_offset,SEEK_SET);
		}

	fputc('\x0',destfd);			/* output final packet nul-nul */
	fputc('\x0',destfd);
	fflush(destfd);
	chsize(fileno(destfd),ftell(destfd));		/* make sure this is the EOF! */
	return diskerror;
	}



int append_outfile(FILE *srcfd,FILE *destfd)		/* copies one routing file to another */
	{
	char buffer[BUFLEN];
	long start_offset;
	long offset;
	int diskerror = 0;
	int inchar;

	fseek(destfd,0L,SEEK_END);
	start_offset = ftell(destfd);
	offset = -1L;
	fseek(destfd,offset,SEEK_END);
	while ((inchar = fgetc(destfd)) == EOF_MARKER)		/* tear off EOF markers in case */
		{
		--offset;
		fseek(destfd,offset,SEEK_END);
		}

	fseek(srcfd,0L,SEEK_SET);
	if (inchar != '\r' && inchar != '\n')		/* make sure we end with newline */
		fputc('\n',destfd);

	while (!diskerror && fgets(buffer,BUFLEN,srcfd))
		{
		if (!fputs(buffer,srcfd))
			diskerror = 1;
		}

	if (diskerror)
		{
		fflush(destfd);
		fseek(destfd,0L,SEEK_SET);
		chsize(fileno(destfd),start_offset);
		fseek(destfd,start_offset,SEEK_SET);
		}
	return diskerror;
	}
