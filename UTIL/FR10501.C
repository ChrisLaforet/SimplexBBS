/* fr10501.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 22 December 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <string.h>




/* configuration flags */

#define CFG_SLOWMODEM				0x1			/* modem is slow */
#define CFG_PRIVATEBBS				0x2			/* allow only private-preregistered bbs operation */
#define CFG_DISABLEKB				0x4			/* disable local keyboard */
#define CFG_LOCKBAUD				0x8			/* lock baudrate */
#define CFG_DIRECT					0x10		/* use direct screen writes */
#define CFG_SNOW					0x20		/* do snow checking on color */


/* color macros */

#define COLOR_WHITE					0x0
#define COLOR_GREEN					0x1
#define COLOR_MAGENTA				0x2
#define COLOR_YELLOW				0x3

#define COLOR_ONWHITE				0x0
#define COLOR_ONBLUE				0x1
#define COLOR_ONRED					0x2


#define COMB_AREA					1250



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


struct ocomb				/* combined message board structure */
	{
	int comb_user;				/* user number */
	char comb_areas[COMB_AREA];
	};


struct cfg
	{
	int cfg_baud;					/* maximum baud rate */
	int cfg_port;					/* port number */
	char cfg_init[61];				/* modem initialization string */
	char cfg_resp[21];				/* normal modem response string */
	char cfg_dialpre[21];			/* dial prefix */
	char cfg_dialpost[21];			/* dial postfix */
	char cfg_resp3_1[21];			/* 300 baud response 1 */
	char cfg_resp3_2[21];			/* 300 baud response 2 */
	char cfg_resp12_1[21];			/* 1200 baud response 1 */
	char cfg_resp12_2[21];			/* 1200 baud response 2 */
	char cfg_resp24_1[21];			/* 2400 baud response 1 */
	char cfg_resp24_2[21];			/* 2400 baud response 2 */
	char cfg_resp48_1[21];			/* 4800 baud response 1 */
	char cfg_resp48_2[21];			/* 4800 baud response 2 */
	char cfg_resp96_1[21];			/* 9600 baud response 1 */
	char cfg_resp96_2[21];			/* 9600 baud response 2 */
	char cfg_resp192_1[21];			/* 19200 baud response 1 */
	char cfg_resp192_2[21];			/* 19200 baud response 2 */
	char cfg_resp384_1[21];			/* 38400 baud response 1 */
	char cfg_resp384_2[21];			/* 38400 baud response 2 */
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
	char cfg_qwkname[9];			/* name for QWK packet BBSID */
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

	char cfg_newpriv;				/* priv level of new user */
	int cfg_newflags;				/* v 1.02 new user's flags */
	int cfg_newcredit;				/* credit level of new user */
	int cfg_newtime;				/* logon time in mins for newuser */
	char cfg_askaddress;			/* ask for full address */
	char cfg_askhome;				/* ask for home phone */
	char cfg_askdata;				/* ask for data phone */

	int cfg_minbaud;				/* minimum connect rate allowed on BBS */
	int cfg_ansibaud;				/* minimum baud for ansi */
	char cfg_status;				/* status line background color */
	char cfg_field;					/* entry field background color */
	char cfg_menu;					/* submenu color */
	char cfg_pwdtries;				/* number of attempts to get password */
	char cfg_inactive;				/* time before hanging up for inactivity */
	char cfg_yells;					/* number of yells per session */
	char cfg_yelltime;				/* number of seconds for yell bell */
	int cfg_yellstart;				/* start time for yell-bell */
	int cfg_yellstop;				/* stop time for yell-bell */
	int cfg_dlstart;				/* start time for downloads */
	int cfg_dlstop;					/* stop time for downloads */

	char cfg_welcome[61];			/* command line for welcome program */
	char cfg_viruschk[61];			/* command line for virus checker */
	};


