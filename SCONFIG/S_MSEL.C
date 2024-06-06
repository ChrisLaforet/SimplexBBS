/* s_msel.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 16 August 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_msel.c_v  $
**                       $Date:   25 Oct 1992 14:15:32  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#if defined(PROTECTED)
	#define INCL_DOSFILEMGR
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include <screen.h>
#include <window.h>
#include <editor.h>
#include <keys.h>
#include "sconfig.h"



#define PAGELEN		20


char **menus = NULL;
int max_menus = 0;
int cur_menus = 0;



int fname_sort(char **arg1,char **arg2)
	{
	return stricmp((*arg1),(*arg2));
	}



void draw_menu_select(void)
	{
	_vcw('Ã',color[0],0x1600,1);
	_vcw('´',color[0],0x164f,1);

	_vcw('Ä',color[0],0x1601,78);
		  
	prntnomov(0x1702,13,color[1],"[E] Edit menu");
	prntnomov(0x1711,12,color[1],"[A] Add menu");
	prntnomov(0x171f,13,color[1],"[C] Copy menu");
	prntnomov(0x172e,15,color[1],"[D] Delete menu");
	prntnomov(0x173f,15,color[1],"[R] Rename menu");
	}



void edit_menufile(void)
	{
	struct window *wndw;
	struct window *wndw1;
	struct prompt tprompt;
	char buffer[100];
	char buffer1[100];
	char newname[15];
	char *cptr;
	int rtn;
	int count;
	int quit = 0;
	int out;
	int current = 0;
	int hilite = ON_WHITE;
	int key;
	int okflag;
	int pos;
	int len;
#ifdef PROTECTED
	HDIR hdir;
	USHORT search;
	FILEFINDBUF find;
#else
	struct find_t find;
#endif
	FILE *sfd;
	FILE *dfd;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," Menu Editor Selection Screen ");

		draw_menu_select();
		strcpy(buffer,cfg.cfg_menupath);
		if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
			strcat(buffer,P_SSEP);
		strcat(buffer,"*.mnu");


#ifdef PROTECTED
		search = 1;
		hdir = HDIR_SYSTEM;
		rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
		if (rtn == 3)		/* (ERROR_PATH_NOT_FOUND) dos invalid path error return */
			{
			error(NOTICE,"The specified menu path is invalid, please correct it!",1,CLOCK_POS,color[0]);
			return;
			}

		while (!rtn)
			{
			if (cur_menus >= max_menus)
				{
				if (!(menus = realloc(menus,(max_menus += 20) * sizeof(char *))))
	 				error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
				}
			if (!(menus[cur_menus] = calloc(13,sizeof(char))))
 				error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
			strcpy(menus[cur_menus],find.achName);
			++cur_menus;
			search = 1;
			rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
			}
#else
		rtn = _dos_findfirst(buffer,_A_NORMAL | _A_ARCH,&find);
		if (rtn == 3)		/* dos invalid path error return */
			{
			error(NOTICE,"The specified menu path is invalid, please correct it!",1,CLOCK_POS,color[0]);
			return;
			}

		while (!rtn)
			{
			if (cur_menus >= max_menus)
				{
				if (!(menus = realloc(menus,(max_menus += 20) * sizeof(char *))))
	 				error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
				}
			if (!(menus[cur_menus] = calloc(13,sizeof(char))))
 				error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
			strcpy(menus[cur_menus],find.name);
			++cur_menus;
			rtn = _dos_findnext(&find);
			}
