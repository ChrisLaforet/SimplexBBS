/* c12to13.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 17 November 1990
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <string.h>


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
	int cfg_slowflag;				/* flag is true if modem is slow */

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
	char cfg_editor[71];			/* command line for external editor */

	char cfg_netpath[61];			/* path for net messages */
	char cfg_inboundpath[61];		/* path for inbound message pkts */
	char cfg_outboundpath[61];		/* path for outbound message pkts */
	char cfg_packetpath[61];		/* path for unpacking message pkts */
	char cfg_badmsgpath[61];		/* path for placing bad messages */
	char cfg_unarc[61];				/* command for unarcing ARCed files */
	char cfg_unzip[61];				/* command for unarcing ZIPped files */
	char cfg_unzoo[61];				/* command for unarcing ZOOed files */

	unsigned char cfg_newpriv;		/* priv level of new user */
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
	char cfg_ring[21];				/* ring response */
	char cfg_answer[21];			/* answer string */
	char cfg_hangup[21];			/* hangup string */
	char cfg_busy[21];				/* offhook string */
	int cfg_slowflag;				/* flag is true if modem is slow */

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
	char cfg_editor[71];			/* command line for external editor */

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


struct ocfg ocfg;
struct ncfg ncfg;


int main(void)
	{
	FILE *fd;
	
	if (!(fd = fopen("config.bbs","r+b")))
		{
		printf("Solly chalie, we no finda CONFIG.BBSa filea!\n");
		return 1;
		}
	if (!fread(&ocfg,sizeof(struct ocfg),1,fd))
		{
		printf("Solly chalie, we no able reada CONFIG.BBSa filea!\n");
		return 1;
		}
	memset(&ncfg,0,sizeof(struct ncfg));
	memcpy(&ncfg,&ocfg,1252);
	memcpy(&ncfg.cfg_newpriv,&ocfg.cfg_newpriv,sizeof(struct ocfg) - 1252);
	fseek(fd,0L,SEEK_SET);
	if (!fwrite(&ncfg,sizeof(struct ncfg),1,fd))
		{
		printf("Solly chalie, we no able writea CONFIG.BBSa filea!\n");
		return 1;
		}
	fclose(fd);
	return 0;
	}
