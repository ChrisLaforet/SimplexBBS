/* s_misc.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_misc.c_v  $
**                       $Date:   25 Oct 1992 14:15:28  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <screen.h>
#include <window.h>
#include <editor.h>
#include <keys.h>
#include "sconfig.h"



struct choice yn[2] =
	{
		{'Y',"Yes"},
		{'N',"No"},
	};



struct choice background[5] =
	{
		{'1',"Blue"},
		{'2',"Red"},
		{'3',"Brown"},
		{'4',"Cyan"},
		{'5',"White"},
	};



struct choice foreground[5] =
	{
		{'1',"Green"},
		{'2',"Cyan"},
		{'3',"Magenta"},
		{'4',"Yellow"},
		{'5',"White"},
	};




void draw_miscellaneous_info(void)
	{
	prntnomov(0x308,25,color[0],"Status line background:");
	_vcw('_',color[3],0x320,7);
	prntnomov(0x408,25,color[0],"Entry field background:");
	_vcw('_',color[3],0x420,7);
	prntnomov(0x510,25,color[0],"Sub menu color:");
	_vcw('_',color[3],0x520,7);

#ifndef PROTECTED
	prntnomov(0x606,25,color[0],"Use direct screen writes?");
	_vcw('_',color[3],0x620,1);
	prntnomov(0x70a,21,color[0],"Enable snow checking?");
	_vcw('_',color[3],0x720,1);
#endif

	prntnomov(0x904,27,color[0],"Preregistered BBS use only?");
	_vcw('_',color[3],0x920,1);
	prntnomov(0xa08,23,color[0],"Disable local keyboard?");
	_vcw('_',color[3],0xa20,1);

	prntnomov(0xc07,24,color[0],"Priv level for new user:");
	_vcw('_',color[3],0xc20,3);
	prntnomov(0xc2c,20,color[0],"New user flags:");
	_vcw(' ',color[3],0xc3c,16);
	prntnomov(0xd07,24,color[0],"Net credit for new user:");
	_vcw('_',color[3],0xd20,4);
	prntnomov(0xd25,10,color[0],"cents");
	prntnomov(0xe0d,18,color[0],"Time for new user:");
	prntnomov(0xe20,10,color[3],"__");
	prntnomov(0xe24,10,color[0],"minutes");
	prntnomov(0xf0b,20,color[0],"Ask for home number?");
	_vcw('_',color[3],0xf20,1);
	prntnomov(0xf27,25,color[0],"Ask for data number?");
	_vcw('_',color[3],0xf3c,1);
	prntnomov(0x100a,25,color[0],"Ask for full address?");
	_vcw('_',color[3],0x1020,1);
	prntnomov(0x1104,30,color[0],"Allow cross-area downloads?");
	_vcw('_',color[3],0x1120,1);
	prntnomov(0x1207,24,color[0],"Total tries at password:");
	_vcw('_',color[3],0x1220,1);
	prntnomov(0x1303,28,color[0],"Inactive time before logoff:");
	_vcw('_',color[3],0x1320,2);
	prntnomov(0x1323,10,color[0],"minutes");
	prntnomov(0x1403,28,color[0],"Number of yells per session:");
	_vcw('_',color[3],0x1420,2);
	prntnomov(0x1425,22,color[0],"Duration of yell bell:");
	prntnomov(0x143c,10,color[3],"__");
	prntnomov(0x143f,10,color[0],"seconds");
	prntnomov(0x1506,25,color[0],"Time frame for yell bell:");
	prntnomov(0x1520,13,color[3],"__:__");
	prntnomov(0x1526,10,color[0],"through");
	prntnomov(0x152e,5,color[3],"__:__");
	prntnomov(0x1604,27,color[0],"Time frame for downloading:");
	prntnomov(0x1620,13,color[3],"__:__");
	prntnomov(0x1626,10,color[0],"through");
	prntnomov(0x162e,5,color[3],"__:__");

#if 0
	prntnomov(0x1415,60,color[0],"These use same $ options as \"run external prog\" in menus!");
	prntnomov(0x1504,13,color[0],"Welcome Prog:");
	_vcw('_',color[3],0x1512,60);
	prntnomov(0x1603,14,color[0],"Virus Checker:");
	_vcw('_',color[3],0x1612,60);
#endif
	}



char *get_status(char color)
	{
	char *cptr;

	switch (color)
		{
		case COLOR_ONBLUE:
			cptr = "Blue";
			break;
		case COLOR_ONRED:
			cptr = "Red";
			break;
		case COLOR_ONBROWN:
			cptr = "Brown";
			break;
		case COLOR_ONCYAN:
			cptr = "Cyan";
			break;
		case COLOR_ONWHITE:
		default:
			cptr = "White";
			break;
		}
	return cptr;
	}



char *get_field(char color)
	{
	char *cptr;

	switch (color)
		{
		case COLOR_ONBROWN:
			cptr = "Brown";
			break;
		case COLOR_ONCYAN:
			cptr = "Cyan";
			break;
		case COLOR_ONRED:
			cptr = "Red";
			break;
		case COLOR_ONBLUE:
		default:
			cptr = "Blue";
			break;
		}
	return cptr;
	}



char *get_menu(char color)
	{
	char *cptr;

	switch (color)
		{
		case COLOR_GREEN:
			cptr = "Green";
			break;
		case COLOR_CYAN:
			cptr = "Cyan";
			break;
		case COLOR_MAGENTA:
			cptr = "Magenta";
			break;
		case COLOR_YELLOW:
			cptr = "Yellow";
			break;
		case COLOR_WHITE:
		default:
			cptr = "White";
			break;
		}
	return cptr;
	}



void fill_miscellaneous_info(void)
	{
	char *cptr;

	cptr = get_status(cfg.cfg_status);
	prntnomov(0x320,7,color[3],cptr);
	cptr = get_field(cfg.cfg_field);
	prntnomov(0x420,7,color[3],cptr);
	cptr = get_menu(cfg.cfg_menu);
	prntnomov(0x520,7,color[3],cptr);

#ifndef PROTECTED
	_vcw((char)(cfg.cfg_flags & CFG_DIRECT ? 'Y' : 'N'),color[3],0x620,1);
	_vcw((char)(cfg.cfg_flags & CFG_SNOW ? 'Y' : 'N'),color[3],0x720,1);
#endif

	_vcw((char)(cfg.cfg_flags & CFG_PRIVATEBBS ? 'Y' : 'N'),color[3],0x920,1);
	_vcw((char)(cfg.cfg_flags & CFG_DISABLEKB ? 'Y' : 'N'),color[3],0xa20,1);

	prntnomovf(0xc20,3,color[3],"%u",(int)(unsigned char)cfg.cfg_newpriv);
	fill_flags(0xc3c,cfg.cfg_newflags,color[3]);
	prntnomovf(0xd20,4,color[3],"%u",cfg.cfg_newcredit);
	prntnomovf(0xe20,4,color[3],"%u",cfg.cfg_newtime);
	_vcw((char)(cfg.cfg_askhome ? 'Y' : 'N'),color[3],0xf20,1);
	_vcw((char)(cfg.cfg_askdata ? 'Y' : 'N'),color[3],0xf3c,1);
	_vcw((char)(cfg.cfg_askaddress ? 'Y' : 'N'),color[3],0x1020,1);
	_vcw((char)(cfg.cfg_flags & CFG_CROSSAREA_DL ? 'Y' : 'N'),color[3],0x1120,1);

	prntnomovf(0x1220,1,color[3],"%u",cfg.cfg_pwdtries);
	prntnomovf(0x1320,2,color[3],"%u",cfg.cfg_inactive);
	prntnomovf(0x1420,2,color[3],"%u",cfg.cfg_yells);
	prntnomovf(0x143c,2,color[3],"%u",cfg.cfg_yelltime);
	prntnomovf(0x1520,5,color[3],"%02u:%02u",(unsigned int)cfg.cfg_yellstart >> 11,(cfg.cfg_yellstart >> 5) & 63);
	prntnomovf(0x152e,5,color[3],"%02u:%02u",(unsigned int)cfg.cfg_yellstop >> 11,(cfg.cfg_yellstop >> 5) & 63);
	prntnomovf(0x1620,5,color[3],"%02u:%02u",(unsigned int)cfg.cfg_dlstart >> 11,(cfg.cfg_dlstart >> 5) & 63);
	prntnomovf(0x162e,5,color[3],"%02u:%02u",(unsigned int)cfg.cfg_dlstop >> 11,(cfg.cfg_dlstop >> 5) & 63);

#if 0
	prntnomov(0x1512,60,color[3],cfg.cfg_welcome);
	prntnomov(0x1612,60,color[3],cfg.cfg_viruschk);
#endif
	}



void edit_miscellaneous_info(void)
	{
	struct window *wndw;
	char buffer[10];
	char *cptr;
	int change = 0;
	int current = 0;
	int thour;
	int tmin;
	int tint;
	int redo;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,27,color[1]," Miscellaneous Information ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		draw_miscellaneous_info();
		fill_miscellaneous_info();
		do
			{
			switch (current)
				{
				case 0:
					rtn = pop_menu(0x12a,color[4],color[3],"Status Color?",5,background,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						switch (rtn)
							{
							case '1':
								cfg.cfg_status = COLOR_ONBLUE;
								rtn = E_EXIT;
								break;
							case '2':
								cfg.cfg_status = COLOR_ONRED;
								rtn = E_EXIT;
								break;
							case '3':
								cfg.cfg_status = COLOR_ONBROWN;
								rtn = E_EXIT;
								break;
							case '4':
								cfg.cfg_status = COLOR_ONCYAN;
								rtn = E_EXIT;
								break;
							case '5':
								cfg.cfg_status = COLOR_ONWHITE;
								rtn = E_EXIT;
								break;
							default:
								rtn = E_FORE;
							}
						if (rtn == E_EXIT)
							{
							cptr = get_status(cfg.cfg_status);
							_vcw('_',color[3],0x320,7);
							prntnomov(0x320,7,color[3],cptr);
							}
						}
					break;
				case 1:
					rtn = pop_menu(0x22c,color[4],color[3],"Field Color?",4,background,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						switch (rtn)
							{
							case '1':
								cfg.cfg_field = COLOR_ONBLUE;
								rtn = E_EXIT;
								break;
							case '2':
								cfg.cfg_field = COLOR_ONRED;
								rtn = E_EXIT;
								break;
							case '3':
								cfg.cfg_field = COLOR_ONBROWN;
								rtn = E_EXIT;
								break;
							case '4':
								cfg.cfg_field = COLOR_ONCYAN;
								rtn = E_EXIT;
								break;
							default:
								rtn = E_FORE;
							}
						if (rtn == E_EXIT)
							{
							cptr = get_field(cfg.cfg_field);
							_vcw('_',color[3],0x420,7);
							prntnomov(0x420,7,color[3],cptr);
							}
						}
					break;
				case 2:
					rtn = pop_menu(0x32e,color[4],color[3],"Menu Color?",5,foreground,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						switch (rtn)
							{
							case '1':
								cfg.cfg_menu = COLOR_GREEN;
								rtn = E_EXIT;
								break;
							case '2':
								cfg.cfg_menu = COLOR_CYAN;
								rtn = E_EXIT;
								break;
							case '3':
								cfg.cfg_menu = COLOR_MAGENTA;
								rtn = E_EXIT;
								break;
							case '4':
								cfg.cfg_menu = COLOR_YELLOW;
								rtn = E_EXIT;
								break;
							case '5':
								cfg.cfg_menu = COLOR_WHITE;
								rtn = E_EXIT;
								break;
							default:
								rtn = E_FORE;
							}
						if (rtn == E_EXIT)
							{
							cptr = get_menu(cfg.cfg_menu);
							_vcw('_',color[3],0x520,7);
							prntnomov(0x520,7,color[3],cptr);
							}
						}
					break;

#ifndef PROTECTED
				case 3:
					rtn = pop_menu(0x423,color[4],color[3],"Direct writes?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y' && !(cfg.cfg_flags & CFG_DIRECT))
							{
							cfg.cfg_flags |= CFG_DIRECT;
							rtn = E_EXIT;
							}
						else if (rtn == 'N' && (cfg.cfg_flags & CFG_DIRECT))
							{
							cfg.cfg_flags &= ~CFG_DIRECT;
							rtn = E_EXIT;
							}
						else 
							rtn = E_FORE;
						_vcw((char)(cfg.cfg_flags & CFG_DIRECT ? 'Y' : 'N'),color[3],0x620,1);
						}
					break;
				case 4:
					rtn = pop_menu(0x525,color[4],color[3],"Snow checking?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y' && !(cfg.cfg_flags & CFG_SNOW))
							{
							cfg.cfg_flags |= CFG_SNOW;
							rtn = E_EXIT;
							}
						else if (rtn == 'N' && (cfg.cfg_flags & CFG_SNOW))
							{
							cfg.cfg_flags &= ~CFG_SNOW;
							rtn = E_EXIT;
							}
						else 
							rtn = E_FORE;
						_vcw((char)(cfg.cfg_flags & CFG_SNOW ? 'Y' : 'N'),color[3],0x720,1);
						}
					break;
#endif

				case 5:
					rtn = pop_menu(0x727,color[4],color[3],"Only Preregistered Use?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y' && !(cfg.cfg_flags & CFG_PRIVATEBBS))
							{
							cfg.cfg_flags |= CFG_PRIVATEBBS;
							rtn = E_EXIT;
							}
						else if (rtn == 'N' && (cfg.cfg_flags & CFG_PRIVATEBBS))
							{
							cfg.cfg_flags &= ~CFG_PRIVATEBBS;
							rtn = E_EXIT;
							}
						else 
							rtn = E_FORE;
						_vcw((char)(cfg.cfg_flags & CFG_PRIVATEBBS ? 'Y' : 'N'),color[3],0x920,1);
						}
					break;
				case 6:
					rtn = pop_menu(0x829,color[4],color[3],"Disable Keyboard?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y' && !(cfg.cfg_flags & CFG_DISABLEKB))
							{
							cfg.cfg_flags |= CFG_DISABLEKB;
							rtn = E_EXIT;
							}
						else if (rtn == 'N' && (cfg.cfg_flags & CFG_DISABLEKB))
							{
							cfg.cfg_flags &= ~CFG_DISABLEKB;
							rtn = E_EXIT;
							}
						else 
							rtn = E_FORE;
						_vcw((char)(cfg.cfg_flags & CFG_DISABLEKB ? 'Y' : 'N'),color[3],0xa20,1);
						}
					break;

				case 7:
					sprintf(buffer,"%u",(int)(unsigned char)cfg.cfg_newpriv);
					rtn = edit_number(0xc20,color[4],color[3],buffer,1,255,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tint = atoi(buffer);
						cfg.cfg_newpriv = (char)(unsigned char)tint;
						}
					break;
				case 8:
					rtn = edit_flags(0xc3c,&cfg.cfg_newflags,color[4],color[3],1,CLOCK_POS,color[0]);
					break;
				case 9:
					sprintf(buffer,"%u",(int)cfg.cfg_newcredit);
					rtn = edit_field(0xd20,color[4],color[3],1,CLOCK_POS,color[0],"F04",buffer,NULL);
					if (rtn == E_EXIT)
						cfg.cfg_newcredit = atoi(buffer);
					break;
				case 10:
					sprintf(buffer,"%u",(int)cfg.cfg_newtime);
					rtn = edit_field(0xe20,color[4],color[3],1,CLOCK_POS,color[0],"F02",buffer,NULL);
					if (rtn == E_EXIT)
						cfg.cfg_newtime = atoi(buffer);
					break;
				case 11:
					rtn = pop_menu(0xc22,color[4],color[3],"Home #?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							{
							if (!cfg.cfg_askhome)
								{
								cfg.cfg_askhome = 1;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						else
							{
							if (cfg.cfg_askhome)
								{
								cfg.cfg_askhome = 0;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						_vcw((char)(cfg.cfg_askhome ? 'Y' : 'N'),color[3],0xf20,1);
						}
					break;
				case 12:
					rtn = pop_menu(0xc3e,color[4],color[3],"Data #?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							{
							if (!cfg.cfg_askdata)
								{
								cfg.cfg_askdata = 1;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						else
							{
							if (cfg.cfg_askdata)
								{
								cfg.cfg_askdata = 0;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						_vcw((char)(cfg.cfg_askdata ? 'Y' : 'N'),color[3],0xf3c,1);
						}
					break;
				case 13:
					rtn = pop_menu(0xd22,color[4],color[3],"Full Address?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							{
							if (!cfg.cfg_askaddress)
								{
								cfg.cfg_askaddress = 1;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						else
							{
							if (cfg.cfg_askaddress)
								{
								cfg.cfg_askaddress = 0;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						_vcw((char)(cfg.cfg_askaddress ? 'Y' : 'N'),color[3],0x1020,1);
						}
					break;
				case 14:
					rtn = pop_menu(0xe26,color[4],color[3],"X-Area D/L?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							{
							if (!(cfg.cfg_flags & CFG_CROSSAREA_DL))
								{
								cfg.cfg_flags |= CFG_CROSSAREA_DL;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						else
							{
							if (cfg.cfg_flags & CFG_CROSSAREA_DL)
								{
								cfg.cfg_flags &= ~CFG_CROSSAREA_DL;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						_vcw((char)(cfg.cfg_flags & CFG_CROSSAREA_DL ? 'Y' : 'N'),color[3],0x1120,1);
						}
					break;

				case 15:
					sprintf(buffer,"%u",(int)cfg.cfg_pwdtries);
					rtn = edit_number(0x1220,color[4],color[3],buffer,1,9,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_pwdtries = (char)atoi(buffer);
					break;
				case 16:
					sprintf(buffer,"%u",(int)cfg.cfg_inactive);
					rtn = edit_number(0x1320,color[4],color[3],buffer,1,60,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_inactive = (char)atoi(buffer);
					break;
				case 17:
					sprintf(buffer,"%u",(int)cfg.cfg_yells);
					rtn = edit_number(0x1420,color[4],color[3],buffer,1,99,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_yells = (char)atoi(buffer);
					break;
				case 18:
					sprintf(buffer,"%u",(int)cfg.cfg_yelltime);
					rtn = edit_number(0x143c,color[4],color[3],buffer,1,90,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						cfg.cfg_yelltime = (char)atoi(buffer);
					break;
				case 19:
					do
						{
						redo = 0;
						sprintf(buffer,"%02u%02u",cfg.cfg_yellstart >> 11,(cfg.cfg_yellstart >> 5) & 63);
						_vcw('_',color[3],0x1520,5);
						prntnomovf(0x1520,5,color[3],"%2.2s:%2.2s",buffer,buffer + 2);
						rtn = edit_field(0x1520,color[4],color[3],1,CLOCK_POS,color[0],"00J00",buffer,NULL);
						if (rtn == E_EXIT)
							{
							tmin = atoi(buffer + 2);
							buffer[2] = '\0';
							thour = atoi(buffer);
							if (thour > 23 || tmin > 59)
								{
								error(NOTICE,"Invalid time!  Times must be between 00:00 and 23:59!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_yellstart = (thour << 11) | (tmin << 5);
							}
						}
					while (redo);
					break;
				case 20:
					do
						{
						redo = 0;
						sprintf(buffer,"%02u%02u",(unsigned int)cfg.cfg_yellstop >> 11,(cfg.cfg_yellstop >> 5) & 63);
						_vcw('_',color[3],0x152e,5);
						prntnomovf(0x152e,5,color[3],"%2.2s:%2.2s",buffer,buffer + 2);
						rtn = edit_field(0x152e,color[4],color[3],1,CLOCK_POS,color[0],"00J00",buffer,NULL);
						if (rtn == E_EXIT)
							{
							tmin = atoi(buffer + 2);
							buffer[2] = '\0';
							thour = atoi(buffer);
							if (thour > 23 || tmin > 59)
								{
								error(NOTICE,"Invalid time!  Times must be between 00:00 and 23:59!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_yellstop = (thour << 11) | (tmin << 5);
							}
						}
					while (redo);
					break;
				case 21:
					do
						{
						redo = 0;
						sprintf(buffer,"%02u%02u",(unsigned int)cfg.cfg_dlstart >> 11,(cfg.cfg_dlstart >> 5) & 63);
						_vcw('_',color[3],0x1620,5);
						prntnomovf(0x1620,5,color[3],"%2.2s:%2.2s",buffer,buffer + 2);
						rtn = edit_field(0x1620,color[4],color[3],1,CLOCK_POS,color[0],"00J00",buffer,NULL);
						if (rtn == E_EXIT)
							{
							tmin = atoi(buffer + 2);
							buffer[2] = '\0';
							thour = atoi(buffer);
							if (thour > 23 || tmin > 59)
								{
								error(NOTICE,"Invalid time!  Times must be between 00:00 and 23:59!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_dlstart = (thour << 11) | (tmin << 5);
							}
						}
					while (redo);
					break;
				case 22:
					do
						{
						redo = 0;
						sprintf(buffer,"%02u%02u",(unsigned int)cfg.cfg_dlstop >> 11,(cfg.cfg_dlstop >> 5) & 63);
						_vcw('_',color[3],0x162e,5);
						prntnomovf(0x162e,5,color[3],"%2.2s:%2.2s",buffer,buffer + 2);
						rtn = edit_field(0x162e,color[4],color[3],1,CLOCK_POS,color[0],"00J00",buffer,NULL);
						if (rtn == E_EXIT)
							{
							tmin = atoi(buffer + 2);
							buffer[2] = '\0';
							thour = atoi(buffer);
							if (thour > 23 || tmin > 59)
								{
								error(NOTICE,"Invalid time!  Times must be between 00:00 and 23:59!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								cfg.cfg_dlstop = (thour << 11) | (tmin << 5);
							}
						}
					while (redo);
					break;
				}
			if (rtn == E_EXIT)
				change = 1;
			if (rtn == E_FORE || rtn == E_EXIT)
				{
				if (current < 22)
					++current;
				else
					current = 0;
				}
			if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 22;
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