struct comb				/* combined message board structure */
	{
	int comb_user;				/* user number */
	int comb_flags;
	char comb_areas[COMB_AREA];
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
	char msg_source;				/* source net/node for this netmail/echomail area */
	char msg_deleted;				/* true if deleted */
	};


struct msg
	{
	int msg_number;					/* message board number */
	char msg_areaname[41];			/* message area name */
	char msg_flags;					/* message area flags */
	unsigned char msg_readpriv;
	int msg_readflags;				/* 1.05.02 */
	unsigned char msg_writepriv;
	int msg_writeflags;				/* 1.05.02 */
	unsigned char msg_sysoppriv;
	char msg_origin[66];			/* special origin line for this if echomail area */
	char msg_source;				/* source net/node for this netmail/echomail area */
	char msg_deleted;				/* true if deleted */
	};


struct ofile
	{
	int file_number;				/* file board number */
	char file_areaname[41];			/* file area name */
	char file_pathname[61];			/* path to file area */
	char file_descname[61];			/* if set, this is the path to files.bbs */
	unsigned char file_priv;		/* priv level to search this area */
	char file_deleted;				/* true if deleted */
	};


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


struct ocfg ocfg;
struct cfg cfg;
struct ocomb ocomb;
struct comb comb;
struct omsg omsg;
struct msg msg;
struct ofile ofile;
struct file file;



