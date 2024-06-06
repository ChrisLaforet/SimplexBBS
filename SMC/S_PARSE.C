/* s_parse.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 December 1990
**
** Revision Information: $Logfile:   G:/simplex/smc/vcs/s_parse.c_v  $
**                       $Date:   25 Oct 1992 14:25:38  $
**                       $Revision:   1.7  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "smc.h"




struct m mtype[NUM_MENUS] =
	{
		{MENU_SHOW,"Show menu line"},
		{MENU_CALL,"Call new menu - Add to return stack"},
		{MENU_GOTO,"Goto new menu - Clear return stack"},
		{MENU_REPLACE,"Goto new menu - Replace current in stack"},
		{MENU_RET,"Return to previous menu"},
		
		{MENU_ENTER,"Enter a message on board"},
		{MENU_READ,"Read messages on board"},
		{MENU_SCAN,"Scan messages on board"},
		{MENU_QSCAN,"Quickscan messages on board"},
		{MENU_USERLIST,"Search userlist"},
		{MENU_NETSEARCH,"Search for Net address"},
		{MENU_FREQ,"Generate FREQ to Net address"},
		
		{MENU_LIST,"List files on file board"},
		{MENU_UPLOAD,"Upload file to file board"},
		{MENU_DOWNLOAD,"Download file from file board"},
		{MENU_RAW,"Raw directory list of file board"},
		{MENU_CONTENTS,"Contents of archive on file board"},
		{MENU_NEW,"Show new files across file boards"},
		{MENU_SRCHNAME,"Search for filename across file boards"},
		{MENU_SRCHKEY,"Search for keyword across file boards"},
		{MENU_KILL,"Delete file on file board"},
		{MENU_READFILE,"Read a text file on file board"},
		{MENU_DOWNLOAD_FILE,"Download ONE special file from board"},
		{MENU_MOVEFILES,"Move file(s) from one file board to another"},
		{MENU_REVLIST,"List files on file board in reverse order"},
		
		{MENU_SHOWFILE,"Show ascii/ansi file"},
		{MENU_SHOWHOT,"Show ascii/ansi menu with hotkeys"},
		{MENU_SHOWWAIT,"Show ascii/ansi file with pause at end"},
		{MENU_QUESTION,"Answer questionaire file"},
		{MENU_QUOTE,"Show the quote of the moment"},
		{MENU_SHOWHOT_NOEXPERT,"Show non-expert ascii/ansi menu with hotkeys"},
		
		{MENU_CITY,"Change user's city/state"},
		{MENU_PASSWORD,"Change user's password"},
		{MENU_CLS,"Change user's clearscreen status"},
		{MENU_MORE,"Change user's MORE status"},
		{MENU_SCREENLEN,"Change user's screen length"},
		{MENU_ANSI,"Change user's ANSI status"},
		{MENU_EDITOR,"Change user's default editor status"},
		{MENU_EXPERT,"Change user's novice/expert mode"},
		{MENU_HOMEPHONE,"Change user's home phone number"},
		{MENU_DATAPHONE,"Change user's data/work phone number"},
		{MENU_CHGADD1,"Change user's address line 1"},
		{MENU_CHGADD2,"Change user's address line 2"},
		{MENU_CHGCITY,"Change user's city line"},
		{MENU_CHGSTATE,"Change user's state line"},
		{MENU_CHGZIP,"Change user's zip line"},
		{MENU_CHGALIAS,"Change user aliases"},

		{MENU_ADENTER,"Enter an advertisement in ad area"},
		{MENU_ADREAD,"Read advertisements in ad area"},
		{MENU_ADSCAN,"Scan advertisements in ad area"},

		{MENU_NEWPRIV,"Set user's priv to new level"},
		{MENU_UPTIME,"Add minutes from user's time"},
		{MENU_DOWNTIME,"Subtract minutes from user's time"},
		{MENU_ADDFLAGS,"Add flags to a user's flag setting"},
		{MENU_DELFLAGS,"Delete flags from a user's flag setting"},
		{MENU_SETTIME,"Set the time of the user to an absolute time"},
		{MENU_LOGENTRY,"Log data line to Sysop log"},
		{MENU_SYSOPALERT,"Beep tones to alert Sysop of entry to menu"},

		{MENU_SETCOMB,"Set up combined message boards"},
		{MENU_READCOMB,"Read messages on combined message boards"},
		{MENU_SCANCOMB,"Scan messages on combined message boards"},
		{MENU_QSCANCOMB,"Quickscan messages on combined message boards"},
		{MENU_DLCOMB,"Download messages from combined message boards"},
		{MENU_CFGCOMB,"Configure options for combined message boards"},
		{MENU_ULCOMB,"Upload QWK messages to combined message boards"},

		{MENU_XMITXMODEM,"Download files using Xmodem"},
		{MENU_XMITXMODEM1K,"Download files using Xmodem-1K"},
		{MENU_XMITYMODEM,"Download files using Ymodem"},
		{MENU_XMITYMODEMG,"Download files using Ymodem-G"},
		{MENU_XMITZMODEM,"Download files using Zmodem"},

		{MENU_RECVXMODEM,"Upload files using Xmodem"},
		{MENU_RECVXMODEM1K,"Upload files using Xmodem-1K"},
		{MENU_RECVYMODEM,"Upload files using Ymodem"},
		{MENU_RECVYMODEMG,"Upload files using Ymodem-G"},
		{MENU_RECVZMODEM,"Upload files using Zmodem"},

		{MENU_RUN,"Run a program from within SIMPLEX"},
		{MENU_VERS,"Show version/copyright information"},
		{MENU_USAGE,"Show user's usage statistics"},
		{MENU_YELL,"Yell (limited time) for chat with Sysop"},
		{MENU_CLOCK,"Show current time of day"},
		{MENU_CHECKMAIL,"Check for mail"},
		{MENU_USERUPGRADE,"Sysop's user-upgrade option"},
		{MENU_TIMELESS_YELL,"Yell (unlimited time) for chat with Sysop"},
		{MENU_PHONEDUP,"Check for dup phone numbers & send message to sysop if match"},
		{MENU_LOGOFF,"Logoff/hangup connection"},
	};


void parse_file(char *outpath)
	{
	struct prompt tprompt;
	struct menu tmenu;
	char buffer[100];
	char defname[32];
	char menuname[9];
	int in_menu;
	int in_prompt;
	int in_line;
	int menu_lines;
	int got_prompt;
	int got_ident;
	int count;
	int tval;
	int cur_state = IDLE;
	int ret;
	FILE *fd;

	in_prompt = 0;
	in_menu = 0;
	in_line = 0;
	menuname[0] = '\0';
	while ((ret = yygettoken()) != EOFILE)
		{
		if (!in_menu && ret != MENU && ret != COMMENT && ret != DEFINE && cur_state == IDLE)
			error("Expected keyword \"menu\" or a comment",WARNING);
		else
			{
			switch (ret)
				{
				case STRING:
					switch (cur_state)
						{
						case MENU:
							strncpy(menuname,yytext,8);
							menuname[8] = '\0';
							strlwr(menuname);
							sprintf(buffer,"%s%s.mnu",outpath,menuname);
							printf("\nCreating menu file \"%s.mnu\"\n",menuname);
							if (!(fd = fopen(buffer,"wb")))
								error("Unable to open file",FATAL);
							cur_state = IDLE;
							break;
						case PROMPT:
							memset(tprompt.prompt_string,0,56);
							strncpy(tprompt.prompt_string,yytext,55);
							fseek(fd,0L,SEEK_SET);
							if (!fwrite(&tprompt,sizeof(struct prompt),1,fd))
								error("Unable to write to menu file",FATAL);
							cur_state = IDLE;
							break;
						case FLAGS:
							tmenu.menu_flags = xlat_flags(yytext);
							cur_state = IDLE;
							break;
						case LINE:
							memset(tmenu.menu_line,0,71);
							strncpy(tmenu.menu_line,yytext,70);
							cur_state = IDLE;
							break;
						case KEY:
							if (strlen(yytext) > 1)
								error("Menu line key choice should be only one character",WARNING);
							else if (strlen(yytext))
								tmenu.menu_key = yytext[0];
							else 
								tmenu.menu_key = '\0';
							cur_state = IDLE;
							break;
						case DATA:
							memset(tmenu.menu_data,0,71);
							strncpy(tmenu.menu_data,yytext,70);
							cur_state = IDLE;
							break;
						case DEFINE:
							if (got_ident)
								{
								add_define(STRING,defname,yytext);
								cur_state = IDLE;
								}
							else
								error("Unexpected string in define statement",FATAL);
							break;
						default:
							error("Unexpected string encountered -- skipping",WARNING);
							break;
						}
					break;
				case CONST:
					tval = atoi(yytext);
					switch (cur_state)
						{
						case COLOR:
							if (in_prompt)
								{
								tprompt.prompt_color = (unsigned char)tval;
								fseek(fd,0L,SEEK_SET);
								if (!fwrite(&tprompt,sizeof(struct prompt),1,fd))
									error("Unable to write to menu file",FATAL);
								}
							else if (in_line)
								tmenu.menu_color = (unsigned char)tval;
							cur_state = IDLE;
							break;
						case HILITE:
							if (in_prompt)
								{
								tprompt.prompt_hilite = (unsigned char)tval;
								fseek(fd,0L,SEEK_SET);
								if (!fwrite(&tprompt,sizeof(struct prompt),1,fd))
									error("Unable to write to menu file",FATAL);
								}
							else if (in_line)
								tmenu.menu_hilite = (unsigned char)tval;
							cur_state = IDLE;
							break;
						case TYPE:
							for (count = 0; count < NUM_MENUS; count++)
								{
								if ((int)(unsigned int)mtype[count].m_type == tval)
									break;
								}
							if (count >= NUM_MENUS)
								{
								sprintf(buffer,"Invalid menu type \"%u\" encountered - skipping",tval);
								error(buffer,WARNING);
								}
							else
								{
								tmenu.menu_type = (unsigned char)tval;
								cur_state = IDLE;
								}
							break;
						case PRIV:
							tmenu.menu_priv = (unsigned char)atoi(yytext);
							cur_state = IDLE;
							break;
						case DEFINE:
							if (got_ident)
								{
								add_define(CONST,defname,yytext);
								cur_state = IDLE;
								}
							else
								error("Unexpected constant in define statement",FATAL);
							break;
						default:
							error("Unexpected constant encountered -- skipping",WARNING);
							break;
						}
					break;
				case COMMENT:
					break;
				case MENU:
					if (in_menu)
						{
						if (in_line)
							{
							if (got_prompt)
								{
								++menu_lines;
								tmenu.menu_number = menu_lines;
								fseek(fd,0L,SEEK_END);
								if (!fwrite(&tmenu,sizeof(struct menu),1,fd))
									error("Unable to write to menu file",FATAL);
								printf("Found and wrote %d menu line%s.\n",menu_lines,menu_lines == 1 ? "" : "s");
								}
							else
								error("Unable to write menu line since there is no prompt",WARNING);
							}
						fclose(fd);
						fd = NULL;
						}
					memset(&tmenu,0,sizeof(struct menu));
					menuname[0] = '\0';
					in_menu = 1;
					in_line = 0;
					menu_lines = 0;
					in_prompt = 0;
					got_prompt = 0;
					cur_state = MENU;
					break;
				case PROMPT:
					if (in_menu && menuname[0] && cur_state == IDLE)
						{
						if (!got_prompt)
							{
							in_prompt = 1;
							in_line = 0;
							cur_state = PROMPT;
							memset(&tprompt,0,sizeof(struct prompt));
							tprompt.prompt_sig = (int)0xa442;		/* signiture */
							tprompt.prompt_color = WHITE;
							tprompt.prompt_hilite = WHITE | BRIGHT;
							strcpy(tprompt.prompt_string,"What is your choice? ");
							fseek(fd,0L,SEEK_SET);
							if (!fwrite(&tprompt,sizeof(struct prompt),1,fd))
								error("Unable to write to menu file",FATAL);
							got_prompt = 1;
							}
						else
							{
							sprintf(buffer,"Already have a prompt in menu \"%s\" - skipping",menuname);
							error(buffer,WARNING);
							}
						}
					else
						error("Unexpected keyword \"prompt\"",FATAL);
					break;
				case LINE:
					if (in_menu && menuname[0] && cur_state == IDLE)
						{
						if (in_line)
							{
							++menu_lines;
							tmenu.menu_number = menu_lines;
							fseek(fd,0L,SEEK_END);
							if (!fwrite(&tmenu,sizeof(struct menu),1,fd))
								error("Unable to write to menu file",FATAL);
							}
						in_line = 1;
						in_prompt = 0;
						cur_state = LINE;
						memset(&tmenu,0,sizeof(struct menu));
						tmenu.menu_type = MENU_SHOW;
						tmenu.menu_color = WHITE;
						tmenu.menu_hilite = WHITE | BRIGHT;
						}
					else
						error("Unexpected keyword \"line\"",FATAL);
					break;
				case COLOR:
					if (cur_state == IDLE && (in_line || in_prompt))
						cur_state = COLOR;
					else
						error("Unexpected keyword \"color\"",WARNING);
					break;
				case HILITE:
					if (cur_state == IDLE && (in_line || in_prompt))
						cur_state = HILITE;
					else
						error("Unexpected keyword \"hilite\"",WARNING);
					break;
				case KEY:
					if (cur_state == IDLE && in_line)
						cur_state = KEY;
					else
						error("Unexpected keyword \"key\"",WARNING);
					break;
				case FLAGS:
					if (cur_state == IDLE && in_line)
						cur_state = FLAGS;
					else
						error("Unexpected keyword \"flags\"",WARNING);
					break;
				case TYPE:
					if (cur_state == IDLE && in_line)
						cur_state = TYPE;
					else
						error("Unexpected keyword \"type\"",WARNING);
					break;
				case AUTO:
					if (cur_state == IDLE && in_line)
						tmenu.menu_auto = 1;
					else
						error("Unexpected keyword \"auto\"",WARNING);
					break;
				case EXPERT:
					if (cur_state == IDLE && in_line)
						tmenu.menu_expert = 1;
					else
						error("Unexpected keyword \"expert\"",WARNING);
					break;
				case DATA:
					if (cur_state == IDLE && in_line)
						cur_state = DATA;
					else
						error("Unexpected keyword \"data\"",WARNING);
					break;
				case PRIV:
					if (cur_state == IDLE && in_line)
						cur_state = PRIV;
					else
						error("Unexpected keyword \"priv\"",WARNING);
					break;
				case DEFINE:
					if (cur_state == IDLE)
						{
						cur_state = DEFINE;
						got_ident = 0;
						}
					else 
						error("Unexpected keyword \"define\"",WARNING);
					break;
				case IDENT:
					if (cur_state == DEFINE)
						{
						strncpy(defname,yytext,31);
						defname[31] = '\0';
						got_ident = 1;
						}
					else
						error("Unexpected identifier",WARNING);
					break;
				default:
					sprintf(buffer,"Internal Error in Compiler: Parser is in unknown state \"%d\"",ret);
					error(buffer,FATAL);
					break;
				}
			}
		}
	if (in_menu)
		{
		if (in_line)
			{
			if (got_prompt)
				{
				++menu_lines;
				tmenu.menu_number = menu_lines;
				if (!fwrite(&tmenu,sizeof(struct menu),1,fd))
					error("Unable to write to menu file",FATAL);
				printf("Found and wrote %d menu line%s.\n",menu_lines,menu_lines == 1 ? "" : "s");
				}
			else 
				error("Unable to write menu line since there is no prompt",WARNING);
			}
		fclose(fd);
		}
	}
