/* fr10600.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 1 May 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>




typedef unsigned short DATE_T;
typedef unsigned short TIME_T;



struct ouser
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
	DATE_T user_sdate;				/* expiration date of special levels */

	char user_screenlen;			/* length of users screen in lines */
	int user_credit;				/* credit in cents for fidomail */
	int user_flags;					/* bit mapped flags for options */

	DATE_T user_firstdate; 			/* first date user called */
	DATE_T user_lastdate; 			/* last date called */
	TIME_T user_lasttime; 			/* last time called */
	int user_calls;					/* number of calls to the BBS */
	int user_timeused;				/* amount of time used for the day (minutes) */

	int user_upload;				/* number of uploads */
	long user_uploadbytes;			/* number of bytes uploaded */
	int user_dnload;				/* number of downloads */
	long user_dnloadbytes;			/* number of bytes downloaded */
	long user_msgsent;				/* number of messages sent */
	long user_msgread;				/* number of messages read */
	char user_reserved;
	};


struct nuser
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
	char user_alias[4][41];			/* registered aliases added in v 1.07.00 */

	unsigned char user_priv;		/* users default privilege level */
	int user_uflags;				/* 16 default definable flags */
	unsigned char user_spriv;		/* users special privilege level */
	int user_sflags;				/* 16 special definable flags */
	DATE_T user_sdate;				/* expiration date of special levels */

	char user_screenlen;			/* length of users screen in lines */
	int user_credit;				/* credit in cents for fidomail */
	int user_flags;					/* bit mapped flags for options */

	DATE_T user_firstdate; 			/* first date user called */
	DATE_T user_lastdate; 			/* last date called */
	TIME_T user_lasttime; 			/* last time called */
	int user_calls;					/* number of calls to the BBS */
	int user_timeused;				/* amount of time used for the day (minutes) */

	int user_upload;				/* number of uploads */
	long user_uploadbytes;			/* number of bytes uploaded */
	int user_dnload;				/* number of downloads */
	long user_dnloadbytes;			/* number of bytes downloaded */
	long user_msgsent;				/* number of messages sent */
	long user_msgread;				/* number of messages read */
	};



struct ouser ouser;
struct nuser nuser;


int main(void)
	{
	FILE *sfd;
	FILE *dfd;
	int count;

	fprintf(stderr,"FR10600: Converts Simplex Files from pre version 1.07.00.\n");
	fprintf(stderr,"Copyright (c) 1993, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	fprintf(stderr,"\n1...Convert USERLIST.BBS.\n");
	unlink("userlist.bak");
	if (rename("userlist.bbs","userlist.bak"))
		{
		fprintf(stderr,"Error: Unable to find/rename userlist.bbs....Exiting.\n");
		return 1;
		}
	if (!(sfd = fopen("userlist.bak","rb")))
		{
		rename("userlist.bak","userlist.bbs");
		fprintf(stderr,"Error: Unable to open old userlist.bbs....Exiting.\n");
		return 1;
		}
	if (!(dfd = fopen("userlist.bbs","wb")))
		{
		rename("userlist.bak","userlist.bbs");
		fprintf(stderr,"Error: Unable to create new userlist.bbs....Exiting.\n");
		return 1;
		}

	count = 0;
	while (fread(&ouser,sizeof(struct ouser),1,sfd))
		{
		fprintf(stderr,"Converting user %d\r",count);

		memset(&nuser,0,sizeof(struct nuser));
		strcpy(nuser.user_name,ouser.user_name);
		strcpy(nuser.user_address1,ouser.user_address1);
		strcpy(nuser.user_address2,ouser.user_address2);
		strcpy(nuser.user_city,ouser.user_city);
		strcpy(nuser.user_state,ouser.user_state);
		strcpy(nuser.user_password,ouser.user_password);
		strcpy(nuser.user_home,ouser.user_home);
		strcpy(nuser.user_data,ouser.user_data);

		nuser.user_priv = ouser.user_priv;
		nuser.user_uflags = ouser.user_uflags;
		nuser.user_spriv = ouser.user_spriv;
		nuser.user_sflags = ouser.user_sflags;
		nuser.user_sdate = ouser.user_sdate;

		nuser.user_screenlen = ouser.user_screenlen;
		nuser.user_credit = ouser.user_credit;
		nuser.user_flags = ouser.user_flags;

		nuser.user_firstdate = ouser.user_firstdate;
		nuser.user_lastdate = ouser.user_lastdate;
		nuser.user_lasttime = ouser.user_lasttime;
		nuser.user_calls = ouser.user_calls;
		nuser.user_timeused = ouser.user_timeused;

		nuser.user_upload = ouser.user_upload;
		nuser.user_uploadbytes = ouser.user_uploadbytes;
		nuser.user_dnload = ouser.user_dnload;
		nuser.user_dnloadbytes = ouser.user_dnloadbytes;
		nuser.user_msgsent = ouser.user_msgsent;
		nuser.user_msgread = ouser.user_msgread;

		fwrite(&nuser,sizeof(struct nuser),1,dfd);
		++count;
		}
	fprintf(stderr,"Converted %u user records\n\n",count);

	fclose(dfd);
	fclose(sfd);

	return 0;
	}
