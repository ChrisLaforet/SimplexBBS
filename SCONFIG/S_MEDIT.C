/* s_medit.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 5 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_medit.c_v  $
**                       $Date:   25 Oct 1992 14:15:40  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#if !defined(PROTECTED)
	#include <dos.h>
#endif
#include <screen.h>
#include <window.h>
#include <editor.h>
#include <keys.h>
#include "sconfig.h"


struct mfile
	{
	char *mfile_name;
	int mfile_fields;
	};


struct menu **menu = NULL;
int max_menu = 0;
int cur_menu = 0;
int view_level = 0;
int view_flags = 0;

extern struct choice yn[2];


struct mtype menutype[NUM_MENUS] =
	{
		{MENU_SHOW,"MENU> Show menu line","None"},
		{MENU_CALL,"MENU> Call new menu: Add to return stack","Menuname [Password] [/ Template Macros]"},
		{MENU_GOTO,"MENU> Goto new menu: CLEAR return stack","Menuname [Password] [/ Template Macros]"},
		{MENU_REPLACE,"MENU> Goto new menu: REPLACE current menu","Menuname [Password] [/ Template Macros]"},
		{MENU_RET,"MENU> Return to previous menu","None"},
		{MENU_RUN,"MENU> Run a program from within SIMPLEX","Command_line"},
		{MENU_VERS,"MENU> Show version/copyright information","None"},
		{MENU_USAGE,"MENU> Show user's usage statistics","None"},
		{MENU_YELL,"MENU> Yell (limited hours) for Sysop chat","None"},
		{MENU_TIMELESS_YELL,"MENU> Yell (unlimited hours) for Sysop chat","None"},
		{MENU_CLOCK,"MENU> Show current time of day","None"},
		{MENU_CHECKMAIL,"MENU> Check for mail","None"},
		{MENU_USERUPGRADE,"MENU> Sysop's user-upgrade option","None"},
		{MENU_LOGOFF,"MENU> Logoff/hangup connection","None"},
		{MENU_LOGENTRY,"MENU> Log data line to Sysop log","Information_to_log"},
		{MENU_SYSOPALERT,"MENU> Beep tones to alert Sysop of entry to menu","None"},
		{MENU_PHONEDUP,"MENU> Check for dup phone numbers","Msg_board# [Flags (CASE IS IMPORTANT!!)]"},

		{MENU_ENTER,"MSG>  Enter a message on board","Msg_board# [/L] [/T=user_name]"},
		{MENU_READ,"MSG>  Read messages on board","Msg_board#"},
		{MENU_SCAN,"MSG>  Scan messages on board","Msg_board#"},
		{MENU_QSCAN,"MSG>  Quickscan messages on board","Msg_board#"},
		{MENU_NETSEARCH,"MSG>  Search for Net address","None"},
		{MENU_FREQ,"MSG>  Generate FREQ to Net address","None"},
		{MENU_USERLIST,"MSG>  Search userlist","None"},

		{MENU_SETCOMB,"COMB> Set up combined message boards","None"},
		{MENU_CFGCOMB,"COMB> Set up options for combined boards","None"},
		{MENU_READCOMB,"COMB> Read messages on combined message boards","None"},
		{MENU_SCANCOMB,"COMB> Scan messages on combined message boards","None"},
		{MENU_QSCANCOMB,"COMB> Quickscan messages on combined message boards","None"},
		{MENU_DLCOMB,"COMB> Download messages from combined message boards","None"},
		{MENU_ULCOMB,"COMB> Upload .QWK packets to message boards","None"},

		{MENU_CITY,"CFG>  Change user's city/state in simple address","None"},
		{MENU_PASSWORD,"CFG>  Change user's password","None"},
		{MENU_CLS,"CFG>  Change user's clearscreen status","None"},
		{MENU_MORE,"CFG>  Change user's MORE status","None"},
		{MENU_SCREENLEN,"CFG>  Change user's screen length","None"},
		{MENU_ANSI,"CFG>  Change user's ANSI status","None"},
		{MENU_EDITOR,"CFG>  Change user's default editor status","None"},
		{MENU_EXPERT,"CFG>  Change user's novice/expert mode","None"},
		{MENU_HOMEPHONE,"CFG>  Change user's home phone number","None"},
		{MENU_DATAPHONE,"CFG>  Change user's data/work phone number","None"},
		{MENU_CHGADD1,"CFG>  Change user's address 1 in full address","None"},
		{MENU_CHGADD2,"CFG>  Change user's address 2 in full address","None"},
		{MENU_CHGCITY,"CFG>  Change user's city in full address","None"},
		{MENU_CHGSTATE,"CFG>  Change user's state in full address","None"},
		{MENU_CHGZIP,"CFG>  Change user's zip in full address","None"},
		{MENU_CHGALIAS,"CFG>  Change user's aliases","None"},

		{MENU_LIST,"FILE> Forward list files on file board","File_board# [/NU]"},
		{MENU_REVLIST,"FILE> Reverse list files on file board","File_board# [/NU]"},
		{MENU_UPLOAD,"FILE> Upload file to file board","File_board#"},
		{MENU_DOWNLOAD,"FILE> Download file from file board","File_board#"},
		{MENU_DOWNLOAD_FILE,"FILE> Download ONE special file from board","File_board#"},
		{MENU_RAW,"FILE> Raw directory list of file board","File_board#"},
		{MENU_CONTENTS,"FILE> Contents of archive on file board","File_board#"},
		{MENU_NEW,"FILE> Show new files across file boards","File_board# [/NU]"},
		{MENU_SRCHNAME,"FILE> Search for filename across file boards","File_board# [/NU]"},
		{MENU_SRCHKEY,"FILE> Search for keyword across file boards","File_board# [/NU]"},
		{MENU_KILL,"FILE> Delete file on file board","File_board#"},
		{MENU_READFILE,"FILE> Read a text file on file board","File_board#"},
		{MENU_MOVEFILES,"FILE> Move files between file boards","Source_file_board#"},

		{MENU_SHOWFILE,"TEXT> Show ascii/ansi file","Filename"},
		{MENU_SHOWHOT,"TEXT> Show ascii/ansi menu with hotkeys","Filename"},
		{MENU_SHOWHOT_NOEXPERT,"TEXT> Show non-expert ascii/ansi menu with hotkeys","Filename"},
		{MENU_SHOWWAIT,"TEXT> Show ascii/ansi file with pause at end","Filename"},
		{MENU_QUESTION,"TEXT> Answer questionaire file","Filename"},
		{MENU_QUOTE,"TEXT> Show the quote of the moment","Filename"},

		{MENU_ADENTER,"AD>   Enter an advertisement in ad area","Ad_filename /D=days [/R=Msg_board#]"},
		{MENU_ADREAD,"AD>   Read advertisements in ad area","Ad_filename [/R=Msg_board#]"},
		{MENU_ADSCAN,"AD>   Scan advertisements in ad area","Ad_filename"},

		{MENU_NEWPRIV,"USER> Set user's privilege level","Priv_level"},
		{MENU_SETTIME,"USER> Set the time of a user to an absolute time","Minutes"},
		{MENU_UPTIME,"USER> Add minutes to a user's time","Minutes"},
		{MENU_DOWNTIME,"USER> Subtract minutes from a user's time","Minutes"},
		{MENU_ADDFLAGS,"USER> Add flags to a user's flag setting","Flags"},
		{MENU_DELFLAGS,"USER> Delete flags from a user's flag setting","Flags"},

		{MENU_XMITXMODEM,"DL>   Download using Xmodem","File_board#"},
		{MENU_XMITXMODEM1K,"DL>   Download using Xmodem-1K","File_board#"},
		{MENU_XMITYMODEM,"DL>   Download using Ymodem","File_board#"},
		{MENU_XMITYMODEMG,"DL>   Download using Ymodem-G","File_board#"},
		{MENU_XMITZMODEM,"DL>   Download using Zmodem","File_board#"},

		{MENU_RECVXMODEM,"UL>   Upload using Xmodem","File_board#"},
		{MENU_RECVXMODEM1K,"UL>   Upload using Xmodem-1K","File_board#"},
		{MENU_RECVYMODEM,"UL>   Upload using Ymodem","File_board#"},
		{MENU_RECVYMODEMG,"UL>   Upload using Ymodem-G","File_board#"},
		{MENU_RECVZMODEM,"UL>   Upload using Zmodem","File_board#"},
	};


char *mtypelines[NUM_MENUS];



int menu_comp(struct menu **arg1,struct menu **arg2)
	{
	return (*arg1)->menu_number - (*arg2)->menu_number;
	}



void draw_menu(void)
	{
	prntnomov(0x303,12,color[0],"Line Number:");
	_vcw('_',color[3],0x310,4);

	prntnomov(0x503,10,color[0],"Menu line:");
	_vcw('_',color[3],0x603,70);
	prntnomov(0x803,10,color[0],"Menu data:");
	_vcw(' ',color[0],0x80d,50);
	_vcw('_',color[3],0x903,70);

	prntnomov(0xb05,10,color[0],"Menu type:");
	_vcw('_',color[3],0xb10,56);
	prntnomov(0xc07,8,color[0],"Hot key:");
	_vcw('_',color[3],0xc10,1);
	prntnomov(0xd04,11,color[0],"Menu color:");
	_vcw('_',color[3],0xd10,30);
	prntnomov(0xe03,12,color[0],"Menu hilite:");
	_vcw('_',color[3],0xe10,30);
	prntnomov(0xf05,10,color[0],"Privilege:");
	_vcw('_',color[3],0xf10,3);
	prntnomov(0xf1a,10,color[0],"Flags:");
	prntnomov(0xf21,16,color[3],"abcdefghijklmnop");

	prntnomov(0x1003,70,color[0],"Expert mode:     <--- Show this line even when Expert mode is on?");
	_vcw('_',color[3],0x1010,1);
	prntnomov(0x1103,60,color[0],"Autoexecute:     <--- Execute this option when showing menu?");
	_vcw('_',color[3],0x1110,1);
	prntnomov(0x1203,17,color[0],"Delete line?");
	_vcw('_',color[3],0x1210,1);
	}



void fill_menu(struct menu *tmenu)
	{
	int count;

	prntnomovf(0x310,4,color[3],"%u",tmenu->menu_number);
	prntnomov(0x603,70,color[3],tmenu->menu_line);
	prntnomov(0x903,70,color[3],tmenu->menu_data);

	for (count = 0; count < NUM_MENUS; count++)
		{
		if (menutype[count].mtype_type == tmenu->menu_type)
			{
			prntnomov(0xb10,56,color[3],menutype[count].mtype_name);
			prntnomovf(0x803,56,color[0],"Menu data (%s):",menutype[count].mtype_help);
			break;
			}
		}

	_vcw((char)(tmenu->menu_key ? tmenu->menu_key : '_'),color[3],0xc10,1);
	prntnomov(0xd10,30,color[3],parse_color(tmenu->menu_color));
	prntnomov(0xe10,30,color[3],parse_color(tmenu->menu_hilite));

	prntnomovf(0xf10,3,color[3],"%u",(int)(unsigned char)tmenu->menu_priv);
	fill_flags(0xf21,tmenu->menu_flags,color[3]);

	_vcw((char)(tmenu->menu_expert ? 'Y' : 'N'),color[3],0x1010,1);
	_vcw((char)(tmenu->menu_auto ? 'Y' : 'N'),color[3],0x1110,1);
	_vcw((char)(tmenu->menu_deleted ? 'Y' : 'N'),color[3],0x1210,1);
	}



int edit_menu(char *filename)
	{
	struct window *wndw;
	struct menu tmenu;
	char buffer[100];
	int change;
	int master_change = 0;
	int current;
	int tcurrent;
	int count;
	int rtn;
	int current_menu = 0;
	int tint;
	int redo;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomovf(0x1,50,color[1]," Menu Editing Screen [%s] ",filename);
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		for (count = 0; count < 70; count++)	/* draw measuring line */
			{
			if (!count)
				_vcw('Ó',color[0],0x703 + count,1);
			else if (!(count % 10))
				_vcw('Å',color[0],0x703 + count,1);
			else if (!(count % 5))
				_vcw('Ð',color[0],0x703 + count,1);
			else 
				_vcw('Á',color[0],0x703 + count,1);
			}  
		_vcw('´',color[0],0x703 + count,1);

		_vcw('Ã',color[0],0x1400,1);
		_vcw('´',color[0],0x144f,1);
		_vcw('Ä',color[0],0x1401,78);
		prntcenter(0x1400,80,color[1]," Escape codes ");
		prntcenter(0x1500,80,color[0]," ^: hilite     $$: $ sign      $c: continue    $r: time left  $##: area path");
		prntcenter(0x1600,80,color[0],"$d: cur date   $t: cur time    $f: first name  $l: last name   $a: ansi flag");
		prntcenter(0x1700,80,color[0],"$p: comm port  $u: user #  $b: baud  $*: hot fossil  $e: pause  $k: keep time");

		do	   
			{
			draw_menu();
			if (current_menu == cur_menu)
				{
				memset(&tmenu,0,sizeof(struct menu));
				tmenu.menu_color = 7;
				tmenu.menu_hilite = 15;
				for (count = 0, tint = 0; count < cur_menu; count++)
					{
					if (tint < menu[count]->menu_number)
						tint = menu[count]->menu_number;
					}
				tmenu.menu_number = tint + 1;
				}
			else
				memcpy(&tmenu,menu[current_menu],sizeof(struct menu));
			fill_menu(&tmenu);
			current = 1;
			change = 0;
			do
				{
				switch (current)
					{
					case 0:
						do
							{
							redo = 0;
							if (tmenu.menu_number)
								sprintf(buffer,"%u",tmenu.menu_number);
							else
								{
								buffer[0] = '\0';
								_vcw('_',color[3],0x310,4);
								}
							rtn = edit_field(0x310,color[4],color[3],1,CLOCK_POS,color[0],"F04",buffer,NULL);
							if (rtn == E_EXIT)
								{
								tint = atoi(buffer);
								if (tint)
									{
									for (count = 0; count < cur_menu; count++)
										{
										if (menu[count]->menu_number == tint && count != current_menu)
											{
											sprintf(buffer,"There is already another line numbered %u!  Insert this line?",tint);
											if (askyn("Insert new menu line?",buffer,color[4],color[3],1,CLOCK_POS,color[0]))
												{
												for (count = 0; count < cur_menu; count++)
													{
													if (menu[count]->menu_number >= tint)
														{
														++menu[count]->menu_number;
														master_change = 1;
														}
													}
												}
											else
												redo = 1;
											break;
											}
										}
									}
								else
									{
									error(NOTICE,"Line number 0 is not a valid menu line!",1,CLOCK_POS,color[0]);
									redo = 1;
									}
								if (!redo)
									{
									tmenu.menu_number = tint;
									if (!tmenu.menu_number)
										_vcw('_',color[3],0x310,4);
									}
								}
							}
						while (redo);
						break;

					case 1:
						rtn = edit_field(0x603,color[4],color[3],1,CLOCK_POS,color[0],"F070",tmenu.menu_line,get_anychar);
						break;

					case 2:
						rtn = edit_field(0x903,color[4],color[3],1,CLOCK_POS,color[0],"F070",tmenu.menu_data,get_anychar);
						break;

					case 3:
						for (count = 0; count < NUM_MENUS; count++)			/* hilight current in list */
							{
							if (tmenu.menu_type == menutype[count].mtype_type)
								break;
							}
						rtn = select_menu(0xc1a,color[4],color[3],color[0],"Select Menu Type",NUM_MENUS,mtypelines,50,count,1,1,CLOCK_POS,color[0]);
						if ((rtn & 0xf) == E_EXIT)
							{
							tmenu.menu_type = menutype[rtn >> 4].mtype_type;
							_vcw('_',color[3],0xb10,56);
							prntnomov(0xb10,56,color[3],menutype[rtn >> 4].mtype_name);
							_vcw(' ',color[0],0x80d,50);
							prntnomovf(0x803,56,color[0],"Menu data (%s):",menutype[rtn >> 4].mtype_help);
							rtn = E_EXIT;
							}
						break;

					case 4:
						buffer[0] = tmenu.menu_key;
						buffer[1] = '\0';
						rtn = edit_field(0xc10,color[4],color[3],1,CLOCK_POS,color[0],"FPU1",buffer,get_anychar_first);
						if (rtn == E_EXIT)
							tmenu.menu_key = buffer[0];
						break;

					case 5:
			   			tint = tmenu.menu_color;
						if ((rtn = select_color("Normal Line Color",&tint)) == E_EXIT)
							{
							if (tint != tmenu.menu_color)
								{
								tmenu.menu_color = (char)tint;
								_vcw('_',color[3],0xd10,30);
								prntnomov(0xd10,30,color[3],parse_color(tmenu.menu_color));
								change = 1;
								}
							}
						break;

					case 6:
			   			tint = tmenu.menu_hilite;
						if ((rtn = select_color("Higlighted Line Color",&tint)) == E_EXIT)
							{
							if (tint != tmenu.menu_hilite)
								{
								tmenu.menu_hilite = (char)tint;
								_vcw('_',color[3],0xe10,30);
								prntnomov(0xe10,30,color[3],parse_color(tmenu.menu_hilite));
								change = 1;
								}
							}
						break;

					case 7:
						sprintf(buffer,"%u",(int)(unsigned char)tmenu.menu_priv);
						rtn = edit_number(0xf10,color[4],color[3],buffer,1,255,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tint = atoi(buffer);
							tmenu.menu_priv = (char)(unsigned char)tint;
							}
						break;

					case 8:
						rtn = edit_flags(0xf21,&tmenu.menu_flags,color[4],color[3],1,CLOCK_POS,color[0]);
						break;

					case 9:
						rtn = pop_menu(0xe12,color[4],color[3],"Expert mode?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								if (!tmenu.menu_expert)
									{
									tmenu.menu_expert = 1;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							else
								{
								if (tmenu.menu_expert)
									{
									tmenu.menu_expert = 0;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							_vcw((char)(tmenu.menu_expert ? 'Y' : 'N'),color[3],0x1010,1);
							}
						break;

					case 10:
						rtn = pop_menu(0xf14,color[4],color[3],"Autoexecute?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								if (!tmenu.menu_auto)
									{
									tmenu.menu_auto = 1;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							else
								{
								if (tmenu.menu_auto)
									{
									tmenu.menu_auto = 0;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							_vcw((char)(tmenu.menu_auto ? 'Y' : 'N'),color[3],0x1110,1);
							}
						break;

					case 11:
						rtn = pop_menu(0x1017,color[4],color[3],"Delete Line",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								if (!tmenu.menu_deleted)
									{
									tmenu.menu_deleted = 1;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							else
								{
								if (tmenu.menu_deleted)
									{
									tmenu.menu_deleted = 0;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							_vcw((char)(tmenu.menu_deleted ? 'Y' : 'N'),color[3],0x1210,1);
							}
						break;
					}

				if (rtn == E_EXIT)
					change = 1;
				if (rtn == E_FORE || rtn == E_EXIT)
					{
					if (current < 11)
						++current;
					else
						current = 0;
					}
				if (rtn == E_BACK)
					{
					if (current)
						--current;
					else
						current = 11;
					}
				if (rtn == E_END && !change)
					rtn = E_FORE;
				}
			while (rtn != E_END && rtn != E_QUIT && rtn != E_PGUP && rtn != E_PGDN);

			if ((rtn == E_END || rtn == E_PGUP || rtn == E_PGDN) && change)
				{
				if (current_menu == cur_menu)
					{
					if (cur_menu >= max_menu)
						{
						if (!(menu = realloc(menu,(max_menu += 10) * sizeof(struct menu *))))
							error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
						}
					if (!(menu[cur_menu] = calloc(1,sizeof(struct menu))))
	 					error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
					memcpy(menu[cur_menu],&tmenu,sizeof(struct menu));
					++cur_menu;
					}
				else
					memcpy(menu[current_menu],&tmenu,sizeof(struct menu));
				master_change = 1;

				tcurrent = tmenu.menu_number;
				qsort(menu,cur_menu,sizeof(struct menu *),menu_comp);
				for (count = 0; count < cur_menu; count++)
					{
					if (tcurrent == menu[count]->menu_number)
						{
						current_menu = count;
						break;
						}
					}
				}
			if (rtn == E_PGDN)
				{
				if (current_menu == cur_menu)
					current_menu = 0;
				else
					++current_menu;
				}
			else if (rtn == E_PGUP)
				{
				if (current_menu)
					--current_menu;
				else
					current_menu = cur_menu;
				}
			else if (rtn == E_END)
				current_menu = cur_menu;
			}
		while (rtn != E_QUIT);

		close_window(wndw);
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);

	return master_change;
	}



void draw_menu_screen(void)
	{
	prntnomov(0x505,14,color[0],"Prompt String:");
	prntnomov(0x606,13,color[0],"Normal Color:");
	prntnomov(0x703,16,color[0],"Highlight Color:");
	prntnomov(0x904,15,color[0],"Fields in Menu:");
	prntnomov(0xa03,16,color[0],"View Priv Level:");
	prntnomov(0xb08,16,color[0],"View Flags:");

	drawbox(0x1102,0x164d,color[0],0);
	prntnomov(0x1204,3,color[1],"[S]");
	prntnomov(0x1209,17,color[0],"Show current menu");
	prntnomov(0x1304,3,color[1],"[L]");
	prntnomov(0x1309,25,color[0],"Change view priv level");
	prntnomov(0x1404,3,color[1],"[F]");
	prntnomov(0x1409,17,color[0],"Change view flags");
	prntnomov(0x1504,3,color[1],"[E]");
	prntnomov(0x1509,17,color[0],"Edit menu choices");
	prntnomov(0x1228,3,color[1],"[P]");
	prntnomov(0x122d,18,color[0],"Edit prompt string");
	prntnomov(0x1328,3,color[1],"[N]");
	prntnomov(0x132d,26,color[0],"Change normal prompt color");
	prntnomov(0x1428,3,color[1],"[H]");
	prntnomov(0x142d,29,color[0],"Change highlight prompt color");
	}



void load_menu(FILE *fd,char *filename)
	{
	struct window *wndw;
	struct prompt tprompt;
	struct menu tmenu;
	char buffer[81];
	int current;
	int count;
	int quit = 0;
	int change = 0;
	int rtn;
	int tval;
	int out;
	int key;

	if (!fread(&tprompt,1,sizeof(struct prompt),fd) || tprompt.prompt_sig != M_SIGNATURE)
		{
		sprintf(buffer,"File \"%s\" is not a valid menu file...aborting edit!",filename);
		error(NOTICE,buffer,1,CLOCK_POS,color[0]);
		return;
		}

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomovf(0x1,40,color[1]," Menu Editor [%s] ",filename);
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		draw_menu_screen();

		while (fread(&tmenu,1,sizeof(struct menu),fd))
			{
			if (cur_menu >= max_menu)
				{
				if (!(menu = realloc(menu,(max_menu += 20) * sizeof(struct menu *))))
	 				error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
				}
			if (!(menu[cur_menu] = calloc(1,sizeof(struct menu))))
 				error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
			memcpy(menu[cur_menu],&tmenu,sizeof(struct menu));
			++cur_menu;
			}
		if (cur_menu)
			qsort(menu,cur_menu,sizeof(struct menu *),menu_comp);

		for (count = 0; count < NUM_MENUS; count++)
			mtypelines[count] = menutype[count].mtype_name;
		do
			{
			_vcw('_',color[3],0x514,55);
			_vcw('_',color[3],0x614,30);
			_vcw('_',color[3],0x714,30);
			_vcw('_',color[3],0x914,4);
			_vcw('_',color[3],0xa14,3);
			prntnomov(0x514,55,color[3],tprompt.prompt_string);
			prntnomov(0x614,30,color[3],parse_color(tprompt.prompt_color));
			prntnomov(0x714,30,color[3],parse_color(tprompt.prompt_hilite));
			prntnomovf(0x914,4,color[3],"%u",cur_menu);
			prntnomovf(0xa14,3,color[3],"%u",view_level);
			fill_flags(0xb14,view_flags,color[3]);

			out = 0;
			do
				{
				set_cursor_type(HIDDEN);
				key = read_keyboard(1,CLOCK_POS,color[0]);
				switch (key)
					{
					case 'P':
					case 'p':
			  			rtn = edit_field(0x514,color[4],color[3],1,CLOCK_POS,color[0],"F055",tprompt.prompt_string,get_anychar);
						if (rtn == E_EXIT)
							{
							change = 1;
							out = 1;
							}
						break;
					case 'N':
					case 'n':
			   			tval = tprompt.prompt_color;
						if (select_color("Normal Prompt Color",&tval) == E_EXIT)
							{
							if (tval != tprompt.prompt_color)
								{
								tprompt.prompt_color = (char)tval;
								change = 1;
								out = 1;
								}
							}
						break;
					case 'F':
					case 'f':
						edit_flags(0xb14,&view_flags,color[4],color[3],1,CLOCK_POS,color[0]);
						break;
					case 'H':
					case 'h':
			   			tval = tprompt.prompt_hilite;
						if (select_color("Highlight Prompt Color",&tval) == E_EXIT)
							{
							if (tval != tprompt.prompt_hilite)
								{
								tprompt.prompt_hilite = (char)tval;
								change = 1;
								out = 1;
								}
							}
						break;
					case 'L':
					case 'l':
						sprintf(buffer,"%u",view_level);
						if (edit_field(0xa14,color[4],color[3],1,CLOCK_POS,color[0],"F03",buffer,NULL) == E_EXIT)
							{
							view_level = atoi(buffer);
							if (view_level > 255)
								view_level = 255;
							out = 1;
							}
						break;
					case 'S':
					case 's':
						show_screen(&tprompt,filename,view_level,view_flags);
						break;
					case 'E':
					case 'e':
					case CR:
						if (edit_menu(filename))
							{
							out = 1;
							change = 1;
							}
						break;
					case ESC:
						out = quit = 1;
						break;
					}
				}
			while (!out);
			set_cursor_type(LINE);
			}
		while (!quit);

		if (change)
			{
			if (askyn("Save Menu Changes?","Do you want to save the changes made to this menu? (Y/N) ",color[4],color[3],1,CLOCK_POS,color[0]))
				{
				fseek(fd,0L,SEEK_SET);
				chsize(fileno(fd),0L);
				fwrite(&tprompt,1,sizeof(struct prompt),fd);
				for (count = 0, current = 1; count < cur_menu; count++)
					{
					if (!menu[count]->menu_deleted)
						{
						menu[count]->menu_number = current++;		/* renumber lines as they are written out */
						fwrite(menu[count],1,sizeof(struct menu),fd);
						}
					}
				}
			}
		if (max_menu)
			{
			for (count = 0; count < cur_menu; count++)
				free(menu[count]);
			free(menu);
			menu = NULL;
			cur_menu = max_menu = 0;
			}

		close_window(wndw);
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);
	}



void show_screen(struct prompt *tprompt,char *filename,int level,int flags)
	{
	struct window *wndw;
	char flgs[17];
	char *cptr;
	char tchar;
	int hicolor;
	int locolor;
	int which = 0;
	int count;
	int do_nl;
	int xpos = 0;
	int ypos = 2;
	int val = 1;

	if (wndw = open_window(0x0,0x184f,BLACK,4))
		{
		tchar = 'A';
		for (count = 0; count < 16; count++)
			{
			if (flags & val)
				flgs[count] = tchar;
			else 
				flgs[count] = (char)tolower(tchar);
			val <<= 1;
			++tchar;
			}
		flgs[16] = (char)'\0';

		prntnomovf(0x0,80,YELLOW,"Screen for menu \"%s\" at priv level %u and flags \"%s\"",filename,level,flgs);
		for (count = 0; count < cur_menu; count++)
			{
			which = 0;
			if (!menu[count]->menu_deleted && level >= (int)(unsigned char)menu[count]->menu_priv)
				{
				locolor = menu[count]->menu_color;
				hicolor = menu[count]->menu_hilite;
				cptr = menu[count]->menu_line;
				do_nl = 1;
				while (*cptr)
					{
					if (*cptr != '$' && *cptr != '^')
						{
						_vcw(*cptr,which ? hicolor : locolor,(ypos << 8) | xpos,1);
						++xpos;
						if (xpos >= 80)
							{
							xpos = 0;
							++ypos;
							if (ypos >= 25)
								{
								scroll_up(0x0,0x184f,NORMAL,1);
								ypos = 24;
								}
							}
						}
					else if (*cptr == '^')
						which = which ? 0 : 1;
					else 
						{
						switch (*(cptr + 1))
							{
							case '$':
								_vcw('$',which ? hicolor : locolor,(ypos << 8) | xpos,1);
								++xpos;
								if (xpos >= 80)
									{
									xpos = 0;
									++ypos;
									if (ypos >= 25)
										{
										scroll_up(0x0,0x184f,NORMAL,1);
										ypos = 24;
										}
									}
								++cptr;
								break;
							case 'D':
							case 'd':
							case 'T':
							case 't':
							case 'R':
							case 'r':
							case 'B':
							case 'b':
							case 'F':
							case 'f':
							case 'L':
							case 'l':
							case 'U':
							case 'u':
							case 'A':
							case 'a':
								_vcw('$',which ? hicolor : locolor,(ypos << 8) | xpos,1);
								++xpos;
								if (xpos >= 80)
									{
									xpos = 0;
									++ypos;
									if (ypos >= 25)
										{
										scroll_up(0x0,0x184f,NORMAL,1);
										ypos = 24;
										}
									}
								break;
							case 'C':
							case 'c':
								if (*(cptr + 2))
									++cptr;		/* continue must be LAST on line */
								else
									{
									do_nl = 0;		/* skip NL */
									++cptr;
									}
								break;
							}
						}
					++cptr;
					}
				if (do_nl)
					{
					xpos = 0;
					++ypos;
					if (ypos >= 25)
						{
						scroll_up(0x0,0x184f,NORMAL,1);
						ypos = 24;
						}
					}
				}
			}

		xpos = 0;
		++ypos;
		if (ypos >= 25)
			{
			scroll_up(0x0,0x184f,NORMAL,1);
			ypos = 24;
			}
		locolor = tprompt->prompt_color;
		hicolor = tprompt->prompt_hilite;
		cptr = tprompt->prompt_string;
		do_nl = 1;
		which = 0;
		while (*cptr)
			{
			if (*cptr != '$' && *cptr != '^')
				{
				_vcw(*cptr,which ? hicolor : locolor,(ypos << 8) | xpos,1);
				++xpos;
				if (xpos >= 80)
					{
					xpos = 0;
					++ypos;
					if (ypos >= 25)
						{
						scroll_up(0x0,0x184f,NORMAL,1);
						ypos = 24;
						}
					}
				}
			else if (*cptr == '^')
				which = which ? 0 : 1;
			else 
				{
				switch (*(cptr + 1))
					{
					case '$':
						_vcw('$',which ? hicolor : locolor,(ypos << 8) | xpos,1);
						++xpos;
						if (xpos >= 80)
							{
							xpos = 0;
							++ypos;
							if (ypos >= 25)
								{
								scroll_up(0x0,0x184f,NORMAL,1);
								ypos = 24;
								}
							}
						++cptr;
						break;
					case 'D':
					case 'd':
					case 'T':
					case 't':
					case 'R':
					case 'r':
					case 'B':
					case 'b':
					case 'F':
					case 'f':
					case 'L':
					case 'l':
					case 'A':
					case 'a':
					case 'U':
					case 'u':
						_vcw('$',which ? hicolor : locolor,(ypos << 8) | xpos,1);
						++xpos;
						if (xpos >= 80)
							{
							xpos = 0;
							++ypos;
							if (ypos >= 25)
								{
								scroll_up(0x0,0x184f,NORMAL,1);
								ypos = 24;
								}
							}
						break;
					}
				}
			++cptr;
			}
		xpos = 0;
		ypos += 3;
		if (ypos >= 25)
			{
			scroll_up(0x0,0x184f,NORMAL,ypos - 24);
			ypos = 24;
			}
		prntmov((ypos << 8) | xpos,80,YELLOW | BLINK,"Press any key to continue....");
		read_keyboard(0,0,0);

		close_window(wndw);
		}
	else
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);
	}
