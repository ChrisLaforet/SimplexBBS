/* s_gen.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_gen.c_v  $
**                       $Date:   25 Oct 1992 14:15:24  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <screen.h>
#include <window.h>
#include <editor.h>
#include "sconfig.h"



void draw_general_info(void)
	{
	int count;

	prntnomov(0x303,9,color[0],"BBS Name:");
	_vcw('_',color[3],0x30d,60);
	prntnomov(0x406,6,color[0],"Sysop:");
	_vcw('_',color[3],0x40d,40);
	prntnomov(0x505,7,color[0],"Origin:");
	_vcw('_',color[3],0x50d,65);
	prntnomov(0x603,9,color[0],"QWK Name:");
	prntnomov(0x617,80,color[0],"<--- Used for QWK (Offline Reader) Filename");
	_vcw('_',color[3],0x60d,8);
	prntnomov(0x803,20,color[0],"Net Address: [Zone]");
	_vcw('_',color[3],0x818,3);
	prntnomov(0x81d,5,color[0],"[Net]");
	_vcw('_',color[3],0x823,5);
	prntnomov(0x82a,6,color[0],"[Node]");
	_vcw('_',color[3],0x831,5);
	for (count = 0; count < 5; count++)
		{
		prntnomov(0xa03 + (count << 8),12,color[0],"AKA Address:");
		prntnomov(0xa10 + (count << 8),6,color[0],"[Zone]");
		_vcw('_',color[3],0xa17 + (count << 8),4);
		prntnomov(0xa1d + (count << 8),5,color[0],"[Net]");
		_vcw('_',color[3],0xa23 + (count << 8),5);
		prntnomov(0xa2a + (count << 8),6,color[0],"[Node]");
		_vcw('_',color[3],0xa31 + (count << 8),5);
		}
	}



void fill_general_info(void)
	{
	int count;

	prntnomov(0x30d,60,color[3],cfg.cfg_bbsname);
	prntnomov(0x40d,40,color[3],cfg.cfg_sysopname);
	prntnomov(0x50d,65,color[3],cfg.cfg_origin);
	prntnomov(0x60d,8,color[3],cfg.cfg_qwkname);
	
	prntnomovf(0x817,4,color[3],"%u",cfg.cfg_zone);
	prntnomovf(0x823,5,color[3],"%u",cfg.cfg_net);
	prntnomovf(0x831,5,color[3],"%u",cfg.cfg_node);

	for (count = 0; count < 5; count++)
		{
		prntnomovf(0xa17 + (count << 8),4,color[3],"%u",cfg.cfg_akazone[count]);
		prntnomovf(0xa23 + (count << 8),5,color[3],"%u",cfg.cfg_akanet[count]);
		prntnomovf(0xa31 + (count << 8),5,color[3],"%u",cfg.cfg_akanode[count]);
		}
	}



void edit_general_info(void)
	{
	struct window *wndw;
	char buffer[10];
	long tlong;
	int change = 0;
	int current = 0;
	int rtn;
	int redo;
	int temp;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,25,color[1]," General BBS Information ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");
		drawbox(0x1002,0x174d,color[0],0);
		prntcenter(0x1100,80,color[0],"Including the sysop's login name ensures that mail addressed to \"SYSOP\"");
		prntcenter(0x1200,80,color[0],"will be converted to the sysop's real name.");
		prntcenter(0x1400,80,color[0],"It is important to include your zone number if you are configuring this");
		prntcenter(0x1500,80,color[0],"program for Netmail.  While some systems allow you to avoid entering the");
		prntcenter(0x1600,80,color[0],"zone, Simplex enforces its use.");

		draw_general_info();
		fill_general_info();
		do
			{
			switch (current)
				{
				case 0:
					rtn = edit_field(0x30d,color[4],color[3],1,CLOCK_POS,color[0],"F060",cfg.cfg_bbsname,get_anychar);
					break;
				case 1:
					rtn = edit_field(0x40d,color[4],color[3],1,CLOCK_POS,color[0],"FPC40",cfg.cfg_sysopname,NULL);
					break;
				case 2:
					rtn = edit_field(0x50d,color[4],color[3],1,CLOCK_POS,color[0],"F065",cfg.cfg_origin,get_anychar);
					break;
				case 3:
					rtn = edit_field(0x60d,color[4],color[3],1,CLOCK_POS,color[0],"FA8",cfg.cfg_qwkname,get_filename);
					break;
				case 4:
					sprintf(buffer,"%u",cfg.cfg_zone);
					rtn = edit_number(0x817,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_zone = atoi(buffer);
					break;
				case 5:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_net);
						rtn = edit_number(0x823,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Net numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_net = (int)tlong;
							}
						}
					while (redo);
					break;
				case 6:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_node);
						rtn = edit_number(0x831,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Node numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_node = (int)tlong;
							}
						}
					while (redo);
					break;

				case 7:
					sprintf(buffer,"%u",cfg.cfg_akazone[0]);
					rtn = edit_number(0xa17,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_akazone[0] = atoi(buffer);
					break;
				case 8:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanet[0]);
						rtn = edit_number(0xa23,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Net numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanet[0] = (int)tlong;
							}
						}
					while (redo);
					break;
				case 9:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanode[0]);
						rtn = edit_number(0xa31,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Node numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanode[0] = (int)tlong;
							}
						}
					while (redo);
					break;

				case 10:
					sprintf(buffer,"%u",cfg.cfg_akazone[1]);
					rtn = edit_number(0xb17,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_akazone[1] = atoi(buffer);
					break;
				case 11:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanet[1]);
						rtn = edit_number(0xb23,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Net numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanet[1] = (int)tlong;
							}
						}
					while (redo);
					break;
				case 12:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanode[1]);
						rtn = edit_number(0xb31,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Node numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanode[1] = (int)tlong;
							}
						}
					while (redo);
					break;

				case 13:
					sprintf(buffer,"%u",cfg.cfg_akazone[2]);
					rtn = edit_number(0xc17,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_akazone[2] = atoi(buffer);
					break;
				case 14:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanet[2]);
						rtn = edit_number(0xc23,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Net numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanet[2] = (int)tlong;
							}
						}
					while (redo);
					break;
				case 15:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanode[2]);
						rtn = edit_number(0xc31,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Node numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanode[2] = (int)tlong;
							}
						}
					while (redo);
					break;

				case 16:
					sprintf(buffer,"%u",cfg.cfg_akazone[3]);
					rtn = edit_number(0xd17,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_akazone[3] = atoi(buffer);
					break;
				case 17:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanet[3]);
						rtn = edit_number(0xd23,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Net numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanet[3] = (int)tlong;
							}
						}
					while (redo);
					break;
				case 18:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanode[3]);
						rtn = edit_number(0xd31,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Node numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanode[3] = (int)tlong;
							}
						}
					while (redo);
					break;

				case 19:
					sprintf(buffer,"%u",cfg.cfg_akazone[4]);
					rtn = edit_number(0xe17,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
						cfg.cfg_akazone[4] = atoi(buffer);
					break;
				case 20:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanet[4]);
						rtn = edit_number(0xe23,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Net numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_akanet[4] = (int)tlong;
							}
						}
					while (redo);
					break;
				case 21:
					do
						{
						redo = 0;
						sprintf(buffer,"%u",cfg.cfg_akanode[4]);
						rtn = edit_number(0xe31,color[4],color[3],buffer,0,(int)0xffff,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tlong = atol(buffer);
							if (tlong > 0xffffL)
								{
								error(NOTICE,"Invalid number!  Node numbers are between 0 and 65535!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								{
								cfg.cfg_akanode[4] = (int)tlong;
								}
							}
						}
					while (redo);
					break;
				}
			if (rtn == E_EXIT)
				change = 1;
			if (rtn == E_FORE || rtn == E_EXIT)
				{
				if (current < 21)
					++current;
				else
					current = 0;
				}
			if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 21;
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



