/* s_flist.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_flist.c_v  $
**                       $Date:   25 Oct 1992 14:15:22  $
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


#define PAGE_LEN			19
#define CURLINE				(current - top)


static char search_text[21] = "";



int find_file_list(void)
	{
	struct window *wndw;
	char buffer[81];
	int found = 0;
	int count;
	int rtn;
	int retval = -1;

	if (wndw = open_shadow_window(0xa11,0xe3e,color[3],1))
		{
		prntnomov(0xa12,40,color[4]," Search for a File Area ");
		prntnomov(0xc14,40,color[3],"What to search for? ");
		_vcw('_',color[0],0xc29,20);
		do
			{
			search_text[0] = '\0';
			rtn = edit_field(0xc29,color[1],color[0],1,CLOCK_POS,color[0],"FPU20",search_text,NULL);
			if (rtn == E_EXIT && search_text[0])
				rtn = E_END;
			if (rtn == E_END && !search_text[0])
				rtn = E_EXIT;
			}
		while (rtn != E_QUIT && rtn != E_END);
		close_shadow_window(wndw);

		if (search_text[0] && rtn == E_END)
			{
			wndw = open_message("Please wait.  Searching for pattern in file areas.",color[3]);
			for (count = 0; count < cur_files; count++)
				{
				strcpy(buffer,files[count]->file_areaname);
				strupr(buffer);
				if (strstr(buffer,search_text))
					{
					found = 1;
					break;
					}
				}
			if (wndw)
				close_message(wndw);
			if (found)
				retval = count;
			else
				{
				sprintf(buffer,"Nothing with the pattern \"%s\" was found!",search_text);
				error(NOTICE,buffer,1,CLOCK_POS,color[0]);
				}
			}
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);
	return retval;
	}



int next_file_list(int start)
	{
	struct window *wndw;
	char buffer[81];
	int found = 0;
	int count;
	int retval = -1;

	if (search_text[0])
		{
		wndw = open_message("Please wait.  Searching for pattern among file areas.",color[3]);
		for (count = (start + 1); count < cur_files; count++)
			{
			strcpy(buffer,files[count]->file_areaname);
			strupr(buffer);
			if (strstr(buffer,search_text))
				{
				found = 1;
				break;
				}
			}
		if (wndw)
			close_message(wndw);
		if (found)
			retval = count;
		else
			{
			sprintf(buffer,"Nothing with the pattern \"%s\" was found!",search_text);
			error(NOTICE,buffer,1,CLOCK_POS,color[0]);
			}
		}
	else
		error(NOTICE,"No previous pattern established for searching!",1,CLOCK_POS,color[0]);
	return retval;
	}



void draw_file_list(void)
	{
	int count;

	for (count = 0; count < (PAGE_LEN + 2); count++)
		{
		_vcw('³',color[0],0x206 + (count << 8),1);
		_vcw('³',color[0],0x20d + (count << 8),1);
		_vcw('³',color[0],0x237 + (count << 8),1);
		}
	_vcw('Ä',color[0],0x302,76);
	_vcw('Å',color[0],0x306,1);
	_vcw('Å',color[0],0x30d,1);
	_vcw('Å',color[0],0x337,1);
    
	prntnomov(0x208,8,color[0],"Area");
	prntnomov(0x20f,20,color[0],"Area Name");
	prntnomov(0x239,20,color[0],"Access Priv & Flags");

#if 0
	drawbox(0x1400,0x184f,color[3],1);
	clrblk(0x1501,0x174e,color[3]);
	_vcw('Æ',color[3],0x1400,1);
	_vcw('µ',color[3],0x144f,1);
	_vcw('Ï',color[3],0x1406,1);
	_vcw('Ï',color[3],0x140d,1);
	_vcw('Ï',color[3],0x1437,1);
	prntnomov(0x1502,30,color[4]," or ");
	prntnomov(0x150e,30,color[3],"- Select Prev/Next Area");
	prntnomov(0x1528,30,color[4],"PgUp or PgDn");
	prntnomov(0x1535,30,color[3],"- Select Prev/Next Page");
	prntnomov(0x1602,30,color[4],"Home or End");
	prntnomov(0x160e,30,color[3],"- Select First/Last Area");
	prntnomov(0x1628,30,color[4],"F5 or F6");
	prntnomov(0x1635,30,color[3],"- Search for an Area");
	prntnomov(0x1702,30,color[4],"Esc");
	prntnomov(0x170e,30,color[3],"- Exit");
#endif
	}



void edit_file_lists(void)
	{
	struct window *wndw;
	int current = 0;
	int inverse;
	int redraw = 1;
	int count;
	int top = 0;
	int key;
	int rtn;

	if (cur_files)
		{
		if (wndw = open_window(0x0,0x184f,color[0],1))
			{
			prntnomov(0x1,40,color[1]," Select File Area List for Editing ");
			prntnomov(0x1801,40,color[0]," Press F1 for Help ");
			draw_file_list();

			search_text[0] = '\0';

			set_cursor_type(HIDDEN);
			register_help("item select");
			if ((color[0] & ON_WHITE) != ON_WHITE)
				inverse = ON_WHITE;
			else
				inverse = LT_GRAY;

			do
				{
				if (redraw)
					{
					clrblk(0x402,0x1605,color[0]);
					clrblk(0x407,0x160c,color[0]);
					clrblk(0x40e,0x1636,color[0]);
					clrblk(0x438,0x164d,color[0]);

					for (count = 0; count < PAGE_LEN; count++)
						{
						if ((top + count) >= cur_files)
							break;
						prntnomovf(0x402 + (count << 8),4,color[0],"%4u",top + count + 1);
						prntnomovf(0x408 + (count << 8),4,color[0],"%4u",files[top + count]->file_number);
						prntnomov(0x40f + (count << 8),40,color[0],files[top + count]->file_areaname);
						prntnomovf(0x439 + (count << 8),40,color[0],"%3u",files[top + count]->file_priv);
						fill_flags(0x43e + (count << 8),files[top + count]->file_flags,color[0]);
						}
					redraw = 0;
					}

				phantom(0x402 + (CURLINE << 8),76,inverse);
				key = read_keyboard(1,CLOCK_POS,color[0]);
				switch (key)
					{
					case UpArrow:
						if (current)
							{
							phantom(0x402 + (CURLINE << 8),76,color[0]);
							--current;
							if (current < top)
								{
								if (top >= 8)
									top -= 8;
								else
									top = 0;
								redraw = 1;
								}
							/* otherwise merely move the phantom bar */
							}
						break;
					case DownArrow:
						if (current < (cur_files - 1))
							{
							phantom(0x402 + (CURLINE << 8),76,color[0]);
							++current;
							if (current >= (top + PAGE_LEN))
								{
								top += 8;
								redraw = 1;
								}
							/* otherwise merely move the phantom bar */
							}
						break;
            		case Home:
						if (current)
							{
							phantom(0x402 + (CURLINE << 8),76,color[0]);
							current = 0;
							if (top)
								{
								top = 0;
								redraw = 1;
								}
							}
						break;
					case End:
						if (current != (cur_files - 1))
							{
							phantom(0x402 + (CURLINE << 8),76,color[0]);
							current = cur_files - 1;
							if (current > (top + PAGE_LEN))
								{
								top = cur_files - PAGE_LEN;
								redraw = 1;
								}
							}
						break;
            		case PgUp:
						if (current)
							{
							phantom(0x402 + (CURLINE << 8),76,color[0]);
							if (current >= PAGE_LEN)
								{
								current -= PAGE_LEN;
								if (top >= PAGE_LEN)
									{
									top -= PAGE_LEN;
									redraw = 1;
									}
								else if (top)
									{
									top = 0;
									redraw = 1;
									}
								}
							else
								{
								if (top)
									{
									top = 0;
									redraw = 1;
									}
								current = 0;
								}
                    		}
						break;
            		case PgDn:
						if (current < (cur_files - 1))
							{
							phantom(0x402 + (CURLINE << 8),76,color[0]);
							if ((current + PAGE_LEN) <= cur_files)
								{
								current += PAGE_LEN;
								top += PAGE_LEN;
								redraw = 1;
								}
							else if ((top + PAGE_LEN) < cur_files)
								{
								current = cur_files - 1;
								top += PAGE_LEN;
								redraw = 1;
								}
							else
								current = cur_files - 1;
							}
						break;
					case F5:
						if ((rtn = find_file_list()) != -1)
							{
							phantom(0x402 + (CURLINE << 8),76,color[0]);
							current = rtn;
							if (current < top || current >= (top + PAGE_LEN))
								{
								top = (current / PAGE_LEN) * PAGE_LEN;
								redraw = 1;
								}
							}
						break;
					case F6:
						if ((rtn = next_file_list(current)) != -1)
							{
							phantom(0x402 + (CURLINE << 8),76,color[0]);
							current = rtn;
							if (current < top || current >= (top + PAGE_LEN))
								{
								top = (current / PAGE_LEN) * PAGE_LEN;
								redraw = 1;
								}
							}
						break;
					case CR:
						fedit_list(files[current]);
						break;
            		case ESC:
						break;
					}
				}
			while (key != ESC);

			close_window(wndw);
			deregister_help();
			}
		else 
			error(NOTICE,"Unable to open window!",1,CLOCK_POS + 0x1700,color[0]);
		}
	else 
		error(NOTICE,"There are no file areas currently set up!",1,CLOCK_POS + 0x1700,color[0]);
	}
