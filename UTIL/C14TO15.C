/* c14to15.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 7 December 1990
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>




struct omsg
	{
	int msg_number;					/* message board number */
	char msg_areaname[41];			/* message area name */
	char msg_password[16];			/* message area password */
	char msg_flags;					/* message area flags */
	unsigned char msg_readpriv;
	unsigned char msg_writepriv;
	unsigned char msg_sysoppriv;
	char msg_deleted;				/* true if deleted */
	};


struct ofile
	{
	int file_number;				/* file board number */
	char file_areaname[41];			/* file area name */
	char file_password[16];			/* file area password if any */
	char file_pathname[61];			/* path to file area */
	unsigned char file_priv;		/* priv level to search this area */
	char file_deleted;				/* true if deleted */
	};



struct nmsg
	{
	int msg_number;					/* message board number */
	char msg_areaname[41];			/* message area name */
	char msg_flags;					/* message area flags */
	unsigned char msg_readpriv;
	unsigned char msg_writepriv;
	unsigned char msg_sysoppriv;
	char msg_deleted;				/* true if deleted */
	};


struct nfile
	{
	int file_number;				/* file board number */
	char file_areaname[41];			/* file area name */
	char file_pathname[61];			/* path to file area */
	char file_descname[61];			/* if set, this is the path to files.bbs */
	unsigned char file_priv;		/* priv level to search this area */
	char file_deleted;				/* true if deleted */
	};



int main(void)
	{
	struct omsg omsg;
	struct nmsg nmsg;
	struct ofile ofile;
	struct nfile nfile;
	int current;
	FILE *mfd;
	FILE *ffd;
	FILE *bfd;

	printf("C14TO15: Converts Simplex File and Message Area Files.\n\n");
	mfd = fopen("msgarea.bbs","r+b");
	ffd = fopen("filearea.bbs","rb");
	bfd = fopen("filearea.bak","wb");

	if (!mfd)
		printf("Warning: No message area file msgarea.bbs found!\n");
	if (!ffd)
		printf("Warning: No file area file filearea.bbs found!\n");
	if (!mfd && !ffd)
		{
		printf("Error: Nothing to do.\n");
		return 1;
		}

	if (mfd)
		{
		current = 0;
		while (fread(&omsg,sizeof(struct omsg),1,mfd))
			{
			printf("  Converting msg %d: Area #%d \"%s\"...\n",current + 1,omsg.msg_number,omsg.msg_areaname);
			memset(&nmsg,0,sizeof(struct nmsg));
			nmsg.msg_number = omsg.msg_number;
			strcpy(nmsg.msg_areaname,omsg.msg_areaname);
			nmsg.msg_flags = omsg.msg_flags;
			nmsg.msg_readpriv = omsg.msg_readpriv;
			nmsg.msg_writepriv = omsg.msg_writepriv;
			nmsg.msg_sysoppriv = omsg.msg_sysoppriv;
			nmsg.msg_deleted = omsg.msg_deleted;
			fseek(mfd,(long)current * (long)sizeof(struct nmsg),SEEK_SET);
			fwrite(&nmsg,sizeof(struct nmsg),1,mfd);
			++current;
			fseek(mfd,(long)current * (long)sizeof(struct omsg),SEEK_SET);
			}
		chsize(fileno(mfd),(long)current * (long)sizeof(struct nmsg));
		fclose(mfd);
		}
	if (ffd && bfd)
		{
		current = 0;
		while (fread(&ofile,sizeof(struct ofile),1,ffd))
			{
			printf("  Converting file %d: Area #%d \"%s\"...\n",current + 1,ofile.file_number,ofile.file_areaname);
			memset(&nfile,0,sizeof(struct nfile));
			nfile.file_number = ofile.file_number;
			strcpy(nfile.file_areaname,ofile.file_areaname);
			strcpy(nfile.file_pathname,ofile.file_pathname);
			nfile.file_priv = ofile.file_priv;
			nfile.file_deleted = ofile.file_deleted;
			fwrite(&nfile,sizeof(struct nfile),1,bfd);
			++current;
			}
		fclose(ffd);
		fclose(bfd);
		if (current)
			{
			unlink("filearea.bbs");
			if (rename("filearea.bak","filearea.bbs"))
				printf("Error: Unable to rename filearea.bak to filearea.bbs.\n");
			}
		}

	return 0;
	}
