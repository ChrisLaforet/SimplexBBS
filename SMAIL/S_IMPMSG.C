/* s_impmsg.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 9 August 1992
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_impmsg.c_v  $
**                       $Date:   25 Oct 1992 14:18:10  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include "smail.h"



int import_message(struct msgh *tmsgh,struct m *tm)
	{
	struct mdata tmdata;
	struct mlink tmlink;
	struct mc tmc;
	char buffer[100];
	char buffer1[20];
	char *cptr;
	char *cptr1;
	char sysop[41];
	long total = 0L;
	int lastnet = 0;
	int sysop_name = 0;
	int offset;
	int found = 0;
	int count;
	int len;

	memset(&tmdata,0,sizeof(struct mdata));
	memset(&tmlink,0,sizeof(struct mlink));
	fseek(datafd,0L,SEEK_SET);
	if (!fread(&tmdata,sizeof(struct mdata),1,datafd))
		{
		tmdata.mdata_msgs = 0;
		tmdata.mdata_del = 0;
		}
	++tmdata.mdata_msgs;

	if (!stricmp(tmsgh->msgh_to,"sysop") && cfg.cfg_sysopname[0] && sysop_convert)
		{
		strcpy(sysop,tmsgh->msgh_to);
		sysop_name = 1;
		strncpy(tmsgh->msgh_to,cfg.cfg_sysopname,40);		/* substitute in sysop's name if we have it! */
		}

	while (fread(&tmc,sizeof(struct mc),1,datafd))
		{
		if (tmc.mc_area == tmsgh->msgh_area)
			{
			found = 1;
			break;
			}
		}
	if (found)
		{
		fseek(datafd,ftell(datafd) - (long)sizeof(struct mc),SEEK_SET);
		++tmc.mc_msgs;
		}
	else
		{
		fseek(datafd,0L,SEEK_END);
		tmc.mc_area = tmsgh->msgh_area;
		tmc.mc_msgs = 1;
		}
	dataoffset = ftell(datafd);
	fwrite(&tmc,sizeof(struct mc),1,datafd);
	fflush(datafd);

	fseek(bodyfd,0L,SEEK_END);
	bodyoffset = ftell(bodyfd);
	tmsgh->msgh_number = tmc.mc_msgs;		/* start preparing header */
	tmsgh->msgh_offset = ftell(bodyfd);		/* we are positioned, so get position! */
					   	
	if (!quiet_flag)
		fprintf(stderr,"(#%u in %d) ",tmsgh->msgh_number,tmsgh->msgh_area);

	/* ok now to write out and word-wrap message */
	offset = 0;
	cptr = tm->m_body;
	while (cptr < (tm->m_body + tm->m_curbody))
		{
		if (*cptr != '\x8d' && *cptr != '\n' && *cptr != '\r')
			{
			buffer[offset++] = *cptr;

			if (offset >= WRAP)			/* word wrap at 76 */
				{
				cptr1 = buffer + (offset - 1);
				if (isspace(*cptr1))
					{
					*cptr1 = '\r';
					if (!fwrite(buffer,offset,1,bodyfd))
						{
						fseek(bodyfd,bodyoffset,SEEK_SET);
						chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

						fseek(datafd,dataoffset,SEEK_SET);
						fread(&tmc,sizeof(struct mc),1,datafd);
						--tmc.mc_msgs;
						fseek(datafd,dataoffset,SEEK_SET);
						fwrite(&tmc,sizeof(struct mc),1,datafd);

						return 1;
						}
					total += (long)offset;
					offset = 0;
					}
				else
					{
					while (cptr1 > buffer && !isspace(*cptr1))
						--cptr1;
					++cptr1;

					len = offset - (cptr1 - buffer);
					if (len <= 35)				/* 35 character wrap limit */
						{
						if (!fwrite(buffer,offset - len,1,bodyfd))
							{
							fseek(bodyfd,bodyoffset,SEEK_SET);
							chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

							fseek(datafd,dataoffset,SEEK_SET);
							fread(&tmc,sizeof(struct mc),1,datafd);
							--tmc.mc_msgs;
							fseek(datafd,dataoffset,SEEK_SET);
							fwrite(&tmc,sizeof(struct mc),1,datafd);

							return 1;
							}
						total += (long)(offset - len);
						if (fputc('\r',bodyfd) == EOF)
							{
							fseek(bodyfd,bodyoffset,SEEK_SET);
							chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

							fseek(datafd,dataoffset,SEEK_SET);
							fread(&tmc,sizeof(struct mc),1,datafd);
							--tmc.mc_msgs;
							fseek(datafd,dataoffset,SEEK_SET);
							fwrite(&tmc,sizeof(struct mc),1,datafd);

							return 1;
							}
						++total;						/* +1 for \r */
						memmove(buffer,cptr1,len);		/* move the data to the new line */
						offset = len;
						}
					else
						{
						buffer[offset++] = '\r';
						if (!fwrite(buffer,offset,1,bodyfd))
							{
							fseek(bodyfd,bodyoffset,SEEK_SET);
							chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

							fseek(datafd,dataoffset,SEEK_SET);
							fread(&tmc,sizeof(struct mc),1,datafd);
							--tmc.mc_msgs;
							fseek(datafd,dataoffset,SEEK_SET);
							fwrite(&tmc,sizeof(struct mc),1,datafd);

							return 1;
							}
						total += (long)offset;
						offset = 0;
						}
					}
				}
			}
		else if (*cptr == '\r')
			{
			buffer[offset++] = *cptr;
			if (!fwrite(buffer,offset,1,bodyfd))
				{
				fseek(bodyfd,bodyoffset,SEEK_SET);
				chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

				fseek(datafd,dataoffset,SEEK_SET);
				fread(&tmc,sizeof(struct mc),1,datafd);
				--tmc.mc_msgs;
				fseek(datafd,dataoffset,SEEK_SET);
				fwrite(&tmc,sizeof(struct mc),1,datafd);

				return 1;
				}
			total += (long)offset;
			offset = 0;
			}
		++cptr;
		}

	if (offset)			/* do we need to flush buffer */
		{
		if (!fwrite(buffer,offset,1,bodyfd))
			{
			fseek(bodyfd,bodyoffset,SEEK_SET);
			chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

			fseek(datafd,dataoffset,SEEK_SET);
			fread(&tmc,sizeof(struct mc),1,datafd);
			--tmc.mc_msgs;
			fseek(datafd,dataoffset,SEEK_SET);
			fwrite(&tmc,sizeof(struct mc),1,datafd);

			return 1;
			}
		total += (long)offset;
		if (fputc('\r',bodyfd) == EOF)
			{
			fseek(bodyfd,bodyoffset,SEEK_SET);
			chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

			fseek(datafd,dataoffset,SEEK_SET);
			fread(&tmc,sizeof(struct mc),1,datafd);
			--tmc.mc_msgs;
			fseek(datafd,dataoffset,SEEK_SET);
			fwrite(&tmc,sizeof(struct mc),1,datafd);

			return 1;
			}
		++total;
		}

	fflush(bodyfd);

	if (tmsgh->msgh_flags & MSGH_ECHO && keep_path)			/* path and seen-by kludge */
		{
		if (tm->m_actualseenby)
			{
			buffer[0] = '\0';
			for (count = 0; count < tm->m_actualseenby; count++)
				{
				if (!count)
					strcpy(buffer,"\x01SEEN-BY:");
				if (lastnet != tm->m_seenby[count]->sb_net)
					sprintf(buffer1," %u/%u",tm->m_seenby[count]->sb_net,tm->m_seenby[count]->sb_node);
				else 
					sprintf(buffer1," %u",tm->m_seenby[count]->sb_node);
				lastnet = tm->m_seenby[count]->sb_net;
				if (strlen(buffer) + strlen(buffer1) >= 78)
					{
					strcat(buffer,"\r");
					if (!fwrite(buffer,strlen(buffer),1,bodyfd))
						{
						fseek(bodyfd,bodyoffset,SEEK_SET);
						chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

						fseek(datafd,dataoffset,SEEK_SET);
						fread(&tmc,sizeof(struct mc),1,datafd);
						--tmc.mc_msgs;
						fseek(datafd,dataoffset,SEEK_SET);
						fwrite(&tmc,sizeof(struct mc),1,datafd);

						return 1;
						}
					total += (long)strlen(buffer);
					sprintf(buffer,"\x01SEEN-BY: %u/%u",tm->m_seenby[count]->sb_net,tm->m_seenby[count]->sb_node);
					}
				else
					strcat(buffer,buffer1);
				}
			if (buffer[0])
				{
				strcat(buffer,"\r");
				if (!fwrite(buffer,strlen(buffer),1,bodyfd))
					{
					fseek(bodyfd,bodyoffset,SEEK_SET);
					chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

					fseek(datafd,dataoffset,SEEK_SET);
					fread(&tmc,sizeof(struct mc),1,datafd);
					--tmc.mc_msgs;
					fseek(datafd,dataoffset,SEEK_SET);
					fwrite(&tmc,sizeof(struct mc),1,datafd);

					return 1;
					}
				total += (long)strlen(buffer);
				}
			}

		if (tm->m_actualpath)
			{
			buffer[0] = '\0';
			for (count = 0; count < tm->m_actualpath; count++)
				{
				if (!count)
					strcpy(buffer,"\x01PATH:");
				sprintf(buffer1," %u/%u",tm->m_path[count]->p_net,tm->m_path[count]->p_node);
				if (strlen(buffer) + strlen(buffer1) >= 80)
					{
					strcat(buffer,"\r");
					if (!fwrite(buffer,strlen(buffer),1,bodyfd))
						{
						fseek(bodyfd,bodyoffset,SEEK_SET);
						chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

						fseek(datafd,dataoffset,SEEK_SET);
						fread(&tmc,sizeof(struct mc),1,datafd);
						--tmc.mc_msgs;
						fseek(datafd,dataoffset,SEEK_SET);
						fwrite(&tmc,sizeof(struct mc),1,datafd);

						return 1;
						}
					total += (long)strlen(buffer);

					sprintf(buffer,"\x01PATH:%s",buffer1);
					}
				else
					strcat(buffer,buffer1);
				}

			if (buffer[0])
				{
				strcat(buffer,"\r");
				if (!fwrite(buffer,strlen(buffer),1,bodyfd))
					{
					fseek(bodyfd,bodyoffset,SEEK_SET);
					chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

					fseek(datafd,dataoffset,SEEK_SET);
					fread(&tmc,sizeof(struct mc),1,datafd);
					--tmc.mc_msgs;
					fseek(datafd,dataoffset,SEEK_SET);
					fwrite(&tmc,sizeof(struct mc),1,datafd);

					return 1;
					}
				total += (long)strlen(buffer);
				}
			}
		}

	fseek(headfd,0L,SEEK_END);				/* write out header */
	headoffset = ftell(headfd);
	tmsgh->msgh_length = total;
	if (!fwrite(tmsgh,sizeof(struct msgh),1,headfd))
		{
		fseek(headfd,headoffset,SEEK_SET);
		chsize(fileno(headfd),headoffset);		/* hack off changes */

		fseek(bodyfd,bodyoffset,SEEK_SET);
		chsize(fileno(bodyfd),bodyoffset);		/* hack off changes */

		fseek(datafd,dataoffset,SEEK_SET);
		fread(&tmc,sizeof(struct mc),1,datafd);
		--tmc.mc_msgs;
		fseek(datafd,dataoffset,SEEK_SET);
		fwrite(&tmc,sizeof(struct mc),1,datafd);

		return 1;
		}
	fflush(headfd);

	tmlink.mlink_area = tmsgh->msgh_area;
	tmlink.mlink_number = tmsgh->msgh_number;
	tmlink.mlink_number = tmsgh->msgh_flags;
	tmlink.mlink_cksum = 0;

	strcpy(buffer,tmsgh->msgh_to);			/* strip out any "on x:x/x" or "at x:x/x" messages from the checksum */
	cptr = buffer;
	while (*cptr)
		{
		if (!strnicmp(cptr," on ",4) || !strnicmp(cptr," at ",4))
			{
			while (*cptr && cptr != buffer)
				{
				if (!isspace(*cptr))
					{
					++cptr;
					break;
					}
				--cptr;
				}
			*cptr = (char)'\0';
			break;
			}
		++cptr;
		}

	cptr = buffer;
	while (*cptr)
		{
		tmlink.mlink_cksum += toupper(*cptr);
		++cptr;
		}

	fseek(linkfd,0L,SEEK_END);
	linkoffset = ftell(linkfd);

	if (!fwrite(&tmlink,sizeof(struct mlink),1,linkfd))
		{
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

		return 1;
		}
	fflush(linkfd);

	fseek(datafd,0L,SEEK_SET);
	if (!fwrite(&tmdata,sizeof(struct mdata),1,datafd))
		{
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

		return 1;
		}
	if (sysop_name)
		strcpy(tmsgh->msgh_to,sysop);
	return 0;
	}
