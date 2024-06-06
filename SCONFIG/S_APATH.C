/* s_apath.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_apath.c_v  $
**                       $Date:   25 Oct 1992 14:15:18  $
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




void draw_archiver_paths(void)
	{
	prntnomov(0x703,12,color[0],"ARC Command:");
	prntnomov(0x803,12,color[0],"ZIP Command:");
	prntnomov(0xa03,12,color[0],"LZH Command:");
	prntnomov(0xb03,12,color[0],"ZOO Command:");
	prntnomov(0xc03,12,color[0],"ARJ Command:");

	_vcw('_',color[3],0x710,60);
	_vcw('_',color[3],0x810,60);
	_vcw('_',color[3],0xa10,60);
	_vcw('_',color[3],0xb10,60);
	_vcw('_',color[3],0xc10,60);

	drawbox(0xf02,0x164d,color[0],0);
	prntcenter(0x1000,80,color[0],"The ARC, ZIP, LZH, ZOO, ARJ commands are used by SMP to prepare");
	prntcenter(0x1100,80,color[0],"outgoing archived Net/Echo mail and also to prepare QWK mail archives.");
	prntcenter(0x1200,80,color[0],"Notice that unless you have use for these options, they do not have to");
	prntcenter(0x1300,80,color[0],"be configured.  In the strings, substitute %A as the file to archive,");
	prntcenter(0x1400,80,color[0],"and %F as the file to insert into the archive.  Notice that while");
	prntcenter(0x1500,80,color[0],"many Fidonet systems handle ARC and ZIP, others are more dubious.");
	}



void fill_archiver_paths(void)
	{
	prntnomov(0x710,60,color[3],cfg.cfg_arc);
	prntnomov(0x810,60,color[3],cfg.cfg_zip);
	prntnomov(0xa10,60,color[3],cfg.cfg_lzh);
	prntnomov(0xb10,60,color[3],cfg.cfg_zoo);
	prntnomov(0xc10,60,color[3],cfg.cfg_arj);
	}



void edit_archiver_paths(void)
	{
	struct window *wndw;
	int change = 0;
	int current = 0;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," BBS Archiver Information ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");
		draw_archiver_paths();
		fill_archiver_paths();

		do
			{
			switch (current)
				{
				case 0:
					rtn = edit_field(0x710,color[4],color[3],0,0,0,"F060",cfg.cfg_arc,get_anychar);
					break;
				case 1:
					rtn = edit_field(0x810,color[4],color[3],0,0,0,"F060",cfg.cfg_zip,get_anychar);
					break;
				case 2:
					rtn = edit_field(0xa10,color[4],color[3],0,0,0,"F060",cfg.cfg_lzh,get_anychar);
					break;
				case 3:
					rtn = edit_field(0xb10,color[4],color[3],0,0,0,"F060",cfg.cfg_zoo,get_anychar);
					break;
				case 4:
					rtn = edit_field(0xc10,color[4],color[3],0,0,0,"F060",cfg.cfg_arj,get_anychar);
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
