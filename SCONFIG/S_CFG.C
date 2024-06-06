/* s_cfg.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 28 August 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_cfg.c_v  $
**                       $Date:   25 Oct 1992 14:15:20  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <screen.h>
#include <window.h>
#include <editor.h>
#include <keys.h>
#include "sconfig.h"



struct ccfg ccfg;			/* default configuration */

extern struct choice yn[2];



int cfg_color(char *title,int *pcolor)
	{
	struct window *wndw;
	char buffer[80];
	char save_help[41];
	int save[20];
	int rtn = 0;
	int quit = 0;
	int fore;
	int back;
	int key;

	if (wndw = open_window(0x41c,0xf4f,WHITE,3))
		{
		strcpy(save_help,query_help_topic());
	   	register_help("color menu");
		set_cursor_type(HIDDEN);

		sprintf(buffer," Select Color for %s ",title);
		prntcenter(0x41c,50,WHITE | BLINK,buffer);
		prntcenter(0xf1c,50,WHITE," Press Enter to select, ESC to abort ");
		for (back = 0; back < 8; back++)
			{
			for (fore = 0; fore < 16; fore++)
				prntnomov(((0x6 + back) << 8) + (fore * 3 + 0x1e),3,(back << 4) + fore," x ");
			}
		fore = *pcolor & 0xf;
		back = ((int)*pcolor >> 4) & 0xf;
		do
			{
			_vbr(save,((0x5 + back) << 8) + (fore * 3 + 0x1d),5);
			_vbr(save + 5,((0x6 + back) << 8) + (fore * 3 + 0x1d),5);
			_vbr(save + 10,((0x7 + back) << 8) + (fore * 3 + 0x1d),5);
			drawbox(((0x5 + back) << 8) + (fore * 3 + 0x1d),((0x7 + back) << 8) + (fore * 3 + 0x21),WHITE,0);
			key = read_keyboard(1,CLOCK_POS + 0x1700,color[0]);
			_vbw(save,((0x5 + back) << 8) + (fore * 3 + 0x1d),5);
			_vbw(save + 5,((0x6 + back) << 8) + (fore * 3 + 0x1d),5);
			_vbw(save + 10,((0x7 + back) << 8) + (fore * 3 + 0x1d),5);
			switch (key)
				{
				case UpArrow:
					if (back)
						--back;
					else
						back = 7;
					break;
				case DownArrow:
					if (back < 7)
						++back;
					else
						back = 0;
					break;
				case LeftArrow:
					if (fore)
						--fore;
					else
						fore = 15;
					break;
				case RightArrow:
					if (fore < 15)
						++fore;
					else
						fore = 0;
					break;
				case TAB:
					rtn = E_FORE;
					quit = 1;
					break;
				case STAB:
					rtn = E_BACK;
					quit = 1;
					break;
				case CR:
					if (fore == (back & 7))
						{
						error(NOTICE,"You cannot select both background and foreground colors to be the same!",1,CLOCK_POS,color[0]);
						rtn = 0;
						}
					else
						{
						*pcolor = (back << 4) + fore;
						rtn = E_EXIT;
						quit = 1;
						}
					break;
				case ESC:
				case 0x110:			/* Alt-Q */
					rtn = E_QUIT;
					quit = 1;
					break;
				case CEnd:
				case 0x12d:			/* Alt-X */
					rtn = E_END;
					quit = 1;
					break;
				case PgUp:
					rtn = E_PGUP;
					quit = 1;
					break;
				case PgDn:
					rtn = E_PGDN;
					quit = 1;
					break;
				default:
					beep();
					break;
				}
			}
		while (!quit);
		set_cursor_type(LINE);
	   	register_help(save_help);
		close_window(wndw);
		}
	return rtn;
	}



void draw_config(void)
	{
	prntnomov(0x805,19,color[3],"Box 1 Normal Color:");
	_vcw('_',color[0],0x819,30);
	prntnomov(0x905,19,color[3],"Box 1 Hilite Color:");
	_vcw('_',color[0],0x919,30);
	prntnomov(0xa08,16,color[3],"Error Box Color:");
	_vcw('_',color[0],0xa19,30);
	prntnomov(0xb05,19,color[3],"Box 2 Normal Color:");
	_vcw('_',color[0],0xb19,30);
	prntnomov(0xc05,19,color[3],"Box 2 Hilite Color:");
	_vcw('_',color[0],0xc19,30);
	prntnomov(0xd06,18,color[3],"Help Normal Color:");
	_vcw('_',color[0],0xd19,30);
	prntnomov(0xe06,18,color[3],"Help Hilite Color:");
	_vcw('_',color[0],0xe19,30);
	prntnomov(0x1003,21,color[3],"Enable Snow Checking?");
	_vcw('_',color[0],0x1019,3);
	prntnomov(0x101e,32,color[3],"<--- Needed for old CGA monitors");
	}



void fill_config(struct ccfg *tccfg)
	{
	int count;

	for (count = 0; count < 7; count++)
		{
		_vcw('_',color[0],0x819 + (count << 8),30);
		prntnomov(0x819 + (count << 8),30,color[0],parse_color(tccfg->ccfg_color[count]));
		prntnomov(0x838 + (count << 8),6,tccfg->ccfg_color[count],"(XXXX)");
		}
	prntnomov(0x1019,3,color[0],(char *)(tccfg->ccfg_snow ? "Yes" : "No_"));
	}



