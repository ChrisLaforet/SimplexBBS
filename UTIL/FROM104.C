/* from104.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 6 October 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>




struct ouser
	{
	char user_name[41];
	char user_password[16];
	char user_city[31];
	char user_home[15];				/* users home number */
	char user_data[15];				/* users data/business number */
	unsigned char user_priv;		/* users privilege level */
	int user_uflags;				/* v 1.02 added 16 definable flags */
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

	char user_reserved[4];			/* reserved for later use */
	};



struct ocfg
	{
	int cfg_baud;					/* maximum baud rate */
	int cfg_port;					/* port number */
	char cfg_init[61];				/* modem initialization string */
	char cfg_resp[21];				/* normal modem response string */
	char cfg_resp3[21];				/* 300 baud response */
	char cfg_resp12[21];			/* 1200 baud response */
	char cfg_resp24[21];			/* 2400 baud response */
	char cfg_resp48[21];			/* 4800 baud response */
	char cfg_resp96[21];			/* 9600 baud response */
	char cfg_resp192[21];			/* 19200 baud response */
	char cfg_ring[21];				/* ring response */
	char cfg_answer[21];			/* answer string */
	char cfg_hangup[21];			/* hangup string */
	char cfg_busy[21];				/* offhook string */
	int cfg_flags;					/* flags for setup */

	int cfg_minbaud;				/* minimum connect rate allowd on BBS */
	int cfg_ansibaud;				/* minimum baud for ansi */
	int cfg_dlstart;				/* start time for downloads */
	int cfg_dlstop;					/* stop time for downloads */

	int cfg_zone;
	int cfg_net;
	int cfg_node;
	int cfg_akazone[5];
	int cfg_akanet[5];
	int cfg_akanode[5];

	char cfg_sysopname[41];			/* sysop's name */
	char cfg_bbsname[61];			/* BBS name */
	char cfg_origin[66];			/* origin line */
	char cfg_menupath[61];			/* path to menu files */
	char cfg_screenpath[61];		/* path to screen files */
	char cfg_nodepath[61];			/* path to nodelist */
	char cfg_fapath[61];			/* path to local fileattaches */

	char cfg_netpath[61];			/* path for net messages */
	char cfg_inboundpath[61];		/* path for inbound message pkts */
	char cfg_outboundpath[61];		/* path for outbound message pkts */
	char cfg_packetpath[61];		/* path for unpacking message pkts */
	char cfg_badmsgpath[61];		/* path for placing bad messages */
	char cfg_unarc[61];				/* command for unarcing ARCed files */
	char cfg_unzip[61];				/* command for unarcing ZIPped files */
	char cfg_unzoo[61];				/* command for unarcing ZOOed files */
	char cfg_unlzh[61];				/* command for unarcing LZHed files */
	char cfg_arc[61];				/* command for arcing ARCed files */
	char cfg_zip[61];				/* command for arcing ZIPped files */
	char cfg_zoo[61];				/* command for arcing ZOOed files */
	char cfg_lzh[61];				/* command for arcing LZHed files */

	unsigned char cfg_newpriv;		/* priv level of new user */
	int cfg_newflags;				/* v 1.02 new user's flags */
	int cfg_newcredit;				/* credit level of new user */
	int cfg_newtime;				/* logon time in mins for newuser */
	char cfg_askhome;				/* ask for home phone */
	char cfg_askdata;				/* ask for data phone */

	char cfg_ansi;					/* use ansi on local console */
	char cfg_direct;				/* use direct screen writes? */
	char cfg_snow;					/* use snow checking on screen writes */
	char cfg_pwdtries;				/* number of attempts to get password */
	char cfg_inactive;				/* time before hanging up for inactivity */
	char cfg_yells;					/* number of yells per session */
	char cfg_yelltime;				/* number of seconds for yell bell */
	int cfg_yellstart;				/* start time for yell-bell */
	int cfg_yellstop;				/* stop time for yell-bell */
	};



