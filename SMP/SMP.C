/* smp.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 17 November 1990
**
** Revision Information: $Logfile:   G:/simplex/smp/vcs/smp.c_v  $
**                       $Date:   25 Oct 1992 14:26:22  $
**                       $Revision:   1.10  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <direct.h>
#include <io.h>
#include "smp.h"



struct cfg cfg;
char bbspath[100];
char outbound[256];
int zone = 0;
int net = -1;
int node = -1;
int verbose = 0;
time_t curtime;
struct tm *tmptr;
int echosec_flag = 0;
struct pp **passwords = NULL;	/* packet-level passwording introduced in v 1.09 */
int cur_passwords = 0;
int max_passwords = 0;



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
	char buffer[120];
	int sched = 0;
	int pack = 0;
	char *cptr;
	int count;
	FILE *fd;

	setbuf(stderr,NULL);		/* make sure that stderr is completely unbuffered */

#if defined(PROTECTED)
	fprintf(stderr,"SMP/2 (v %u.%02u of %s): Simplex/2 BBS Net Mail Packer and Router.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#else
	fprintf(stderr,"SMP (v %u.%02u of %s): Simplex BBS Net Mail Packer and Router.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
#endif
	fprintf(stderr,"Copyright (c) 1990-92, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");
	if (argc > 1)
		{
		for (count = 1; count < argc; count++)
			{
			cptr = argv[count];
			while (*cptr && (*cptr == (char)'-' || *cptr == (char)'/'))
				++cptr;
			if (*cptr == (char)'S' || *cptr == (char)'s')
				{
				++cptr;
				if (isalpha(*cptr))
					sched = (int)toupper(*cptr);
				else 
					printf("Unknown option \"%s\" on command line!\n",cptr);
				}
			else if (!stricmp(cptr,"es"))
				echosec_flag = 1;
			else if (*cptr == (char)'V' || *cptr == (char)'v')
				verbose = 1;
			else if (*cptr == (char)'P' || *cptr == (char)'p')
				pack = 1;
			else
				printf("Unknown option \"%s\" on command line!\n",cptr);
			}
		}

	if (pack)
		{
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
			printf("Fatal Error: Unable to find/open config.bbs file!\n");
			return 1;
			}
		if (!fread(&cfg,sizeof(struct cfg),1,fd))
			{
			printf("Fatal Error: Unable to read from config.bbs file!\n");
			return 1;
			}
		fclose(fd);

		if (!cfg.cfg_netpath[0])
			{
			printf("Fatal Error: No matrix path specified in config.bbs!\n");
			return 1;
			}
		if (!cfg.cfg_outboundpath[0])
			{
			printf("Fatal Error: No outbound path specified in config.bbs!\n");
			return 1;
			}

		if (cfg.cfg_zone)
			zone = cfg.cfg_zone;
		if (cfg.cfg_net)
			net = cfg.cfg_net;
		node = cfg.cfg_node;
		if (net == -1 || node == -1)
			{
			printf("Fatal Error: Net or node number undefined in config.bbs!\n");
			return 1;
			}
		if (!zone)
			{
			printf("Fatal Error: Zone number undefined in config.bbs!\n");
			return 1;
			}

		time(&curtime);
		tmptr = localtime(&curtime);

		if (echosec_flag)
			{
			sprintf(buffer,"%sechopass.bbs",bbspath);
			if (fd = fopen(buffer,"rb"))
				{
				read_passwords(fd);
				fclose(fd);
				}
			}

		sprintf(buffer,"%ssmp.ctl",bbspath);
		if (!(fd = fopen(buffer,"r+b")))
			{
			printf("Fatal Error: Unable to find/open smp.ctl file!\n");
			return 1;
			}
		load_actions(fd,sched);
		fclose(fd);
		if (!cur_actions)
			{
			if (sched)
				printf("Fatal Error: Nothing to do in schedule %c!\n",sched);
			else 
				printf("Fatal Error: Nothing to do!\n");
			return 1;
			}
		if (verbose)
			dump_actions();

		for (count = 0; count < 256; count++)		/* prepare the outbound array */
			outbound[count] = (char)0;

		delete_truncated();		/* kills other day's truncated archives */

		load_mlist();
		route_mlist();
		if (verbose)
			dump_mlist();
		if (cur_mlist)
			{
			packet_mail();
			for (count = 0; count < cur_mlist; count++)		/* free messagelist memory */
				{
				free(mlist[count]->mlist_msgs);
				free(mlist[count]);
				}
			free(mlist);
			}

		load_outbound();
		if (verbose)
			dump_outbound();
	 	process_outbound();
		if (cur_olist)
			{
			if (verbose)
				dump_outbound();
			for (count = 0; count < cur_olist; count++)		/* free outboundlist memory */
				{
				free(olist[count]->olist_file);
				free(olist[count]);
				}
			free(olist);
			}

		for (count = 0; count < cur_actions; count++)		/* free action memory */
			free(actions[count]);
		free(actions);

		fprintf(stderr,"Finished!\n\n");
		}
	else
		{
#if defined(PROTECTED)
		printf("Usage is: smp2 -p [-s(tag)] [-v]\n\n");
#else
		printf("Usage is: smp -p [-s(tag)] [-v]\n\n");
#endif
		printf("where options may be:\n");
		printf("\t-p        Pack and route files in matrix and outbound areas\n");
		printf("\t-s(tag)   Use information for sched (tag) in \"smp.ctl\" where\n");
		printf("\t          (tag) is a letter from A to Z\n");
		printf("\t-es       enable echo security: Encode packet passwords found in\n");
		printf("\t            echopass.bbs if registered for node.\n");
		printf("\t-v        Verbose flag - show debugging information\n\n");
		return 1;
		}
	return 0;
	}

