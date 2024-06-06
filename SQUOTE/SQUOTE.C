/* squote.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 7 May 1991
**
** Revision Information: $Logfile:   G:/simplex/squote/vcs/squote.c_v  $
**                       $Date:   25 Oct 1992 14:28:26  $
**                       $Revision:   1.1  $
**
*/


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "squote.h"


#define MAXLEN				512


struct line **lines = NULL;
int cur_lines = 0;
int max_lines = 0;

char buffer[MAXLEN + 1];



int main(int argc,char *argv[])
	{
	struct qh tqh;
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char *cptr;
	long offset;
	int count;
	FILE *sfd;
	FILE *dfd;

	fprintf(stderr,"SQUOTE (v %d.%02d of %s): Simplex Quote File Compiler.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1991-92, Chris Laforet Software and Chris Laforet.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (argc >= 2)
		{
		strlwr(argv[1]);
		_splitpath(argv[1],drive,path,fname,ext);
		strcpy(ext,"q");
		_makepath(buffer,drive,path,fname,ext);
		if (sfd = fopen(buffer,"rb"))
			{
			_makepath(buffer,drive,path,fname,"cq");
			if (dfd = fopen(buffer,"wb"))
				{
				printf("Reading \"%s.q\"...",fname);
				offset = ftell(sfd);
				while (fgets(buffer,MAXLEN,sfd))
					{
					cptr = buffer;
					while (*cptr && !iscntrl(*cptr))
						++cptr;
					if (cptr != buffer)
						{
						*cptr = (char)'\0';
						if (cur_lines >= max_lines)
							{
							if (!(lines = realloc(lines,(max_lines += 50) * sizeof(struct line *))))
								{
								fprintf(stderr,"Fatal Error: Out of memory.\n\n");
								return 1;
								}
							}
						if (!(lines[cur_lines] = malloc(sizeof(struct line))))
							{
							fprintf(stderr,"Fatal Error: Out of memory.\n\n");
							return 1;
							}
						lines[cur_lines]->line_len = strlen(buffer);
						lines[cur_lines]->line_offset = offset;
						++cur_lines;
						}
					offset = ftell(sfd);
					}

				printf("Found %d quote line%s!\nWriting \"%s.cq\"...\n",cur_lines,cur_lines != 1 ? "s" : "",fname);
				if (cur_lines)
					{
					strcpy(tqh.qh_sig,"Simplex Quote");
					tqh.qh_lines = cur_lines;
					fwrite(&tqh,sizeof(struct qh),1,dfd);
					offset = ftell(dfd) + ((long)cur_lines * (long)sizeof(long));
					for (count = 0; count < cur_lines; count++)
						{
						fwrite(&offset,sizeof(long),1,dfd);
						offset += (long)lines[count]->line_len;
						++offset;
						}
					for (count = 0; count < cur_lines; count++)
						{
						fseek(sfd,lines[count]->line_offset,SEEK_SET);
						fread(buffer,lines[count]->line_len,1,sfd);
						buffer[lines[count]->line_len] = (char)'\0';
						fwrite(buffer,lines[count]->line_len + 1,1,dfd);
						}
					}
				printf("Done!\n\n");
				fclose(dfd);
				}
			else 
				fprintf(stderr,"Fatal Error: Unable to open destination file \"%s.cq\"\n",fname);
			fclose(sfd);
			}
		else
			fprintf(stderr,"Fatal Error: Unable to open source file \"%s.q\"\n",fname);
		}
	else
		printf("Usage is: \"SQUOTE source.q\" to compile quote file.\n\n");
	return 0;
	}



