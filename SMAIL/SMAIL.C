/* smail.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 9 December 1989
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/smail.c_v  $
**                       $Date:   25 Oct 1992 14:18:06  $
**                       $Revision:   1.12  $
**
*/


#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <direct.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include "smail.h"


struct cfg cfg;
char bbspath[100];
int zone = 0;
int net = -1;
int node = -1;
int keep_path = 0;
int export_routed = 0;
struct nm *netmail_areas = NULL;
int cur_netmail_areas = 0;
int max_netmail_areas = 0;
int netmail_tossed = 0;
int netmail_forwarded = 0;
int message_limit = 0;
int limit_flag = 0;				/* True if we are exiting because we hit message limit */
int echosec_flag = 0;			/* True if we want echomail security (only accept from nodes in the areas.bbs & use passwords in echopass.bbs) */
int quiet_flag = 0;				/* True if we want to quiet tossing information */
int twit_flag = 0;				/* True if we want twit-filtration */
struct area **areas = NULL;
int cur_areas = 0;
int max_areas = 0;
int sysop_convert = 1;
FILE *headfd;
FILE *bodyfd;
FILE *linkfd;
FILE *datafd;
FILE *msgfd;



void report_tossed(void)
	{
	char buffer[120];
	int current = 1;
	int count;
	int ok = 0;
	FILE *fd;

	sprintf(buffer,"%sslink.bbs",bbspath);		/* output areas in slink.bbs */
	fd = fopen(buffer,"wb");

	printf("\nThe following areas were imported and forwarded:\n\n");
	if (netmail_tossed || netmail_forwarded)
		{
		for (count = 0; count < cur_netmail_areas; count++)
			{
			if (netmail_areas[count].nm_tossed || netmail_areas[count].nm_forwarded)
				{
				printf("%3d> Area \"Net Mail for %u:%u/%u\", area %d: ",current++,netmail_areas[count].nm_zone,netmail_areas[count].nm_net,\
					netmail_areas[count].nm_node,netmail_areas[count].nm_area);
				if (netmail_areas[count].nm_tossed)
					{
					printf("Imported %d msg%s\n",netmail_areas[count].nm_tossed,netmail_areas[count].nm_tossed != 1 ? "s" : "");
					if (netmail_areas[count].nm_forwarded)
						printf("and ");
			
					if (fd)
						fwrite(&netmail_areas[count].nm_area,sizeof(int),1,fd);		/* write message area number */
					}
				if (netmail_areas[count].nm_forwarded)
					printf("Forwarded %d msg%s ",netmail_areas[count].nm_forwarded,netmail_areas[count].nm_forwarded != 1 ? "s" : "");
				printf("\n");
				}
			}
		ok = 1;
		}
	for (count = 0; count < cur_areas; count++)
		{
		if (areas[count]->area_forwarded || areas[count]->area_tossed || areas[count]->area_dups)
			{
			printf("%3d> Area \"%s\"",current++,areas[count]->area_name);
			if (areas[count]->area_msgarea != PASSTHRU)
				printf(", area %d: ",areas[count]->area_msgarea);
			else
				printf(", passthru area: ");
			if (areas[count]->area_tossed)
				{
				printf("Imported %ld msg%s ",areas[count]->area_tossed,areas[count]->area_tossed != 1 ? "s" : "");
				if (areas[count]->area_forwarded)
					printf("and ");

				if (fd)
					fwrite(&areas[count]->area_msgarea,sizeof(int),1,fd);	/* write message area number */
				}
			if (areas[count]->area_forwarded)
				printf("Forwarded %ld msg%s ",areas[count]->area_forwarded,areas[count]->area_forwarded != 1 ? "s" : "");
			printf("\n");

			if (areas[count]->area_dups)
				printf("     Notice: Stopped %ld duplicate message%s.\n",areas[count]->area_dups,areas[count]->area_dups != 1 ? "s" : "");
			ok = 1;
			}
		}
	if (!ok)
		printf("   No Areas were imported or forwarded.\n");

	printf("\n");

	for (count = 0; count < cur_tosslist; count++)
		{
		if (tosslist[count]->toss_total)
			printf("=-> Tossed %d msg%s to \"%s\" into %s\n",tosslist[count]->toss_total,(char *)(tosslist[count]->toss_total != 1 ? "s" : ""),tosslist[count]->toss_to,tosslist[count]->toss_path);
		printf("\n");
		}

	if (fd)
		fclose(fd);

	
	sprintf(buffer,"%ssmail.lim",bbspath);		/* indicate by creation/deletion of file that SMAIL is finished */
	if (message_limit && limit_flag)
		{
		fd = fopen(buffer,"wb");
		fclose(fd);
		}
	else
		unlink(buffer);
	}



