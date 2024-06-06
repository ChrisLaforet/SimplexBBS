/* s_menu.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_menu.c_v  $
**                       $Date:   25 Oct 1992 14:15:28  $
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




struct window *save_screen(int tlc,int brc)
	{
	struct window *scrn;
	int length;
	int height;
	int count;

	if (!(scrn = (struct window *)malloc(sizeof(struct window))))
		return(NULL);
	length = (brc & 0xff) - (tlc & 0xff) + 1;	 /* from one to the other inclusive */
	height = (brc >> 8) - (tlc >> 8) + 1;	 /* from one to the other inclusive */
	if ((scrn->save = (int *)calloc((length * height),sizeof(int))) == NULL)
		{
		free(scrn);
		return(NULL);
		}
	scrn->start = tlc;
	scrn->stop = brc;
#ifndef PROTECTED
	scrn->curpos = cur_ega->cursor_posn[(cur_ega->active_page)];
	scrn->curmode = cur_ega->cursor_mode;
#else
	scrn->curpos = _getcurpos();
	scrn->curmode = get_cursor_type(&scrn->curattr);
#endif
	for (count = 0; count < height; count++)
		_vbr((scrn->save + (count * length)),(((count + (tlc >> 8)) << 8) + (tlc & 0xFF)),length);
	return scrn;
	}



void restore_screen(struct window *scrn)
	{
	int length;
	int height;
	int count;

	length = (scrn->stop & 0xFF) - (scrn->start & 0xFF) + 1;	/* from one to the other inclusive */
	height = (scrn->stop >> 8) - (scrn->start >> 8) + 1;	/* from one to the other inclusive */
	for (count = 0; count < height; count++)
		_vbw((scrn->save + (count * length)),(((count + (scrn->start >> 8)) << 8) + (scrn->start & 0xFF)),length);
	_setcurpos(scrn->curpos);
#ifdef PROTECTED
	_setcurmode(scrn->curmode,scrn->curattr);
#else
	_setcurmode(scrn->curmode);
#endif
	free((char *)scrn->save);
	free((char *)scrn);
	}



void print_help(char *string)
	{
	_vcw(' ',color[3],0x1800,80);
	prntcenter(0x1800,80,color[3],string);
	}



void print_menu_line(int cursor,int width,int hicolor,int locolor,char *string)
	{
	char buffer[100];
	char letter;
	char *cptr;
	char *cptr1;
	int offset = 0;
	int found = 0;

	cptr = buffer;
	cptr1 = string;
	while (*cptr1)
		{
		if (*cptr1 == '^')
			{
			++cptr1;
			letter = (char)toupper(*cptr1);
			found = 1;
			}
		*cptr++ = *cptr1++;
		if (!found)
			++offset;
		}
	*cptr = '\0';
	prntnomov(cursor,width,locolor,buffer);
	if (found)
		_vcw(letter,hicolor,cursor + offset,1);
	}



void push_menu(int tlc,int hicolor,int locolor,char *title,struct mnu *choices,int max_choices)
	{
	struct window *wndw;
	struct window *wndw1;
	char buffer[60];
	int phantom_color;
	int offset = tlc + 0x202;
	int current = 0;
	int width = 0;
	int count;
	int kount;
	int found;
	int key;
	int brc;
	int quit = 0;

	if ((locolor & ON_WHITE) != ON_WHITE)
		phantom_color = ON_WHITE;
	else
		phantom_color = NORMAL;
	width = strlen(title);
	for (count = 0; count < max_choices; count++)
		{
		if (width < (int)strlen(choices[count].mnu_string))
			width = (int)strlen(choices[count].mnu_string);
		}

	if (max_choices)
		{
		register_help("menus");
		brc = tlc;
		brc += (max_choices << 8) + 0x300;
		brc += width + 3;

		if (wndw = open_shadow_window(tlc,brc,locolor,1))
			{
			prntcenter(tlc + 2,width,hicolor,title);
			for (count = 0; count < max_choices; count++)
				{
				print_menu_line(offset,width,hicolor,locolor,choices[count].mnu_string);
				offset += 0x100;
				}
			do
				{
				set_cursor_type(HIDDEN);
				phantom(tlc + 0x201 + (current << 8),width + 2,phantom_color);
				for (count = 0, kount = 0; count < max_choices; count++)
					{
					if (current == kount)
						{
						print_help(choices[count].mnu_help);
						break;
						}
					else
						++kount;
					}
				key = read_keyboard(1,CLOCK_POS + 0x1700,color[0]);
				switch (key)
					{
					case 0x1b:
						quit = 1;
						break;
					case 0xd:
						if (choices[count].mnu_function == NULL)
							/* do nothing */;
						else if (choices[count].mnu_function == EXIT_MENU)
							quit = 1;
						else
							{
							if (wndw1 = save_screen(0,0x184f))
								{
								deregister_help();
								prntcenter(tlc + 2,width,locolor,title);
								(*choices[count].mnu_function)();
								restore_screen(wndw1);
								register_help("menus");
								}
							else
								{
								sprintf(buffer,"Fatal Error: Out of memory [ %s : %u ]!",__FILE__,__LINE__);
								error(FATAL,buffer,1,CLOCK_POS + 0x1700,color[0]);
								}
							}
						break;
					case UpArrow:
						phantom(tlc + 0x201 + (current << 8),width + 2,locolor);
						print_menu_line(tlc + 0x202 + (current << 8),width,hicolor,locolor,choices[current].mnu_string);
						if (max_choices > 1)
							{
							if (current)
								--current;
							else
								current = (max_choices - 1);
							}
						else
							beep();
						break;
					case DownArrow:
						phantom(tlc + 0x201 + (current << 8),width + 2,locolor);
						print_menu_line(tlc + 0x202 + (current << 8),width,hicolor,locolor,choices[current].mnu_string);
						if (max_choices > 1)
							{
							if (current < (max_choices - 1))
								++current;
							else
								current = 0;
							}
						else
							beep();
						break;
					default:
						phantom(tlc + 0x201 + (current << 8),width + 2,locolor);
						print_menu_line(tlc + 0x202 + (current << 8),width,hicolor,locolor,choices[current].mnu_string);
						for (count = 0, found = 0; count < max_choices; count++)
							{
							if (toupper(key) == toupper(choices[count].mnu_shortcut))
								{
								current = count;
								phantom(tlc + 0x201 + (current << 8),width + 2,phantom_color);
								if (choices[count].mnu_function == NULL)
									/* do nothing */;
								else if (choices[count].mnu_function == EXIT_MENU)
									{
									quit = 1;
									}
								else
									{
									if (wndw1 = save_screen(0,0x184f))
										{
										deregister_help();
										prntcenter(tlc + 2,width,locolor,title);
										(*choices[count].mnu_function)();
										restore_screen(wndw1);
										register_help("menus");
										}
									else
										{
										sprintf(buffer,"Fatal Error: Out of memory [ %s : %u ]!",__FILE__,__LINE__);
										error(FATAL,buffer,1,CLOCK_POS + 0x1700,color[0]);
										}
									}
								found = 1;
								}
							}
						if (!found)
							beep();
						break;
					}
				if (color_change_flag)
					quit = 1;		/* exit back */
				}
			while (!quit);
			close_shadow_window(wndw);

			deregister_help();
			}
		else
			{
			sprintf(buffer,"Error: Unable to open menu window!");
			error(NON_FATAL,buffer,1,CLOCK_POS + 0x1700,color[0]);
			}
		}
	}



