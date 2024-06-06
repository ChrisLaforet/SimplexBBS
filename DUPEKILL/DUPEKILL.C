/* dupekill.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 27 June 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>



struct ln
	{
	unsigned short ln_chksum;
	char *ln_line;
	char ln_deleted;
	};


struct ln **lines = NULL;
unsigned short max_lines = 0;
unsigned short cur_lines = 0;

char buffer[512];


int main(int argc,char *argv[])
	{
	char *cptr;
	unsigned short in = 0;
	unsigned short out = 0;
	unsigned short chksum;
	unsigned short count;
	unsigned short kount;
	unsigned short matches = 0;
	FILE *fd;

	fprintf(stderr,"DUPEKILL: Kills duplicate lines from logs\n");
	fprintf(stderr,"Written by Chris Laforet, 1993.\n\n");

	if (argc == 2)
		{
		if (fd = fopen(argv[1],"r+b"))
			{
			while (fgets(buffer,sizeof(buffer) - 1,fd))
				{
				buffer[sizeof(buffer) - 1] = (char)'\0';
				cptr = buffer;
				chksum = 0;
				while (*cptr && *cptr != '\r' && *cptr != '\n')
					{
					chksum += (unsigned short)(unsigned char)*cptr;
					++cptr;
					}
				*cptr = (char)'\0';

				if (cur_lines >= max_lines)
					{
					if (!(lines = realloc(lines,(max_lines += 100) * sizeof(struct ln *))))
						{
						fprintf(stderr,"Fatal Error: Unable to allocate memory.\n");
						return 1;
						}
					}

				if (!(lines[cur_lines] = malloc(sizeof(struct ln))))
					{
					fprintf(stderr,"Fatal Error: Unable to allocate memory.\n");
					return 1;
					}

				if (!(lines[cur_lines]->ln_line = malloc(strlen(buffer) + 1)))
					{
					fprintf(stderr,"Fatal Error: Unable to allocate memory.\n");
					return 1;
					}

				strcpy(lines[cur_lines]->ln_line,buffer);
				lines[cur_lines]->ln_deleted = 0;
				lines[cur_lines]->ln_chksum = chksum;
				++cur_lines;

				if (!(in % 100))
					{
					fprintf(stderr,"Read %u\r",in);
					fflush(stderr);
					}
				++in;
				}

			fprintf(stderr,"\rRead %u lines\n",in);
			for (count = 0; count < cur_lines; count++)
				{
				if (!lines[count]->ln_deleted && lines[count]->ln_line[0])		/* not a blank filler line */
					{
					for (kount = count + 1; kount < cur_lines; kount++)
						{
						if (kount != count && !lines[kount]->ln_deleted)
							{
							if (lines[count]->ln_chksum == lines[kount]->ln_chksum)
								{
								if (!strcmp(lines[count]->ln_line,lines[kount]->ln_line))
									{
									lines[kount]->ln_deleted = 1;
									++matches;
									}
								}
							}
						}
					}

				if (!(count % 100))
					{
					fprintf(stderr,"Checked %u\r",count);
					fflush(stderr);
					}
				}

			fprintf(stderr,"\rChecked %u lines and eliminated %u dup%s.\n",cur_lines,matches,(char *)(matches == 1 ? "" : "s"));

			if (matches)
				{
				fseek(fd,0L,SEEK_SET);
				chsize(fileno(fd),0L);

				for (count = 0; count < cur_lines; count++)
					{
					if (!lines[count]->ln_deleted)
						{
						fprintf(fd,"%s\r\n",lines[count]->ln_line);
						++out;
						}

					if (!(out % 100))
						{
						fprintf(stderr,"Wrote %u\r",out);
						fflush(stderr);
						}
					}

				fprintf(stderr,"\rWrote %u lines\n",out);
				}
			else
				fprintf(stderr,"Nothing to write.\n");

			fclose(fd);
			}
		else
			{
			fprintf(stderr,"Error: Unable to open \"%s\".\n",argv[1]);
			return 1;
			}
		}
	else 
		{
		fprintf(stderr,"Usage is: DUPEKILL [filename]\n");
		return 1;
		}
	
	return 0;
	}
