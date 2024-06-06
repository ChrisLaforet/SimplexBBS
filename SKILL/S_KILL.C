/* s_kill.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 16 March 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <io.h>
#include "skill.h"



char **list = NULL;
short cur_list = 0;
short max_list = 0;



short getyn(void)
	{
	int key;

	while (1)
		{
		key = getch();
		if (key == 'Y' || key == 'y')
			{
			fprintf(stderr,"Yes\n");
			return 1;
			}
		else if (key == 'N' || key == 'n')
			{
			fprintf(stderr,"No\n");
			return 0;
			}
		}
	}



void kill_files(struct file *fptr)
	{
	struct fe tfe;
	struct fi tfi;
	char buffer[100];
	char buffer1[100];
	int count;
	short killed = 0;
	short fail = 0;
	short ok;
	FILE *fd;
	FILE *bfd;

	if (fptr->file_descname[0])
		strcpy(buffer,fptr->file_descname);
	else 
		strcpy(buffer,fptr->file_pathname);
	if (buffer[0] && (buffer[strlen(buffer) - 1] != P_CSEP))
		strcat(buffer,P_SSEP);
	strcat(buffer,"filelist.bbs");

	if (fd = fopen(buffer,"rb"))
		{
		while (fread(&tfe,sizeof(struct fe),1,fd))
			{
			if (tfe.fe_dldate <= kill_date)
				{
				strcpy(buffer,fptr->file_pathname);
				if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
					strcat(buffer,P_SSEP);
				strcat(buffer,tfe.fe_name);

				if (get_firstfile(&tfi,buffer))
					{
					ok = 0;
					if (tfi.fi_date <= kill_date)
						{
						if (kill_flag && confirm_flag)
							{
							fprintf(stderr,"Delete \"%s\" [%lu bytes] (y/n)? ",tfe.fe_name,tfi.fi_size);
							if (getyn())
								ok = 1;
							}
						else
							{
							if (kill_flag)
								printf("Deleting \"%s\" [%lu bytes].\n",tfe.fe_name,tfi.fi_size);
							else 
								printf("Would delete \"%s\" [%lu bytes].\n",tfe.fe_name,tfi.fi_size);
							ok = 1;
							}

						if (ok)
							{
							if (cur_list >= max_list)
								{
								if (!(list = realloc(list,(max_list += 50) * sizeof(char *))))
									{
									fprintf(stderr,"Error: Out of memory.\n");
									exit(1);
									}
								}
							if (!(list[cur_list] = malloc(strlen(tfe.fe_name) + 1)))
								{
								fprintf(stderr,"Error: Out of memory.\n");
								exit(1);
								}
							strcpy(list[cur_list],tfe.fe_name);
							++cur_list;
							freed += tfi.fi_size;
							}
						}
					}
				get_closefile();
				}
			}

		if (kill_flag)
			{
			for (count = 0; count < cur_list; count++)
				{
	  			strcpy(buffer,fptr->file_pathname);
				if (buffer[0] && (buffer[strlen(buffer) - 1] != P_CSEP))
					strcat(buffer,P_SSEP);
				strcat(buffer,list[count]);
				if (!unlink(buffer))
					killed = 1;
				else
					list[count][0] = (char)'\0';
				}
			}

		if (killed)
			{
			if (fptr->file_descname[0])
				strcpy(buffer,fptr->file_descname);
			else 
				strcpy(buffer,fptr->file_pathname);
			if (buffer[0] && (buffer[strlen(buffer) - 1] != P_CSEP))
				strcat(buffer,P_SSEP);
			strcat(buffer,"filelist.bak");

			if (bfd = fopen(buffer,"wb"))
				{
				fseek(fd,0L,SEEK_SET);
				while (fread(&tfe,sizeof(struct fe),1,fd))
					{
					ok = 1;
					for (count = 0; count < cur_list; count++)
						{
						if (!stricmp(tfe.fe_name,list[count]))
							{
							ok = 0;
							break;
							}
						}
					if (ok)
						{
						if (!fwrite(&tfe,sizeof(struct fe),1,bfd))
							fail = 1;
						}
					else
						freed += (long)sizeof(struct fe);
					}
				fclose(bfd);
				}
			else
				fprintf(stderr,"Unable to create/open \"filelist.bak\"...\n");
			}

		fclose(fd);
		if (killed && !fail)
			{
			if (fptr->file_descname[0])
				strcpy(buffer,fptr->file_descname);
			else 
				strcpy(buffer,fptr->file_pathname);
			if (buffer[0] && (buffer[strlen(buffer) - 1] != P_CSEP))
				strcat(buffer,P_SSEP);
			strcpy(buffer1,buffer);
			strcat(buffer,"filelist.bbs");
			strcat(buffer1,"filelist.bak");

			unlink(buffer);
			if (rename(buffer1,buffer))
				fprintf(stderr,"Unable to rename \"filelist.bak\" to \"filelist.bbs\".\n");
			}

		for (count = 0; count < cur_list; count++)
			free(list[count]);
		cur_list = 0;
		}
	else
		fprintf(stderr,"Unable to open \"filelist.bbs\"...\n");
	}
