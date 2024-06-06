#include <stdio.h>



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

	char user_padding;				/* makes record 256 bytes */
	};



int main(int argc,char *argv[])
	{
	struct user tuser;
	FILE *fd;

	if (fd = fopen("userlist.bbs","r+b"))
		{
		while (fread(&tuser,sizeof(struct user),1,fd))
			{
			tuser.user_flags &= 0x8a7d;
			tuser.user_flags |= 0x8000;
			if ((unsigned int)tuser.user_priv >= 200)
				{
				tuser.user_flags |= 0x4000;
				printf("Warning: %s has shell\n",tuser.user_name);
				}
			fseek(fd,-(long)sizeof(struct user),SEEK_CUR);
			fwrite(&tuser,sizeof(struct user),1,fd));
			fseek(fd,0L,SEEK_SET);
			}
		fclose(fd);
		}
	}

