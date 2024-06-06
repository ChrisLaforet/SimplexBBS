/* s_main.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_main.c_v  $
**                       $Date:   25 Oct 1992 14:15:26  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <screen.h>
#include <window.h>
#include <editor.h>
#include "sconfig.h"



int color_change_flag;



void shell(void)
	{
	struct window *wndw;
	char *cptr;

	if (wndw = open_window(0x0,0x184f,NORMAL,4))
		{
		_setcurpos(0x300);
		set_cursor_type(LINE);
#ifdef PROTECTED
		prntnomovf(0x0,80,YELLOW,"SCONFIG/2 (v %u.%02u):  Simplex/2 BBS Configuration Editor.",MAJOR_VERSION,MINOR_VERSION);
		prntnomov(0x100,80,WHITE,"OS/2 Shell. Type      to return to the program.");
		prntnomov(0x111,4,LT_CYAN | BLINK,"EXIT");
#else
		prntnomovf(0x0,80,YELLOW,"SCONFIG (v %u.%02u):  Simplex BBS Configuration Editor.",MAJOR_VERSION,MINOR_VERSION);
		prntnomov(0x100,80,WHITE,"DOS Shell. Type      to return to the program.");
		prntnomov(0x110,4,LT_CYAN | BLINK,"EXIT");
#endif
		if (!(cptr = getenv("COMSPEC")))
			{
#ifdef PROTECTED
			cptr = "CMD.EXE";
#else
			cptr = "COMMAND.COM";
#endif
			}
		if (spawnlp(P_WAIT,cptr,cptr,NULL) == -1)
			error(NON_FATAL,"Unable to find or run command processor",0,0,0);
		close_window(wndw);
		}
	}



/*******************************/

#define MAX_BBSINFO 		3


struct mnu bbsinfo_choices[MAX_BBSINFO] =
	{
		{
			"^1...Edit general BBS info",
			'1',
			"Edit the BBS names and addressing information",
			edit_general_info,
		},
		{
			"^2...Edit miscellaneous info",
			'2',
			"Edit basic operating parameters for the BBS",
			edit_miscellaneous_info,
		},
		{
			"^X...Exit to main menu",
			'X',
			"Return to the main menu",
			EXIT_MENU,
		},
	};



void bbsinfo_menu(void)
	{
	push_menu(0x711,color[4],color[3]," BBS Info Menu ",bbsinfo_choices,MAX_BBSINFO);
	}



/*******************************/

#define MAX_PATHINFO 		5


struct mnu pathinfo_choices[MAX_PATHINFO] =
	{
		{
			"^1...Edit general BBS paths",
			'1',
			"Edit the basic pathnames needed for BBS operation",
			edit_general_paths,
		},
		{
			"^2...Edit network BBS paths",
			'2',
			"Edit the network pathnames needed for BBS operation as a node",
			edit_network_paths,
		},
		{
			"^3...Edit archiver paths",
			'3',
			"Edit the pathnames for archivers needed for network and QWK mail",
			edit_archiver_paths,
		},
		{
			"^4...Edit unarchiver paths",
			'4',
			"Edit the pathnames for unarchivers needed for network and QWK mail",
			edit_unarchiver_paths,
		},
		{
			"^X...Exit to main menu",
			'X',
			"Return to the main menu",
			EXIT_MENU,
		},
	};



void pathinfo_menu(void)
	{
	push_menu(0x911,color[4],color[3]," Path Info Menu ",pathinfo_choices,MAX_PATHINFO);
	}



/*******************************/

#define MAX_FILEINFO 		3


struct mnu fileinfo_choices[MAX_FILEINFO] =
	{
		{
			"^1...Edit file boards",
			'1',
			"Add/change/delete file area boards",
			edit_file_boards,
		},
		{
			"^2...Edit file listings",
			'2',
			"Edit filelist.bbs files in each file board area",
			edit_file_lists,
		},
		{
			"^X...Exit to main menu",
			'X',
			"Return to the main menu",
			EXIT_MENU,
		},
	};



void fileinfo_menu(void)
	{
	push_menu(0xb11,color[4],color[3]," File Info Menu ",fileinfo_choices,MAX_FILEINFO);
	}



/*******************************/

#define MAX_MAIN		12

struct mnu main_menu_choices[MAX_MAIN] =
	{
		{
			"^1...BBS info menu",
			'1',
			"Go to a submenu to edit general and miscellaneous BBS information",
			bbsinfo_menu,
		},
		{
			"^2...Edit modem info",
			'2',
			"Change modem parameters and strings",
			edit_modem_info,
		},
		{
			"^3...Path/archiver info menu",
			'3',
			"Go to a submenu to edit BBS paths, archivers and unarchivers",
			pathinfo_menu,
		},
		{
			"^4...Edit message boards",
			'4',
			"Add/change/delete message area boards",
			edit_msg_boards,
		},
		{
			"^5...File board info menu",
			'5',
			"Add/change/delete file area boards",
			fileinfo_menu,
		},
		{
			"^6...Edit event info",
			'6',
			"Add/change/delete Simplex events",
			edit_events,
		},
		{
			"^7...Configure menus",
			'7',
			"Add/change/view/delete Simplex menus",
			edit_menufile,
		},
		{
			"^8...Edit userlist info",
			'8',
			"Add/change/view/delete users in userlist",
			edit_user_list,
		},
		{
			"^9...Edit user online times",
			'9',
			"Add/change/view/delete online times for user priv levels",
			edit_times,
		},
		{
			"^C...Configure colors",
			'C',
			"Change SCONFIG's color configuration and snow checking",
			configure_colors,
		},
#ifdef PROTECTED
		{
			"^S...Shell to OS/2",
			'S',
			"Start a temporary copy of OS/2's CMD.EXE command processor",
			shell,
		},
#else
		{
			"^S...Shell to DOS",
			'S',
			"Start a temporary copy of DOS' COMMAND.COM command processor",
			shell,
		},
#endif
		{
#ifdef PROTECTED
			"^X...Exit back to OS/2",
#else
			"^X...Exit back to DOS",
#endif
			'X',
			"Quit using this program and return to DOS",
			EXIT_MENU,
		},
	};



void main_menu(void)
	{
	int quit = 0;

	do
		{
		color_change_flag = 0;
		_cls(color[0]);
		clrblk(0x0,0x44f,color[3]);
		drawbox(0x0,0x44f,color[3],1);
		drawbox(0x500,0x174f,color[0],0);
#ifdef PROTECTED
		prntcenter(0x200,80,color[3],"SCONFIG/2: Simplex/2 BBS Configuration Editor.");
#else
		prntcenter(0x200,80,color[3],"SCONFIG: Simplex BBS Configuration Editor.");
#endif
		_vcw(' ',color[3],0x1800,80);
#ifdef PROTECTED
 		prntnomovf(0x1702,40,color[0],"[ SCONFIG/2: Version %u.%02u ]",MAJOR_VERSION,MINOR_VERSION);
#else
 		prntnomovf(0x1702,40,color[0],"[ SCONFIG: Version %u.%02u ]",MAJOR_VERSION,MINOR_VERSION);
#endif
		do
			{
			push_menu(0x602,color[4],color[3]," M a i n   M e n u ",main_menu_choices,MAX_MAIN);
			if (!color_change_flag)
				{
				_vcw(' ',color[3],0x1800,80);
				if (askyn("Confirm Exit to DOS","Are you SURE that you want to exit this program? ",color[4],color[3],1,CLOCK_POS + 0x1700,color[0]))
					quit = 1;
				}
			}
		while (!quit && !color_change_flag);
		}
	while (!quit);
	}



