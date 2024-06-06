/* s_times.c
**
** Copyright (c) 1994, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 26 August 1994
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <screen.h>
#include <window.h>
#include <editor.h>
#include <keys.h>
#include "sconfig.h"


#define MAX_LEN					18


void draw_times(void)
	{
	short count;

	_vcw('#',color[0],0x304,1);
	prntnomov(0x308,4,color[0],"Priv");
	prntnomov(0x30f,4,color[0],"Time");
	_vcw('#',color[0],0x31a,1);
	prntnomov(0x31e,4,color[0],"Priv");
	prntnomov(0x325,4,color[0],"Time");
	_vcw('-',color[0],0x404,2);
	_vcw('-',color[0],0x408,5);
	_vcw('-',color[0],0x40f,4);
	_vcw('-',color[0],0x41a,2);
	_vcw('-',color[0],0x41e,5);
	_vcw('-',color[0],0x425,4);

	for (count = 0; count < MAX_LEN; count++)
		{
		prntnomovf(0x504 + (count << 8),2,color[0],"%2u",1 + count);
//		_vcw('_',color[3],0x508 + (count << 8),3);
//		_vcw('_',color[3],0x50f + (count << 8),4);
		prntnomovf(0x51a + (count << 8),2,color[0],"%2u",19 + count);
//		_vcw('_',color[3],0x51e + (count << 8),3);
//		_vcw('_',color[3],0x525 + (count << 8),4);
		}
	}



int comp_priv(struct ut **a,struct ut **b)
	{
	return (*a)->ut_priv - (*b)->ut_priv;
	}



void fill_times(void)
	{
	short count;

	qsort(uts,cur_uts,sizeof(struct ut *),comp_priv);
	for (count = 0; count < (MAX_LEN * 2); count++)
		{
		if (count < MAX_LEN)
			{
			_vcw('_',color[3],0x508 + (count << 8),3);
			_vcw('_',color[3],0x50f + (count << 8),4);
			if (count < cur_uts)
				{
				prntnomovf(0x508 + (count << 8),3,color[3],"%u",uts[count]->ut_priv);
				prntnomovf(0x50f + (count << 8),4,color[3],"%u",uts[count]->ut_time);
				}
			}
		else
			{
			_vcw('_',color[3],0x51e + ((count - MAX_LEN) << 8),3);
			_vcw('_',color[3],0x525 + ((count - MAX_LEN) << 8),4);
			if (count < cur_uts)
				{
				prntnomovf(0x51e + ((count - MAX_LEN) << 8),3,color[3],"%u",uts[count]->ut_priv);
				prntnomovf(0x525 + ((count - MAX_LEN) << 8),4,color[3],"%u",uts[count]->ut_time);
				}
			}
		}
	}



short edit_uts(struct ut *utptr,short current)
	{
	struct window *wndw;
	char buffer[10];
	short change = 0;
	short which = 0;
	short rtn;

	if (wndw = open_shadow_window(0x915,0xd39,color[3],1))
		{
		prntnomovf(0x916,18,color[4]," Editing Entry #%u ",current + 1);
		prntnomov(0xd16,19,color[3]," Press F1 for Help ");

		prntnomov(0xb19,6,color[3],"Priv =");
		_vcw('_',color[0],0xb20,3);
		prntnomovf(0xb20,3,color[0],"%u",utptr->ut_priv);
		prntnomov(0xb26,6,color[3],"Time =");
		_vcw('_',color[0],0xb2d,4);
		prntnomovf(0xb2d,4,color[0],"%u",utptr->ut_time);

		do
			{
			if (which)
				{
				sprintf(buffer,"%u",utptr->ut_time);
				rtn = edit_number(0xb2d,color[1],color[0],buffer,0,1440,1,CLOCK_POS,color[0]);
				if (rtn == E_EXIT)
					{
					utptr->ut_time = (unsigned short)atoi(buffer);
					change = 1;
					}
				}
			else
				{
				sprintf(buffer,"%u",utptr->ut_priv);
				rtn = edit_number(0xb20,color[1],color[0],buffer,0,255,1,CLOCK_POS,color[0]);
				if (rtn == E_EXIT)
					{
					utptr->ut_priv = (unsigned short)atoi(buffer);
					change = 1;
					}
				}
			if (rtn == E_EXIT || rtn == E_FORE || rtn == E_BACK)
				which = which ? 0 : 1;
			if (rtn == E_QUIT)
				change = 0;
			if (rtn == E_END && (!utptr->ut_priv || !utptr->ut_time))
				{
				if (!askyn("Cannot Save Changes","Cannot save with 0 priv or 0 time.  SURE you want to quit w/o saving? ",color[4],color[3],1,CLOCK_POS,color[0]))
					rtn = E_FORE;
				}
			}
		while (rtn != E_QUIT && rtn != E_END);

		close_shadow_window(wndw);
		}
	else
		error(NOTICE,"Unable to open window!",1,CLOCK_POS + 0x1700,color[0]);

	return change;
	}



void edit_times(void)
	{
	struct window *wndw;
	struct ut tut;
	short phantom_color;
	short current = 0;
	short quit = 0;
	short key;
	short pos;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		if ((color[3] & ON_WHITE) != ON_WHITE)
			phantom_color = ON_WHITE;
		else
			phantom_color = NORMAL;

		prntnomov(0x1,17,color[1]," Edit User Times ");
		prntnomov(0x1801,19,color[0]," Press F1 for Help ");
		draw_times();

		drawbox(0x72e,0x124d,color[0],0);
		prntnomov(0x830,30,color[0],"This section permits you to");
		prntnomov(0x930,30,color[0],"create and edit the basic");
		prntnomov(0xa30,30,color[0],"daily online times for users");
		prntnomov(0xb30,30,color[0],"based on priv level. These");
		prntnomov(0xc30,30,color[0],"times can be from 1 minute");
		prntnomov(0xd30,30,color[0],"to 1440 minutes (24 hours)");
		prntnomov(0xe30,30,color[0],"per day.");
		prntnomov(0x1030,30,color[0],"Enter = edit an entry.");
		prntnomov(0x1130,30,color[0],"  Del = delete an entry.");

		fill_times();

		do
			{
			if (current < MAX_LEN)
				pos = 0x508 + (current << 8);
			else
				pos = 0x51e + ((current - MAX_LEN) << 8);

			phantom(pos,3,phantom_color);
			phantom(pos + 7,4,phantom_color);
			key = read_keyboard(1,CLOCK_POS,color[0]);
			switch (key)
				{
				case 0x1b:
					quit = 1;
					break;

				case 0xd:
					if (current < cur_uts)
						memcpy(&tut,uts[current],sizeof(struct ut));
					else
						memset(&tut,0,sizeof(struct ut));
					if (edit_uts(&tut,current))
						{
						if (current >= cur_uts)
							{
							if (cur_uts >= max_uts)
								{
								if (!(uts = realloc(uts,(max_uts += 5) * sizeof(struct ut *))))
									error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
								}

							if (!(uts[cur_uts] = malloc(sizeof(struct ut))))
								error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
							memcpy(uts[cur_uts],&tut,sizeof(struct ut));
							++cur_uts;
							}
						else
							memcpy(uts[current],&tut,sizeof(struct ut));
						fill_times();

						current = 0;
						phantom(pos,3,color[3]);
						phantom(pos + 7,4,color[3]);

						master_change = 1;
						}
					break;

				case Del:
					if (current < cur_uts)
						{
						if (askyn("Sure you want to delete entry?","Are you DEFINITE that you want to delete this entry? ",color[4],color[3],1,CLOCK_POS,color[0]))
							{
							free(uts[current]);
							if (current < (cur_uts - 1))
								memmove(uts + current,uts + (current + 1),(cur_uts - current - 1) * sizeof(char *));
							--cur_uts;
							fill_times();

							if (current >= cur_uts && current)
								--current;
							master_change = 1;
							}
						}
					break;

				case UpArrow:
					if (current)
						{
						phantom(pos,3,color[3]);
						phantom(pos + 7,4,color[3]);
						--current;
						}
					else
						beep();
					break;

				case DownArrow:
					if ((current < max_uts) && (current < (MAX_LEN << 1)))
						{
						phantom(pos,3,color[3]);
						phantom(pos + 7,4,color[3]);
						++current;
						}
					else
						beep();
					break;
				}
			}
		while (!quit);

#if 0
		do
			{

			if ((current >> 1) < cur_uts)
				memcpy(&tut,uts[current >> 1],sizeof(struct ut));
			else
				memset(&tut,0,sizeof(struct ut));
				if (tshort < cur_uts)
					sprintf(buffer,"%u",tut.ut_priv);
				else
					buffer[0] = (char)'\0';
				rtn = edit_number(pos,color[4],color[3],buffer,0,255,1,CLOCK_POS,color[0]);
				if (rtn == E_EXIT)
					{
					tut.ut_priv = (unsigned short)atoi(buffer);

					if (tshort >= cur_uts)
						{
						if (cur_uts >= max_uts)
							{
							if (!(uts = realloc(uts,(max_uts += 5) * sizeof(struct ut *))))
								{

								}
							}

						if (!(uts[cur_uts] = malloc(sizeof(struct ut))))
							{

							}
						memcpy(uts[cur_uts],&tut,sizeof(struct ut));
						uts[cur_uts]->ut_time = 1;
						++cur_uts;
						}
					else
						memcpy(uts[tshort],&tut,sizeof(struct ut));

					master_change = 1;
					}
				}

			}
		while (rtn != E_QUIT && rtn != E_END);
#endif

		close_window(wndw);
		}
	else
		error(NOTICE,"Unable to open window!",1,CLOCK_POS + 0x1700,color[0]);
	}
