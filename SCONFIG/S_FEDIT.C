/* s_fedit.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 1 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_fedit.c_v  $
**                       $Date:   25 Oct 1992 14:15:36  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>
#include <screen.h>
#include <window.h>
#include <editor.h>
#include <keys.h>
#include "sconfig.h"


#define PAGE_LEN			5
#define CURLINE				(current - top)


static struct fe tfes[PAGE_LEN];
static char search_text[21] = "";


void draw_fedit(void)
	{
	prntnomov(0x203,10,color[0],"File Area:");
	_vcw('_',color[3],0x20e,4);
	_vcw('_',color[3],0x213,40);
	prntnomov(0x23e,6,color[0],"Files:");
	_vcw('_',color[3],0x245,4);

	_vcw('Ã',color[0],0x400,1);
	_vcw('Ä',color[0],0x401,78);
	_vcw('´',color[0],0x44f,1);
	_vcw('#',color[0],0x502,3);
	prntnomov(0x507,8,color[0],"Filename");
	prntnomov(0x51c,7,color[0],"UL Date");
	prntnomov(0x525,11,color[0],"Uploaded By");
	prntnomov(0x607,19,color[0],"Access Priv & Flags");
	prntnomov(0x61c,7,color[0],"DL Date");
	prntnomov(0x625,4,color[0],"#DLs");
	prntnomov(0x62b,16,color[0],"Location of File");
	prntnomov(0x707,16,color[0],"File Description");
	_vcw('Ã',color[0],0x800,1);
	_vcw('Ä',color[0],0x801,78);
	_vcw('´',color[0],0x84f,1);
	}



void blank_fedit_entry(int number,int top,int color)
	{
	int base = (((number - top) * 3) + 0x9) << 8;

	_vcw('_',color,base + 0x2,4);
	_vcw('_',color,base + 0x7,16);
	prntnomov(base + 0x1c,8,color,"__/__/__");
	_vcw('_',color,base + 0x25,40);
	prntnomov(base + 0x107,20,color,"___");
	prntnomov(base + 0x10b,20,color,"abcdefghijklmnop");
	prntnomov(base + 0x11c,8,color,"__/__/__");
	_vcw('_',color,base + 0x125,5);
	_vcw('_',color,base + 0x12b,4);
	_vcw('_',color,base + 0x131,28);
	_vcw('_',color,base + 0x207,70);
	}



void fill_fedit_entry(struct fe *tfe,struct file *curfile,int number,int top,int color)
	{
	struct file *tfile = NULL;
	int count;
	int base = (((number - top) * 3) + 0x9) << 8;

	prntnomovf(base + 0x2,4,color,"%u",number + 1);
	prntnomov(base + 0x7,16,color,tfe->fe_name);
	fill_date(base + 0x1c,tfe->fe_uldate,color);
	prntnomov(base + 0x25,40,color,tfe->fe_uploader);
	prntnomovf(base + 0x107,3,color,"%3u",tfe->fe_priv);
	fill_flags(base + 0x10b,tfe->fe_flags,color);
	fill_date(base + 0x11c,tfe->fe_dldate,color);
	prntnomovf(base + 0x125,5,color,"%u",tfe->fe_dl);

	prntnomovf(base + 0x12b,4,color,"%u",tfe->fe_location);

	if (curfile->file_number == tfe->fe_location)
		tfile = curfile;
	else
		{
		for (count = 0; count < cur_files; count++)
			{
			if (files[count]->file_number == tfe->fe_location)
				{
				tfile = files[count];
				break;
				}
			}
		}

	if (tfile)
		prntnomov(base + 0x131,28,color,tfile->file_areaname);
	else 
		prntnomov(base + 0x131,28,color,"<Unknown File Area>");

	prntnomov(base + 0x207,70,color,tfe->fe_descrip);
	}



void unphantom(int base)
	{
	phantom(base + 0x2,4,color[3]);
	_vcw(' ',color[0],base + 0x6,1);
	phantom(base + 0x7,16,color[3]);
	_vcw(' ',color[0],base + 0x17,5);
	phantom(base + 0x1c,8,color[3]);
	_vcw(' ',color[0],base + 0x24,1);
	phantom(base + 0x25,40,color[3]);
	_vcw(' ',color[0],base + 0x4d,1);

	_vcw(' ',color[0],base + 0x102,5);
	phantom(base + 0x107,3,color[3]);
	_vcw(' ',color[0],base + 0x10a,1);
	phantom(base + 0x10b,16,color[3]);
	_vcw(' ',color[0],base + 0x11b,1);
	phantom(base + 0x11c,8,color[3]);
	_vcw(' ',color[0],base + 0x124,1);
	phantom(base + 0x125,5,color[3]);
	_vcw(' ',color[0],base + 0x12a,1);
	phantom(base + 0x12b,4,color[3]);
	_vcw(' ',color[0],base + 0x12f,2);
	phantom(base + 0x131,28,color[3]);
	_vcw(' ',color[0],base + 0x14d,1);

	_vcw(' ',color[0],base + 0x202,5);
	phantom(base + 0x207,70,color[3]);
	_vcw(' ',color[0],base + 0x24d,1);
	}




int edit_flist_entry(struct fe *tfe,int number)
	{
	struct window *wndw;
	struct file *tfile = NULL;
	char buffer[10];
	int current = 0;
	int change = 0;
	int count;
	int tint;
	int rtn;

	if (wndw = open_shadow_window(0x501,0x114e,color[3],1))
		{
		prntnomovf(0x502,40,color[4]," Edit File Entry #%u ",number + 1);
		prntnomov(0x1102,19,color[3]," Press F1 for Help ");
		prntnomov(0x703,9,color[3],"Filename:");
		_vcw('_',color[0],0x70d,16);
		prntnomov(0x803,9,color[3],"View Prv:");
		_vcw('_',color[0],0x80d,3);
		prntnomov(0x814,20,color[3],"View Flags:");
		prntnomov(0x903,9,color[3],"U/L Date:");
		prntnomov(0x90d,8,color[0],"__/__/__");
		prntnomov(0x918,7,color[3],"U/L By:");
		_vcw('_',color[0],0x920,40);
		prntnomov(0xa03,9,color[3],"D/L Date:");
		prntnomov(0xa0d,8,color[0],"__/__/__");
		prntnomov(0xa1a,5,color[3],"D/Ls:");
		_vcw('_',color[0],0xa20,5);
		prntnomov(0xb03,9,color[3],"Filearea:");
		_vcw('_',color[0],0xb0d,4);
		_vcw('_',color[0],0xb13,40);
		prntnomov(0xc04,8,color[3],"Descrip:");
		_vcw('_',color[0],0xc0d,64);
		_vcw('_',color[0],0xd0d,64);
		_vcw('_',color[0],0xe0d,64);
		_vcw('_',color[0],0xf0d,64);

		prntnomov(0x70d,16,color[0],tfe->fe_name);
		prntnomovf(0x80d,3,color[0],"%u",tfe->fe_priv);
		fill_flags(0x820,tfe->fe_flags,color[0]);
		fill_date(0x90d,tfe->fe_uldate,color[0]);
		prntnomov(0x920,40,color[0],tfe->fe_uploader);
		fill_date(0xa0d,tfe->fe_dldate,color[0]);
		prntnomovf(0xa20,5,color[0],"%u",tfe->fe_dl);
		box_fill(0xc0d,tfe->fe_descrip,64,4,color[0]);

		for (count = 0; count < cur_files; count++)
			{
			if (files[count]->file_number == tfe->fe_location)
				{
				tfile = files[count];
				prntnomovf(0xb0d,4,color[0],"%u",tfile->file_number);
				prntnomov(0xb13,40,color[0],tfile->file_areaname);
				break;
				}
			}

		if (!tfile)
			{
			prntnomovf(0xb0d,4,color[0],"%u",tfe->fe_location);
			prntnomov(0xb13,40,color[0],"<Unknown File Area>");
			}

		do
			{
			switch (current)
				{
				case 0:
				   	rtn = edit_field(0x70d,color[1],color[0],1,CLOCK_POS,color[0],"F016",tfe->fe_name,get_fileext);
					break;
				case 1:
					sprintf(buffer,"%u",tfe->fe_priv);
					rtn = edit_number(0x80d,color[1],color[0],buffer,0,255,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tint = atoi(buffer);
						tfe->fe_priv = (unsigned int)tint;
						}
					break;
				case 2:
					rtn = edit_flags(0x820,&tfe->fe_flags,color[1],color[0],1,CLOCK_POS,color[0]);
					break;
				case 3:
					strcpy(buffer,datetoa(tfe->fe_uldate));
					rtn = edit_date(0x90d,buffer,color[1],color[0],1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						tfe->fe_uldate = atodate(buffer);
					break;
				case 4:
				   	rtn = edit_field(0x920,color[1],color[0],1,CLOCK_POS,color[0],"FPC40",tfe->fe_uploader,NULL);
					break;
				case 5:
					strcpy(buffer,datetoa(tfe->fe_dldate));
					rtn = edit_date(0xa0d,buffer,color[1],color[0],1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						tfe->fe_dldate = atodate(buffer);
					break;
				case 6:
					if (tfe->fe_dl)
						sprintf(buffer,"%u",tfe->fe_dl);
					else
						{
						buffer[0] = (char)'\0';
						_vcw('_',color[0],0xa20,5);
						}
					rtn = edit_number(0xa20,color[1],color[0],buffer,0,32000,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						tfe->fe_dl = atoi(buffer);
					break;
				case 7:
					sprintf(buffer,"%u",(int)(unsigned char)tfe->fe_location);
					rtn = edit_number(0xb0d,color[1],color[0],buffer,1,9999,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tfe->fe_location = atoi(buffer);
						_vcw('_',color[0],0xb13,40);

						for (count = 0; count < cur_files; count++)
							{
							if (files[count]->file_number == tfe->fe_location)
								{
								tfile = files[count];
								prntnomov(0xb13,40,color[0],tfile->file_areaname);
								break;
								}
							}

						if (!tfile)
							prntnomov(0xb13,40,color[0],"<Unknown File Area>");
						}
					break;
				case 8:
					rtn = box_edit(0xc0d,tfe->fe_descrip,64,4,color[1],color[0],1,CLOCK_POS,color[0]);
					break;
				}

			if (rtn == E_EXIT)
				change = 1;
			if (rtn == E_FORE || rtn == E_EXIT)
				{
				if (current < 8)
					++current;
				else
					current = 0;
				}
			if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 8;
				}
			}
		while (rtn != E_END && rtn != E_QUIT);

		if (rtn == E_QUIT)
			change = 0;

		close_shadow_window(wndw);
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);

	return change;
	}




int find_fedit_list(FILE *fd)
	{
	struct window *wndw;
	struct fe tfe;
	char buffer[81];
	int found = 0;
	int rtn;
	int retval = -1;

	if (wndw = open_shadow_window(0xa11,0xe3e,color[3],1))
		{
		prntnomov(0xa12,40,color[4]," Search for a Filename or Description ");
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
			wndw = open_message("Please wait.  Searching for pattern among file names and descriptions.",color[3]);

			retval = 0;
			fseek(fd,0L,SEEK_SET);
			while (fread(&tfe,sizeof(struct fe),1,fd))
				{
				strupr(tfe.fe_name);
				strupr(tfe.fe_descrip);
				if (strstr(tfe.fe_name,search_text) || strstr(tfe.fe_descrip,search_text))
					{
					found = 1;
					break;
					}
				++retval;
				}

			if (wndw)
				close_message(wndw);
			if (!found)
				{
				sprintf(buffer,"Nothing with the pattern \"%s\" was found!",search_text);
				error(NOTICE,buffer,1,CLOCK_POS,color[0]);
				retval = -1;
				}
			}
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);
	return retval;
	}



int next_fedit_list(int start,FILE *fd)
	{
	struct window *wndw;
	struct fe tfe;
	char buffer[81];
	int found = 0;
	int retval = -1;

	if (search_text[0])
		{
		wndw = open_message("Please wait.  Searching for pattern among file names and descriptions.",color[3]);

		retval = start + 1;
		fseek(fd,(long)(retval) * (long)sizeof(struct fe),SEEK_SET);
		while (fread(&tfe,sizeof(struct fe),1,fd))
			{
			strupr(tfe.fe_name);
			strupr(tfe.fe_descrip);
			if (strstr(tfe.fe_name,search_text) || strstr(tfe.fe_descrip,search_text))
				{
				found = 1;
				break;
				}
			++retval;
			}
		if (wndw)
			close_message(wndw);
		if (!found)
			{
			sprintf(buffer,"Nothing with the pattern \"%s\" was found!",search_text);
			error(NOTICE,buffer,1,CLOCK_POS,color[0]);
			retval = -1;
			}
		}
	else
		error(NOTICE,"No previous pattern established for searching!",1,CLOCK_POS,color[0]);
	return retval;
	}



void fedit_list(struct file *tfile)
	{
	struct window *wndw;
	struct window *wndw1;
	struct fe tfe;
	char buffer[100];
	int current = 0;
	int inverse;
	int offset;
	int redraw = 0;
	int base;
	int top = 0;
	int count;
	int recs;
	int key;
	FILE *fd;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," Edit File Area Listing ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		draw_fedit();

		prntnomovf(0x20e,4,color[3],"%u",tfile->file_number);
		prntnomov(0x213,40,color[3],tfile->file_areaname);

		if ((color[3] & ON_WHITE) != ON_WHITE)
			inverse = ON_WHITE;
		else
			inverse = color[3];

		if (tfile->file_descname[0])
			strcpy(buffer,tfile->file_descname);
		else 
			strcpy(buffer,tfile->file_pathname);
		if (buffer[0])
			{
			if (buffer[strlen(buffer) - 1] != P_CSEP)
				strcat(buffer,P_SSEP);

			strcat(buffer,"filelist.bbs");

			if (!(fd = fopen(buffer,"r+b")))
				fd = fopen(buffer,"w+b");
			if (fd)
				{
				recs = (int)(filelength(fileno(fd)) / (long)sizeof(struct fe));

				prntnomovf(0x245,4,color[3],"%u",recs);

				do
					{
					redraw = 0;

					memset(&tfes,0,sizeof(struct fe) * PAGE_LEN);

					clrblk(0x902,0x174d,color[0]);

					fseek(fd,(long)top * (long)sizeof(struct fe),SEEK_SET);
					for (count = 0; count < PAGE_LEN; count++)
						{
						blank_fedit_entry(top + count,top,color[3]);
						if (!fread(&tfes[count],sizeof(struct fe),1,fd))
							break;
						fill_fedit_entry(&tfes[count],tfile,top + count,top,color[3]);
						}

					do
						{
						base = ((CURLINE * 3) + 0x9) << 8;
						phantom(base | 0x2,76,inverse);
						phantom(base + 0x102,76,inverse);
						phantom(base + 0x202,76,inverse);

						register_help("select edit");
						key = read_keyboard(1,CLOCK_POS,color[0]);
						deregister_help();

						switch (key)
							{
							case UpArrow:
								if (current)
									{
									unphantom(base);
									--current;
									if (current < top)
										{
										if (top >= 3)
											top -= 3;
										else
											top = 0;
										redraw = 1;
										}
									/* otherwise merely move the phantom bar */
									}
								break;
							case DownArrow:
								if (current < recs)
									{
									unphantom(base);
									++current;
									if (current >= (top + PAGE_LEN))
										{
										top += 3;
										redraw = 1;
										}
									/* otherwise merely move the phantom bar */
									}
								break;
            				case Home:
								if (current)
									{
									unphantom(base);
									current = 0;
									if (top)
										{
										top = 0;
										redraw = 1;
										}
									}
								break;
							case End:
								if (current != recs)
									{
									unphantom(base);
									current = recs;
									if (current >= (top + PAGE_LEN))
										{
										top = (recs - PAGE_LEN) + 1;
										redraw = 1;
										}
									}
								break;
            				case PgUp:
								if (current)
									{
									unphantom(base);
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
								if (current < recs)
									{
									unphantom(base);
									if ((current + PAGE_LEN) <= recs)
										{
										current += PAGE_LEN;
										top += PAGE_LEN;
										redraw = 1;
										}
									else if ((top + PAGE_LEN) <= recs)
										{
										current = recs;
										top += PAGE_LEN;
										redraw = 1;
										}
									else
										current = recs;
									}
								break;
							case F5:
								if (((offset = find_fedit_list(fd)) != -1) && offset != current)
									{
									unphantom(base);
									if (offset < top || offset >= (top + PAGE_LEN))
										{
										top = offset;
										redraw = 1;
										}
									current = offset;
									}
								break;
							case F6:
								if (((offset = next_fedit_list(current,fd)) != -1) && offset != current)
									{
									unphantom(base);
									if (offset < top || offset >= (top + PAGE_LEN))
										{
										top = offset;
										redraw = 1;
										}
									current = offset;
									}
								break;
							case Del:
								if (recs && current < recs)
									{
									sprintf(buffer,"Do you REALLY want to delete record #%u (%s)? ",current + 1,tfes[CURLINE].fe_name);
									if (askyn("Delete this File Record?",buffer,color[4],color[3],1,CLOCK_POS,color[0]))
										{
										sprintf(buffer,"Do you want to delete the file \"%s\" also? ",tfes[CURLINE].fe_name);
										if (askyn("Delete the Corresponding File?",buffer,color[4],color[3],1,CLOCK_POS,color[0]))
											{
											strcpy(buffer,tfile->file_pathname);
											if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
												strcat(buffer,P_SSEP);
											strcat(buffer,tfes[CURLINE].fe_name);

											if (unlink(buffer))
												{
												sprintf(buffer,"Error while deleting file \"%s\"!",tfes[CURLINE].fe_name);
												error(NON_FATAL,buffer,1,CLOCK_POS,color[0]);
												}
											}
										wndw1 = open_message("Please wait.  Deleting entry and consolidating file listing records.",color[3]);
										if (current < (recs - 1))
											{
											for (count = (current + 1); count < recs; count++)
												{
												fseek(fd,(long)count * (long)sizeof(struct fe),SEEK_SET);
												fread(&tfe,sizeof(struct fe),1,fd);
												fseek(fd,(long)(count - 1) * (long)sizeof(struct fe),SEEK_SET);
												fwrite(&tfe,sizeof(struct fe),1,fd);
												}
											}
										fflush(fd);
										fseek(fd,0L,SEEK_SET);
										--recs;
										chsize(fileno(fd),(long)recs * (long)sizeof(struct fe));
										if (wndw1)
											close_message(wndw1);

										if (current > recs)
											{
											--current;
											if (current >= PAGE_LEN)
												{
												current -= PAGE_LEN;
												if (top >= PAGE_LEN)
													top -= PAGE_LEN;
												else if (top)
													top = 0;
												}
											}
										redraw = 1;
										}
									}
								break;
							case CR:
								if (current < recs)
									memcpy(&tfe,&tfes[CURLINE],sizeof(struct fe));
								else
									{
									memset(&tfe,0,sizeof(struct fe));
									tfe.fe_location = tfile->file_number;
									tfe.fe_priv = tfile->file_priv;
									tfe.fe_flags = tfile->file_flags;
									tfe.fe_uldate = (unsigned int)get_cdate();
									}

								if (edit_flist_entry(&tfe,current))
									{
									memcpy(&tfes[CURLINE],&tfe,sizeof(struct fe));
									fseek(fd,(long)current * (long)sizeof(struct fe),SEEK_SET);
									fwrite(&tfe,sizeof(struct fe),1,fd);
									fflush(fd);
									blank_fedit_entry(current,top,inverse);
									fill_fedit_entry(&tfe,tfile,current,top,inverse);

									if (current == recs)
										{
										++recs;
										if (CURLINE < (PAGE_LEN - 1))
											blank_fedit_entry(current + 1,top,color[3]);
										}

									}
								break;
            				case ESC:
								break;
            				default:
						 		beep();
								break;
							}
						}
					while (!redraw && key != ESC);
					}
				while (key != ESC);

				fclose(fd);
				}
			else
				error(NON_FATAL,"Unable to open/create filelist.bbs in this area!",1,CLOCK_POS,color[0]);
			}
		else 
			error(NOTICE,"There is no path indicated for this file area!",1,CLOCK_POS,color[0]);

		close_window(wndw);
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);
	}
