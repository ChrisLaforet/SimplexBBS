/* s_upath.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_upath.c_v  $
**                       $Date:   25 Oct 1992 14:15:36  $
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




void draw_unarchiver_paths(void)
	{
	prntnomov(0x702,14,color[0],"UnARC Command:");
	prntnomov(0x802,14,color[0],"UnZIP Command:");
	prntnomov(0xa02,14,color[0],"UnLZH Command:");
	prntnomov(0xb02,14,color[0],"UnZOO Command:");
	prntnomov(0xc02,14,color[0],"UnARJ Command:");

	_vcw('_',color[3],0x711,60);
	_vcw('_',color[3],0x811,60);
	_vcw('_',color[3],0xa11,60);
	_vcw('_',color[3],0xb11,60);
	_vcw('_',color[3],0xc11,60);

	drawbox(0xf02,0x164d,color[0],0);
	prntcenter(0x1000,80,color[0],"The unARC, unZIP, unLZH, unZOO, unARJ commands are used by SMAIL to");
	prntcenter(0x1100,80,color[0],"unarchive incoming archived Net/Echo mail and also to unarchive");
	prntcenter(0x1200,80,color[0],"inbound QWK mail archives.  Notice that unless you have use for these");
	prntcenter(0x1300,80,color[0],"options, they do not have to be configured.  In the strings, substitute");
	prntcenter(0x1400,80,color[0],"%S as the name of the file to unarchive, %D (%P) as the destination path for");
	prntcenter(0x1500,80,color[0],"the files which come out of the archive (%P without trailing backslash).");
	}



void fill_unarchiver_paths(void)
	{
	prntnomov(0x711,60,color[3],cfg.cfg_unarc);
	prntnomov(0x811,60,color[3],cfg.cfg_unzip);
	prntnomov(0xa11,60,color[3],cfg.cfg_unlzh);
	prntnomov(0xb11,60,color[3],cfg.cfg_unzoo);
	prntnomov(0xc11,60,color[3],cfg.cfg_unarj);
	}



void edit_unarchiver_paths(void)
	{
	struct window *wndw;
	int change = 0;
	int current = 0;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," BBS Unarchiver Information ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");
		draw_unarchiver_paths();
		fill_unarchiver_paths();

		do
			{
			switch (current)
				{
				case 0:
					rtn = edit_field(0x711,color[4],color[3],0,0,0,"F060",cfg.cfg_unarc,get_anychar);
					break;
				case 1:
					rtn = edit_field(0x811,color[4],color[3],0,0,0,"F060",cfg.cfg_unzip,get_anychar);
					break;
				case 2:
					rtn = edit_field(0xa11,color[4],color[3],0,0,0,"F060",cfg.cfg_unlzh,get_anychar);
					break;
				case 3:
					rtn = edit_field(0xb11,color[4],color[3],0,0,0,"F060",cfg.cfg_unzoo,get_anychar);
					break;
				case 4:
					rtn = edit_field(0xc11,color[4],color[3],0,0,0,"F060",cfg.cfg_unarj,get_anychar);
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