void free_netmail(void)
	{
	if (cur_netmail_areas)
		free(netmail_areas);
	if (max_body)
		free(body);
	}



void free_passwords(void)
	{
	int count;

	if (cur_passwords)
		{
		for (count = 0; count < cur_passwords; count++)
			{
			free(passwords[count]->pp_password);
			free(passwords[count]);
			}
		free(passwords);
		}
	}



void free_twits(void)
	{
	int count;

	if (cur_twits)
		{
		for (count = 0; count < cur_twits; count++)
			{
			free(twits[count]->tf_name);
			free(twits[count]);
			}
		free(twits);
		}
	}



void signal_trap(int signo)
	{
	/* this function does nothing except receive
	** calls from SIGINT and SIGBREAK.  This is
	** done this way since not all compilers handle
	** SIG_IGN correctly.
	*/
	}



int main(int argc,char *argv[])
	{
	struct msg tmsg;
	char buffer[120];
	char *cptr;
	int count;
	int report = 0;
	int impmail = 0;
	int expnet = 0;
	int expecho = 0;
	int unarcmail = 0;
	int tossbad = 0;
	int cleansig = 0;
	int tval;
	FILE *fd;

	setbuf(stderr,NULL);		/* make sure that stderr is completely unbuffered */

#if defined(PROTECTED)
	fprintf(stderr,"SMAIL/2 (v %u.%02u of %s): Simplex/2 BBS Net Mail Utility\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#else
	fprintf(stderr,"SMAIL (v %u.%02u of %s): Simplex BBS Net Mail Utility\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#endif
	fprintf(stderr,"Copyright (c) 1989-92, Christopher Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

#if !defined(PROTECTED)
	fclose(stdaux);
	fclose(stdprn);
#endif


	if (argc > 1)
		{
		for (count = 1; count < argc; count++)
			{
			cptr = argv[count];
			while (*cptr && (*cptr == '-' || *cptr == '/'))
				++cptr;
			if (!stricmp(cptr,"it"))
				++impmail;
			else if (!stricmp(cptr,"ib"))
				++tossbad;
			else if (!stricmp(cptr,"nx"))
				++expnet;
			else if (!stricmp(cptr,"ex"))
				++expecho;
			else if (!stricmp(cptr,"ua"))
				++unarcmail;
			else if (!stricmp(cptr,"re"))
				++report;
			else if (!stricmp(cptr,"es"))
				echosec_flag = 1;
			else if (!stricmp(cptr,"tf"))
				twit_flag = 1;
			else if (!stricmp(cptr,"s"))
				sysop_convert = 0;
			else if (!stricmp(cptr,"q"))
				quiet_flag = 1;
			else if (!stricmp(cptr,"r"))
				export_routed = 1;
			else if (!stricmp(cptr,"k"))
				++keep_path;
			else if ((*cptr == 'C' || *cptr == 'c') && isdigit(*(cptr + 1)))
				cleansig = atoi(cptr + 1);
			else if (*cptr == '#' && isdigit(*(cptr + 1)))
				message_limit = atoi(cptr + 1);
			else
				printf("Unknown option \"%s\" on command line!\n",cptr);
			}
		if (!impmail && !tossbad && !expnet && !expecho && !unarcmail && !cleansig && !report)
			{
			printf("Error: Nothing to do!  Exiting....\n");
			return 1;
			}

		if (cptr = getenv("SIMPLEX"))		/* now for our path */
			strcpy(buffer,cptr);
		else
			getcwd(buffer,sizeof(buffer));
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		strcpy(bbspath,buffer);

		signal(SIGINT,signal_trap);				/* first, down with Ctrl-C */
		signal(SIGBREAK,signal_trap);			/* then down with Ctrl-Break */

		strcat(buffer,"config.bbs");
		if (!(fd = fopen(buffer,"rb")))
			{
			fprintf(stderr,"Fatal Error: Unable to find/open config.bbs file!\n");
			return 1;
			}
		if (!fread(&cfg,sizeof(struct cfg),1,fd))
			{
			fprintf(stderr,"Fatal Error: Unable to read from config.bbs file!\n");
			return 1;
			}
		fclose(fd);

		if (!cfg.cfg_netpath[0])
			{
			fprintf(stderr,"Fatal Error: No matrix path specified in config.bbs!\n");
			return 1;
			}
		if (!cfg.cfg_inboundpath[0])
			{
			fprintf(stderr,"Fatal Error: No inbound path specified in config.bbs!\n");
			return 1;
			}
		if (!cfg.cfg_outboundpath[0])
			{
			fprintf(stderr,"Fatal Error: No outbound path specified in config.bbs!\n");
			return 1;
			}

		if (!cfg.cfg_net)
			{
			fprintf(stderr,"Fatal Error: No primary network address specified!\n");
			return 1;
			}
		zone = cfg.cfg_zone;
		net = cfg.cfg_net;
		node = cfg.cfg_node;

		if (!zone)
			{
			fprintf(stderr,"Fatal Error: No Zone number found...Smail will behave as a zone-dead application!\n");
			return 1;
			}

		sprintf(buffer,"%smsgarea.bbs",bbspath);
		if (!(msgfd = fopen(buffer,"rb")))
			{
			fprintf(stderr,"Fatal Error: Unable to find/open msgarea.bbs file!\n");
			return 1;
			}
		while (fread(&tmsg,sizeof(struct msg),1,msgfd))
			{
			if (tmsg.msg_flags & MSG_NET && !tmsg.msg_deleted)
				{
				if (cur_netmail_areas >= max_netmail_areas)
					{
					if (!(netmail_areas = realloc(netmail_areas,(max_netmail_areas += 3) * sizeof(struct nm))))
						{
						fprintf(stderr,"Fatal Error: Out of memory to allocate netmail structures!\n");
						return 1;
						}
					}
				netmail_areas[cur_netmail_areas].nm_area = tmsg.msg_number;
				tval = tmsg.msg_source;
				if (tval < 0 || tval > 5)
					tval = 0;
				if (tval)
					{
					if (cfg.cfg_akanet[tval - 1])
						{
						netmail_areas[cur_netmail_areas].nm_zone = cfg.cfg_akazone[tval - 1];
						netmail_areas[cur_netmail_areas].nm_net = cfg.cfg_akanet[tval - 1];
						netmail_areas[cur_netmail_areas].nm_node = cfg.cfg_akanode[tval - 1];
						}
					else
						tval = 0;
					}
				if (!tval)
					{
					netmail_areas[cur_netmail_areas].nm_zone = cfg.cfg_zone;
					netmail_areas[cur_netmail_areas].nm_net = cfg.cfg_net;
					netmail_areas[cur_netmail_areas].nm_node = cfg.cfg_node;
					}
				netmail_areas[cur_netmail_areas].nm_tossed = 0;
				netmail_areas[cur_netmail_areas].nm_forwarded = 0;
				++cur_netmail_areas;
				}
			}
		atexit(free_netmail);			/* warning! atexit is a LIFO function -- cannot free areas before reporting tossed (see below) */
		
		if (echosec_flag)
			{
			sprintf(buffer,"%sechopass.bbs",bbspath);
			if (fd = fopen(buffer,"rb"))
				{
				read_passwords(fd);
				fclose(fd);
				atexit(free_passwords);			/* warning! atexit is a LIFO function -- cannot free areas before reporting tossed (see below) */
				}
			}

		if (twit_flag)
			{
			sprintf(buffer,"%stwits.bbs",bbspath);
			if (fd = fopen(buffer,"rb"))
				{
				read_twits(fd);
				fclose(fd);
				atexit(free_twits);				/* warning! atexit is a LIFO function -- cannot free areas before reporting tossed (see below) */
				}
			}

		sprintf(buffer,"%sareas.bbs",bbspath);
		if (!(fd = fopen(buffer,"rb")))
			{
			fprintf(stderr,"Fatal Error: Unable to find/open areas.bbs file!\n");
			return 1;
			}
		read_areas(fd);
		fclose(fd);
		atexit(free_areas);			/* warning! atexit is a LIFO function -- cannot free areas before reporting tossed (see below) */

		sprintf(buffer,"%smsghead.bbs",bbspath);
		if (!(headfd = fopen(buffer,"rb+")))
			{
			fprintf(stderr,"Fatal Error: Unable to find/open msghead.bbs file!\n");
			return 1;
			}

		sprintf(buffer,"%smsgbody.bbs",bbspath);
		if (!(bodyfd = fopen(buffer,"rb+")))
			{
			fprintf(stderr,"Fatal Error: Unable to find/open msgbody.bbs file!\n");
			return 1;
			}

		sprintf(buffer,"%smsglink.bbs",bbspath);
		if (!(linkfd = fopen(buffer,"rb+")))
			{
			fprintf(stderr,"Fatal Error: Unable to find/open msglink.bbs file!\n");
			return 1;
			}

		sprintf(buffer,"%smsgdata.bbs",bbspath);
		if (!(datafd = fopen(buffer,"rb+")))
			{
			fprintf(stderr,"Fatal Error: Unable to find/open msgdata.bbs file!\n");
			return 1;
			}

		if (cleansig)				/* clean signitures before loading */
			clean_signatures(cleansig);
		load_signatures();
		if (report)
			report_traffic();

		if (unarcmail || impmail || tossbad || expnet || expecho)
			{
			atexit(free_signatures);

			atexit(report_tossed);

			if (unarcmail)
				unarc_packets();
			if (impmail)
				import_mail();
			if (tossbad)
				import_bad();
			if (expnet)
				export_net();
			if (expecho)
				export_echo();
			}

		fclose(bodyfd);
		fclose(headfd);
		fclose(linkfd);
		fclose(datafd);
		fclose(msgfd);
		}
	else
		{
#if defined(PROTECTED)
		printf("Usage is: smail2 -options\n\n");
#else
		printf("Usage is: smail -options\n\n");
#endif
		printf("where options may be:\n");
		printf("\t-ua       unarc inbound mail packets.\n");
		printf("\t-it       import/toss net and echo mail.\n");
		printf("\t-ib       import/toss net and echo mail in bad messages area.\n");
		printf("\t-nx       export netmail to other systems.\n");
		printf("\t-ex       export echomail to other systems.\n");
		printf("\t-re       report on echomail passing through the system.\n");
		printf("\t-es       enable echo security: accept echos ONLY from nodes in\n");
		printf("\t             listed in areas.bbs.  Use packet passwords found in\n");
		printf("\t             echopass.bbs if registered for node.\n");
		printf("\t-tf       twit filtration of inbound mail for names found in\n");
		printf("\t             twits.bbs.\n");
		printf("\t-q        quiet mode -- keep printing to screen minimized.\n");
		printf("\t-k        keep PATH and SEEN-BY kludge in echomail.\n");
		printf("\t-r        export echomail as ROUTED (normal) instead of DIRECT (CM).\n");
		printf("\t-s        disable conversion of \"sysop\" messages to sysop's name.\n");
		printf("\t-#xxx     limit number of .MSG files im matrix to xxx.\n");	
		printf("\t-c[days]  clean signatures older than \"days\" out of SMAIL.SIG.\n\n");
		return 1;
		}
	return 0;
	}