struct omsg
	{
	int msg_number;					/* message board number */
	char msg_areaname[41];			/* message area name */
	char msg_flags;					/* message area flags */
	unsigned char msg_readpriv;
	unsigned char msg_writepriv;
	unsigned char msg_sysoppriv;
	char msg_origin[66];			/* special origin line for this if echomail area */
	char msg_deleted;				/* true if deleted */
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


struct ncfg
	{
	int cfg_baud;					/* maximum baud rate */
	int cfg_port;					/* port number */
	char cfg_init[61];				/* modem initialization string */
	char cfg_resp[21];				/* normal modem response string */
	char cfg_resp3[21];				/* 300 baud response */
	char cfg_resp12[21];			/* 1200 baud response */
	char cfg_resp24[21];			/* 2400 baud response */
	char cfg_resp48[21];			/* 4800 baud response */
	char cfg_resp96[21];			/* 9600 baud response */
	char cfg_resp192[21];			/* 19200 baud response */
	char cfg_resp384[21];			/* 38400 baud response */
	char cfg_ring[21];				/* ring response */
	char cfg_answer[21];			/* answer string */
	char cfg_hangup[21];			/* hangup string */
	char cfg_busy[21];				/* offhook string */
	int cfg_flags;					/* flags for setup */

	int cfg_zone;
	int cfg_net;
	int cfg_node;
	int cfg_point;					/* point for later use */
	int cfg_akazone[5];
	int cfg_akanet[5];
	int cfg_akanode[5];
	int cfg_akapoint[5];			/* points for later use */

	char cfg_sysopname[41];			/* sysop's name */
	char cfg_bbsname[61];			/* BBS name */
	char cfg_origin[66];			/* origin line */
	char cfg_menupath[61];			/* path to menu files */
	char cfg_screenpath[61];		/* path to screen files */
	char cfg_nodepath[61];			/* path to nodelist */
	char cfg_fapath[61];			/* path to local fileattaches */

	char cfg_netpath[61];			/* path for net messages */
	char cfg_inboundpath[61];		/* path for inbound message pkts */
	char cfg_outboundpath[61];		/* path for outbound message pkts */
	char cfg_packetpath[61];		/* path for unpacking message pkts */
	char cfg_badmsgpath[61];		/* path for placing bad messages */
	char cfg_unarc[61];				/* command for unarcing ARCed files */
	char cfg_unzip[61];				/* command for unarcing ZIPped files */
	char cfg_unzoo[61];				/* command for unarcing ZOOed files */
	char cfg_unlzh[61];				/* command for unarcing LZHed files */
	char cfg_unarj[61];				/* command for unarcing ARJed files */
	char cfg_arc[61];				/* command for arcing ARCed files */
	char cfg_zip[61];				/* command for arcing ZIPped files */
	char cfg_zoo[61];				/* command for arcing ZOOed files */
	char cfg_lzh[61];				/* command for arcing LZHed files */
	char cfg_arj[61];				/* command for arcing ARJed files */

	unsigned char cfg_newpriv;		/* priv level of new user */
	int cfg_newflags;				/* v 1.02 new user's flags */
	int cfg_newcredit;				/* credit level of new user */
	int cfg_newtime;				/* logon time in mins for newuser */
	char cfg_askaddress;			/* ask for full address */
	char cfg_askhome;				/* ask for home phone */
	char cfg_askdata;				/* ask for data phone */

	int cfg_minbaud;				/* minimum connect rate allowed on BBS */
	int cfg_ansibaud;				/* minimum baud for ansi */
	char cfg_ansi;					/* use ansi on local console */
	char cfg_direct;				/* use direct screen writes? */
	char cfg_snow;					/* use snow checking on screen writes */
	char cfg_pwdtries;				/* number of attempts to get password */
	char cfg_inactive;				/* time before hanging up for inactivity */
	char cfg_yells;					/* number of yells per session */
	char cfg_yelltime;				/* number of seconds for yell bell */
	int cfg_yellstart;				/* start time for yell-bell */
	int cfg_yellstop;				/* stop time for yell-bell */
	int cfg_dlstart;				/* start time for downloads */
	int cfg_dlstop;					/* stop time for downloads */
	};


struct nmsg
	{
	int msg_number;					/* message board number */
	char msg_areaname[41];			/* message area name */
	char msg_flags;					/* message area flags */
	unsigned char msg_readpriv;
	unsigned char msg_writepriv;
	unsigned char msg_sysoppriv;
	char msg_origin[66];			/* special origin line for this if echomail area */
	char msg_source;				/* source net/node for this netmail/echomail area */
	char msg_deleted;				/* true if deleted */
	};




struct ocfg ocfg;
struct ncfg ncfg;
struct ouser ouser;
struct nuser nuser;
struct omsg omsg;
struct nmsg nmsg;



int main(void)
	{
	FILE *sfd;
	FILE *dfd;
	int count;

	fprintf(stderr,"FROM104: Converts Simplex Files from pre version 1.05.\n");
	fprintf(stderr,"Copyright (c) 1991, Chris Laforet.  All Rights Reserved.\n\n");

	fprintf(stderr,"1...Convert CONFIG.BBS.\n");
	unlink("config.bak");
	if (rename("config.bbs","config.bak"))
		{
		fprintf(stderr,"Error: Unable to find/rename config.bbs....Exiting.\n");
		return 1;
		}
	if (!(sfd = fopen("config.bak","rb")))
		{
		rename("config.bak","config.bbs");
		fprintf(stderr,"Error: Unable to open old config.bbs....Exiting.\n");
		return 1;
		}
	if (!(dfd = fopen("config.bbs","wb")))
		{
		rename("config.bak","config.bbs");
		fprintf(stderr,"Error: Unable to create new config.bbs....Exiting.\n");
		return 1;
		}

	fread(&ocfg,sizeof(struct ocfg),1,sfd);
	memset(&ncfg,0,sizeof(struct ncfg));

	ncfg.cfg_baud = ocfg.cfg_baud;
	ncfg.cfg_port = ocfg.cfg_port;
	strcpy(ncfg.cfg_init,ocfg.cfg_init);
	strcpy(ncfg.cfg_resp,ocfg.cfg_resp);
	strcpy(ncfg.cfg_resp3,ocfg.cfg_resp3);
	strcpy(ncfg.cfg_resp12,ocfg.cfg_resp12);
	strcpy(ncfg.cfg_resp24,ocfg.cfg_resp24);
	strcpy(ncfg.cfg_resp48,ocfg.cfg_resp48);
	strcpy(ncfg.cfg_resp96,ocfg.cfg_resp96);
	strcpy(ncfg.cfg_resp192,ocfg.cfg_resp192);
	strcpy(ncfg.cfg_ring,ocfg.cfg_ring);
	strcpy(ncfg.cfg_answer,ocfg.cfg_answer);
	strcpy(ncfg.cfg_hangup,ocfg.cfg_hangup);
	strcpy(ncfg.cfg_busy,ocfg.cfg_busy);
	ncfg.cfg_flags = ocfg.cfg_flags;
	ncfg.cfg_minbaud = ocfg.cfg_minbaud;
	ncfg.cfg_ansibaud = ocfg.cfg_ansibaud;
	ncfg.cfg_dlstart = ocfg.cfg_dlstart;
	ncfg.cfg_dlstop = ocfg.cfg_dlstop;
	ncfg.cfg_zone = ocfg.cfg_zone;
	ncfg.cfg_net = ocfg.cfg_net;
	ncfg.cfg_node = ocfg.cfg_node;
	for (count = 0; count < 5; count++)
		{
		ncfg.cfg_akazone[0] = ocfg.cfg_akazone[0];
		ncfg.cfg_akanet[0] = ocfg.cfg_akanet[0];
		ncfg.cfg_akanode[0] = ocfg.cfg_akanode[0];
		}

	strcpy(ncfg.cfg_sysopname,ocfg.cfg_sysopname);
	strcpy(ncfg.cfg_bbsname,ocfg.cfg_bbsname);
	strcpy(ncfg.cfg_origin,ocfg.cfg_origin);
	strcpy(ncfg.cfg_menupath,ocfg.cfg_menupath);
	strcpy(ncfg.cfg_screenpath,ocfg.cfg_screenpath);
	strcpy(ncfg.cfg_nodepath,ocfg.cfg_nodepath);
	strcpy(ncfg.cfg_fapath,ocfg.cfg_fapath);
	strcpy(ncfg.cfg_netpath,ocfg.cfg_netpath);
	strcpy(ncfg.cfg_inboundpath,ocfg.cfg_inboundpath);
	strcpy(ncfg.cfg_outboundpath,ocfg.cfg_outboundpath);
	strcpy(ncfg.cfg_packetpath,ocfg.cfg_packetpath);
	strcpy(ncfg.cfg_badmsgpath,ocfg.cfg_badmsgpath);
	strcpy(ncfg.cfg_unarc,ocfg.cfg_unarc);
	strcpy(ncfg.cfg_unzip,ocfg.cfg_unzip);
	strcpy(ncfg.cfg_unzoo,ocfg.cfg_unzoo);
	strcpy(ncfg.cfg_unlzh,ocfg.cfg_unlzh);
	strcpy(ncfg.cfg_arc,ocfg.cfg_arc);
	strcpy(ncfg.cfg_zip,ocfg.cfg_zip);
	strcpy(ncfg.cfg_zoo,ocfg.cfg_zoo);
	strcpy(ncfg.cfg_lzh,ocfg.cfg_lzh);

	ncfg.cfg_newpriv = ocfg.cfg_newpriv;
	ncfg.cfg_newflags = ocfg.cfg_newflags;
	ncfg.cfg_newcredit = ocfg.cfg_newcredit;
	ncfg.cfg_newtime = ocfg.cfg_newtime;

	ncfg.cfg_askhome = ocfg.cfg_askhome;
	ncfg.cfg_askdata = ocfg.cfg_askdata;
	ncfg.cfg_ansi = ocfg.cfg_ansi;
	ncfg.cfg_direct = ocfg.cfg_direct;
	ncfg.cfg_snow = ocfg.cfg_snow;
	ncfg.cfg_pwdtries = ocfg.cfg_pwdtries;
	ncfg.cfg_yells = ocfg.cfg_yells;
	ncfg.cfg_yelltime = ocfg.cfg_yelltime;
	ncfg.cfg_yellstart = ocfg.cfg_yellstart;
	ncfg.cfg_yellstop = ocfg.cfg_yellstop;

	if (!fwrite(&ncfg,sizeof(struct ncfg),1,dfd))
		{
		fclose(dfd);
		unlink("config.bbs");
		rename("config.bak","config.bbs");
		fprintf(stderr,"Error: Unable to write new config.bbs....Exiting.\n");
		return 1;
		}

	fclose(dfd);
	fclose(sfd);


	fprintf(stderr,"\n2...Convert USERLIST.BBS.\n");
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
		++count;
		fprintf(stderr,"Converting user %d\r",count);

		memset(&nuser,0,sizeof(struct nuser));
		strcpy(nuser.user_name,ouser.user_name);
		strcpy(nuser.user_password,ouser.user_password);
		strcpy(nuser.user_city,ouser.user_city);
		strcpy(nuser.user_home,ouser.user_home);
		strcpy(nuser.user_data,ouser.user_data);
		nuser.user_priv = ouser.user_priv;
		nuser.user_uflags = ouser.user_uflags;
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

		fwrite(&nuser,sizeof(struct nuser),1,dfd);
		}

	fclose(dfd);
	fclose(sfd);


	fprintf(stderr,"\n3...Convert MSGAREA.BBS.\n");
	unlink("msgarea.bak");
	if (rename("msgarea.bbs","msgarea.bak"))
		{
		fprintf(stderr,"Error: Unable to find/rename msgarea.bbs....Exiting.\n");
		return 1;
		}
	if (!(sfd = fopen("msgarea.bak","rb")))
		{
		rename("msgarea.bak","msgarea.bbs");
		fprintf(stderr,"Error: Unable to open old msgarea.bbs....Exiting.\n");
		return 1;
		}
	if (!(dfd = fopen("msgarea.bbs","wb")))
		{
		rename("msgarea.bak","msgarea.bbs");
		fprintf(stderr,"Error: Unable to create new msgarea.bbs....Exiting.\n");
		return 1;
		}

	count = 0;
	while (fread(&omsg,sizeof(struct omsg),1,sfd))
		{
		++count;
		fprintf(stderr,"Converting msg area %d\r",count);

		memset(&nmsg,0,sizeof(struct nmsg));
		strcpy(nmsg.msg_areaname,omsg.msg_areaname);
		nmsg.msg_number = omsg.msg_number;
		nmsg.msg_flags = omsg.msg_flags;
		nmsg.msg_readpriv = omsg.msg_readpriv;
		nmsg.msg_writepriv = omsg.msg_writepriv;
		nmsg.msg_sysoppriv = omsg.msg_sysoppriv;
		strcpy(nmsg.msg_origin,omsg.msg_origin);
		nmsg.msg_deleted = omsg.msg_deleted;

		fwrite(&nmsg,sizeof(struct nmsg),1,dfd);
		}

	fclose(dfd);
	fclose(sfd);

	return 0;
	}
