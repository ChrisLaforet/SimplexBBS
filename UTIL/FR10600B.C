/* fr10600b.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 27 October 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <io.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


typedef unsigned short TIME_T;


struct ocfg
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


struct cfg
	{
	unsigned long cfg_version;		/* 1.07 current version of Simplex (Major << 16L | Minor << 8 | Sub) */
	unsigned short cfg_baud;		/* maximum baud rate */
	short cfg_port;					/* port number */
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
	short cfg_flags;				/* flags for setup */

	short cfg_zone;
	short cfg_net;
	short cfg_node;
	short cfg_point;				/* point for later use */
	short cfg_akazone[5];
	short cfg_akanet[5];
	short cfg_akanode[5];
	short cfg_akapoint[5];			/* points for later use */

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

	unsigned char cfg_newpriv;		/* priv level of new user */
	unsigned short cfg_newflags;	/* v 1.02 new user's flags */
	short cfg_newcredit;			/* credit level of new user */
	short cfg_newtime;				/* logon time in mins for newuser */
	char cfg_askaddress;			/* ask for full address */
	char cfg_askhome;				/* ask for home phone */
	char cfg_askdata;				/* ask for data phone */

	unsigned short cfg_minbaud; 	/* minimum connect rate allowed on BBS */
	unsigned short cfg_ansibaud;	/* minimum baud for ansi */
	char cfg_status;				/* status line background color */
	char cfg_field;					/* entry field background color */
	char cfg_menu;					/* submenu color */
	char cfg_pwdtries;				/* number of attempts to get password */
	char cfg_inactive;				/* time before hanging up for inactivity */
	char cfg_yells;					/* number of yells per session */
	char cfg_yelltime;				/* number of seconds for yell bell */
	TIME_T cfg_yellstart;			/* start time for yell-bell */
	TIME_T cfg_yellstop;			/* stop time for yell-bell */
	short cfg_dlstart;				/* start time for downloads */
	short cfg_dlstop;				/* stop time for downloads */

	char cfg_welcome[61];			/* command line for welcome program */
	char cfg_viruschk[61];			/* command line for virus checker */

	short cfg_uts;					/* 1.07 number of user time structs following cfg */
	char cfg_reserved[120];			/* 1.07 */
	};


struct ut				/* 1.07 - users time structure */
	{
	unsigned char ut_priv;
	short ut_time;
	};


struct ocfg ocfg;
struct cfg cfg;
struct ut **times = NULL;
short cur_times = 0;
short max_times = 0;



int main(int argc,char *argv[])
	{
	char buffer[61];
	char buffer1[6];
	char *cptr;
	char *cptr1;
	FILE *sfd;
	FILE *dfd;
	int priv;
	int max_time;
	int count;

	fprintf(stderr,"FR10501D: Converts Simplex Files from pre-version 1.05.02.\n");
	fprintf(stderr,"Copyright (c) 1991, Chris Laforet.  All Rights Reserved.\n\n");

	//-----------------
	fprintf(stderr,"Converting TIMES.BBS.\n");
	if (!(sfd = fopen("times.bbs","rb")))
		{
		fprintf(stderr,"Error: Unable to open times.bbs....Exiting.\n");
		return 1;
		}

	while (fgets(buffer,sizeof(buffer),sfd))
		{
		cptr = buffer;
		if (isdigit(*cptr))
			{
			cptr1 = buffer1;
			while (*cptr && (cptr1 - buffer1) < 3 && isdigit(*cptr))
				*cptr1++ = *cptr++;
			*cptr1 = '\0';
			while (isdigit(*cptr))		/* in case we ended our scan early */
				++cptr;
			priv = atoi(buffer1);
			if (priv < 256)
				{
				while (*cptr && !isdigit(*cptr))
					++cptr;
				cptr1 = buffer1;
				while (*cptr && (cptr1 - buffer1) < 4 && isdigit(*cptr))
					*cptr1++ = *cptr++;
				*cptr1 = '\0';
				max_time = atoi(buffer1);
				if (max_time > 1440)		/* 24 hours in minutes */
					max_time = 1440;
				else if (!max_time)
					max_time = 1;

				if (cur_times >= max_times)
					{
					if (!(times = realloc(times,(max_times += 10) * sizeof(struct ut *))))
						{
						fprintf(stderr,"Fatal error: Out of memory for conversion.\n");
						return 1;
						}
					}
				if (!(times[cur_times] = malloc(sizeof(struct ut))))
					{
					fprintf(stderr,"Fatal error: Out of memory for conversion.\n");
					return 1;
					}
				times[cur_times]->ut_priv = (unsigned char)priv;
				times[cur_times]->ut_time = max_time;
				++cur_times;
				}
			}
		}

	fclose(sfd);

	//-----------------
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
	memcpy(&cfg.cfg_baud,&ocfg,sizeof(struct ocfg));
	cfg.cfg_version = 0x10700L;
	cfg.cfg_uts = cur_times;

	if (!fwrite(&cfg,sizeof(struct cfg),1,dfd))
		{
		fclose(dfd);
		unlink("config.bbs");
		rename("config.bak","config.bbs");
		fprintf(stderr,"Error: Unable to write new config.bbs....Exiting.\n");
		return 1;
		}

	for (count = 0; count < cur_times; count++)
		{
		if (!fwrite(times[count],sizeof(struct ut),1,dfd))
			{
			fclose(dfd);
			unlink("config.bbs");
			rename("config.bak","config.bbs");
			fprintf(stderr,"Error: Unable to write new config.bbs....Exiting.\n");
			return 1;
			}
		}

	fclose(dfd);
	fclose(sfd);

	fprintf(stderr,"\aFinished!\n\n");
	return 0;
	}






