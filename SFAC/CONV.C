/* conv.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 20 June 1992
**
** Revision Information: $Logfile:   G:/simplex/sfac/vcs/conv.c_v  $
**                       $Date:   25 Oct 1992 14:16:32  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <direct.h>
#include <ctype.h>
#include <stdlib.h>
#include <io.h>
#include <string.h>


#define P_CSEP						'\\'	/* character form of path separator */
#define P_SSEP						"\\"	/* string form of path separator */


struct file
	{
	int file_number;				/* file board number */
	char file_areaname[41];			/* file area name */
	char file_pathname[61];			/* path to file area */
	char file_descname[61];			/* if set, this is the path to files.bbs */
	unsigned char file_priv;		/* priv level to search this area */
	int file_flags;					/* 1.05.02 - flags for search access */
	char file_deleted;				/* true if deleted */
	};


struct ofe		/* 512-byte file entry structure */
	{
	char fe_name[16];
	int fe_location;			/* area number this file is located in */
	char fe_descrip[385];
	char fe_group[41];			/* grouping for this item */
	char fe_uploader[41];
	unsigned int fe_uldate;
	unsigned int fe_dldate;		/* last date downloaded */
	unsigned int fe_dl;			/* number of downloads */
	unsigned int fe_priv;		/* min priv level to see/dl this file */
	int fe_flags;				/* flags to see/dl this file */
	char fe_reserved[17];
	};



struct fe		/* 384-byte file entry structure */
	{
	char fe_name[16];
	int fe_location;			/* area number this file is located in */
	char fe_descrip[257];
	char fe_group[41];			/* grouping for this item */
	char fe_uploader[41];
	unsigned int fe_uldate;
	unsigned int fe_dldate;		/* last date downloaded */
	unsigned int fe_dl;			/* number of downloads */
	unsigned int fe_priv;		/* min priv level to see/dl this file */
	int fe_flags;				/* flags to see/dl this file */
	char fe_reserved[17];
	};




char workbuffer[512];
char buffer[512];
char bbspath[120];


int main(int argc,char *argv[])
	{
	struct file tfile;
	struct ofe ofe;
	struct fe nfe;
	char *cptr;
	long rpos;
	long wpos;
	int found = 0;
	FILE *fd;
	FILE *fd1;

	fprintf(stderr,"CONV\n");

	if (argc < 2)
		{
		fprintf(stderr,"Usage is: CONV FileAreaNum\n");
		fprintf(stderr,"   where  FileAreaNum = file area to convert.\n\n"); 
		}
	else
		{
		if (cptr = getenv("SIMPLEX"))		/* now for our path */
			strcpy(buffer,cptr);
		else
			getcwd(buffer,sizeof(buffer));
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		sprintf(bbspath,buffer);

		strcat(buffer,"filearea.bbs");
		if (fd = fopen(buffer,"rb"))
			{
			while (fread(&tfile,sizeof(struct file),1,fd))
				{
				if (tfile.file_number == atoi(argv[1]))
					{
					printf("Preparing to convert area %d: \"%s\"....\n",tfile.file_number,tfile.file_areaname);
					found = 1;

					if (tfile.file_descname[0])				/* load and show the file header */
						strcpy(buffer,tfile.file_descname);
					else 
						strcpy(buffer,tfile.file_pathname);
					if (buffer[0])
						{
						if (buffer[strlen(buffer) - 1] != P_CSEP)
							strcat(buffer,P_SSEP);
						}
					strcat(buffer,"filelist.bbs");

					if (fd1 = fopen(buffer,"r+b"))
						{
						rpos = wpos = 0L;

						while (fread(&ofe,1,sizeof(struct ofe),fd1))
							{
							printf("Read at %lu and ",rpos);
							rpos += (long)sizeof(struct ofe);

							memset(&nfe,0,sizeof(struct fe));
							strcpy(nfe.fe_name,ofe.fe_name);
							strncpy(nfe.fe_descrip,ofe.fe_descrip,256);
							nfe.fe_descrip[256] = (char)'\0';
							nfe.fe_location = ofe.fe_location;
							strcpy(nfe.fe_group,ofe.fe_group);
							strcpy(nfe.fe_uploader,ofe.fe_uploader);
							nfe.fe_uldate = ofe.fe_uldate;
							nfe.fe_dldate = ofe.fe_dldate;
							nfe.fe_dl = ofe.fe_dl;
							nfe.fe_priv = ofe.fe_priv;
							nfe.fe_flags = ofe.fe_flags;

							fseek(fd1,wpos,SEEK_SET);
							printf("write at %lu\n",wpos);
							fwrite(&nfe,1,sizeof(struct fe),fd1);
							wpos += (long)sizeof(struct fe);

							fseek(fd1,rpos,SEEK_SET);
							}

						if (wpos)
							chsize(fileno(fd1),wpos);
						fclose(fd1);

						}
					else
						{
						fprintf(stderr,"Fatal Error: Unable to find/open \"files.bbs\"!\n");
						return 1;
						}

					break;
					}
				}

			fclose(fd);

			if (!found)
				{
				fprintf(stderr,"Fatal Error: Unable to find area # %u!\n",atoi(argv[1]));
				return 1;
				}
			}
		else
			{
			fprintf(stderr,"Fatal Error: Unable to find/open \"filearea.bbs\"!\n");
			return 1;
			}
		}

	return 0;
	}
