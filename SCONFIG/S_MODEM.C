/* s_modem.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_modem.c_v  $
**                       $Date:   25 Oct 1992 14:15:30  $
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




struct choice port[8] =
	{
		{'1',"Com1"},
		{'2',"Com2"},
		{'3',"Com3"},
		{'4',"Com4"},
		{'5',"Com5"},
		{'6',"Com6"},
		{'7',"Com7"},
		{'8',"Com8"},
	};



struct choice baud[7] =
	{
		{'1',"300"},
		{'2',"1200"},
		{'3',"2400"},
		{'4',"4800"},
		{'5',"9600"},
		{'6',"19200"},
		{'7',"38400"},
	};



extern struct choice yn[2];



void draw_modem_info(void)
	{
	prntnomov(0x308,12,color[0],"Com Port:");
	_vcw('_',color[3],0x312,4);
	prntnomov(0x404,13,color[0],"Maximum Baud:");
	_vcw('_',color[3],0x412,5);
	prntnomov(0x41d,20,color[0],"Locked Baud?");
	_vcw('_',color[3],0x42a,3);
	prntnomov(0x504,13,color[0],"Minimum Baud:");
	_vcw('_',color[3],0x512,5);
	prntnomov(0x603,18,color[0],"Min ANSI Baud:");
	_vcw('_',color[3],0x612,5);
	prntnomov(0x706,20,color[0],"Modem Slow?");
	_vcw('_',color[3],0x712,3);
	prntnomov(0x716,60,color[0],"<--- only if modem is slow to accept commands");
	prntnomov(0x806,11,color[0],"Modem Init:");
	_vcw('_',color[3],0x812,60);
	prntnomov(0x905,12,color[0],"Ok Response:");
	_vcw('_',color[3],0x912,20);
	prntnomov(0xa05,12,color[0],"Dial Prefix:");
	_vcw('_',color[3],0xa12,20);
	prntnomov(0xb04,13,color[0],"Dial Postfix:");
	_vcw('_',color[3],0xb12,20);
	prntnomov(0xc05,12,color[0],"300 Connect:");
	_vcw('_',color[3],0xc12,20);
	_vcw('_',color[3],0xc28,20);
	prntnomov(0xd04,13,color[0],"1200 Connect:");
	_vcw('_',color[3],0xd12,20);
	_vcw('_',color[3],0xd28,20);
	prntnomov(0xe04,13,color[0],"2400 Connect:");
	_vcw('_',color[3],0xe12,20);
	_vcw('_',color[3],0xe28,20);
	prntnomov(0xf04,13,color[0],"4800 Connect:");
	_vcw('_',color[3],0xf12,20);
	_vcw('_',color[3],0xf28,20);
	prntnomov(0x1004,13,color[0],"9600 Connect:");
	_vcw('_',color[3],0x1012,20);
	_vcw('_',color[3],0x1028,20);
	prntnomov(0x1103,14,color[0],"19200 Connect:");
	_vcw('_',color[3],0x1112,20);
	_vcw('_',color[3],0x1128,20);
	prntnomov(0x1203,14,color[0],"38400 Connect:");
	_vcw('_',color[3],0x1212,20);
	_vcw('_',color[3],0x1228,20);
	prntnomov(0x1305,14,color[0],"Ring String:");
	_vcw('_',color[3],0x1312,20);
	prntnomov(0x1403,14,color[0],"Answer String:");
	_vcw('_',color[3],0x1412,20);
	prntnomov(0x1503,14,color[0],"Hangup String:");
	_vcw('_',color[3],0x1512,20);
	prntnomov(0x1528,35,color[0],"<-- used only if dropping DTR fails");
	prntnomov(0x1605,14,color[0],"Busy String:");
	_vcw('_',color[3],0x1612,20);
	prntnomov(0x1628,35,color[0],"<-- used to make BBS look busy");
	}



void fill_modem_info(void)
	{
	prntnomovf(0x312,4,color[3],"Com%u",cfg.cfg_port + 1);
	prntnomovf(0x412,5,color[3],"%u",cfg.cfg_baud);
	prntnomovf(0x42a,3,color[3],"%s",cfg.cfg_flags & CFG_LOCKBAUD ? "Yes" : "No_");
	prntnomovf(0x512,5,color[3],"%u",cfg.cfg_minbaud);
	prntnomovf(0x612,5,color[3],"%u",cfg.cfg_ansibaud);
	prntnomovf(0x712,3,color[3],"%s",cfg.cfg_flags & CFG_SLOWMODEM ? "Yes" : "No_");
	prntnomov(0x812,60,color[3],cfg.cfg_init);
	prntnomov(0x912,20,color[3],cfg.cfg_resp);
	prntnomov(0xa12,20,color[3],cfg.cfg_dialpre);
	prntnomov(0xb12,20,color[3],cfg.cfg_dialpost);
	prntnomov(0xc12,20,color[3],cfg.cfg_resp3_1);
	prntnomov(0xc28,20,color[3],cfg.cfg_resp3_2);
	prntnomov(0xd12,20,color[3],cfg.cfg_resp12_1);
	prntnomov(0xd28,20,color[3],cfg.cfg_resp12_2);
	prntnomov(0xe12,20,color[3],cfg.cfg_resp24_1);
	prntnomov(0xe28,20,color[3],cfg.cfg_resp24_2);
	prntnomov(0xf12,20,color[3],cfg.cfg_resp48_1);
	prntnomov(0xf28,20,color[3],cfg.cfg_resp48_2);
	prntnomov(0x1012,20,color[3],cfg.cfg_resp96_1);
	prntnomov(0x1028,20,color[3],cfg.cfg_resp96_2);
	prntnomov(0x1112,20,color[3],cfg.cfg_resp192_1);
	prntnomov(0x1128,20,color[3],cfg.cfg_resp192_2);
	prntnomov(0x1212,20,color[3],cfg.cfg_resp384_1);
	prntnomov(0x1228,20,color[3],cfg.cfg_resp384_2);
	prntnomov(0x1312,20,color[3],cfg.cfg_ring);
	prntnomov(0x1412,20,color[3],cfg.cfg_answer);
	prntnomov(0x1512,20,color[3],cfg.cfg_hangup);
	prntnomov(0x1612,20,color[3],cfg.cfg_busy);
	}



void edit_modem_info(void)
	{
	struct window *wndw;
	int current = 0;
	int change = 0;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,19,color[1]," Modem Information ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");
		draw_modem_info();
		fill_modem_info();
		do
			{
			switch (current)
				{
				case 0:
					rtn = pop_menu(0x219,color[4],color[3],"Com Port",8,port,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						cfg.cfg_port = rtn - '1';
						prntnomovf(0x312,4,color[3],"Com%u",cfg.cfg_port + 1);
						rtn = E_EXIT;
						}
					break;
				case 1:
					rtn = pop_menu(0x31a,color[4],color[3],"Max Baud",7,baud,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						switch (rtn)
							{
							case '1':
								cfg.cfg_baud = 300;
								break;
							case '2':
								cfg.cfg_baud = 1200;
								break;
							case '3':
								cfg.cfg_baud = 2400;
								break;
							case '4':
								cfg.cfg_baud = 4800;
								break;
							case '5':
								cfg.cfg_baud = 9600;
								break;
							case '6':
								cfg.cfg_baud = 19200;
								break;
							case '7':
								cfg.cfg_baud = 38400U;
								break;
							}
						_vcw('_',color[3],0x412,5);
						prntnomovf(0x412,5,color[3],"%u",cfg.cfg_baud);
						rtn = E_EXIT;
						}
					break;
				case 2:
					rtn = pop_menu(0x32f,color[4],color[3],"Lock Baud?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							cfg.cfg_flags |= CFG_LOCKBAUD;
						else
							cfg.cfg_flags &= ~CFG_LOCKBAUD;
						prntnomovf(0x42a,3,color[3],"%s",cfg.cfg_flags & CFG_LOCKBAUD ? "Yes" : "No_");
						rtn = E_EXIT;
						}
					break;
				case 3:
					rtn = pop_menu(0x41b,color[4],color[3],"Min Baud",7,baud,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						switch (rtn)
							{
							case '1':
								cfg.cfg_minbaud = 300;
								break;
							case '2':
								cfg.cfg_minbaud = 1200;
								break;
							case '3':
								cfg.cfg_minbaud = 2400;
								break;
							case '4':
								cfg.cfg_minbaud = 4800;
								break;
							case '5':
								cfg.cfg_minbaud = 9600;
								break;
							case '6':
								cfg.cfg_minbaud = 19200;
								break;
							case '7':
								cfg.cfg_minbaud = 38400U;
								break;
							}
						_vcw('_',color[3],0x512,5);
						prntnomovf(0x512,5,color[3],"%u",cfg.cfg_minbaud);
						rtn = E_EXIT;
						}
					break;
				case 4:
					rtn = pop_menu(0x51c,color[4],color[3],"Min Baud/ANSI",7,baud,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						switch (rtn)
							{
							case '1':
								cfg.cfg_ansibaud = 300;
								break;
							case '2':
								cfg.cfg_ansibaud = 1200;
								break;
							case '3':
								cfg.cfg_ansibaud = 2400;
								break;
							case '4':
								cfg.cfg_ansibaud = 4800;
								break;
							case '5':
								cfg.cfg_ansibaud = 9600;
								break;
							case '6':
								cfg.cfg_ansibaud = 19200;
								break;
							case '7':
								cfg.cfg_ansibaud = 38400U;
								break;
							}
						_vcw('_',color[3],0x612,5);
						prntnomovf(0x612,5,color[3],"%u",cfg.cfg_ansibaud);
						rtn = E_EXIT;
						}
					break;
				case 5:
					rtn = pop_menu(0x620,color[4],color[3],"Slow Modem?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							cfg.cfg_flags |= CFG_SLOWMODEM;
						else
							cfg.cfg_flags &= ~CFG_SLOWMODEM;
						prntnomovf(0x712,3,color[3],"%s",(cfg.cfg_flags & CFG_SLOWMODEM) ? "Yes" : "No_");
						rtn = E_EXIT;
						}
					break;
				case 6:
					rtn = edit_field(0x812,color[4],color[3],1,CLOCK_POS,color[0],"FPU60",cfg.cfg_init,get_anychar_first);
					break;
				case 7:
					rtn = edit_field(0x912,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp,get_anychar_first);
					break;
				case 8:
					rtn = edit_field(0xa12,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_dialpre,get_anychar_first);
					break;
				case 9:
					rtn = edit_field(0xb12,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_dialpost,get_anychar_first);
					break;
				case 10:
					rtn = edit_field(0xc12,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp3_1,get_anychar_first);
					break;
				case 11:
					rtn = edit_field(0xc28,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp3_2,get_anychar_first);
					break;
				case 12:
					rtn = edit_field(0xd12,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp12_1,get_anychar_first);
					break;
				case 13:
					rtn = edit_field(0xd28,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp12_2,get_anychar_first);
					break;
				case 14:
					rtn = edit_field(0xe12,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp24_1,get_anychar_first);
					break;
				case 15:
					rtn = edit_field(0xe28,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp24_2,get_anychar_first);
					break;
				case 16:
					rtn = edit_field(0xf12,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp48_1,get_anychar_first);
					break;
				case 17:
					rtn = edit_field(0xf28,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp48_2,get_anychar_first);
					break;
				case 18:
					rtn = edit_field(0x1012,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp96_1,get_anychar_first);
					break;
				case 19:
					rtn = edit_field(0x1028,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp96_2,get_anychar_first);
					break;
				case 20:
					rtn = edit_field(0x1112,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp192_1,get_anychar_first);
					break;
				case 21:
					rtn = edit_field(0x1128,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp192_2,get_anychar_first);
					break;
				case 22:
					rtn = edit_field(0x1212,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp384_1,get_anychar_first);
					break;
				case 23:
					rtn = edit_field(0x1228,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_resp384_2,get_anychar_first);
					break;
				case 24:
					rtn = edit_field(0x1312,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_ring,get_anychar_first);
					break;
				case 25:
					rtn = edit_field(0x1412,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_answer,get_anychar_first);
					break;
				case 26:
					rtn = edit_field(0x1512,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_hangup,get_anychar_first);
					break;
				case 27:
					rtn = edit_field(0x1612,color[4],color[3],1,CLOCK_POS,color[0],"FPU20",cfg.cfg_busy,get_anychar_first);
					break;
				}
			if (rtn == E_EXIT)
				change = 1;
 			if (rtn == E_FORE || rtn == E_EXIT)
				{
				if (current < 27)
					++current;
				else
					current = 0;
				}
			if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 27;
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