int main(int argc,char *argv[])
	{
	FILE *sfd;
	FILE *dfd;
	int count;

	fprintf(stderr,"FR10501: Converts Simplex Files from pre-version 1.05.02.\n");
	fprintf(stderr,"Copyright (c) 1991, Chris Laforet.  All Rights Reserved.\n\n");

	fprintf(stderr,"Converting CONFIG.BBS.\n");
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
	memset(&cfg,0,sizeof(struct cfg));

	cfg.cfg_baud = ocfg.cfg_baud;
	cfg.cfg_port = ocfg.cfg_port;
	strcpy(cfg.cfg_init,ocfg.cfg_init);
	strcpy(cfg.cfg_resp,ocfg.cfg_resp);
	strcpy(cfg.cfg_resp3_1,ocfg.cfg_resp3);
	strcpy(cfg.cfg_resp12_1,ocfg.cfg_resp12);
	strcpy(cfg.cfg_resp24_1,ocfg.cfg_resp24);
	strcpy(cfg.cfg_resp48_1,ocfg.cfg_resp48);
	strcpy(cfg.cfg_resp96_1,ocfg.cfg_resp96);
	strcpy(cfg.cfg_resp192_1,ocfg.cfg_resp192);
	strcpy(cfg.cfg_resp384_1,ocfg.cfg_resp384);
	strcpy(cfg.cfg_ring,ocfg.cfg_ring);
	strcpy(cfg.cfg_answer,ocfg.cfg_answer);
	strcpy(cfg.cfg_hangup,ocfg.cfg_hangup);
	strcpy(cfg.cfg_busy,ocfg.cfg_busy);
	cfg.cfg_flags = ocfg.cfg_flags;
	strcpy(cfg.cfg_dialpre,"ATDT");
	strcpy(cfg.cfg_dialpost,"|");

	cfg.cfg_zone = ocfg.cfg_zone;
	cfg.cfg_net = ocfg.cfg_net;
	cfg.cfg_node = ocfg.cfg_node;
	for (count = 0; count < 5; count++)
		{
		cfg.cfg_akazone[count] = ocfg.cfg_akazone[count];
		cfg.cfg_akanet[count] = ocfg.cfg_akanet[count];
		cfg.cfg_akanode[count] = ocfg.cfg_akanode[count];
		}

	strcpy(cfg.cfg_sysopname,ocfg.cfg_sysopname);
	strcpy(cfg.cfg_bbsname,ocfg.cfg_bbsname);
	strcpy(cfg.cfg_origin,ocfg.cfg_origin);
	strcpy(cfg.cfg_menupath,ocfg.cfg_menupath);
	strcpy(cfg.cfg_screenpath,ocfg.cfg_screenpath);
	strcpy(cfg.cfg_nodepath,ocfg.cfg_nodepath);
	strcpy(cfg.cfg_fapath,ocfg.cfg_fapath);
	
	strcpy(cfg.cfg_netpath,ocfg.cfg_netpath);
	strcpy(cfg.cfg_inboundpath,ocfg.cfg_inboundpath);
	strcpy(cfg.cfg_outboundpath,ocfg.cfg_outboundpath);
	strcpy(cfg.cfg_packetpath,ocfg.cfg_packetpath);
	strcpy(cfg.cfg_badmsgpath,ocfg.cfg_badmsgpath);
	strcpy(cfg.cfg_unarc,ocfg.cfg_unarc);
	strcpy(cfg.cfg_unzip,ocfg.cfg_unzip);
	strcpy(cfg.cfg_unzoo,ocfg.cfg_unzoo);
	strcpy(cfg.cfg_unlzh,ocfg.cfg_unlzh);
	strcpy(cfg.cfg_unarj,ocfg.cfg_unarj);
	strcpy(cfg.cfg_arc,ocfg.cfg_arc);
	strcpy(cfg.cfg_zip,ocfg.cfg_zip);
	strcpy(cfg.cfg_zoo,ocfg.cfg_zoo);
	strcpy(cfg.cfg_lzh,ocfg.cfg_lzh);
	strcpy(cfg.cfg_arj,ocfg.cfg_arj);

	cfg.cfg_newpriv = ocfg.cfg_newpriv;
	cfg.cfg_newflags = ocfg.cfg_newflags;
	cfg.cfg_newcredit = ocfg.cfg_newcredit;
	cfg.cfg_newtime = ocfg.cfg_newtime;
	cfg.cfg_askaddress = ocfg.cfg_askaddress;
	cfg.cfg_askhome = ocfg.cfg_askhome;
	cfg.cfg_askdata = ocfg.cfg_askdata;

	cfg.cfg_minbaud = ocfg.cfg_minbaud;
	cfg.cfg_ansibaud = ocfg.cfg_ansibaud;

	cfg.cfg_dlstart = ocfg.cfg_dlstart;
	cfg.cfg_dlstop = ocfg.cfg_dlstop;

	if (ocfg.cfg_direct)
		cfg.cfg_flags |= CFG_DIRECT;
	if (ocfg.cfg_snow)
		cfg.cfg_flags |= CFG_SNOW;

	cfg.cfg_status = COLOR_ONWHITE;
	cfg.cfg_field = COLOR_ONBLUE;
	cfg.cfg_menu = COLOR_WHITE;
	cfg.cfg_pwdtries = ocfg.cfg_pwdtries;
	cfg.cfg_inactive = ocfg.cfg_inactive;
	cfg.cfg_yells = ocfg.cfg_yells;
	cfg.cfg_yelltime = ocfg.cfg_yelltime;
	cfg.cfg_yellstart = ocfg.cfg_yellstart;
	cfg.cfg_yellstop = ocfg.cfg_yellstop;
	cfg.cfg_dlstart = ocfg.cfg_dlstart;
	cfg.cfg_dlstop = ocfg.cfg_dlstop;

	if (!fwrite(&cfg,sizeof(struct cfg),1,dfd))
		{
		fclose(dfd);
		unlink("config.bbs");
		rename("config.bak","config.bbs");
		fprintf(stderr,"Error: Unable to write new config.bbs....Exiting.\n");
		return 1;
		}

	fclose(dfd);
	fclose(sfd);

	//-----------------------

	fprintf(stderr,"Converting MSGCOMB.BBS.\n");
	unlink("msgcomb.bak");
	if (rename("msgcomb.bbs","msgcomb.bak"))
		{
		fprintf(stderr,"Error: Unable to find/rename msgcomb.bbs....Exiting.\n");
		return 1;
		}
	if (!(sfd = fopen("msgcomb.bak","rb")))
		{
		rename("msgcomb.bak","msgcomb.bbs");
		fprintf(stderr,"Error: Unable to open old msgcomb.bbs....Exiting.\n");
		return 1;
		}
	if (!(dfd = fopen("msgcomb.bbs","wb")))
		{
		rename("msgcomb.bak","msgcomb.bbs");
		fprintf(stderr,"Error: Unable to create new msgcomb.bbs....Exiting.\n");
		return 1;
		}

	
	while (fread(&ocomb,sizeof(struct ocomb),1,sfd))
		{
		memset(&comb,0,sizeof(struct comb));

		comb.comb_user = ocomb.comb_user;
		memcpy(comb.comb_areas,ocomb.comb_areas,COMB_AREA * sizeof(char));

		if (!fwrite(&comb,sizeof(struct comb),1,dfd))
			{
			fclose(dfd);
			unlink("msgcomb.bbs");
			rename("msgcomb.bak","msgcomb.bbs");
			fprintf(stderr,"Error: Unable to write new msgcomb.bbs....Exiting.\n");
			return 1;
			}
		}


	fclose(dfd);
	fclose(sfd);


	//-----------------------

	fprintf(stderr,"Converting MSGAREA.BBS.\n");
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

	
	while (fread(&omsg,sizeof(struct omsg),1,sfd))
		{								 
		memset(&msg,0,sizeof(struct msg));

		msg.msg_number = omsg.msg_number;
		strcpy(msg.msg_areaname,omsg.msg_areaname);
		msg.msg_flags = omsg.msg_flags;
		msg.msg_readpriv = omsg.msg_readpriv;
		msg.msg_writepriv = omsg.msg_writepriv;
		msg.msg_sysoppriv = omsg.msg_sysoppriv;
		strcpy(msg.msg_origin,omsg.msg_origin);
		msg.msg_source = omsg.msg_source;

		if (!fwrite(&msg,sizeof(struct msg),1,dfd))
			{
			fclose(dfd);
			unlink("msgarea.bbs");
			rename("msgarea.bak","msgarea.bbs");
			fprintf(stderr,"Error: Unable to write new msgarea.bbs....Exiting.\n");
			return 1;
			}
		}


	fclose(dfd);
	fclose(sfd);


	//-----------------------

	fprintf(stderr,"Converting FILEAREA.BBS.\n");
	unlink("filearea.bak");
	if (rename("filearea.bbs","filearea.bak"))
		{
		fprintf(stderr,"Error: Unable to find/rename filearea.bbs....Exiting.\n");
		return 1;
		}
	if (!(sfd = fopen("filearea.bak","rb")))
		{
		rename("filearea.bak","filearea.bbs");
		fprintf(stderr,"Error: Unable to open old filearea.bbs....Exiting.\n");
		return 1;
		}
	if (!(dfd = fopen("filearea.bbs","wb")))
		{
		rename("filearea.bak","filearea.bbs");
		fprintf(stderr,"Error: Unable to create new filearea.bbs....Exiting.\n");
		return 1;
		}

	
	while (fread(&ofile,sizeof(struct ofile),1,sfd))
		{								 
		memset(&file,0,sizeof(struct file));

		file.file_number = ofile.file_number;
		strcpy(file.file_areaname,ofile.file_areaname);
		strcpy(file.file_pathname,ofile.file_pathname);
		strcpy(file.file_descname,ofile.file_descname);
		file.file_priv = ofile.file_priv;

		if (!fwrite(&file,sizeof(struct file),1,dfd))
			{
			fclose(dfd);
			unlink("filearea.bbs");
			rename("filearea.bak","filearea.bbs");
			fprintf(stderr,"Error: Unable to write new filearea.bbs....Exiting.\n");
			return 1;
			}
		}


	fclose(dfd);
	fclose(sfd);

	fprintf(stderr,"\aFinished!\n\n");
	return 0;
	}
