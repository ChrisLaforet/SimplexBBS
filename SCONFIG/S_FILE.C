/* s_file.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_file.c_v  $
**                       $Date:   25 Oct 1992 14:15:22  $
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



#define PAGE_LEN			19
#define CURLINE				(current - top)



extern struct choice yn[2];
static char search_text[21] = "";




void draw_farea(void)
	{
	drawbox(0x507,0x1247,color[0],0);
	prntnomov(0x709,18,color[0],"File board number:");
	_vcw('_',color[3],0x71d,4);
	prntnomov(0x809,16,color[0],"File board name:");
	_vcw('_',color[3],0x81d,40);
	prntnomov(0x909,18,color[0],"Access priv level:");
	_vcw('_',color[3],0x91d,3);
	prntnomov(0x927,20,color[0],"Access flags:");
	_vcw(' ',color[3],0x935,16);
	prntnomov(0xb09,16,color[0],"File board path:");
	_vcw('_',color[3],0xc09,60);
	prntnomov(0xd09,60,color[0],"Location of \"filelist.bbs\" ONLY if different from above:");
	_vcw('_',color[3],0xe09,60);
	prntnomov(0x1009,19,color[0],"File board deleted?");
	_vcw('_',color[3],0x101d,3);
	prntnomov(0x1402,80,color[0],"For systems that run CD-ROM units that do not have a correctly formatted");
	prntnomov(0x1502,80,color[0],"filelist.bbs description file, you can build one and store it in a different");
	prntnomov(0x1602,80,color[0],"directory. In this case, you can give an alternate path to the filelist.bbs.");
	}



void fill_farea(struct file *tfile)
	{
	if (tfile->file_number)
		prntnomovf(0x71d,4,color[3],"%u",tfile->file_number);
	prntnomov(0x81d,40,color[3],tfile->file_areaname);
	prntnomovf(0x91d,3,color[3],"%u",(int)(unsigned char)tfile->file_priv);
	fill_flags(0x935,tfile->file_flags,color[3]);
	prntnomov(0xc09,60,color[3],tfile->file_pathname);
	prntnomov(0xe09,60,color[3],tfile->file_descname);
	prntnomovf(0x101d,3,color[3],"%s",(char *)(tfile->file_deleted ? "Yes" : "No_"));
	}



int edit_farea(struct file *tfile,int record)
	{
	struct window *wndw;
	char buffer[81];
	int current = 0;
	int change = 0;
	int count;
	int rtn;
	int tint;
	int redo;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomovf(0x1,40,color[1]," Edit File Board Entry #%u%s ",record + 1,(char *)(record < cur_files ? "" : " [New]"));
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		draw_farea();
		fill_farea(tfile);
		do
			{
			switch (current)
				{
				case 0:
					do
						{
						redo = 0;
						if (tfile->file_number)
							sprintf(buffer,"%u",tfile->file_number);
						else
							{
							buffer[0] = '\0';
							_vcw('_',color[3],0x71d,4);
							}
						rtn = edit_number(0x71d,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tint = atoi(buffer);
							if (tint)
								{
								for (count = 0; count < cur_files; count++)
									{
									if (files[count]->file_number == tint && count != record)
										{
										sprintf(buffer,"There is already another file area with number %u!  Choose another.",tint);
										error(NOTICE,buffer,1,CLOCK_POS,color[0]);
										redo = 1;
										break;
										}
									}
								}
							else
								{
								error(NOTICE,"File area 0 is not a valid file area choice!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								{
								tfile->file_number = tint;
								if (!tfile->file_number)
									_vcw('_',color[3],0x71d,4);
								}
							}
						}
					while (redo);
					break;
				case 1:
					rtn = edit_field(0x81d,color[4],color[3],1,CLOCK_POS,color[0],"FPC40",tfile->file_areaname,NULL);
					break;
				case 2:
					sprintf(buffer,"%u",(int)(unsigned char)tfile->file_priv);
					rtn = edit_number(0x91d,color[4],color[3],buffer,1,255,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tint = atoi(buffer);
						tfile->file_priv = (char)(unsigned char)tint;
						}
					break;
				case 3:
					rtn = edit_flags(0x935,&tfile->file_flags,color[4],color[3],1,CLOCK_POS,color[0]);
					break;
				case 4:
					rtn = edit_field(0xc09,color[4],color[3],1,CLOCK_POS,color[0],"F060",tfile->file_pathname,get_pathname);
					break;
				case 5:
					rtn = edit_field(0xe09,color[4],color[3],1,CLOCK_POS,color[0],"F060",tfile->file_descname,get_pathname);
					break;
				case 6:
					rtn = pop_menu(0xe20,color[4],color[3],"Delete Area",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							{
							if (!tfile->file_deleted)
								{
								tfile->file_deleted = 1;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						else
							{
							if (tfile->file_deleted)
								{
								tfile->file_deleted = 0;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						prntnomovf(0x101d,3,color[3],"%s",(char *)(tfile->file_deleted ? "Yes" : "No_"));
						}
					break;
				}
			if (rtn == E_EXIT)
				change = 1;
			if (rtn == E_FORE || rtn == E_EXIT)
				{
				if (current < 6)
					++current;
				else
					current = 0;
				}
			if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 6;
				}
			if (rtn == E_END && !change)
				rtn = E_FORE;
			}
		while (rtn != E_END && rtn != E_QUIT);

		close_window(wndw);

		if (change)
			master_change = 1;
		}
	else
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);

	return change;
	}



void edit_file_boards(void)
	{
	struct window *wndw;
	struct file tfile;
	int current = 0;
	int inverse;
	int redraw = 1;
	int count;
	int top = 0;
	int key;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," Select File Area for Editing ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");
		draw_file_list();

		search_text[0] = '\0';

		set_cursor_type(HIDDEN);
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
					if ((top + count) > cur_files)
						break;
					else if ((top + count) == cur_files)
						{
						prntnomovf(0x402 + (count << 8),4,color[0],"%4u",top + count + 1);
						prntnomov(0x40f + (count << 8),40,color[0],"< Add a new file area >");
						break;
						}
					else
						{
						prntnomovf(0x402 + (count << 8),4,color[0],"%4u",top + count + 1);
						prntnomovf(0x408 + (count << 8),4,color[0],"%4u",files[top + count]->file_number);
						prntnomov(0x40f + (count << 8),40,color[0],files[top + count]->file_areaname);
						prntnomovf(0x439 + (count << 8),40,color[0],"%3u",files[top + count]->file_priv);
						fill_flags(0x43e + (count << 8),files[top + count]->file_flags,color[0]);
						}
					}
				redraw = 0;
				}

			if (cur_files)
				phantom(0x402 + (CURLINE << 8),76,inverse);

			register_help("item select");
			key = read_keyboard(1,CLOCK_POS,color[0]);
			deregister_help();

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
					if (current < cur_files)
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
					if (current != cur_files)
						{
						phantom(0x402 + (CURLINE << 8),76,color[0]);
						current = cur_files;
						if (current >= (top + PAGE_LEN))
							{
							top = cur_files - (PAGE_LEN - 1);
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
					if (current < cur_files)
						{
						phantom(0x402 + (CURLINE << 8),76,color[0]);
						if ((current + PAGE_LEN) <= cur_files)
							{
							current += PAGE_LEN;
							top += PAGE_LEN;
							redraw = 1;
							}
						else if ((top + PAGE_LEN) <= cur_files)
							{
							current = cur_files;
							top += PAGE_LEN;
							redraw = 1;
							}
						else
							current = cur_files;
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
					if (current < cur_files)
						memcpy(&tfile,files[current],sizeof(struct file));
					else
						{
						memset(&tfile,0,sizeof(struct file));
						tfile.file_priv = cfg.cfg_newpriv;
						tfile.file_flags = cfg.cfg_newflags;
						}
					if (edit_farea(&tfile,current))
						{
						if (current == cur_files)
							{
							if (cur_files >= max_files)
								{
								if (!(files = realloc(files,(max_files += 10) * sizeof(struct file *))))
									error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
								}
							if (!(files[cur_files] = calloc(1,sizeof(struct file))))
	 							error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
							memcpy(files[cur_files],&tfile,sizeof(struct file));
							++cur_files;
							}
						else
							memcpy(files[current],&tfile,sizeof(struct file));

						qsort(files,cur_files,sizeof(struct file *),file_comp);

						redraw = 1;
						}

					break;
            	case ESC:
					break;
				}
			}
		while (key != ESC);

		close_window(wndw);
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS + 0x1700,color[0]);
	}


