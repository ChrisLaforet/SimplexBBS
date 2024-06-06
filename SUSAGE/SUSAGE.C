/* susage.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 11 December 1990
**
** Revision Information: $Logfile:   G:/simplex/susage/vcs/susage.c_v  $
**                       $Date:   25 Oct 1992 14:30:14  $
**                       $Revision:   1.5  $
**
*/



#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <io.h>
#include <string.h>
#include "susage.h"



char bbspath[100];
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



int main(int argc,char *argv[])
	{
	struct ui userinfo;
	struct use usage;
	struct cfg cfg;
	char buffer[100];
	char *cptr;
	long total;
	long hours;
	long minutes;
	int add = 1;
	int percent[24];
	int max_scale;
	int scale;
	int used;
	int diff;
	int max_mins;
	int start;
	int box = 0;
	int count;
	int ok = 0;
	int temp;
	FILE *usefd;
	FILE *fd;
	
	fprintf(stderr,"SUSAGE (v %d.%02d of %s): Simplex Active Usage Chart Generator.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1990-91, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (argc > 1)
		{
		for (count = 1; count < argc; count++)
			{
			cptr = argv[count];
			if (*cptr == '-' || *cptr == '/')
				{
				++cptr;
				if (*cptr == 'G' || *cptr == 'g')
					ok = 1;
				else if (*cptr == 'I' || *cptr == 'i')
					box = 1;
				else
					printf("Error: Invalid command line option \"-%s\"!\n",cptr);
				}
			else
				printf("Error: Invalid command line option \"%s\"!\n",cptr);

			}
		}

	if (ok)
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

		fprintf(stderr,"Opening susage.dat data file....\n");
		sprintf(buffer,"%ssusage.dat",bbspath);
		if (!(usefd = fopen(buffer,"r+b")))
			{
			if (!(usefd = fopen(buffer,"w+b")))
				{
				printf("Fatal Error: Unable to find/open/create susage.dat!\n");
				return 1;
				}
			}

		fprintf(stderr,"Reading records from susage.dat data file....\n");
		fseek(usefd,0L,SEEK_SET);
		if (fread(&usage,sizeof(struct use),1,usefd))
			{
			if (!stricmp(usage.use_name,userinfo.ui_name) && usage.use_date == userinfo.ui_date && usage.use_time == userinfo.ui_time)
				{
				fprintf(stderr,"Skipping duplicate record for \"%s\"....\n",userinfo.ui_name);
				add = 0;
				}
			}
		else
			{
			memset(&usage,0,sizeof(struct use));
			usage.use_firstdate = userinfo.ui_date;
			}

		if (add)
			{
			strcpy(usage.use_name,userinfo.ui_name);
			usage.use_date = userinfo.ui_date;
			usage.use_time = userinfo.ui_time;
			start = ((unsigned int)userinfo.ui_time >> 11);
			max_mins = (userinfo.ui_time >> 5) & 63;
			used = 0;
			if (!userinfo.ui_online)
				++userinfo.ui_online;		/* if set to 0 minutes, credit one minute! */

			while (used < userinfo.ui_online)
				{
				diff = userinfo.ui_online - used;
				if (max_mins <= diff)
					{
					usage.use_hour[start] += (long)max_mins;
					used += max_mins;
					}
				else
					{
					usage.use_hour[start] += (long)diff;
					used += diff;
					}
				max_mins = 60;
				++start;
				if (start >= 24)
					start = 0;
				}
			fseek(usefd,0L,SEEK_SET);
			fwrite(&usage,sizeof(struct use),1,usefd);
			}
		fclose(usefd);

		total = 0L;
		for (count = 0; count < 24; count++)
			total += usage.use_hour[count];

		max_scale = 0;
		for (count = 0; count < 24; count++)
			{
			percent[count] = (int)(usage.use_hour[count] * 100L / total);
			if (percent[count] > max_scale)
				max_scale = percent[count];
			}

		if (max_scale > 80)
			scale = 5;
		else if (max_scale > 60)
			scale = 4;
		else if (max_scale > 40)
			scale = 3;
		else if (max_scale > 20)
			scale = 2;
		else
			scale = 1;

		strcpy(buffer,cfg.cfg_screenpath);
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		fprintf(stderr,"Generating susage.ans usage ansi file....\n");
		strcat(buffer,"susage.ans");
		if (fd = fopen(buffer,"w+b"))
			{
			chsize(fileno(fd),0L);
			temp = ((usage.use_firstdate >> 5) & 0xf) - 1;
			if (temp && (temp >= 12 || temp < 0))
					temp = 11;
			fprintf(fd,"[2J[0;33;1mChart of Average Usage since %s %u, %u %s\r\n",months[temp],usage.use_firstdate & 0x1f,(((unsigned int)usage.use_firstdate >> 9) + 80) % 100 + 1900,cfg.cfg_bbsname[0] ? "for" : "");
			if (cfg.cfg_bbsname[0])
				fprintf(fd,"[36;1m%s\r\n",cfg.cfg_bbsname);
			else 
				fprintf(fd,"\r\n");
			hours = total / 60L;
			minutes = total % 60L;
			fprintf(fd,"[37;1mChart's 100%% is equal to %lu hour%s %lu minute%s of use\r\n",hours,hours == 1 ? "" : "s",minutes,minutes == 1 ? "" : "s");
			temp = 20;
			while (temp > 0)
				{
				sprintf(buffer,"[32;1m");
				max_scale = temp * scale;
				switch (temp)
					{
					case 20:
						sprintf(buffer + strlen(buffer),"%3d%% %c",max_scale,box ? 'П' : '+');
						break;
					case 15:
					case 10:
					case 5:
						sprintf(buffer + strlen(buffer),"%3d%% %c",max_scale,box ? 'Д' : '+');
						break;
					default:					  
						sprintf(buffer + strlen(buffer),"     %c",box ? 'Г' : '|');
						break;
					}
				sprintf(buffer + strlen(buffer),"[31;1m");
				for (count = 0; count < 24; count++)
					{
					if (percent[count] >= max_scale)
						sprintf(buffer + strlen(buffer)," %s",box ? "лл" : "**");
					else
						sprintf(buffer + strlen(buffer),"   ");
					}

				cptr = buffer;		/* strip useless spaces */
				while (*cptr)
					++cptr;
				while (*(unsigned char *)cptr <= ' ')
					--cptr;
				++cptr;
				*cptr = '\0';
				fprintf(fd,"%s\r\n",buffer);

				--temp;
				}
			fprintf(fd,"[32;1m");		
			fprintf(fd,"  0%% %c",box ? 'С' : '+');
			for (count = 0; count < 72; count++)
				fprintf(fd,"%c",box ? 'Ф' : '-');
			fprintf(fd,"\r\n");
			fprintf(fd,"[35;1m");
			fprintf(fd,"       12  1  2  3  4  5  6  7  8  9 10 11 12  1  2  3  4  5  6  7  8  9 10 11\r\n");
			fprintf(fd,"[0;35m");
			fprintf(fd,"       +------------ Morning ------------+ +------------ Evening ------------+\r\n");
			fprintf(fd,"[0;36mPress [36;1;5mENTER [0;36mto continue...[K`4\r\n");

			fclose(fd);
			}
		else
			printf("Error: Unable to open susage.ans file.\n");

		strcpy(buffer,cfg.cfg_screenpath);
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		fprintf(stderr,"Generating susage.asc usage ascii file....\n");
		strcat(buffer,"susage.asc");
		if (fd = fopen(buffer,"w+b"))
			{
			chsize(fileno(fd),0L);
			temp = ((usage.use_firstdate >> 5) & 0xf) - 1;
			if (temp && (temp >= 12 || temp < 0))
					temp = 11;
			fprintf(fd,"Chart of Average Usage since %s %u, %u %s\r\n",months[temp],usage.use_firstdate & 0x1f,(((unsigned int)usage.use_firstdate >> 9) + 80) % 100 + 1900,cfg.cfg_bbsname[0] ? "for" : "");
			if (cfg.cfg_bbsname[0])
				fprintf(fd,"%s\r\n",cfg.cfg_bbsname);
			else
				fprintf(fd,"\r\n");
			fprintf(fd,"Chart's 100%% is equal to %lu hour%s %lu minute%s of use\r\n",hours,hours == 1 ? "" : "s",minutes,minutes == 1 ? "" : "s");
			temp = 20;
			while (temp > 0)
				{
				max_scale = temp * scale;
				switch (temp)
					{
					case 20:
						sprintf(buffer,"%3d%% %c",max_scale,box ? 'П' : '+');
						break;
					case 15:
					case 10:
					case 5:
						sprintf(buffer,"%3d%% %c",max_scale,box ? 'Д' : '+');
						break;
					default:					  
						sprintf(buffer,"     %c",box ? 'Г' : '|');
						break;
					}
				for (count = 0; count < 24; count++)
					{
					if (percent[count] >= max_scale)
						sprintf(buffer + strlen(buffer)," %s",box ? "лл" : "**");
					else
						sprintf(buffer + strlen(buffer),"   ");
					}

				cptr = buffer;		/* strip useless spaces */
				while (*cptr)
					++cptr;
				while (*(unsigned char *)cptr <= ' ')
					--cptr;
				++cptr;
				*cptr = '\0';
				fprintf(fd,"%s\r\n",buffer);

				--temp;
				}
			fprintf(fd,"  0%% %c",box ? 'С' : '+');
			for (count = 0; count < 72; count++)
				fprintf(fd,"%c",box ? 'Ф' : '-');
			fprintf(fd,"\r\n");
			fprintf(fd,"       12  1  2  3  4  5  6  7  8  9 10 11 12  1  2  3  4  5  6  7  8  9 10 11\r\n");
			fprintf(fd,"       +------------ Morning ------------+ +------------ Evening ------------+\r\n");
			fprintf(fd,"Press ENTER to continue...`4\r\n");

			fclose(fd);
			}
		else
			printf("Error: Unable to open susage.asc file.\n");
		fprintf(stderr,"Finished!\n");
		}
	else
		{
		printf("Usage is: SUSAGE -G [-I]\n");
		printf("   where  -G generates usage files.\n");
		printf("     and  -I uses IBM box characters.\n\n");
		}
	return 0;
	}


#if 0

Chart of Average Usage since xx/xx/xx for
"The Programmer's Oasis BBS"

100% +
     |
     |
     |
     |
 75% +
     |
     |
     |
     |
 50% +
     |						xx
     | xx					xx
     | xx					xx
     | xx xx				xx
 25% + xx xx				xx
     | xx xx				xx
     | xx xx				xx
     | xx xx xx				xx
     | xx xx xx				xx
  0% +------------------------------------------------------------------------
       12  1  2  3  4  5  6  7  8  9 10 11 12  1  2  3  4  5  6  7  8  9 10 11
	   +------------ Morning ------------+ +------------ Afternoon ----------+

Press ENTER to continue....

#endif