void configure_colors(void)
	{
	struct window *wndw;
	struct ccfg tccfg;
	int current = 0;
	int change = 0;
	int count;
	int rtn;

	if (wndw = open_shadow_window(0x601,0x124e,color[3],1))
		{
		prntnomov(0x602,40,color[4]," Edit Color Configuration ");
		prntnomov(0x1202,19,color[3]," Press F1 for Help ");

		memcpy(&tccfg,&ccfg,sizeof(struct ccfg));
		draw_config();
		fill_config(&tccfg);

		do
			{
			switch (current)
				{
				case 0:
					rtn = cfg_color("Box 1 Normal Color",&tccfg.ccfg_color[0]);
					if (rtn == E_EXIT)
						{
						_vcw('_',color[0],0x819,30);
						prntnomov(0x819,30,color[0],parse_color(tccfg.ccfg_color[0]));
						prntnomov(0x838,6,tccfg.ccfg_color[0],"(XXXX)");
						}
					break;
				case 1:
					rtn = cfg_color("Box 1 Hilite Color",&tccfg.ccfg_color[1]);
					if (rtn == E_EXIT)
						{
						_vcw('_',color[0],0x919,30);
						prntnomov(0x919,30,color[0],parse_color(tccfg.ccfg_color[1]));
						prntnomov(0x938,6,tccfg.ccfg_color[1],"(XXXX)");
						}
					break;
				case 2:
					rtn = cfg_color("Error Box Color",&tccfg.ccfg_color[2]);
					if (rtn == E_EXIT)
						{
						_vcw('_',color[0],0xa19,30);
						prntnomov(0xa19,30,color[0],parse_color(tccfg.ccfg_color[2]));
						prntnomov(0xa38,6,tccfg.ccfg_color[2],"(XXXX)");
						}
					break;
				case 3:
					rtn = cfg_color("Box 2 Normal Color",&tccfg.ccfg_color[3]);
					if (rtn == E_EXIT)
						{
						_vcw('_',color[0],0xb19,30);
						prntnomov(0xb19,30,color[0],parse_color(tccfg.ccfg_color[3]));
						prntnomov(0xb38,6,tccfg.ccfg_color[3],"(XXXX)");
						}
					break;
				case 4:
					rtn = cfg_color("Box 2 Hilite Color",&tccfg.ccfg_color[4]);
					if (rtn == E_EXIT)
						{
						_vcw('_',color[0],0xc19,30);
						prntnomov(0xc19,30,color[0],parse_color(tccfg.ccfg_color[4]));
						prntnomov(0xc38,6,tccfg.ccfg_color[4],"(XXXX)");
						}
					break;
				case 5:
					rtn = cfg_color("Help Normal Color",&tccfg.ccfg_color[5]);
					if (rtn == E_EXIT)
						{
						_vcw('_',color[0],0xd19,30);
						prntnomov(0xd19,30,color[0],parse_color(tccfg.ccfg_color[5]));
						prntnomov(0xd38,6,tccfg.ccfg_color[5],"(XXXX)");
						}
					break;
				case 6:
					rtn = cfg_color("Help Hilite Color",&tccfg.ccfg_color[6]);
					if (rtn == E_EXIT)
						{
						_vcw('_',color[0],0xe19,30);
						prntnomov(0xe19,30,color[0],parse_color(tccfg.ccfg_color[6]));
						prntnomov(0xe38,6,tccfg.ccfg_color[6],"(XXXX)");
						}
					break;
				case 7:
					rtn = pop_menu(0xd1e,color[1],color[0],"Snow Checking?",2,yn,1,1,1,CLOCK_POS + 0x1700,color[0]);
					if (rtn > ' ')
						{
						switch (rtn)
							{
							case 'Y':
							case 'y':
								tccfg.ccfg_snow = 1;
								break;
							case 'N':
							case 'n':
								tccfg.ccfg_snow = 0;
								break;
							}
						rtn = E_EXIT;
						}
					prntnomov(0x1019,3,color[0],(char *)(tccfg.ccfg_snow ? "Yes" : "No_"));
					break;
				}
			if (rtn == E_EXIT)
				change = 1;
			if (rtn == E_EXIT || rtn == E_FORE)
				{
				if (current < 7)
					++current;
				else
					current = 0;
				}
			else if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 7;
				}
			else if (rtn == E_END && !change)
				rtn = E_QUIT;
			}
		while (rtn != E_END && rtn != E_QUIT);

		if (rtn == E_END)
			{
			memcpy(&ccfg,&tccfg,sizeof(struct ccfg));
			fseek(ccfgfd,0L,SEEK_SET);
			fwrite(&ccfg,sizeof(struct ccfg),1,ccfgfd);
			fflush(ccfgfd);

			for (count = 0; count < 7; count++)
				color[count] = ccfg.ccfg_color[count];

			set_help_colors(color[6],color[5]);

#if !defined(PROTECTED)
			snow_flag = ccfg.ccfg_snow;
#endif

			color_change_flag = 1;
			}
		close_shadow_window(wndw);
		}
	else 
		error(NON_FATAL,"Unable to open window",1,CLOCK_POS + 0x1700,color[0]);
	}