#endif

		qsort(menus,cur_menus,sizeof(char *),fname_sort);
	
		do
			{
			clrblk(0x201,0x154e,color[0]);
			for (count = 0; count < cur_menus && count < 140; count++)
				{
				pos = ((count / PAGELEN) * 11) + 3;
				pos |= (((count % PAGELEN) + 2) << 8);
				cptr = menus[count];
				while (*cptr && *cptr != '.')		/* this way we skip the extension */
					{
					_vcw(*cptr++,color[0],pos,1);
					++pos;
					}
				}

			out = 0;
			do
				{
				set_cursor_type(HIDDEN);
				if (cur_menus)
					{
					pos = ((current / PAGELEN) * 11) + 2;
					pos |= (((current % PAGELEN) + 2) << 8);
					phantom(pos,10,hilite);
					}
				key = read_keyboard(0,0,0);
				switch (key)
					{
					case UpArrow:
						if (cur_menus)
							{
							phantom(pos,10,color[0]);
							if (current)
								--current;
							else
								current = (cur_menus - 1);
							}
						break;
					case DownArrow:
						if (cur_menus)
							{
							phantom(pos,10,color[0]);
							if (current == (cur_menus - 1))
								current = 0;
							else
								++current;
							}
						break;
					case LeftArrow:
						if (cur_menus && cur_menus > PAGELEN)
							{
							phantom(pos,10,color[0]);
							if (current >= PAGELEN)
								current -= PAGELEN;
							else
								{
								current = current + ((cur_menus / PAGELEN) * PAGELEN);
								if (current >= cur_menus)
									current -= PAGELEN;
								}
							}
						break;
					case RightArrow:
						if (cur_menus && cur_menus > PAGELEN)
							{
							phantom(pos,10,color[0]);
							if (current < (cur_menus - PAGELEN))
								current += PAGELEN;
							else
								current = current % PAGELEN;
							}
						break;
					case Home:
						if (cur_menus)
							{
							phantom(pos,10,color[0]);
							current = 0;
							}
						break;
					case End:
						if (cur_menus)
							{
							phantom(pos,10,color[0]);
							current = cur_menus - 1;
							}
						break;
					case 'E':
					case 'e':
					case CR:
						if (cur_menus)
							{
							strcpy(buffer,cfg.cfg_menupath);
							if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
								strcat(buffer,P_SSEP);
							strcat(buffer,menus[current]);
							if (sfd = fopen(buffer,"r+b"))
								{
								load_menu(sfd,menus[current]);
								fclose(sfd);
								}
							else
								{
								sprintf(buffer,"Unable to open \"%s\"...aborting edit!",menus[current]);
								error(NOTICE,buffer,1,CLOCK_POS,color[0]);
								}
							}
						break;
					case 'D':
					case 'd':
						if (cur_menus)
							{
							sprintf(buffer,"Are you sure that you want to delete \"%s\"? (Y/N) ",menus[current]);
							if (askyn("Delete Menu File?",buffer,color[4],color[3],1,CLOCK_POS,color[0]))
								{
								strcpy(buffer,cfg.cfg_menupath);
								if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
									strcat(buffer,P_SSEP);
								strcat(buffer,menus[current]);
								if (unlink(buffer))
									error(NOTICE,"Unable to delete file!",1,CLOCK_POS,color[0]);
								else
									{
									free(menus[current]);
									if (current < (cur_menus - 1))
										memmove(menus + current,menus + (current + 1),(cur_menus - current - 1) * sizeof(char *));
									--cur_menus;
									if (current >= cur_menus)
										current = 0;
									out = 1;
									}
								}
							}
						break;
					case 'R':
					case 'r':
						if (cur_menus)
							{
							if (wndw1 = open_shadow_window(0xa01,0xf4e,color[3],1))
								{
								prntnomov(0xa02,40,color[4]," Rename Menu File? ");
								prntnomovf(0xc03,70,color[3],"Please enter the name to which to rename \"%s\".",menus[current]);
								prntnomov(0xd03,40,color[3],"Rename to:");
								_vcw('_',color[0],0xd0e,8);
								newname[0] = '\0';
								rtn = edit_field(0xd0e,color[1],color[0],1,CLOCK_POS,color[0],"F08",newname,get_filename);
								if (rtn == E_EXIT && newname[0])
									{
									strcat(newname,".MNU");
									if (stricmp(newname,menus[current]))
										{
										strcpy(buffer,cfg.cfg_menupath);
										if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
											strcat(buffer,P_SSEP);
										strcpy(buffer1,buffer);
										strcat(buffer,menus[current]);
										strcat(buffer1,newname);
										if (rename(buffer,buffer1))
											{
											sprintf(buffer,"Unable to rename \"%s\" to \"%s\"!",menus[current],newname);
											error(NOTICE,buffer,1,CLOCK_POS,color[0]);
											}
										else
											{
											strcpy(menus[current],newname);
											qsort(menus,cur_menus,sizeof(char *),fname_sort);
											for (count = 0; count < cur_menus; count++)
												{
												if (!stricmp(menus[count],newname))
													{
													current = count;
													break;
													}
												}
											out = 1;
											}
										}
									}
								close_shadow_window(wndw1);
								}
							else
								error(NON_FATAL,"Unable to open window",1,CLOCK_POS,color[0]);
							}
						break;
					case 'C':
					case 'c':
						if (cur_menus)
							{
							if (wndw1 = open_shadow_window(0xa01,0xf4e,color[3],1))
								{
								prntnomov(0xa02,40,color[4]," Copy Menu File? ");
								prntnomovf(0xc03,70,color[3],"Please enter the name to which to copy \"%s\".",menus[current]);
								prntnomov(0xd03,40,color[3],"Copy to:");
								_vcw('_',color[0],0xd0c,8);
								newname[0] = '\0';
								rtn = edit_field(0xd0c,color[1],color[0],1,CLOCK_POS,color[0],"F08",newname,get_filename);
								if (rtn == E_EXIT && newname[0])
									{
									strcat(newname,".MNU");
									okflag = 1;
									for (count = 0; count < cur_menus; count++)
										{
										if (!stricmp(newname,menus[count]))
											{
											sprintf(buffer,"The menu file \"%s\" already exists...aborting copy!",newname);
											error(NOTICE,buffer,1,CLOCK_POS,color[0]);
											okflag = 0;
											break;
											}
										}
									if (okflag)
										{
										strcpy(buffer,cfg.cfg_menupath);
										if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
											strcat(buffer,P_SSEP);
										strcpy(buffer1,buffer);
										strcat(buffer,menus[current]);
										strcat(buffer1,newname);
										if (sfd = fopen(buffer,"rb"))
											{
											if (dfd = fopen(buffer1,"wb"))
												{
												while (len = read(fileno(sfd),buffer,sizeof(buffer)))
													write(fileno(dfd),buffer,len);
												fclose(dfd);

												if (cur_menus >= max_menus)
													{
													if (!(menus = realloc(menus,(max_menus += 20) * sizeof(char *))))
	 													error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
													}
												if (!(menus[cur_menus] = calloc(13,sizeof(char))))
 													error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
												strcpy(menus[cur_menus],newname);
												++cur_menus;
												qsort(menus,cur_menus,sizeof(char *),fname_sort);
												for (count = 0; count < cur_menus; count++)
													{
													if (!stricmp(menus[count],newname))
														{
														current = count;
														break;
														}
													}
												out = 1;
												}
											else
												error(NOTICE,"Unable to create destination file...aborting copy!",1,CLOCK_POS,color[0]);
											fclose(sfd);
											}
										else
											error(NOTICE,"Unable to open source file...aborting copy!",1,CLOCK_POS,color[0]);
										}
									}
								close_shadow_window(wndw1);
								}
							else
								error(NON_FATAL,"Unable to open window",1,CLOCK_POS,color[0]);
							}
						break;
					case 'A':
					case 'a':
						if (wndw1 = open_shadow_window(0xa01,0xe4e,color[3],1))
							{
							prntnomov(0xa02,40,color[4]," Add a New Menu File? ");
							prntnomov(0xc03,40,color[3],"Enter the new menu name:");
							_vcw('_',color[0],0xc1c,8);
							newname[0] = '\0';
							rtn = edit_field(0xc1c,color[1],color[0],1,CLOCK_POS,color[0],"F08",newname,get_filename);
							if (rtn == E_EXIT && newname[0])
								{
								strcat(newname,".MNU");
								okflag = 1;
								for (count = 0; count < cur_menus; count++)
									{
									if (!stricmp(newname,menus[count]))
										{
										sprintf(buffer,"The menu file \"%s\" already exists...aborting addition!",newname);
										error(NOTICE,buffer,1,CLOCK_POS,color[0]);
										okflag = 0;
										break;
										}
									}
								if (okflag)
									{
									strcpy(buffer,cfg.cfg_menupath);
									if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
										strcat(buffer,P_SSEP);
									strcat(buffer,newname);
									if (sfd = fopen(buffer,"wb"))
										{
										memset(&tprompt,0,sizeof(struct prompt));
										tprompt.prompt_sig = (int)M_SIGNATURE;
										tprompt.prompt_color = 7;
										tprompt.prompt_hilite = 15;
										strcpy(tprompt.prompt_string,"What is your choice? ");
										fwrite(&tprompt,1,sizeof(struct prompt),sfd);
										if (cur_menus >= max_menus)
											{
											if (!(menus = realloc(menus,(max_menus += 20) * sizeof(char *))))
 												error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
											}
										if (!(menus[cur_menus] = calloc(13,sizeof(char))))
											error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
										strcpy(menus[cur_menus],newname);
										++cur_menus;
										qsort(menus,cur_menus,sizeof(char *),fname_sort);
										for (count = 0; count < cur_menus; count++)
											{
											if (!stricmp(menus[count],newname))
												{
												current = count;
												break;
												}
											}
										out = 1;
										fclose(sfd);
										}
									else
										error(NOTICE,"Unable to open source file...aborting copy!",1,CLOCK_POS,color[0]);
									}
								}
							close_shadow_window(wndw1);
							}
						else
							error(NON_FATAL,"Unable to open window",1,CLOCK_POS,color[0]);
						break;
					case ESC:
						out = 1;
						quit = 1;
						break;
					}
				}
			while (!out);
			set_cursor_type(LINE);
			}
		while (!quit);

		if (max_menus)
			{
			for (count = 0; count < cur_menus; count++)
				free(menus[count]);
			free(menus);
			menus = NULL;
			max_menus = cur_menus = 0;
			}
		close_window(wndw);
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS + 0x1700,color[0]);
	}


