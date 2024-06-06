/* s_event.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 4 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_event.c_v  $
**                       $Date:   25 Oct 1992 14:15:20  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <screen.h>
#include <window.h>
#include <editor.h>
#include <keys.h>
#include "sconfig.h"



extern struct choice yn[2];


void draw_event(void)
	{
	prntnomov(0x908,12,color[0],"Event Title:");
	_vcw('_',color[3],0x918,20);
	prntnomov(0xa08,11,color[0],"Event Days:");
	prntnomov(0xa1a,5,color[0],"Sun");
	prntnomov(0xa21,5,color[0],"Mon");
	prntnomov(0xa28,5,color[0],"Tue");
	prntnomov(0xa2f,5,color[0],"Wed");
	prntnomov(0xa36,5,color[0],"Thu");
	prntnomov(0xa3d,5,color[0],"Fri");
	prntnomov(0xa44,5,color[0],"Sat");
	_vcw('_',color[3],0xa18,1);
	_vcw('_',color[3],0xa1f,1);
	_vcw('_',color[3],0xa26,1);
	_vcw('_',color[3],0xa2d,1);
	_vcw('_',color[3],0xa34,1);
	_vcw('_',color[3],0xa3b,1);
	_vcw('_',color[3],0xa42,1);
	prntnomov(0xb08,11,color[0],"Event Time:");
	prntnomov(0xb18,5,color[3],"__:__");
	prntnomov(0xc08,11,color[0],"Errorlevel:");
	_vcw('_',color[3],0xc18,3);
	prntnomov(0xc1d,10,color[0],"(6 -> 255)");
	prntnomov(0xd08,14,color[0],"Event Enabled?");
	_vcw('_',color[3],0xd18,1);
	prntnomov(0xe08,12,color[0],"Force Event?");
	_vcw('_',color[3],0xe18,1);
	prntnomov(0xf08,14,color[0],"Event Deleted?");
	_vcw('_',color[3],0xf18,1);
	prntnomov(0xf1b,32,color[0],"<--- If set, this will be purged");
	}



void fill_event(struct evt *tevt)
	{
	prntnomov(0x918,20,color[3],tevt->evt_title);
	if (tevt->evt_days & 0x40)
		_vcw('Y',color[3],0xa18,1);
	else
		_vcw('N',color[3],0xa18,1);
	if (tevt->evt_days & 0x20)
		_vcw('Y',color[3],0xa1f,1);
	else
		_vcw('N',color[3],0xa1f,1);
	if (tevt->evt_days & 0x10)
		_vcw('Y',color[3],0xa26,1);
	else
		_vcw('N',color[3],0xa26,1);
	if (tevt->evt_days & 0x8)
		_vcw('Y',color[3],0xa2d,1);
	else
		_vcw('N',color[3],0xa2d,1);
	if (tevt->evt_days & 0x4)
		_vcw('Y',color[3],0xa34,1);
	else
		_vcw('N',color[3],0xa34,1);
	if (tevt->evt_days & 0x2)
		_vcw('Y',color[3],0xa3b,1);
	else
		_vcw('N',color[3],0xa3b,1);
	if (tevt->evt_days & 0x1)
		_vcw('Y',color[3],0xa42,1);
	else
		_vcw('N',color[3],0xa42,1);
	prntnomovf(0xb18,5,color[3],"%02u:%02u",tevt->evt_time >> 11,(tevt->evt_time >> 5) & 63);
	prntnomovf(0xc18,5,color[3],"%u",tevt->evt_errorlevel);
	_vcw((char)(tevt->evt_enabled ? 'Y' : 'N'),color[3],0xd18,1);
	_vcw((char)(tevt->evt_forced ? 'Y' : 'N'),color[3],0xe18,1);
	_vcw((char)(tevt->evt_deleted ? 'Y' : 'N'),color[3],0xf18,1);
	}



void edit_events(void)
	{
	struct window *wndw;
	struct evt tevt;
	char buffer[10];
	int cur_evt = 0;
	int ischange = 0;
	int current;
	int change;
	int rtn;
	int thour;
	int tmin;
	int redo;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," Edit Event Configuration ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");
		drawbox(0x706,0x1148,color[0],0);

		do
			{
			draw_event();
			if (cur_evt == cur_evts)
				{
				memset(&tevt,0,sizeof(struct evt));
				tevt.evt_errorlevel = 6;
				}
			else
				memcpy(&tevt,evts[cur_evt],sizeof(struct evt));
			fill_event(&tevt);
			current = 0;
			change = 0;
			do
				{
				switch (current)
					{
					case 0:
						rtn = edit_field(0x918,color[4],color[3],1,CLOCK_POS,color[0],"FPC20",tevt.evt_title,NULL);
						break;
					case 1:
						rtn = pop_menu(0xb17,color[4],color[3],"Sun?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								tevt.evt_days &= 0x3f;
								tevt.evt_days |= 0x40;
								_vcw('Y',color[3],0xa18,1);
								}
							else
								{
								tevt.evt_days &= 0x3f;
								_vcw('N',color[3],0xa18,1);
								}
							rtn = E_EXIT;
							}
						break;
					case 2:
						rtn = pop_menu(0xb1e,color[4],color[3],"Mon?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								tevt.evt_days &= 0x5f;
								tevt.evt_days |= 0x20;
								_vcw('Y',color[3],0xa1f,1);
								}
							else
								{
								tevt.evt_days &= 0x5f;
								_vcw('N',color[3],0xa1f,1);
								}
							rtn = E_EXIT;
							}
						break;
					case 3:
						rtn = pop_menu(0xb25,color[4],color[3],"Tue?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								tevt.evt_days &= 0x6f;
								tevt.evt_days |= 0x10;
								_vcw('Y',color[3],0xa26,1);
								}
							else
								{
								tevt.evt_days &= 0x6f;
								_vcw('N',color[3],0xa26,1);
								}
							rtn = E_EXIT;
							}
						break;
					case 4:
						rtn = pop_menu(0xb2c,color[4],color[3],"Wed?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								tevt.evt_days &= 0x77;
								tevt.evt_days |= 0x8;
								_vcw('Y',color[3],0xa2d,1);
								}
							else
								{
								tevt.evt_days &= 0x77;
								_vcw('N',color[3],0xa2d,1);
								}
							rtn = E_EXIT;
							}
						break;
					case 5:
						rtn = pop_menu(0xb33,color[4],color[3],"Thu?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								tevt.evt_days &= 0x7b;
								tevt.evt_days |= 0x4;
								_vcw('Y',color[3],0xa34,1);
								}
							else
								{
								tevt.evt_days &= 0x7b;
								_vcw('N',color[3],0xa34,1);
								}
							rtn = E_EXIT;
							}
						break;
					case 6:
						rtn = pop_menu(0xb3a,color[4],color[3],"Fri?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								tevt.evt_days &= 0x7d;
								tevt.evt_days |= 0x2;
								_vcw('Y',color[3],0xa3b,1);
								}
							else
								{
								tevt.evt_days &= 0x7d;
								_vcw('N',color[3],0xa3b,1);
								}
							rtn = E_EXIT;
							}
						break;
					case 7:
						rtn = pop_menu(0xb41,color[4],color[3],"Sat?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								tevt.evt_days &= 0x7e;
								tevt.evt_days |= 0x1;
								_vcw('Y',color[3],0xa42,1);
								}
							else
								{
								tevt.evt_days &= 0x7e;
								_vcw('N',color[3],0xa42,1);
								}
							rtn = E_EXIT;
							}
						break;
					case 8:
						do
							{
							redo = 0;
							sprintf(buffer,"%02u%02u",tevt.evt_time >> 11,(tevt.evt_time >> 5) & 63);
							_vcw('_',color[3],0xb18,5);
							prntnomovf(0xb18,5,color[3],"%2.2s:%2.2s",buffer,buffer + 2);
							rtn = edit_field(0xb18,color[4],color[3],1,CLOCK_POS,color[0],"00J00",buffer,NULL);
							if (rtn == E_EXIT)
								{
								tmin = atoi(buffer + 2);
								buffer[2] = '\0';
								thour = atoi(buffer);
								if (thour > 23 || tmin > 59)
									{
									error(NOTICE,"Invalid time!  Times must be between 00:00 and 23:59!",0,0,0);
									redo = 1;
									}
								if (!redo)
									tevt.evt_time = (thour << 11) | (tmin << 5);
								}
							}
						while (redo);
						break;
					case 9:
						sprintf(buffer,"%u",(int)tevt.evt_errorlevel);
						_vcw('_',color[3],0xc18,3);
						prntnomovf(0xc18,3,color[3],"%u",(int)tevt.evt_errorlevel);
						rtn = edit_number(0xc18,color[4],color[3],buffer,6,255,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							tevt.evt_errorlevel = (unsigned char)atoi(buffer);
						_vcw('_',color[3],0xc18,3);
						prntnomovf(0xc18,3,color[3],"%u",(int)tevt.evt_errorlevel);
						break;
					case 10:
						rtn = pop_menu(0xc20,color[4],color[3],"Enable Event?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								if (!tevt.evt_enabled)
									{
									tevt.evt_enabled = (char)1;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							else
								{
								if (tevt.evt_enabled)
									{
									tevt.evt_enabled = (char)0;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							_vcw((char)(tevt.evt_enabled ? 'Y' : 'N'),color[3],0xd18,1);
							}
						break;
					case 11:
						rtn = pop_menu(0xd20,color[4],color[3],"Force Event?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								if (!tevt.evt_forced)
									{
									tevt.evt_forced = (char)1;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							else
								{
								if (tevt.evt_forced)
									{
									tevt.evt_forced = (char)0;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							_vcw((char)(tevt.evt_forced ? 'Y' : 'N'),color[3],0xe18,1);
							}
						break;
					case 12:
						rtn = pop_menu(0xe20,color[4],color[3],"Delete Event?",2,yn,1,1,1,CLOCK_POS,color[0]);
						if (rtn > ' ')
							{
							if (rtn == 'Y')
								{
								if (!tevt.evt_deleted)
									{
									tevt.evt_deleted = (char)1;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							else
								{
								if (tevt.evt_deleted)
									{
									tevt.evt_deleted = (char)0;
									rtn = E_EXIT;
									}
								else
									rtn = E_FORE;
								}
							_vcw((char)(tevt.evt_deleted ? 'Y' : 'N'),color[3],0xf18,1);
							}
						break;
					}
				if (rtn == E_EXIT)
					change = 1;
				if (rtn == E_FORE || rtn == E_EXIT)
					{
					if (current < 12)
						++current;
					else
						current = 0;
					}
				if (rtn == E_BACK)
					{
					if (current)
						--current;
					else
						current = 12;
					}
				if (rtn == E_END && !change)
					rtn = E_FORE;
				}
			while (rtn != E_END && rtn != E_QUIT && rtn != E_PGUP && rtn != E_PGDN);

			if ((rtn == E_END || rtn == E_PGUP || rtn == E_PGDN) && change)
				{
				if (cur_evt == cur_evts)
					{
					if (cur_evts >= max_evts)
						{
						if (!(evts = realloc(evts,(max_evts += 10) * sizeof(struct evt *))))
							error(FATAL,"Error: Out of memory to allocate structures!",0,0,0);
						}
					if (!(evts[cur_evts] = calloc(1,sizeof(struct evt))))
						error(FATAL,"Error: Out of memory to allocate structures!",0,0,0);
					memcpy(evts[cur_evts],&tevt,sizeof(struct evt));
					++cur_evts;
					}
				else
					memcpy(evts[cur_evt],&tevt,sizeof(struct evt));
				ischange = 1;
				}
			if (rtn == E_PGDN)
				{
				if (cur_evt == cur_evts)
					cur_evt = 0;
				else
					++cur_evt;
				}
			else if (rtn == E_PGUP)
				{
				if (cur_evt)
					--cur_evt;
				else
					cur_evt = cur_evts;
				}
			}
		while (rtn != E_QUIT);

		close_window(wndw);
		}
	else
		error(NOTICE,"Unable to open window!",1,CLOCK_POS + 0x1700,color[0]);

	if (ischange)
		master_change = 1;
	}

