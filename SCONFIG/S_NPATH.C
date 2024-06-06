/* s_npath.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_npath.c_v  $
**                       $Date:   25 Oct 1992 14:15:34  $
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




void draw_network_paths(void)
	{
	prntnomov(0x702,13,color[0],"Nodelist Dir:");
	prntnomov(0x904,11,color[0],"Matrix Dir:");
	prntnomov(0xa03,12,color[0],"Inbound Dir:");
	prntnomov(0xb02,13,color[0],"Outbound Dir:");
	prntnomov(0xc04,11,color[0],"Packet Dir:");
	prntnomov(0xd04,11,color[0],"BadMsg Dir:");

	_vcw('_',color[3],0x710,60);
	_vcw('_',color[3],0x910,60);
	_vcw('_',color[3],0xa10,60);
	_vcw('_',color[3],0xb10,60);
	_vcw('_',color[3],0xc10,60);
	_vcw('_',color[3],0xd10,60);

	drawbox(0x1002,0x154d,color[0],0);
	prntcenter(0x1100,80,color[0],"Nodelist path is where the compiled nodelist is located.  The Matrix");
	prntcenter(0x1200,80,color[0],"path is where SMP looks for *.msg files.  Inbound path is where the");
	prntcenter(0x1300,80,color[0],"mailer places incoming mail.  Outbound path is where the mailer looks");
	prntcenter(0x1400,80,color[0],"for outgoing mail.  Packet path is a temporary directory used by SMAIL.");
	}



void fill_network_paths(void)
	{
	prntnomov(0x710,60,color[3],cfg.cfg_nodepath);
	prntnomov(0x910,60,color[3],cfg.cfg_netpath);
	prntnomov(0xa10,60,color[3],cfg.cfg_inboundpath);
	prntnomov(0xb10,60,color[3],cfg.cfg_outboundpath);
	prntnomov(0xc10,60,color[3],cfg.cfg_packetpath);
	prntnomov(0xd10,60,color[3],cfg.cfg_badmsgpath);
	}



void edit_network_paths(void)
	{
	struct window *wndw;
	int change = 0;
	int current = 0;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," Network Path Information ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		draw_network_paths();
		fill_network_paths();

		do
			{
			switch (current)
				{
				case 0:
					rtn = edit_field(0x710,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_nodepath,get_pathname);
					break;
				case 1:
					rtn = edit_field(0x910,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_netpath,get_pathname);
					break;
				case 2:
					rtn = edit_field(0xa10,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_inboundpath,get_pathname);
					break;
				case 3:
					rtn = edit_field(0xb10,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_outboundpath,get_pathname);
					break;
				case 4:
					rtn = edit_field(0xc10,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_packetpath,get_pathname);
					break;
				case 5:
					rtn = edit_field(0xd10,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_badmsgpath,get_pathname);
					break;
				}
			if (rtn == E_EXIT)
				change = 1;
			if (rtn == E_FORE || rtn == E_EXIT)
				{
				if (current < 5)
					++current;
				else
					current = 0;
				}
			if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 5;
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



