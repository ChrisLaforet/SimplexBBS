/* srecent.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 10 June 1990
**
** Revision Information: $Logfile:   G:/simplex/srecent/vcs/srecent.c_v  $
**                       $Date:   25 Oct 1992 14:29:10  $
**                       $Revision:   1.6  $
**
*/



#include <stdio.h>
#include <ctype.h>
#include <io.h>
#include <stdlib.h>
#include <direct.h>
#include <string.h>
#include "srecent.h"



char bbspath[120];
struct cfg cfg;
struct ui userinfo;
FILE *srfd;
int total = TOTAL;
struct sr **recent = NULL;
int cur_recent = 0;
int max_recent = 0;
char **unlisted = NULL;
int cur_unlisted = 0;
int max_unlisted = 0;
char *months[12] =
	{
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December",
	};
char *days[7] =
	{
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",
	};
int day_table[12] =
	{
	31,28,31,30,31,30,
	31,31,30,31,30,31
	};
int jan1[100] =			/* day of week of Jan 1 from 1980 onwards */
	{
	2,4,5,6,0,2,3,4,5,0,1,2,3,5,6,
	0,1,3,4,5,6,1,2,3,4,6,0,1,2,4,
	5,6,0,2,3,4,5,0,1,2,3,5,6,0,1,
	3,4,5,6,1,2,3,4,6,0,1,2,4,5,6,
	0,2,3,4,5,0,1,2,3,5,6,0,1,3,4,
	5,6,1,2,3,4,6,0,1,2,4,5,6,0,2,
	3,4,5,0,1,2,3,5,6,0,
	};




int date_to_day(int date)
	{
	int days = 0;
	int count;
	int leap = 0;
	int year;
	int month;

	year = ((unsigned int)date >> 9) + 1980;
	if (!(year % 4))
		leap = 1;
	year -= 1980;

	month = (date >> 5) & 0xf;
	for (count = 1; count < month; count++)
		days += day_table[count - 1];
	if ((month > 2) && leap)
		++days;

	days += (date & 0x1f) - 1;
	days += jan1[year];
	return days % 7;
	}



int comp(struct sr **arg1,struct sr **arg2)		/* sort in reverse order */
	{
	if ((unsigned int)(*arg1)->sr_date < (unsigned int)(*arg2)->sr_date)
		return 1;
	if ((unsigned int)(*arg1)->sr_date > (unsigned int)(*arg2)->sr_date)
		return -1;
	if ((unsigned int)(*arg1)->sr_time < (unsigned int)(*arg2)->sr_time)
		return 1;
	if ((unsigned int)(*arg1)->sr_time > (unsigned int)(*arg2)->sr_time)
		return -1;
	return 0;
	}



