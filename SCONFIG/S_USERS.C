/* s_users.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 4 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_users.c_v  $
**                       $Date:   25 Oct 1992 14:15:42  $
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



extern struct choice yn[2];
static char search_text[21] = "";




int find_user_list(void)
	{
	struct window *wndw;
	struct window *wndw2;
	char buffer[81];
	int found = 0;
	int count;
	int rtn;
	int retval = -1;

	if (wndw = open_shadow_window(0xa11,0xe3e,color[3],1))
		{
		prntnomov(0xa12,40,color[4]," Search for a User ");
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
			wndw = open_message("Please wait.  Searching for pattern among users.",color[3]);
			for (count = 0; count < cur_usrs; count++)
				{
				strcpy(buffer,usrs[count]->usr_name);
				strupr(buffer);
				if (strstr(buffer,search_text))
					{
					found = 1;
					break;
					}

				strcpy(buffer,usrs[count]->usr_city);
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



int next_user_list(int start)
	{
	struct window *wndw;
	char buffer[81];
	int found = 0;
	int count;
	int retval = -1;

	if (search_text[0])
		{
		wndw = open_message("Please wait.  Searching for pattern among users.",color[3]);
		for (count = (start + 1); count < cur_usrs; count++)
			{
			strcpy(buffer,usrs[count]->usr_name);
			strupr(buffer);
			if (strstr(buffer,search_text))
				{
				found = 1;
				break;
				}

			strcpy(buffer,usrs[count]->usr_city);
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



void draw_user(void)
	{
	int count;

	prntnomov(0x205,10,color[0],"User Name:");
	_vcw('_',color[3],0x210,40);
	prntnomov(0x307,8,color[0],"Address:");
	_vcw('_',color[3],0x310,30);
	prntnomov(0x334,9,color[0],"Password:");
	_vcw('_',color[3],0x33e,15);
	_vcw('_',color[3],0x410,30);
	prntnomov(0x432,11,color[0],"Home Phone:");
	_vcw('_',color[3],0x43e,15);
	prntnomov(0x50a,5,color[0],"City:");
	_vcw('_',color[3],0x510,30);
	prntnomov(0x532,11,color[0],"Data Phone:");
	_vcw('_',color[3],0x53e,15);
	prntnomov(0x609,6,color[0],"State:");
	_vcw('_',color[3],0x610,15);
	prntnomov(0x630,13,color[0],"Deleted User?");
	_vcw('_',color[3],0x63e,3);
	prntnomov(0x70b,4,color[0],"Zip:");
	_vcw('_',color[3],0x710,15);
	prntnomov(0x731,12,color[0],"Guest Login?");
	_vcw('_',color[3],0x73e,3);
	for (count = 0; count < 4; count++)
		{
		prntnomovf(0x806 + (count << 8),9,color[0],"Alias #%d:",count + 1);
		_vcw('_',color[3],0x810 + (count << 8),40);
		}
	prntnomov(0xd06,9,color[0],"Std Priv:");
	_vcw('_',color[3],0xd10,3);
	prntnomov(0xd17,10,color[0],"Std Flags:");
	_vcw('_',color[3],0xd22,16);
	prntnomov(0xe05,10,color[0],"Spec Priv:");
	_vcw('_',color[3],0xe10,3);
	prntnomov(0xe16,11,color[0],"Spec Flags:");
	_vcw('_',color[3],0xe22,16);
	prntnomov(0xe35,8,color[0],"Expires:");
	prntnomov(0xe3e,8,color[3],"__/__/__");
	prntnomov(0x1004,11,color[0],"Screen Len:");
	_vcw('_',color[3],0x1010,2);
	prntnomov(0x1014,13,color[0],"(9->66 lines)");
	prntnomov(0x1031,12,color[0],"Editor Type:");
	_vcw('_',color[3],0x103e,12);
	prntnomov(0x1102,13,color[0],"Clear Screen?");
	_vcw('_',color[3],0x1110,3);
	prntnomov(0x112b,18,color[0],"Allow Net Fattach?");
	_vcw('_',color[3],0x113e,3);
	prntnomov(0x1204,11,color[0],"Page Pause?");
	_vcw('_',color[3],0x1210,3);
	prntnomov(0x1215,6,color[0],"(More)");
	prntnomov(0x122c,17,color[0],"Expert Menu Mode?");
	_vcw('_',color[3],0x123e,3);
	prntnomov(0x1305,10,color[0],"Ansi Mode?");
	_vcw('_',color[3],0x1310,3);
	prntnomov(0x1331,12,color[0],"Net Credits:");
	_vcw('_',color[3],0x133e,4);
	prntnomov(0x1404,11,color[0],"First Call:");
	prntnomov(0x1410,8,color[3],"__/__/__");
	prntnomov(0x141f,5,color[0],"UL's:");
	_vcw('_',color[3],0x1425,5);
	prntnomov(0x1438,5,color[0],"DL's:");
	_vcw('_',color[3],0x143e,5);
	prntnomov(0x151b,9,color[0],"UL bytes:");
	_vcw('_',color[3],0x1525,12);
	prntnomov(0x1534,9,color[0],"DL bytes:");
	_vcw('_',color[3],0x153e,12);
	prntnomov(0x1505,10,color[0],"Last Call:");
	prntnomov(0x1510,8,color[3],"__/__/__");
	prntnomov(0x161a,10,color[0],"Msgs Sent:");
	_vcw('_',color[3],0x1625,12);
	prntnomov(0x1633,10,color[0],"Msgs Read:");
	_vcw('_',color[3],0x163e,12);

#if 0
	prntnomov(0x305,10,color[0],"User Name:");
	_vcw('_',color[3],0x310,40);
	prntnomov(0x407,8,color[0],"Address:");
	_vcw('_',color[3],0x410,30);
	_vcw('_',color[3],0x510,30);
	prntnomov(0x60a,5,color[0],"City:");
	_vcw('_',color[3],0x610,30);
	prntnomov(0x709,6,color[0],"State:");
	_vcw('_',color[3],0x710,15);
	prntnomov(0x732,11,color[0],"Home Phone:");
	_vcw('_',color[3],0x73e,14);
	prntnomov(0x80b,4,color[0],"Zip:");
	_vcw('_',color[3],0x810,15);
	prntnomov(0x832,11,color[0],"Data Phone:");
	_vcw('_',color[3],0x83e,14);
	prntnomov(0x930,13,color[0],"Deleted User?");
	_vcw('_',color[3],0x93e,3);
	prntnomov(0xa06,9,color[0],"Password:");
	_vcw('_',color[3],0xa10,15);
	prntnomov(0xa31,12,color[0],"Guest Login?");
	_vcw('_',color[3],0xa3e,3);
	prntnomov(0xc06,9,color[0],"Std Priv:");
	_vcw('_',color[3],0xc10,3);
	prntnomov(0xc18,27,color[0],"Std Flags:");
	_vcw('_',color[3],0xc23,16);
	prntnomov(0xd05,10,color[0],"Spec Priv:");
	_vcw('_',color[3],0xd10,3);
	prntnomov(0xd17,28,color[0],"Spec Flags:");
	_vcw('_',color[3],0xd23,16);
	prntnomov(0xd38,8,color[0],"Expires:");
	prntnomov(0xd41,8,color[3],"__/__/__");
	prntnomov(0xf04,11,color[0],"Screen Len:");
	_vcw('_',color[3],0xf10,2);
	prntnomov(0xf14,13,color[0],"(9->66 lines)");
	prntnomov(0xf31,12,color[0],"Editor Type:");
	_vcw('_',color[3],0xf3e,12);
	prntnomov(0x1002,13,color[0],"Clear Screen?");
	_vcw('_',color[3],0x1010,3);
	prntnomov(0x102b,18,color[0],"Allow Net Fattach?");
	_vcw('_',color[3],0x103e,3);
	prntnomov(0x1104,11,color[0],"Page Pause?");
	_vcw('_',color[3],0x1110,3);
	prntnomov(0x1115,6,color[0],"(More)");
	prntnomov(0x112c,17,color[0],"Expert Menu Mode?");
	_vcw('_',color[3],0x113e,3);
	prntnomov(0x1205,10,color[0],"Ansi Mode?");
	_vcw('_',color[3],0x1210,3);
	prntnomov(0x1231,12,color[0],"Net Credits:");
	_vcw('_',color[3],0x123e,4);
	prntnomov(0x1404,11,color[0],"First Call:");
	prntnomov(0x1410,8,color[3],"__/__/__");
	prntnomov(0x141f,5,color[0],"UL's:");
	_vcw('_',color[3],0x1425,5);
	prntnomov(0x1438,5,color[0],"DL's:");
	_vcw('_',color[3],0x143e,5);
	prntnomov(0x1505,10,color[0],"Last Call:");
	prntnomov(0x1510,8,color[3],"__/__/__");
	prntnomov(0x151b,9,color[0],"UL bytes:");
	_vcw('_',color[3],0x1525,12);
	prntnomov(0x1534,9,color[0],"DL bytes:");
	_vcw('_',color[3],0x153e,12);
	prntnomov(0x161a,10,color[0],"Msgs Sent:");
	_vcw('_',color[3],0x1625,12);
	prntnomov(0x1633,10,color[0],"Msgs Read:");
	_vcw('_',color[3],0x163e,12);
#endif
	}



void fill_user(struct user *tuser)
	{
	int count;

	prntnomov(0x210,40,color[3],tuser->user_name);
	prntnomov(0x310,30,color[3],tuser->user_address1);
	prntnomov(0x410,30,color[3],tuser->user_address2);
	prntnomov(0x510,30,color[3],tuser->user_city);
	prntnomov(0x610,15,color[3],tuser->user_state);
	prntnomov(0x710,15,color[3],tuser->user_zip);

	prntnomov(0x33e,15,color[3],tuser->user_password);
	prntnomov(0x43e,14,color[3],tuser->user_home);
	prntnomov(0x53e,14,color[3],tuser->user_data);
	prntnomov(0x63e,3,color[3],(char *)(tuser->user_flags & USER_DELETED ? "Yes" : "No"));
	prntnomov(0x73e,3,color[3],(char *)(tuser->user_flags & USER_GUEST ? "Yes" : "No"));

	for (count = 0; count < 4; count++)
		prntnomov(0x810 + (count << 8),40,color[3],tuser->user_alias[count]);

	prntnomovf(0xd10,3,color[3],"%u",(int)(unsigned char)tuser->user_priv);
	fill_flags(0xd22,tuser->user_uflags,color[3]);
	prntnomovf(0xe10,3,color[3],"%u",(int)(unsigned char)tuser->user_spriv);
	fill_flags(0xe22,tuser->user_sflags,color[3]);
	fill_date(0xe3e,tuser->user_sdate,color[3]);

	prntnomovf(0x1010,2,color[3],"%u",tuser->user_screenlen);
	prntnomov(0x1110,3,color[3],(char *)(tuser->user_flags & USER_CLS ? "Yes" : "No"));
	prntnomov(0x1210,3,color[3],(char *)(tuser->user_flags & USER_MORE ? "Yes" : "No"));
	prntnomov(0x1310,3,color[3],(char *)(tuser->user_flags & USER_ANSI ? "Yes" : "No"));

	prntnomov(0x103e,12,color[3],(char *)(tuser->user_flags & USER_EDITOR ? "Full-screen" : "Line"));
	prntnomov(0x113e,3,color[3],(char *)(tuser->user_flags & USER_FILEATTACH ? "Yes" : "No"));
	prntnomov(0x123e,3,color[3],(char *)(tuser->user_flags & USER_EXPERT ? "Yes" : "No"));
	prntnomovf(0x133e,4,color[3],"%u",tuser->user_credit);

	fill_date(0x1410,tuser->user_firstdate,color[3]);
	fill_date(0x1510,tuser->user_lastdate,color[3]);

	prntnomovf(0x1425,5,color[3],"%u",tuser->user_upload);
	prntnomovf(0x1525,12,color[3],"%lu",tuser->user_uploadbytes);
	prntnomovf(0x1625,12,color[3],"%lu",tuser->user_msgsent);

	prntnomovf(0x143e,5,color[3],"%u",tuser->user_dnload);
	prntnomovf(0x153e,12,color[3],"%lu",tuser->user_dnloadbytes);
	prntnomovf(0x163e,12,color[3],"%lu",tuser->user_msgread);
	}			 



int edit_user(struct user *tuser,int record)
	{
	struct window *wndw;
	char buffer[10];
	int current = 0;
	int change = 0;
	int tval;
	int rtn;


	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomovf(0x1,40,color[1]," Edit Userlist Entry #%u%s ",record + 1,(char *)(record < cur_usrs ? "" : " [New]"));
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");

		draw_user();
		fill_user(tuser);

		do
			{
			switch (current)
				{
				case 0:
					rtn = edit_field(0x210,color[4],color[3],1,CLOCK_POS,color[0],"FPC40",tuser->user_name,NULL);
					break;
				case 1:
					rtn = edit_field(0x310,color[4],color[3],1,CLOCK_POS,color[0],"FPC30",tuser->user_address1,NULL);
					break;
				case 2:
					rtn = edit_field(0x410,color[4],color[3],1,CLOCK_POS,color[0],"FPC30",tuser->user_address2,NULL);
					break;
				case 3:
					rtn = edit_field(0x510,color[4],color[3],1,CLOCK_POS,color[0],"FPC30",tuser->user_city,NULL);
					break;
				case 4:
					rtn = edit_field(0x610,color[4],color[3],1,CLOCK_POS,color[0],"FPC15",tuser->user_state,NULL);
					break;
				case 5:
					rtn = edit_field(0x710,color[4],color[3],1,CLOCK_POS,color[0],"FPC15",tuser->user_zip,NULL);
					break;

				case 6:
					rtn = edit_field(0x33e,color[4],color[3],1,CLOCK_POS,color[0],"FPC15",tuser->user_password,get_anychar);
					break;
				case 7:
					rtn = edit_field(0x43e,color[4],color[3],1,CLOCK_POS,color[0],"FPC14",tuser->user_home,get_anychar);
					break;
				case 8:
					rtn = edit_field(0x53e,color[4],color[3],1,CLOCK_POS,color[0],"FPC14",tuser->user_data,get_anychar);
					break;
				case 9:
					rtn = pop_menu(0x426,color[4],color[3],"Deleted?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							tuser->user_flags |= USER_DELETED;
						else				  
							tuser->user_flags &= ~USER_DELETED;
						prntnomov(0x63e,3,color[3],tuser->user_flags & USER_DELETED ? "Yes" : "No_");
						rtn = E_EXIT;
						}
					break;
				case 10:
					rtn = pop_menu(0x52a,color[4],color[3],"Guest Login?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							tuser->user_flags |= USER_GUEST;
						else				  
							tuser->user_flags &= ~USER_GUEST;
						prntnomov(0x73e,3,color[3],(char *)(tuser->user_flags & USER_GUEST ? "Yes" : "No_"));
						rtn = E_EXIT;
						}
					break;
				case 11:
					rtn = edit_field(0x810,color[4],color[3],1,CLOCK_POS,color[0],"FPC40",tuser->user_alias[0],NULL);
					break;
				case 12:
					rtn = edit_field(0x910,color[4],color[3],1,CLOCK_POS,color[0],"FPC40",tuser->user_alias[1],NULL);
					break;
				case 13:
					rtn = edit_field(0xa10,color[4],color[3],1,CLOCK_POS,color[0],"FPC40",tuser->user_alias[2],NULL);
					break;
				case 14:
					rtn = edit_field(0xb10,color[4],color[3],1,CLOCK_POS,color[0],"FPC40",tuser->user_alias[3],NULL);
					break;
				case 15:
					sprintf(buffer,"%u",(int)(unsigned char)tuser->user_priv);
					rtn = edit_number(0xd10,color[4],color[3],buffer,1,255,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tval = atoi(buffer);
						tuser->user_priv = (char)(unsigned char)tval;
						}
					break;
				case 16:
					rtn = edit_flags(0xd22,&tuser->user_uflags,color[4],color[3],1,CLOCK_POS,color[0]);
					break;
				case 17:
					sprintf(buffer,"%u",(int)(unsigned char)tuser->user_spriv);
					rtn = edit_number(0xe10,color[4],color[3],buffer,1,255,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tval = atoi(buffer);
						tuser->user_spriv = (char)(unsigned char)tval;
						}
					break;
				case 18:
					rtn = edit_flags(0xe22,&tuser->user_sflags,color[4],color[3],1,CLOCK_POS,color[0]);
					break;
				case 19:
					strcpy(buffer,datetoa(tuser->user_sdate));
					rtn = edit_date(0xe3e,buffer,color[4],color[3],1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						tuser->user_sdate = atodate(buffer);
					break;

				case 20:
					sprintf(buffer,"%u",tuser->user_screenlen);
					rtn = edit_number(0x1010,color[4],color[3],buffer,9,66,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						{
						tval = atoi(buffer);
						tuser->user_screenlen = (char)tval;
						}
					break;
				case 21:
					rtn = pop_menu(0xe15,color[4],color[3],"CLS?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							tuser->user_flags |= USER_CLS;
						else				  
							tuser->user_flags &= ~USER_CLS;
						prntnomov(0x1110,3,color[3],(char *)(tuser->user_flags & USER_CLS ? "Yes" : "No_"));
						rtn = E_EXIT;
						}
					break;
				case 22:
					rtn = pop_menu(0xf17,color[4],color[3],"More?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							tuser->user_flags |= USER_MORE;
						else				  
							tuser->user_flags &= ~USER_MORE;
						prntnomov(0x1210,3,color[3],(char *)(tuser->user_flags & USER_MORE ? "Yes" : "No_"));
						rtn = E_EXIT;
						}
					break;
				case 23:
					rtn = pop_menu(0x1019,color[4],color[3],"ANSI?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							tuser->user_flags |= USER_ANSI;
						else				  
							tuser->user_flags &= ~USER_ANSI;
						prntnomov(0x1310,3,color[3],(char *)(tuser->user_flags & USER_ANSI ? "Yes" : "No_"));
						rtn = E_EXIT;
						}
					break;

				case 24:
					rtn = pop_menu(0xe26,color[4],color[3],"ANSI Editor?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							tuser->user_flags |= USER_EDITOR;
						else				  
							tuser->user_flags &= ~USER_EDITOR;
						_vcw('_',color[3],0x103e,12);
						prntnomov(0x103e,12,color[3],(char *)(tuser->user_flags & USER_EDITOR ? "Full-screen" : "Line"));
						rtn = E_EXIT;
						}
					break;
				case 25:
					rtn = pop_menu(0xf28,color[4],color[3],"F/Att?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							tuser->user_flags |= USER_FILEATTACH;
						else				  
							tuser->user_flags &= ~USER_FILEATTACH;
						prntnomov(0x113e,3,color[3],(char *)(tuser->user_flags & USER_FILEATTACH ? "Yes" : "No_"));
						rtn = E_EXIT;
						}
					break;
				case 26:
					rtn = pop_menu(0x102a,color[4],color[3],"Expert?",2,yn,1,1,1,CLOCK_POS,color[0]);
					if (rtn > ' ')
						{
						if (rtn == 'Y')
							tuser->user_flags |= USER_EXPERT;
						else				  
							tuser->user_flags &= ~USER_EXPERT;
						prntnomov(0x123e,3,color[3],(char *)(tuser->user_flags & USER_EXPERT ? "Yes" : "No_"));
						rtn = E_EXIT;
						}
					break;
				case 27:
					if (!tuser->user_credit)
						buffer[0] = '\0';
					else 
						sprintf(buffer,"%u",tuser->user_credit);
					rtn = edit_number(0x133e,color[4],color[3],buffer,0,9999,1,CLOCK_POS,color[0]);
					if (rtn == E_EXIT)
						tuser->user_credit = atoi(buffer);
					prntnomovf(0x133e,4,color[3],"%u",tuser->user_credit);
					break;
				}
			if (rtn == E_EXIT)
				change = 1;
			if (rtn == E_FORE || rtn == E_EXIT)
				{
				if (current < 27)
					++current;
				else
					current = 0;
				}
			else if (rtn == E_BACK)
				{
				if (current)
					--current;
				else
					current = 27;
				}
			}
		while (rtn != E_END && rtn != E_QUIT);

		if (rtn == E_QUIT)
			change = 0;

		close_window(wndw);
		}
	else 
		error(NOTICE,"Unable to open window!",1,CLOCK_POS,color[0]);

	return change;
	}



void draw_user_list(void)
	{
	int count;

	for (count = 0; count < (PAGE_LEN + 2); count++)
		{
		_vcw('³',color[0],0x207 + (count << 8),1);
		_vcw('³',color[0],0x222 + (count << 8),1);
		_vcw('³',color[0],0x234 + (count << 8),1);
		}
	_vcw('Ä',color[0],0x302,76);
	_vcw('Å',color[0],0x307,1);
	_vcw('Å',color[0],0x322,1);
	_vcw('Å',color[0],0x334,1);
    
	prntnomov(0x202,20,color[0],"User");
	prntnomov(0x209,20,color[0],"User's Name");
	prntnomov(0x224,20,color[0],"City");
	prntnomov(0x236,20,color[0],"Priv & Flags");

#if 0
	drawbox(0x1400,0x184f,color[3],1);
	clrblk(0x1501,0x174e,color[3]);
	_vcw('Æ',color[3],0x1400,1);
	_vcw('µ',color[3],0x144f,1);
	_vcw('Ï',color[3],0x1407,1);
	_vcw('Ï',color[3],0x1422,1);
	_vcw('Ï',color[3],0x1434,1);
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



void edit_user_list(void)
	{
	struct window *wndw;
	struct user tuser;
	int current = 0;
	int inverse;
	int redraw = 1;
	int count;
	int top = 0;
	int key;
	int rtn;

	if (wndw = open_window(0x0,0x184f,color[0],1))
		{
		prntnomov(0x1,40,color[1]," Select User Record for Editing ");
		prntnomov(0x1801,40,color[0]," Press F1 for Help ");
		draw_user_list();

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
				clrblk(0x409,0x1620,color[0]);
				clrblk(0x424,0x1632,color[0]);
				clrblk(0x436,0x164d,color[0]);

				for (count = 0; count < PAGE_LEN; count++)
					{
					if ((top + count) > cur_usrs)
						break;
					else if ((top + count) == cur_usrs)
						{
						prntnomovf(0x402 + (count << 8),4,color[0],"%4u",top + count + 1);
						prntnomov(0x409 + (count << 8),25,color[0],"< Add a new user >");
						break;
						}
					else
						{
						prntnomovf(0x402 + (count << 8),4,color[0],"%4u",top + count + 1);
						prntnomov(0x409 + (count << 8),24,color[0],usrs[top + count]->usr_name);
						prntnomov(0x424 + (count << 8),15,color[0],usrs[top + count]->usr_city);
						prntnomovf(0x436 + (count << 8),3,color[0],"%3u",usrs[top + count]->usr_priv);
						fill_flags(0x43a + (count << 8),usrs[top + count]->usr_flags,color[0]);
						}
					}
				redraw = 0;
				}

			if (cur_usrs)
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
					if (current < cur_usrs)
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
					if (current != cur_usrs)
						{
						phantom(0x402 + (CURLINE << 8),76,color[0]);
						current = cur_usrs;
						if (current >= (top + PAGE_LEN))
							{
							top = cur_usrs - (PAGE_LEN - 1);
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
					if (current < cur_usrs)
						{
						phantom(0x402 + (CURLINE << 8),76,color[0]);
						if ((current + PAGE_LEN) <= cur_usrs)
							{
							current += PAGE_LEN;
							top += PAGE_LEN;
							redraw = 1;
							}
						else if ((top + PAGE_LEN) <= cur_usrs)
							{
							current = cur_usrs;
							top += PAGE_LEN;
							redraw = 1;
							}
						else
							current = cur_usrs;
						}
					break;
				case F5:
					if ((rtn = find_user_list()) != -1)
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
					if ((rtn = next_user_list(current)) != -1)
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
					if (current < cur_usrs)
						{
						fseek(userfd,(long)current * (long)sizeof(struct user),SEEK_SET);
						fread(&tuser,sizeof(struct user),1,userfd);
						}
					else
						{
						memset(&tuser,0,sizeof(struct user));
						tuser.user_screenlen = (char)24;
						tuser.user_priv = cfg.cfg_newpriv;
						tuser.user_uflags = cfg.cfg_newflags;
						tuser.user_credit = cfg.cfg_newcredit;
						tuser.user_flags = USER_MORE;
						}

					if (edit_user(&tuser,current))
						{
						fseek(userfd,(long)current * (long)sizeof(struct user),SEEK_SET);
						fwrite(&tuser,sizeof(struct user),1,userfd);
						if (current < cur_usrs)
							{
							if (strcmp(usrs[current]->usr_name,tuser.user_name))
								{
								free(usrs[current]->usr_name);

								if (!(usrs[current]->usr_name = malloc(strlen(tuser.user_name) + 1)))
									{
									error(FATAL,"Error: Out of memory to allocate structures!\n",1,CLOCK_POS,color[0]);
									exit(1);
									}
								strcpy(usrs[current]->usr_name,tuser.user_name);
								}

							if (strcmp(usrs[current]->usr_city,tuser.user_city))
								{
								free(usrs[current]->usr_city);

								if (!(usrs[current]->usr_city = malloc(strlen(tuser.user_city) + 1)))
									{
									error(FATAL,"Error: Out of memory to allocate structures!\n",1,CLOCK_POS,color[0]);
									exit(1);
									}

								strcpy(usrs[current]->usr_city,tuser.user_city);
								}

							usrs[current]->usr_deleted = (char)((tuser.user_flags & USER_DELETED) ? 1 : 0);
							usrs[current]->usr_priv = tuser.user_priv;
							usrs[current]->usr_flags = tuser.user_uflags;
							}
						else
							{
							if (cur_usrs >= max_usrs)
								{
								if (!(usrs = realloc(usrs,(max_usrs += 100) * sizeof(struct usr *))))
									{
									error(FATAL,"Error: Out of memory to allocate structures!\n",1,CLOCK_POS,color[0]);
									exit(1);
									}
								}
							if (!(usrs[cur_usrs] = malloc(sizeof(struct usr))))
								{
								error(FATAL,"Error: Out of memory to allocate structures!\n",1,CLOCK_POS,color[0]);
								exit(1);
								}

							if (!(usrs[cur_usrs]->usr_name = malloc(strlen(tuser.user_name) + 1)))
								{
								error(FATAL,"Error: Out of memory to allocate structures!\n",1,CLOCK_POS,color[0]);
								exit(1);
								}
							if (!(usrs[cur_usrs]->usr_city = malloc(strlen(tuser.user_city) + 1)))
								{
								error(FATAL,"Error: Out of memory to allocate structures!\n",1,CLOCK_POS,color[0]);
								exit(1);
								}

							strcpy(usrs[cur_usrs]->usr_name,tuser.user_name);
							strcpy(usrs[cur_usrs]->usr_city,tuser.user_city);
							usrs[cur_usrs]->usr_deleted = (char)((tuser.user_flags & USER_DELETED) ? 1 : 0);
							usrs[cur_usrs]->usr_priv = tuser.user_priv;
							usrs[cur_usrs]->usr_flags = tuser.user_uflags;

							++cur_usrs;

							if (CURLINE < (PAGE_LEN - 1))
								{
								prntnomovf(0x402 + ((CURLINE + 1) << 8),4,inverse,"%4u",cur_usrs + 1);
								_vcw(' ',inverse,0x409 + ((CURLINE + 1) << 8),25);
								prntnomov(0x409 + ((CURLINE + 1) << 8),25,inverse,"< Add a new user >");
								}
							}
						_vcw(' ',inverse,0x409 + (CURLINE << 8),25);
						_vcw(' ',inverse,0x424 + (CURLINE << 8),15);
						_vcw(' ',inverse,0x436 + (CURLINE << 8),3);
						_vcw(' ',inverse,0x43a + (CURLINE << 8),16);
						prntnomov(0x409 + (CURLINE << 8),24,inverse,tuser.user_name);
						prntnomov(0x424 + (CURLINE << 8),15,inverse,tuser.user_city);
						prntnomovf(0x436 + (CURLINE << 8),3,inverse,"%3u",tuser.user_priv);
						fill_flags(0x43a + (CURLINE << 8),tuser.user_uflags,inverse);
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




