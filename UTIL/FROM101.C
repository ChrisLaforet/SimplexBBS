/* from101.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 March 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef PROTECTED
	#define INCL_DOSFILEMGR
	#define INCL_DOSPROCESS
	#include <os2.h>
#else
	#include <dos.h>
#endif




struct ouser
	{
	char user_name[41];
	char user_password[16];
	char user_city[31];
	char user_home[15];				/* users home number */
	char user_data[15];				/* users data/business number */
	unsigned char user_priv;		/* users privilege level */
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

	char user_reserved[6];			/* reserved for later use */
	};



struct omenu
	{
	int menu_number;				/* line number */
	unsigned char menu_type;		/* type of choice */
	char menu_key;					/* key to initiate choice */
	unsigned char menu_priv;		/* user priv to see/use choice */
	char menu_auto;					/* automatic execution if true */
	char menu_color;				/* color of choice */
	char menu_hilite;				/* color of hilight */
	char menu_expert;				/* if true show when expert mode is on */
	char menu_line[71];				/* line to show on screen */
	char menu_data[71]; 			/* data line for menu option */
	char menu_deleted;
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



struct nuser
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



struct nmenu
	{
	int menu_number;				/* line number */
	unsigned char menu_type;		/* type of choice */
	char menu_key;					/* key to initiate choice */
	unsigned char menu_priv;		/* user priv to see/use choice */
	int menu_flags;					/* v 1.02 added flags to access option */
	char menu_auto;					/* automatic execution if true */
	char menu_color;				/* color of choice */
	char menu_hilite;				/* color of hilight */
	char menu_expert;				/* if true show when expert mode is on */
	char menu_line[71];				/* line to show on screen */
	char menu_data[71]; 			/* data line for menu option */
	char menu_deleted;
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



struct prompt
	{
	int prompt_sig;					/* signiture for valid menu file 0xa442 */
	char prompt_color;
	char prompt_hilite;
	char prompt_string[56];			/* string to show */
	};



struct ocfg ocfg;
struct ncfg ncfg;
struct ouser ouser;
struct nuser nuser;
struct omenu omenu;
struct nmenu nmenu;
struct prompt tprompt;

char **menus = NULL;
int cur_menus = 0;
int max_menus = 0;



#ifdef PROTECTED
	static HDIR hdir;		/* OS/2 directory */
	USHORT search;
	FILEFINDBUF find;
#else
	struct find_t find;
#endif



char *get_firstfile(char *mask)
	{
	int rtn;
	int found = 0;
	static char name[20];

#ifdef PROTECTED
	search = 1;
	hdir = HDIR_SYSTEM;
	rtn = (int)DosFindFirst(mask,&hdir,FILE_NORMAL | FILE_ARCHIVED | FILE_READONLY,&find,sizeof(FILEFINDBUF),&search,0L);
	if (!rtn)
		{
		strcpy(name,find.achName);
		found = 1;
		}
#else
	rtn = (int)_dos_findfirst(mask,_A_ARCH | _A_RDONLY,&find);
	if (!rtn)
		{
		strcpy(name,find.name);
		found = 1;
		}
#endif
	if (found)
		return name;
	return NULL;
	}



char *get_nextfile(void)
	{
	int rtn;
	int found = 0;
	static char name[20];

#ifdef PROTECTED
	search = 1;
	rtn = (int)DosFindNext(hdir,&find,sizeof(find),&search);
	if (!rtn)
		{
		strcpy(name,find.achName);
		found = 1;
		}
#else
	rtn = (int)_dos_findnext(&find);
	if (!rtn)
		{
		strcpy(name,find.name);
		found = 1;
		}
#endif
	if (found)
		return name;
	return NULL;
	}




int main(void)
	{
	char buffer[100];
	char buffer1[100];
	char *cptr;
	char *cptr1;
	FILE *sfd;
	FILE *dfd;
	int count;

	fprintf(stderr,"FROM101: Converts Simplex Files from pre version 1.02.\n");
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
	memcpy(&ncfg,&ocfg,1548 * sizeof(char));

	ncfg.cfg_newflags = 0;
	ncfg.cfg_newcredit = ocfg.cfg_newcredit;
	ncfg.cfg_newtime = ocfg.cfg_newtime;
	ncfg.cfg_askhome = ocfg.cfg_askhome;
	ncfg.cfg_askdata = ocfg.cfg_askdata;
	ncfg.cfg_ansi = ocfg.cfg_ansi;
	ncfg.cfg_direct = ocfg.cfg_direct;
	ncfg.cfg_snow = ocfg.cfg_snow;
	ncfg.cfg_pwdtries = ocfg.cfg_pwdtries;
	ncfg.cfg_inactive = ocfg.cfg_inactive;
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


	fprintf(stderr,"\n3...Convert MENUS.\n");

	strcpy(buffer,ocfg.cfg_menupath);
	if (buffer[0] && buffer[strlen(buffer) - 1] != '\\')
		strcat(buffer,"\\");
	strcat(buffer,"*.mnu");

	cptr = get_firstfile(buffer);
	while (cptr != NULL)
		{
		cptr1 = cptr;

		while (*cptr1)
			{
			if (*cptr1 == '.')
				*cptr1 = '\0';
			else 
				++cptr1;
			}

		fprintf(stderr,"Found %s.mnu   \r",cptr);
		if (cur_menus >= max_menus)
			{
			if (!(menus = realloc(menus,(max_menus += 25) * sizeof(char *))))
				{
				fprintf(stderr,"Error: Unable to allocate memory for menus....Exiting.\n");
				return 1;
				}
			}

		if (!(menus[cur_menus] = calloc(strlen(cptr) + 1,sizeof(char))))
			{
			fprintf(stderr,"Error: Unable to allocate memory for menus....Exiting.\n");
			return 1;
			}
		strcpy(menus[cur_menus],cptr);
		++cur_menus;

		cptr = get_nextfile();
		}

	if (cur_menus)
		{
		for (count = 0; count < cur_menus; count++)
			{
			strcpy(buffer,ocfg.cfg_menupath);
			if (buffer[0] && buffer[strlen(buffer) - 1] != '\\')
				strcat(buffer,"\\");
			strcat(buffer,menus[count]);
			strcpy(buffer1,buffer);
			strcat(buffer,".mnu");
			strcat(buffer1,".bak");

			unlink(buffer1);
			if (rename(buffer,buffer1))
				fprintf(stderr,"Error: Unable to find/rename %s.mnu...\n",menus[count]);
			else
				{
				if (!(sfd = fopen(buffer1,"rb")))
					fprintf(stderr,"Error: Unable to open %s.bak...\n",menus[count]);
				else 
					{
					if (!(dfd = fopen(buffer,"wb")))
						fprintf(stderr,"Error: Unable to open %s.mnu...\n",menus[count]);
					else
						{
						fprintf(stderr,"Converting menu %s.mnu    \r",menus[count]);
						fread(&tprompt,sizeof(struct prompt),1,sfd);
						fwrite(&tprompt,sizeof(struct prompt),1,dfd);

						while (fread(&omenu,sizeof(struct omenu),1,sfd))
							{
							memset(&nmenu,0,sizeof(struct nmenu));

							nmenu.menu_number = omenu.menu_number;
							nmenu.menu_type = omenu.menu_type;
							nmenu.menu_key = omenu.menu_key;
							nmenu.menu_priv = omenu.menu_priv;
							nmenu.menu_flags = 0;
							nmenu.menu_auto = omenu.menu_auto;
							nmenu.menu_color = omenu.menu_color;
							nmenu.menu_hilite = omenu.menu_hilite;
							nmenu.menu_expert = omenu.menu_expert;
							strcpy(nmenu.menu_line,omenu.menu_line);
							strcpy(nmenu.menu_data,omenu.menu_data);
							nmenu.menu_deleted = omenu.menu_deleted;

							fwrite(&nmenu,sizeof(struct nmenu),1,dfd);
							}
						fclose(dfd);
						}
					fclose(sfd);
					}
				}
			free(menus[count]);
			}
		free(menus);
		}

	return 0;
	}