int main(int argc,char *argv[])
	{
	char buffer[100];
	struct sr tsr;
	char *cptr;
	FILE *fd;
	int count;
	int kount;
	int len;
	int date;
	int temp;
	int add = 1;

	fprintf(stderr,"SRECENT (v %d.%02d of %s): Simplex Recent Callers Screen Generator.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1990-91, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (argc > 1)
		{
		if (cptr = getenv("SIMPLEX"))		/* now for our path */
			strcpy(buffer,cptr);
		else
			getcwd(buffer,sizeof(buffer));
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		sprintf(bbspath,buffer);

		strcat(buffer,"config.bbs");
		if (!(fd = fopen(buffer,"rb")))
			{
			printf("Fatal Error: Unable to find/open config.bbs!\n");
			return 1;
			}
		fprintf(stderr,"Reading config.bbs....\n");
		if (!fread(&cfg,sizeof(struct cfg),1,fd))
			{
			printf("Fatal Error: Unable to read from config.bbs!\n");
			return 1;
			}
		fclose(fd);

		sprintf(buffer,"%suserinfo.bbs",bbspath);
		if (!(fd = fopen(buffer,"rb")))
			{
			printf("Fatal Error: Unable to find/open userinfo.bbs!\n");
			return 1;
			}
		fprintf(stderr,"Reading userinfo.bbs....\n");
		if (!fread(&userinfo,sizeof(struct ui),1,fd))
			{
			printf("Fatal Error: Unable to read from userinfo.bbs!\n");
			return 1;
			}
		fclose(fd);

		fprintf(stderr,"Opening srecent.dat data file....\n");
		sprintf(buffer,"%ssrecent.dat",bbspath);
		if (!(srfd = fopen(buffer,"r+b")))
			{
			if (!(srfd = fopen(buffer,"w+b")))
				{
				printf("Fatal Error: Unable to find/open/create srecent.dat!\n");
				return 1;
				}
			}

		sprintf(buffer,"%ssrecent.cfg",bbspath);
		if (fd = fopen(buffer,"r+b"))
			{
			fprintf(stderr,"Opening srecent.cfg data file and reading unlisted user names....\n");
			while (fgets(buffer,sizeof(buffer),fd))
				{
				cptr = buffer;
				while (*cptr && *cptr != '\r' && *cptr != '\n')
					++cptr;
				*cptr = '\0';
				if (cptr != buffer)
					{
					while (cptr != buffer && isspace(*cptr))
						--cptr;
					if (!isspace(*cptr))
						++cptr;
					*cptr = '\0';
					}
				if (strlen(buffer))
					{
					if (cur_unlisted >= max_unlisted)
						{
						if (!(unlisted = realloc(unlisted,(max_unlisted += 10) * sizeof(char *))))
							{
							printf("Fatal Error: Out of memory....\n");
							return 1;
							}
						}
					if (!(unlisted[cur_unlisted] = calloc(strlen(buffer) + 1,sizeof(char))))
						{
						printf("Fatal Error: Out of memory....\n");
						return 1;
						}
					strcpy(unlisted[cur_unlisted],buffer);
					++cur_unlisted;
					}
				}
			fclose(fd);
			}

		total = atoi(argv[1]);
		if (total < 2 || total > 100)
			total = TOTAL;

		fprintf(stderr,"Reading records from srecent.dat data file....\n");
		fseek(srfd,0L,SEEK_SET);
		while (fread(&tsr,sizeof(struct sr),1,srfd))
			{
			if (!stricmp(tsr.sr_name,userinfo.ui_name))
				{
				if (tsr.sr_date == userinfo.ui_date)
					{
					if (tsr.sr_time == userinfo.ui_time)
						{
						printf("User \"%s\" login is already in the database....\n",userinfo.ui_name);
						add = 0;
						}
					}
				}
			if (cur_recent >= max_recent)
				{
				if (!(recent = realloc(recent,(max_recent += 10) * sizeof(struct sr *))))
					{
					printf("Fatal Error: Out of memory....\n");
					return 1;
					}
				}
			if (!(recent[cur_recent] = calloc(1,sizeof(struct sr))))
				{
				printf("Fatal Error: Out of memory....\n");
				return 1;
				}
			memcpy(recent[cur_recent],&tsr,sizeof(struct sr));
			++cur_recent;
			}

		qsort(recent,cur_recent,sizeof(struct sr *),comp);

		if (add)
			{
			fprintf(stderr,"Checking last username against unlisted user names....\n");
			for (count = 0; count < cur_unlisted; count++)
				{
				if (!stricmp(unlisted[count],userinfo.ui_name))
					{
					fprintf(stderr,"Skipping adding \"%s\"....\n",userinfo.ui_name);
					add = 0;
					break;
					}
				}
			}

		if (add)
			{
			fprintf(stderr,"Adding record to srecent.dat data file....\n");
			if (cur_recent > (total - 1))
				{
				--cur_recent;
				free(recent[cur_recent]);
				}
		
			if (cur_recent >= max_recent)
				{
				if (!(recent = realloc(recent,(max_recent += 10) * sizeof(struct sr *))))
					{
					printf("Fatal Error: Out of memory....\n");
					return 1;
					}
				}
			if (!(recent[cur_recent] = calloc(1,sizeof(struct sr))))
				{
				printf("Fatal Error: Out of memory....\n");
				return 1;
				}
			strcpy(recent[cur_recent]->sr_name,userinfo.ui_name);
			recent[cur_recent]->sr_date = userinfo.ui_date;
			recent[cur_recent]->sr_time = userinfo.ui_time;
			++cur_recent;

			qsort(recent,cur_recent,sizeof(struct sr *),comp);	/* sort again */

			fseek(srfd,0L,SEEK_SET);
			chsize(fileno(srfd),0L);
			for (count = 0; count < cur_recent; count++)
				fwrite(recent[count],sizeof(struct sr),1,srfd);
			fclose(srfd);
			}

		date = -1;
		strcpy(buffer,cfg.cfg_screenpath);
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		fprintf(stderr,"Generating srecent.ans recent callers ansi file....\n");
		strcat(buffer,"srecent.ans");
		if (fd = fopen(buffer,"w+b"))
			{
			chsize(fileno(fd),0L);
			fprintf(fd,"[0m[2J[0;37;41;1mÚ");
			for (count = 0; count < 77; count++)
				fprintf(fd,"Ä");
			fprintf(fd,"¿[0m[K\r\n[37;41;1m³");
			sprintf(buffer,"Callers to %s",cfg.cfg_bbsname);

			len = (77 - strlen(buffer)) >> 1;
			fprintf(fd,"[0;33;41;1m");
			for (count = 0; count < len; count++)
				fprintf(fd," ");
			fprintf(fd,buffer);
			for (count = len + strlen(buffer); count < 77; count++)
				fprintf(fd," ");

			fprintf(fd,"[0;37;41;1m³[0m[K\r\n[37;41;1mÀ");
			for (count = 0; count < 77; count++)
				fprintf(fd,"Ä");
			fprintf(fd,"Ù\r\n");

			fprintf(fd,"[0;37;44;1mÚ");
			for (count = 0; count < 77; count++)
				fprintf(fd,"Ä");
			fprintf(fd,"¿\r\n");
			for (count = 0; count < cur_recent; count++)
				{
				fprintf(fd,"³");
				if (date != recent[count]->sr_date)
					{
					for (kount = 0; kount < 77; kount++)
						fprintf(fd," ");
					fprintf(fd,"³[0m[K\r\n[37;44;1m³[0;32;44;1m");
					date = recent[count]->sr_date;
					temp = ((date >> 5) & 0xf) - 1;
					if (temp && (temp >= 12 || temp < 0))
						temp = 0;
					sprintf(buffer,"%s, %s %d, %d",days[date_to_day(date)],months[temp],date & 0x1f,((unsigned int)date >> 9) + 1980);
					len = (77 - strlen(buffer)) >> 1;
					for (kount = 0; kount < len; kount++)
						fprintf(fd," ");
					fprintf(fd,buffer);
					for (kount = len + strlen(buffer); kount < 77; kount++)
						fprintf(fd," ");
					fprintf(fd,"[0;37;44;1m³[0m[K\r\n[0;37;44;1m³");
					}
				temp = (unsigned int)recent[count]->sr_time >> 11;
				fprintf(fd,"[0;36;44;1m  %2d:%02d %s",(temp + 11) % 12 + 1,(recent[count]->sr_time >> 5) & 0x3f,temp >= 12 ? "pm" : "am");
				temp = count % 3;
				fprintf(fd,"[0;%d;44;1m ",temp == 1 ? 33 : temp ? 35 : 37);
				fprintf(fd,"%-25.25s  ",recent[count]->sr_name);
				if ((count + 1) < cur_recent && date == recent[count + 1]->sr_date)
					{
					++count;
					temp = (unsigned int)recent[count]->sr_time >> 11;
					fprintf(fd,"[0;36;44;1m  %2d:%02d %s",(temp + 11) % 12 + 1,(recent[count]->sr_time >> 5) & 0x3f,temp >= 12 ? "pm" : "am");
					temp = count % 3;
					fprintf(fd,"[0;%d;44;1m ",temp == 1 ? 33 : temp ? 35 : 37);
					fprintf(fd,"%-25.25s   ",recent[count]->sr_name);
					}
				else
					{
					for (kount = 0; kount < 39; kount++)
						fprintf(fd," ");
					}
				fprintf(fd,"[0;37;44;1m³[0m[K\r\n[37;44;1m");
				}
			fprintf(fd,"³");
			for (count = 0; count < 77; count++)
				fprintf(fd," ");
			fprintf(fd,"³[0m[K\r\n[37;44;1mÀ");
			for (count = 0; count < 77; count++)
				fprintf(fd,"Ä");
			fprintf(fd,"Ù\r\n[0;31;1mPress [0;36;1;5mENTER [0;31;1mto continue...[K`4\r\n");
			fclose(fd);
			}
		else
			printf("Error: Unable to open srecent.ans....\n");


		date = -1;
		strcpy(buffer,cfg.cfg_screenpath);
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		fprintf(stderr,"Generating srecent.asc recent callers ascii file....\n");
		strcat(buffer,"srecent.asc");
		if (fd = fopen(buffer,"w+b"))
			{
			chsize(fileno(fd),0L);
			fprintf(fd,"Ú");
			for (count = 0; count < 77; count++)
				fprintf(fd,"Ä");
			fprintf(fd,"¿\r\n³");
			sprintf(buffer,"Callers to %s",cfg.cfg_bbsname);

			len = (77 - strlen(buffer)) >> 1;
			for (count = 0; count < len; count++)
				fprintf(fd," ");
			fprintf(fd,buffer);
			for (count = len + strlen(buffer); count < 77; count++)
				fprintf(fd," ");

			fprintf(fd,"³\r\nÀ");
			for (count = 0; count < 77; count++)
				fprintf(fd,"Ä");
			fprintf(fd,"Ù\r\n");

			fprintf(fd,"Ú");
			for (count = 0; count < 77; count++)
				fprintf(fd,"Ä");
			fprintf(fd,"¿\r\n");
			for (count = 0; count < cur_recent; count++)
				{
				fprintf(fd,"³");
				if (date != recent[count]->sr_date)
					{
					for (kount = 0; kount < 77; kount++)
						fprintf(fd," ");
					fprintf(fd,"³\r\n³");
					date = recent[count]->sr_date;
					temp = ((date >> 5) & 0xf) - 1;
					if (temp && (temp >= 12 || temp < 0))
						temp = 0;
					sprintf(buffer,"%s, %s %d, %d",days[date_to_day(date)],months[temp],date & 0x1f,((unsigned int)date >> 9) + 1980);
					len = (77 - strlen(buffer)) >> 1;
					for (kount = 0; kount < len; kount++)
						fprintf(fd," ");
					fprintf(fd,buffer);
					for (kount = len + strlen(buffer); kount < 77; kount++)
						fprintf(fd," ");
					fprintf(fd,"³\r\n³");
					}
				temp = (unsigned int)recent[count]->sr_time >> 11;
				fprintf(fd,"  %2d:%02d %s",(temp + 11) % 12 + 1,(recent[count]->sr_time >> 5) & 0x3f,temp >= 12 ? "pm" : "am");
				temp = count % 3;
				fprintf(fd," %-25.25s  ",recent[count]->sr_name);
				if ((count + 1) < cur_recent && date == recent[count + 1]->sr_date)
					{
					++count;
					temp = (unsigned int)recent[count]->sr_time >> 11;
					fprintf(fd,"  %2d:%02d %s",(temp + 11) % 12 + 1,(recent[count]->sr_time >> 5) & 0x3f,temp >= 12 ? "pm" : "am");
					temp = count % 3;
					fprintf(fd," %-25.25s   ",recent[count]->sr_name);
					}
				else
					{
					for (kount = 0; kount < 39; kount++)
						fprintf(fd," ");
					}
				fprintf(fd,"³\r\n");
				}
			fprintf(fd,"³");
			for (count = 0; count < 77; count++)
				fprintf(fd," ");
			fprintf(fd,"³\r\nÀ");
			for (count = 0; count < 77; count++)
				fprintf(fd,"Ä");
			fprintf(fd,"Ù\r\nPress ENTER to continue...`4\r\n");
			fclose(fd);
			}
		else
			printf("Error: Unable to open srecent.asc....\n");
		fprintf(stderr,"Finished!\n");
		}
	else
		printf("Usage is: SRECENT number_of_entries\n\n");
	return 0;
	}




