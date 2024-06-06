/* convqtos.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 11 November 1990
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
** cl -AS -Lp -Fb -Zp1 convqtos.c
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>




/* these macros access the QuickBBS user-flags */

#define U_DELETED				1
#define U_CLEARSCREEN			2
#define U_MORE					4
#define U_ANSI					8
#define U_NOKILL				16
#define U_IGNORE_DL_TIME		32
#define U_EDITOR				64



/* these macros access the Simplex user_flags */

#define USER_CLS					0x1			/* send cls codes */
#define USER_MORE					0x2			/* use more? paging */
#define USER_ANSI					0x4			/* use ansi */
#define USER_EDITOR					0x8			/* use full screen editor */
#define USER_EXPERT					0x10		/* expert (abbreviated) menus */
#define USER_FILEATTACH				0x20		/* allow user to fileattach in netmail areas? */
#define USER_DELETED				0x8000		/* user is deleted pending purge */


struct users		/* QuickBBS users.bbs */
	{
	unsigned char namelen;
	unsigned char name[35];
	unsigned char citylen;
	unsigned char city[25];
	unsigned char pwdlen;
	unsigned char pwd[15];
	unsigned char dataphonelen;
	unsigned char dataphone[12];
	unsigned char homephonelen;
	unsigned char homephone[12];
	unsigned char lasttimelen;
	unsigned char lasttime[5];
	unsigned char lastdatelen;
	unsigned char lastdate[8];
	unsigned char attrib;	 		/* see below constants */
	unsigned char flags[4];
	unsigned short credit;
	unsigned short pending;
	unsigned short timesposted;
	unsigned short highread;
	unsigned short security;
	unsigned short times;
	unsigned short uploads;
	unsigned short downloads;
	unsigned short uploadk;
	unsigned short downloadk;
	unsigned short todayk;
	unsigned short elapsed;
	unsigned short len;
	unsigned char waste[8];
	};


struct user
	{
	char user_name[41];
	char user_address1[31];
	char user_address2[31];
	char user_city[31];
	char user_state[16];
	char user_zip[16];
	char user_password[16];
	char user_home[15];				/* users home number */
	char user_data[15];				/* users data/business number */

	unsigned char user_priv;		/* users default privilege level */
	int user_uflags;				/* 16 default definable flags */
	unsigned char user_spriv;		/* users special privilege level */
	int user_sflags;				/* 16 special definable flags */
	int user_sdate;					/* expiration date of special levels */

	char user_screenlen;			/* length of users screen in lines */
	int user_credit;				/* credit in cents for fidomail */
	int user_flags;					/* bit mapped flags for options */

	int user_firstdate;				/* first date user called */
	int user_lastdate;				/* last date called */
	int user_lasttime;				/* last time called */
	int user_calls;					/* number of calls to the BBS */
	int user_timeused;				/* amount of time used for the day (minutes) */

	int user_upload;				/* number of uploads */
	long user_uploadbytes;			/* number of bytes uploaded */
	int user_dnload;				/* number of downloads */
	long user_dnloadbytes;			/* number of bytes downloaded */
	long user_msgsent;				/* number of messages sent */
	long user_msgread;				/* number of messages read */

	char user_padding;				/* makes record 256 bytes */
	};



int main(void)
	{
	struct users quser;
	struct user suser;
	char buffer[3];
	FILE *qfd;
	FILE *sfd;
	int userno = 0;

	fprintf(stderr,"CONVQTOS: Convert QuickBBS 2.xx Userlist to Simplex Userlist\n");
	fprintf(stderr,"Copyright (c) 1990, Chris Laforet Software.  All Rights Reserved.\n\n");

	if (!(qfd = fopen("users.bbs","rb")))
		{
		fprintf(stderr,"Error: Unable to find/open QuickBBS USERS.BBS\n\n");
		return 1;
		}
	if (!(sfd = fopen("userlist.bbs","wb")))
		{
		fprintf(stderr,"Error: Unable to create Simplex USERLIST.BBS\n\n");
		return 1;
		}

	while (fread(&quser,sizeof(struct users),1,qfd))
		{
		++userno;
			printf("Converting user %d",userno);
		if (quser.attrib & U_DELETED)
			printf(" - Deleted.\n");
		else
			{
			memset(&suser,0,sizeof(struct user));

			sprintf(suser.user_name,"%.*s",(int)quser.namelen,quser.name);
			printf(" (%s)\n",suser.user_name);

			sprintf(suser.user_password,"%.*s",(int)quser.pwdlen,quser.pwd);
			sprintf(suser.user_city,"%.*s",(int)quser.citylen,quser.city);
			sprintf(suser.user_home,"(%3.3s) %8.8s",quser.homephone,quser.homephone + 4);
			sprintf(suser.user_data,"(%3.3s) %8.8s",quser.dataphone,quser.dataphone + 4);

			if (quser.security < 200)
				suser.user_priv = (unsigned char)1;
			else if (quser.security == 200)
				suser.user_priv = (unsigned char)100;
			else if (quser.security < 1000)
				suser.user_priv = (unsigned char)200;
			else if (quser.security < 2000)
				suser.user_priv = (unsigned char)254;
			else 
				suser.user_priv = (unsigned char)255;

			sprintf(buffer,"%2.2s",quser.lasttime);
			suser.user_lasttime = (unsigned int)(atoi(buffer) << 11);
			sprintf(buffer,"%2.2s",quser.lasttime + 3);
			suser.user_lasttime |= (unsigned int)(atoi(buffer) << 5);

			sprintf(buffer,"%2.2s",quser.lastdate + 6);
			suser.user_lastdate = (unsigned int)((atoi(buffer) - 80) << 9);
			sprintf(buffer,"%2.2s",quser.lastdate);
			suser.user_lastdate |= (unsigned int)(atoi(buffer) << 5);
			sprintf(buffer,"%2.2s",quser.lastdate + 3);
			suser.user_lastdate |= (unsigned int)(atoi(buffer));

			suser.user_firstdate = suser.user_lastdate;

			suser.user_screenlen = (unsigned char)quser.len;
			suser.user_credit = quser.credit;
			suser.user_calls = quser.times;
			suser.user_upload = quser.uploads;
			suser.user_dnload = quser.downloads;
			suser.user_uploadbytes = (long)quser.uploadk * 1024L;
			suser.user_dnloadbytes = (long)quser.downloadk * 1024L;

			if (quser.attrib & U_CLEARSCREEN)
				suser.user_flags |= USER_CLS;
			if (quser.attrib & U_MORE)
				suser.user_flags |= USER_MORE;
			if (quser.attrib & U_ANSI)
				suser.user_flags |= USER_ANSI;
			if (quser.attrib & U_EDITOR)
				suser.user_flags |= USER_EDITOR;
			if (quser.credit)
				suser.user_flags |= USER_FILEATTACH;

			fwrite(&suser,sizeof(struct user),1,sfd);
			}
		}
	fclose(qfd);
	fclose(sfd);
	return 0;
	}
