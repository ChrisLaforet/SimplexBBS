/* s_msg.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_msg.c_v  $
**                       $Date:   25 Oct 1992 14:15:32  $
**                       $Revision:   1.0  $
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



#define PAGE_LEN			19
#define CURLINE				(current - top)


struct choice board_type[4] =
	{
		{'L',"Local"},
		{'E',"Echomail"},
		{'N',"Netmail"},
		{'F',"Local Fileattach"},
	};


struct choice msg_type[3] =
	{
		{'1',"Public and Private"},
		{'2',"Public Only"},
		{'3',"Private Only"},
	};


struct choice nodes[6] =
	{
		{'0',NULL},
		{'1',NULL},
		{'2',NULL},
		{'3',NULL},
		{'4',NULL},
		{'5',NULL},
	};




extern struct choice yn[2];

static char search_text[21] = "";
char nodeid[6][16];


int find_msgarea_list(void)
	{
	struct window *wndw;
	char buffer[81];
	int found = 0;
	int count;
	int rtn;
	int retval = -1;

	if (wndw = open_shadow_window(0xa11,0xe3e,color[3],1))
		{
		prntnomov(0xa12,40,color[4]," Search for a Message Area ");
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
			wndw = open_message("Please wait.  Searching for pattern in message areas.",color[3]);
			for (count = 0; count < cur_msgs; count++)
				{
				strcpy(buffer,msgs[count]->msg_areaname);
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



int next_msgarea_list(int start)
	{
	struct window *wndw;
	char buffer[81];
	int found = 0;
	int count;
	int retval = -1;

	if (search_text[0])
		{
		wndw = open_message("Please wait.  Searching for pattern among message areas.",color[3]);
		for (count = (start + 1); count < cur_msgs; count++)
			{
			strcpy(buffer,msgs[count]->msg_areaname);
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



void draw_msg_list(void)
	{
	int count;

	prntnomov(0x208,8,color[0],"Area");
	prntnomov(0x20f,20,color[0],"Area Name");
	prntnomov(0x239,4,color[0],"Read");
	prntnomov(0x240,5,color[0],"Write");
	prntnomov(0x248,5,color[0],"Sysop");

	for (count = 0; count < (PAGE_LEN + 2); count++)
		{
		_vcw('³',color[0],0x206 + (count << 8),1);
		_vcw('³',color[0],0x20d + (count << 8),1);
		_vcw('³',color[0],0x237 + (count << 8),1);
		_vcw('³',color[0],0x23e + (count << 8),1);
		_vcw('³',color[0],0x246 + (count << 8),1);
		}
	_vcw('Ä',color[0],0x302,76);
	_vcw('Å',color[0],0x306,1);
	_vcw('Å',color[0],0x30d,1);
	_vcw('Å',color[0],0x337,1);
	_vcw('Å',color[0],0x33e,1);
	_vcw('Å',color[0],0x346,1);

#if 0    
	drawbox(0x1400,0x184f,color[3],1);
	clrblk(0x1501,0x174e,color[3]);
	_vcw('Æ',color[3],0x1400,1);
	_vcw('µ',color[3],0x144f,1);
	_vcw('Ï',color[3],0x1406,1);
	_vcw('Ï',color[3],0x140d,1);
	_vcw('Ï',color[3],0x1437,1);
	_vcw('Ï',color[3],0x143e,1);
	_vcw('Ï',color[3],0x1446,1);
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



void draw_msgarea(void)
	{
	prntnomov(0x505,21,color[0],"Message board number:");
	_vcw('_',color[3],0x51f,4);
	prntnomov(0x605,19,color[0],"Message board name:");
	_vcw('_',color[3],0x61f,40);
	prntnomov(0x805,22,color[0],"Type of message board:");
	_vcw('_',color[3],0x81f,16);
	prntnomov(0x905,25,color[0],"Type of messages allowed:");
	_vcw('_',color[3],0x91f,15);
	prntnomov(0xa05,22,color[0],"Permit alias postings?");
	_vcw('_',color[3],0xa1f,1);
	prntnomov(0xc05,21,color[0],"Read privilege level:");
	_vcw('_',color[3],0xc1f,3);
	prntnomov(0xc2b,20,color[0],"Read flags:");
	_vcw(' ',color[3],0xc37,16);
	prntnomov(0xd05,22,color[0],"Write privilege level:");
	_vcw('_',color[3],0xd1f,3);
	prntnomov(0xd2a,20,color[0],"Write flags:");
	_vcw(' ',color[3],0xd37,16);
	prntnomov(0xe05,22,color[0],"Sysop privilege level:");
	_vcw('_',color[3],0xe1f,3);
	prntnomov(0x1005,40,color[0],"Special origin line (if Echomail area):");
	_vcw('_',color[3],0x1105,65);
	prntnomov(0x1205,22,color[0],"Address for this area:");
	_vcw('_',color[3],0x121f,16);
	prntnomov(0x1231,27,color[0],"<- only if echo or net area");
	prntnomov(0x1405,22,color[0],"Message board deleted?");
	_vcw('_',color[3],0x141f,3);
	}



void fill_msgarea(struct msg *tmsg)
	{
	if (tmsg->msg_number)
		prntnomovf(0x51f,4,color[3],"%u",tmsg->msg_number);
	prntnomov(0x61f,40,color[3],tmsg->msg_areaname);

	switch (tmsg->msg_flags & 0x7)
		{
		case MSG_LOCAL:
			prntnomov(0x81f,8,color[3],"Local");
			break;
		case MSG_ECHO:
			prntnomov(0x81f,8,color[3],"Echomail");
			break;
		case MSG_NET:
			prntnomov(0x81f,8,color[3],"Netmail");
			break;
		case MSG_LOCAL_FILEATTACH:
			prntnomov(0x81f,16,color[3],"Local Fileattach");
			break;
		}
	switch (tmsg->msg_flags & (MSG_PRIVATE | MSG_PUBLIC))
		{
		case MSG_PRIVATE | MSG_PUBLIC:
			prntnomov(0x91f,15,color[3],"Public/Private");
			break;
		case MSG_PRIVATE:
			prntnomov(0x91f,15,color[3],"Private");
			break;
		case MSG_PUBLIC:
			prntnomov(0x91f,15,color[3],"Public");
			break;
		}
	if (tmsg->msg_flags & MSG_ALIAS)
		_vcw('Y',color[3],0xa1f,1);
	else
		_vcw('N',color[3],0xa1f,1);

	prntnomovf(0xc1f,3,color[3],"%u",(int)(unsigned char)tmsg->msg_readpriv);
	prntnomovf(0xd1f,3,color[3],"%u",(int)(unsigned char)tmsg->msg_writepriv);
	prntnomovf(0xe1f,3,color[3],"%u",(int)(unsigned char)tmsg->msg_sysoppriv);

	fill_flags(0xc37,tmsg->msg_readflags,color[3]);
	fill_flags(0xd37,tmsg->msg_writeflags,color[3]);

	prntnomov(0x1105,65,color[3],tmsg->msg_origin);
	if ((tmsg->msg_source >= 0) && (tmsg->msg_source < 6))
		prntnomov(0x121f,15,color[3],nodes[tmsg->msg_source].choice_string);
	else 
		prntnomov(0x121f,15,color[3],"Unknown");
	prntnomovf(0x141f,3,color[3],"%s",(char *)(tmsg->msg_deleted ? "Yes" : "No_"));
	}



int edit_msgarea(struct msg *tmsg,int record)
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
		prntnomovf(0x1,40,color[1]," Edit Message Board Entry #%u%s ",record + 1,(char *)(record < cur_msgs ? "" : " [New]"));
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		drawbox(0x302,0x164d,color[0],0);

		draw_msgarea();
		fill_msgarea(tmsg);
		do
			{
			switch (current)
				{
				case 0:
					do
						{
						redo = 0;
						if (tmsg->msg_number)
							sprintf(buffer,"%u",tmsg->msg_number);
						else
							{
							buffer[0] = '\0';
							_vcw('_',color[3],0x51f,4);
							}
						rtn = edit_number(0x51f,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
						if (rtn == E_EXIT)
							{
							tint = atoi(buffer);
							if (tint)
								{
								for (count = 0; count < cur_msgs; count++)
									{
									if (msgs[count]->msg_number == tint && count != record)
										{
										sprintf(buffer,"There is already another message area with number %u!  Choose another.",tint);
										error(NOTICE,buffer,1,CLOCK_POS,color[0]);
										redo = 1;
										break;
										}
									}
								}
							else
								{
								error(NOTICE,"Message area 0 is not a valid message area choice!",1,CLOCK_POS,color[0]);
								redo = 1;
								}
							if (!redo)
								{
								tmsg->msg_number = tint;
								if (!tmsg->msg_number)
									_vcw('_',color[3],0x51f,4);
								}
							}
						}
					while (redo);
					break;
				case 1:
					rtn = edit_field(0x61f,color[4],color[3],1,CLOCK_POS,color[0],"FPC40",tmsg->msg_areaname,NULL);
					break;
				case 2:
					rtn = pop_menu(0x625,color[4],color[3],"Type of Board",4,board_type,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'L')
							{
							tmsg->msg_flags &= 0xf8;
							tmsg->msg_flags |= MSG_LOCAL;
							_vcw('_',color[3],0x81f,16);
							prntnomov(0x81f,8,color[3],"Local");
							}
						else if (rtn == 'N')
							{
							tmsg->msg_flags &= 0xf8;
							tmsg->msg_flags |= MSG_NET;
							_vcw('_',color[3],0x81f,16);
							prntnomov(0x81f,8,color[3],"Netmail");
							}
						else if (rtn == 'F')
							{
							tmsg->msg_flags &= 0xf8;
							tmsg->msg_flags |= MSG_LOCAL_FILEATTACH;
							_vcw('_',color[3],0x81f,16);
							prntnomov(0x81f,15,color[3],"Local Fileattach");
							}
						else
							{
							tmsg->msg_flags &= 0xf8;
							tmsg->msg_flags |= MSG_ECHO;
							_vcw('_',color[3],0x81f,16);
							prntnomov(0x81f,8,color[3],"Echomail");
							}
						rtn = E_EXIT;
						}
					break;
				case 3:
					rtn = pop_menu(0x727,color[4],color[3],"Message Type",3,msg_type,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == '1')
							{
							tmsg->msg_flags &= 0xe7;
							tmsg->msg_flags |= (MSG_PUBLIC | MSG_PRIVATE);
							_vcw('_',color[3],0x91f,15);
							prntnomov(0x91f,15,color[3],"Public/Private");
							}
						else if (rtn == '2')
							{
							tmsg->msg_flags &= 0xe7;
							tmsg->msg_flags |= MSG_PUBLIC;
							_vcw('_',color[3],0x91f,15);
							prntnomov(0x91f,8,color[3],"Public");
							}
						else
							{
							tmsg->msg_flags &= 0xe7;
							tmsg->msg_flags |= MSG_PRIVATE;
							_vcw('_',color[3],0x91f,15);
							prntnomov(0x91f,15,color[3],"Private");
							}
						rtn = E_EXIT;
						}
					break;
				case 4:
					rtn = pop_menu(0x821,color[4],color[3],"Permit Aliases",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							{
							tmsg->msg_flags &= 0xdf;
							tmsg->msg_flags |= MSG_ALIAS;
							_vcw('Y',color[3],0xa1f,1);
							}
						else
							{
							tmsg->msg_flags &= 0xdf;
							_vcw('N',color[3],0xa1f,1);
							}
						rtn = E_EXIT;
						}
					break;
				case 5:
					sprintf(buffer,"%u",(int)(unsigned char)tmsg->msg_readpriv);
					rtn = edit_number(0xc1f,color[4],color[3],buffer,1,255,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tint = atoi(buffer);
						tmsg->msg_readpriv = (char)(unsigned char)tint;
						}
					break;
				case 6:
					rtn = edit_flags(0xc37,&tmsg->msg_readflags,color[4],color[3],1,CLOCK_POS,color[0]);
					break;
				case 7:
					sprintf(buffer,"%u",(int)(unsigned char)tmsg->msg_writepriv);
					rtn = edit_number(0xd1f,color[4],color[3],buffer,1,255,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tint = atoi(buffer);
						tmsg->msg_writepriv = (char)(unsigned char)tint;
						}
					break;
				case 8:
					rtn = edit_flags(0xd37,&tmsg->msg_writeflags,color[4],color[3],1,CLOCK_POS,color[0]);
					break;
				case 9:
					sprintf(buffer,"%u",(int)(unsigned char)tmsg->msg_sysoppriv);
					rtn = edit_number(0xe1f,color[4],color[3],buffer,1,255,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tint = atoi(buffer);
						tmsg->msg_sysoppriv = (char)(unsigned char)tint;
						}
					break;
				case 10:
					rtn = edit_field(0x1105,color[4],color[3],1,CLOCK_POS,color[0],"F065",tmsg->msg_origin,get_anychar);
					break;
				case 11:
					rtn = pop_menu(0xd30,color[4],color[3],"Source Address",6,nodes,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						tmsg->msg_source = (char)(rtn - '0');
						_vcw('_',color[3],0x121f,15);
						prntnomov(0x121f,15,color[3],nodes[tmsg->msg_source].choice_string);
						rtn = E_EXIT;
						}
					break;
				case 12:
					rtn = pop_menu(0x1023,color[4],color[3],"Delete Area",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							{
							if (!tmsg->msg_deleted)
								{
								tmsg->msg_deleted = 1;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						else
							{
							if (!tmsg->msg_deleted)
								{
								tmsg->msg_deleted = 0;
								rtn = E_EXIT;
								}
							else
								rtn = E_FORE;
							}
						prntnomovf(0x141f,3,color[3],"%s",(char *)(tmsg->msg_deleted ? "Yes" : "No_"));
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
		while (rtn != E_END && rtn != E_QUIT);

		if (change)
			master_change = 1;

		close_window(wndw);
		}
	else
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);

	return change;
	}




void edit_msg_boards(void)
	{
	struct window *wndw;
	struct msg tmsg;
	int current = 0;
	int inverse;
	int redraw = 1;
	int count;
	int top = 0;
	int key;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," Select Message Area for Editing ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");
		draw_msg_list();

		search_text[0] = '\0';

		set_cursor_type(HIDDEN);
		if ((color[0] & ON_WHITE) != ON_WHITE)
			inverse = ON_WHITE;
		else
			inverse = LT_GRAY;


		/* prepare net-address choices */

		if (cfg.cfg_net)
			sprintf(nodeid[0],"%u:%u/%u",cfg.cfg_zone,cfg.cfg_net,cfg.cfg_node);
		else
			strcpy(nodeid[0],"Unknown");
		nodes[0].choice_string = nodeid[0];

		for (count = 0; count < 5; count++)
			{
			if (cfg.cfg_akanet[count])
				sprintf(nodeid[count + 1],"%u:%u/%u",cfg.cfg_akazone[count],cfg.cfg_akanet[count],cfg.cfg_akanode[count]);
			else
				strcpy(nodeid[count + 1],"Unknown");
			nodes[count + 1].choice_string = nodeid[count + 1];
			}

		do
			{
			if (redraw)
				{
				clrblk(0x402,0x1605,color[0]);
				clrblk(0x407,0x160c,color[0]);
				clrblk(0x40e,0x1636,color[0]);
				clrblk(0x438,0x163d,color[0]);
				clrblk(0x440,0x1645,color[0]);
				clrblk(0x448,0x164d,color[0]);

				for (count = 0; count < PAGE_LEN; count++)
					{
					if ((top + count) > cur_msgs)
						break;
					else if ((top + count) == cur_msgs)
						{
						prntnomovf(0x402 + (count << 8),4,color[0],"%4u",top + count + 1);
						prntnomov(0x40f + (count << 8),40,color[0],"< Add a new message area >");
						break;
						}
					else
						{
						prntnomovf(0x402 + (count << 8),4,color[0],"%4u",top + count + 1);
						prntnomovf(0x408 + (count << 8),4,color[0],"%4u",msgs[top + count]->msg_number);
						prntnomov(0x40f + (count << 8),40,color[0],msgs[top + count]->msg_areaname);
						prntnomovf(0x439 + (count << 8),40,color[0],"%3u",msgs[top + count]->msg_readpriv);
						prntnomovf(0x440 + (count << 8),40,color[0],"%3u",msgs[top + count]->msg_writepriv);
						prntnomovf(0x448 + (count << 8),40,color[0],"%3u",msgs[top + count]->msg_sysoppriv);
						}
					}
				redraw = 0;
				}

			if (cur_msgs)
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
					if (current < cur_msgs)
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
					if (current != cur_msgs)
						{
						phantom(0x402 + (CURLINE << 8),76,color[0]);
						current = cur_msgs;
						if (current >= (top + PAGE_LEN))
							{
							top = cur_msgs - (PAGE_LEN - 1);
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
					if (current < cur_msgs)
						{
						phantom(0x402 + (CURLINE << 8),76,color[0]);
						if ((current + PAGE_LEN) <= cur_msgs)
							{
							current += PAGE_LEN;
							top += PAGE_LEN;
							redraw = 1;
							}
						else if ((top + PAGE_LEN) <= cur_msgs)
							{
							current = cur_msgs;
							top += PAGE_LEN;
							redraw = 1;
							}
						else
							current = cur_msgs;
						}
					break;
				case F5:
					if ((rtn = find_msgarea_list()) != -1)
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
					if ((rtn = next_msgarea_list(current)) != -1)
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
					if (current < cur_msgs)
						memcpy(&tmsg,msgs[current],sizeof(struct msg));
					else
						{
						memset(&tmsg,0,sizeof(struct msg));
						tmsg.msg_flags = MSG_PUBLIC | MSG_PRIVATE | MSG_LOCAL;
						tmsg.msg_readpriv = cfg.cfg_newpriv;
						tmsg.msg_readflags = cfg.cfg_newflags;
						tmsg.msg_writepriv = cfg.cfg_newpriv;
						tmsg.msg_writeflags = cfg.cfg_newflags;
						tmsg.msg_sysoppriv = (unsigned char)255;
						}
					if (edit_msgarea(&tmsg,current))
						{
						if (current == cur_msgs)
							{
							if (cur_msgs >= max_msgs)
								{
								if (!(msgs = realloc(msgs,(max_msgs += 10) * sizeof(struct msg *))))
									error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
								}
							if (!(msgs[cur_msgs] = calloc(1,sizeof(struct msg))))
	 							error(FATAL,"Error: Out of memory to allocate structures!",1,CLOCK_POS,color[0]);
							memcpy(msgs[cur_msgs],&tmsg,sizeof(struct msg));
							++cur_msgs;
							}
						else
							memcpy(msgs[current],&tmsg,sizeof(struct msg));

						qsort(msgs,cur_msgs,sizeof(struct msg *),msg_comp);

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
