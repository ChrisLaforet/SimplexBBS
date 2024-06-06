/* s_gpath.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_gpath.c_v  $
**                       $Date:   25 Oct 1992 14:15:26  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <screen.h>
#include <window.h>
#include <editor.h>
#include "sconfig.h"




void draw_general_paths(void)
	{
	prntnomov(0x406,9,color[0],"Menu Dir:");
	prntnomov(0x504,11,color[0],"Screen Dir:");
	prntnomov(0x603,13,color[0],"File-Attach");
	prntnomov(0x703,13,color[0],"and QWK Dir:");

	_vcw('_',color[3],0x410,60);
	_vcw('_',color[3],0x510,60);
	_vcw('_',color[3],0x710,60);

	drawbox(0x902,0xd4d,color[0],0);
	prntcenter(0xa03,74,color[0],"Menu path is where SIMPLEX looks for its menu files.  Screen path is");
	prntcenter(0xb03,74,color[0],"where the ANS and ASC files are.  File-Attach and QWK directory is");
	prntcenter(0xc03,74,color[0],"where file-attaches and QWK up- and downloads are shuttled through.");

	prntnomov(0xf02,13,color[0],"Welcome Prog:");
	prntnomov(0x1002,13,color[0],"Virus Checkr:");
	_vcw('_',color[3],0xf10,60);
	_vcw('_',color[3],0x1010,60);

	drawbox(0x1202,0x164d,color[0],0);
	prntnomov(0x1304,80,color[0],"$$: $ sign    $c: continue  $r: time left  $##: area path  $*: baud     ");
	prntnomov(0x1404,80,color[0],"$d: cur date  $t: cur time  $f: first name  $l: last name  $a: ansi flag");
	prntnomov(0x1504,80,color[0],"$p: commport  $u: user #    $*: hot fossil  $e: pause      $k: keep time");
	}



void fill_general_paths(void)
	{
	prntnomov(0x410,60,color[3],cfg.cfg_menupath);
	prntnomov(0x510,60,color[3],cfg.cfg_screenpath);
	prntnomov(0x710,60,color[3],cfg.cfg_fapath);

	prntnomov(0xf10,60,color[3],cfg.cfg_welcome);
	prntnomov(0x1010,60,color[3],cfg.cfg_viruschk);
	}



void edit_general_paths(void)
	{
	struct window *wndw;
	int change = 0;
	int current = 0;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," General Path Information ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		draw_general_paths();
		fill_general_paths();

		do
			{
			switch (current)
				{
				case 0:
					rtn = edit_field(0x410,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_menupath,get_pathname);
					break;
				case 1:
					rtn = edit_field(0x510,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_screenpath,get_pathname);
					break;
				case 2:
					rtn = edit_field(0x710,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_fapath,get_pathname);
					break;
				case 3:
					rtn = edit_field(0xf10,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_welcome,get_anychar);
					break;
				case 4:
					rtn = edit_field(0x1010,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_viruschk,get_anychar);
					break;
				}
			if (rtn == E_EXIT)
				change = 1;
			if (rtn == E_FORE || rtn == E_EXIT)
				{
				if (current < 4)
					++current;
				else
					current = 0;
				}
			if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 4;
				}
			}
		while (rtn != E_END && rtn != E_QUIT);

		close_window(wndw);
		}
	else
		error(NOTICE,"Unable to open window!",1,CLOCK_POS + 0x1700,color[0]);

	if (change)
		master_change = 1;
	}




