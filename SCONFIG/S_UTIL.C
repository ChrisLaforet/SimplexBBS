/* s_util.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 1 July 1992
**
** Revision Information: $Logfile:   G:/simplex/sconfig/vcs/s_util.c_v  $
**                       $Date:   25 Oct 1992 14:15:38  $
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



/* User flags handling functions */

void fill_flags(int cursor,int flags,int color)
	{
	char tchar;
	int count;
	int val = 1;

	tchar = 'A';
	for (count = 0; count < 16; count++)
		{
		if (flags & val)
			_vcw(tchar,color,cursor + count,1);
		else 
			_vcw((char)tolower(tchar),color,cursor + count,1);
		val <<= 1;
		++tchar;
		}
	}



int edit_flags(int cursor,int *flags,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color)
	{
	char tchar;
	int tflags = *flags;
	int count;
	int rtn = E_FORE;
	int quit = 0;
	int key;
	int val = 1;

	set_cursor_type(HIDDEN);
	phantom(cursor,16,hicolor);
	do
		{
		key = read_keyboard(clock_flag,clock_pos,clock_color);
		if (!(key & 0xff00))
			key = toupper(key);
		switch (key)
			{
			case 'A':
				if (tflags & UF_A)
					{
					tflags &= ~UF_A;
					_vcw('a',hicolor,cursor,1);
					}
				else
					{
					tflags |= UF_A;
					_vcw('A',hicolor,cursor,1);
					}
				break;
			case 'B':
				if (tflags & UF_B)
					{
					tflags &= ~UF_B;
					_vcw('b',hicolor,cursor + 1,1);
					}
				else
					{
					tflags |= UF_B;
					_vcw('B',hicolor,cursor + 1,1);
					}
				break;
			case 'C':
				if (tflags & UF_C)
					{
					tflags &= ~UF_C;
					_vcw('c',hicolor,cursor + 2,1);
					}
				else
					{
					tflags |= UF_C;
					_vcw('C',hicolor,cursor + 2,1);
					}
				break;
			case 'D':
				if (tflags & UF_D)
					{
					tflags &= ~UF_D;
					_vcw('d',hicolor,cursor + 3,1);
					}
				else
					{
					tflags |= UF_D;
					_vcw('D',hicolor,cursor + 3,1);
					}
				break;
			case 'E':
				if (tflags & UF_E)
					{
					tflags &= ~UF_E;
					_vcw('e',hicolor,cursor + 4,1);
					}
				else
					{
					tflags |= UF_E;
					_vcw('E',hicolor,cursor + 4,1);
					}
				break;
			case 'F':
				if (tflags & UF_F)
					{
					tflags &= ~UF_F;
					_vcw('f',hicolor,cursor + 5,1);
					}
				else
					{
					tflags |= UF_F;
					_vcw('F',hicolor,cursor + 5,1);
					}
				break;
			case 'G':
				if (tflags & UF_G)
					{
					tflags &= ~UF_G;
					_vcw('g',hicolor,cursor + 6,1);
					}
				else
					{
					tflags |= UF_G;
					_vcw('G',hicolor,cursor + 6,1);
					}
				break;
			case 'H':
				if (tflags & UF_H)
					{
					tflags &= ~UF_H;
					_vcw('h',hicolor,cursor + 7,1);
					}
				else
					{
					tflags |= UF_H;
					_vcw('H',hicolor,cursor + 7,1);
					}
				break;
			case 'I':
				if (tflags & UF_I)
					{
					tflags &= ~UF_I;
					_vcw('i',hicolor,cursor + 8,1);
					}
				else
					{
					tflags |= UF_I;
					_vcw('I',hicolor,cursor + 8,1);
					}
				break;
			case 'J':
				if (tflags & UF_J)
					{
					tflags &= ~UF_J;
					_vcw('j',hicolor,cursor + 9,1);
					}
				else
					{
					tflags |= UF_J;
					_vcw('J',hicolor,cursor + 9,1);
					}
				break;
			case 'K':
				if (tflags & UF_K)
					{
					tflags &= ~UF_K;
					_vcw('k',hicolor,cursor + 10,1);
					}
				else
					{
					tflags |= UF_K;
					_vcw('K',hicolor,cursor + 10,1);
					}
				break;
			case 'L':
				if (tflags & UF_L)
					{
					tflags &= ~UF_L;
					_vcw('l',hicolor,cursor + 11,1);
					}
				else
					{
					tflags |= UF_L;
					_vcw('L',hicolor,cursor + 11,1);
					}
				break;
			case 'M':
				if (tflags & UF_M)
					{
					tflags &= ~UF_M;
					_vcw('m',hicolor,cursor + 12,1);
					}
				else
					{
					tflags |= UF_M;
					_vcw('M',hicolor,cursor + 12,1);
					}
				break;
			case 'N':
				if (tflags & UF_N)
					{
					tflags &= ~UF_N;
					_vcw('n',hicolor,cursor + 13,1);
					}
				else
					{
					tflags |= UF_N;
					_vcw('N',hicolor,cursor + 13,1);
					}
				break;
			case 'O':
				if (tflags & UF_O)
					{
					tflags &= ~UF_O;
					_vcw('o',hicolor,cursor + 14,1);
					}
				else
					{
					tflags |= UF_O;
					_vcw('O',hicolor,cursor + 14,1);
					}
				break;
			case 'P':
				if (tflags & UF_P)
					{
					tflags &= ~UF_P;
					_vcw('p',hicolor,cursor + 15,1);
					}
				else
					{
					tflags |= UF_P;
					_vcw('P',hicolor,cursor + 15,1);
					}
				break;
			case TAB:
				rtn = E_FORE;
				quit = 1;
				break;
			case STAB:
				rtn = E_BACK;
				quit = 1;
				break;
			case CR:
				*flags = tflags;
				rtn = E_EXIT;
				quit = 1;
				break;
			case ESC:
			case 0x110: 	/* Alt-Q */
				rtn = E_QUIT;
				quit = 1;
				break;
			case CEnd:
			case 0x12d: 	/* Alt-X */
				rtn = E_END;
				quit = 1;
				break;
			case PgUp:
				rtn = E_PGUP;
				quit = 1;
				break;
			case PgDn:
				rtn = E_PGDN;
				quit = 1;
				break;
			default:
				beep();
				break;
			}
		}
	while (!quit);

	tflags = *flags;
	tchar = 'A';
	for (count = 0; count < 16; count++)
		{
		if (tflags & val)
			_vcw(tchar,locolor,cursor + count,1);
		else 
			_vcw((char)tolower(tchar),locolor,cursor + count,1);
		val <<= 1;
		++tchar;
		}

	set_cursor_type(LINE);
	return rtn;
	}



/* Date editing/filling functions */

void fill_date(int cursor,DATE_T date,int color)
	{
	char *cptr;
			   	
	cptr = datetoa(date);
	prntnomovf(cursor,8,color,"%2.2s/%2.2s/%2.2s",cptr,cptr + 2,cptr + 4);
	}



int edit_date(int cursor,char *buffer,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color)
	{
	char invalid[81];
	char save[10];
	int len;
	int rtn;

	strcpy(save,buffer);
	do
		{
		rtn = edit_field(cursor,hicolor,locolor,clock_flag,clock_pos,clock_color,"00J00J00",buffer,NULL);
		len = strlen(buffer);
		if (rtn == E_EXIT)
			{
			if (len == 6)
				{
				if (!check_date(buffer))
					{
					sprintf(invalid,"The date \"%2.2s/%2.2s/%2.2s\" is invalid.  Please enter a valid date....",buffer,buffer + 2,buffer + 4);
					error(NOTICE,invalid,clock_flag,clock_pos,clock_color);
					strcpy(buffer,save);
					prntnomovf(cursor,20,locolor,"__/__/__");

					len = 1;
					}
				}
			else if (len)
				beep();
			}
		}
	while (rtn == E_EXIT && (len != 6 && len));

	if (rtn != E_EXIT)
		strcpy(buffer,save);
	prntnomovf(cursor,8,locolor,"%2.2s/%2.2s/%2.2s",buffer,buffer + 2,buffer + 4);

	return rtn;
	}



/* A decent number-editing function */

int edit_number(int cursor,int hicolor,int locolor,char *buffer,int minimum,int maximum,int clock_flag,int clock_pos,int clock_color)
	{
	char tbuffer[20];
	char save[20];
	unsigned long value;
	int current = 0;
	int retval = E_ERROR;
	int maxlen;
	int key;

	strcpy(save,buffer);
	sprintf(tbuffer,"%u",maximum);
	maxlen = strlen(tbuffer);

	phantom(cursor,maxlen,hicolor);

	_setcurpos(cursor);
	set_cursor_type(FULL);

	register_help("field editor");
	do
		{
		key = read_keyboard(1,CLOCK_POS,color[0]);
		switch (key)
			{
			case BACKSPACE:
				if (current)
					{
					memmove(buffer + (current - 1),buffer + current,strlen(buffer + current) + 2);
					buffer[maxlen] = (char)'\0';
					prntnomov(cursor,maxlen,hicolor,buffer);
					_vcw('_',hicolor,cursor + strlen(buffer),1);

					--current;
					_setcurpos(cursor + current);	/* set new cursor */
					}
				break;
			case Del:
			case '\x7f':
				if (current < (int)strlen(buffer))
					{
					memmove(buffer + current,buffer + (current + 1),strlen(buffer + current) + 1);
					buffer[maxlen] = (char)'\0';
					prntnomov(cursor,maxlen,hicolor,buffer);
					_vcw('_',hicolor,cursor + strlen(buffer),1);
					}
				break;
			case RightArrow:
				if (current < (int)strlen(buffer))
					{
					++current;
					_setcurpos(cursor + current);	/* set new cursor */
					}
				break;
			case CRightArrow:
				current = 0;
				_setcurpos(cursor + current);	/* set new cursor */
				break;
			case LeftArrow:
				if (current)
					{
					--current;
					_setcurpos(cursor + current);	/* set new cursor */
					}
				break;
			case CLeftArrow:
				current = strlen(buffer);
				_setcurpos(cursor + current);	/* set new cursor */
				break;
			case PgUp:
				retval = E_PGUP;
				break;
			case PgDn:
				retval = E_PGDN;
				break;
			case CR:
				retval = E_EXIT;
				break;
			case TAB:
				retval = E_FORE;
				break;
			case STAB:
				retval = E_BACK;
				break;
			case ESC:
			case 0x110: 	/* Alt-Q */
				retval = E_QUIT;
				break;
			case 0x12d: 	/* Alt-X */
			case CEnd:
				retval = E_END;
				break;
			case 0x12e: 	/* Alt-C  ==  Clear a field */
				memset(buffer,0,maxlen + 1);
				current = 0;
				_setcurpos(cursor + current);	/* set new cursor */
				_vcw('_',hicolor,cursor,maxlen);
				break;
			default:
				if (isdigit(key) && maxlen >= (int)strlen(buffer))
					{
					strcpy(tbuffer,buffer);
					memmove(tbuffer + (current + 1),tbuffer + current,strlen(tbuffer + current) + 1);
					tbuffer[current] = (char)key;
					value = atol(tbuffer);
					if (value <= (unsigned long)(unsigned int)maximum)
						{
						strcpy(buffer,tbuffer);
						prntnomov(cursor,maxlen,hicolor,buffer);

						++current;
						_setcurpos(cursor + current);	/* set new cursor */
						}
					}
				break;
			}
		}
	while (retval == E_ERROR);

	deregister_help();

	if (retval == E_EXIT)
		{
		if (atol(buffer) < (long)minimum)
			sprintf(buffer,"%u",minimum);
		}
	else
		strcpy(buffer,save);

	set_cursor_type(LINE);
	_vcw('_',locolor,cursor,maxlen);
	prntnomov(cursor,maxlen,locolor,buffer);

	return retval;
	}



/* Sorting functions */

int file_comp(struct file **arg1,struct file **arg2)
	{
	return (*arg1)->file_number - (*arg2)->file_number;
	}



int msg_comp(struct msg **arg1,struct msg **arg2)
	{
	return (*arg1)->msg_number - (*arg2)->msg_number;
	}




/* Some special field-editor functions */

int get_anychar_first(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action)
	{
	int len;
	int max_length;

	if (action == E_VALID)
		return E_VALID;
	if (keystroke & 0x100)
		return E_INVALID;
	if (*string_offset)
		return E_INVALID;
	if (insert)
		{
		max_length = strlen(format);
		len = strlen(string + *string_offset);
		memmove(string + (*string_offset + 1),string + *string_offset,len);
		if (*string_offset + len + 1 >= max_length)
			string[max_length] = '\0';
		else
			string[*string_offset + len + 1] = '\0';
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		return E_VALID;
		}
	else
		{
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		}
	return E_VALID;
	}



int get_filename(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action)
	{
	int len;
	int max_length;

	if (action == E_VALID)
		return E_VALID;
	max_length = strlen(format);
	if (*string_offset >= max_length)
		return E_INVALID;
	if (keystroke & 0x100)
		return E_INVALID;
	if (keystroke <= 0x20 || keystroke >= 0x7f)
		return E_INVALID;
	keystroke = toupper(keystroke);
	if (keystroke == '.' || keystroke == '"' ||	keystroke == '/' || keystroke == P_CSEP || keystroke == '?' || keystroke == '*' ||
		keystroke == '[' || keystroke == ']' || keystroke == ':' || keystroke == '|' || keystroke == '<' ||
		keystroke == '>' || keystroke == '+' || keystroke == '=' || keystroke == ';' || keystroke == ',')
		{
		return E_INVALID;
		}
	if (insert)
		{
		len = strlen(string + *string_offset);
		if (len >= max_length)
			return E_INVALID;
		memmove(string + (*string_offset + 1),string + *string_offset,len);
		if (*string_offset + len + 1 >= max_length)
			string[max_length] = '\0';
		else
			string[*string_offset + len + 1] = '\0';
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		return E_VALID;
		}
	else
		{
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		}
	return E_VALID;
	}



int get_fileext(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action)		/* allows extensions */
	{
	int len;
	int max_length;

	if (action == E_VALID)
		return E_VALID;
	max_length = strlen(format);
	if (*string_offset >= max_length)
		return E_INVALID;
	if (keystroke & 0x100)
		return E_INVALID;
	if (keystroke <= 0x20 || keystroke >= 0x7f)
		return E_INVALID;
	keystroke = toupper(keystroke);
	if (keystroke == '"' ||	keystroke == '/' || keystroke == P_CSEP || keystroke == '?' || keystroke == '*' ||
		keystroke == '[' || keystroke == ']' || keystroke == ':' || keystroke == '|' || keystroke == '<' ||
		keystroke == '>' || keystroke == '+' || keystroke == '=' || keystroke == ';' || keystroke == ',')
		{
		return E_INVALID;
		}
	if (insert)
		{
		len = strlen(string + *string_offset);
		if (len >= max_length)
			return E_INVALID;
		memmove(string + (*string_offset + 1),string + *string_offset,len);
		if (*string_offset + len + 1 >= max_length)
			string[max_length] = '\0';
		else
			string[*string_offset + len + 1] = '\0';
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		return E_VALID;
		}
	else
		{
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		}
	return E_VALID;
	}



int get_pathname(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action)
	{
	int len;
	int max_length;

	if (action == E_VALID)
		return E_VALID;
	max_length = strlen(format);
	if (*string_offset >= max_length)
		return E_INVALID;
	if (keystroke & 0x100)
		return E_INVALID;
	if (keystroke <= 0x20 || keystroke >= 0x7f)
		return E_INVALID;
	keystroke = toupper(keystroke);
	if (keystroke == '.' || keystroke == '"' ||	keystroke == '/' || keystroke == '[' || keystroke == ']' || keystroke == '|' || keystroke == '?' ||
		keystroke == '<' ||	keystroke == '>' || keystroke == '+' || keystroke == '=' || keystroke == ';' || keystroke == ',' || keystroke == '*')
		{
		return E_INVALID;
		}
	if (insert)
		{
		len = strlen(string + *string_offset);
		if (len >= max_length)
			return E_INVALID;
		memmove(string + (*string_offset + 1),string + *string_offset,len);
		if (*string_offset + len + 1 >= max_length)
			string[max_length] = '\0';
		else
			string[*string_offset + len + 1] = '\0';
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		return E_VALID;
		}
	else
		{
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		}
	return E_VALID;
	}



int get_anychar(int cursor,int hicolor,int locolor,int clock_flag,int clock_pos,int clock_color,char *format,char *string,int *offset,int *string_offset,int insert,int keystroke,int action)
	{
	int len;
	int max_length;

	if (action == E_VALID)
		return E_VALID;
	max_length = strlen(format);
	if (*string_offset >= max_length)
		return E_INVALID;
	if (keystroke & 0x100)
		return E_INVALID;
	if (keystroke < 0x20 || keystroke == 0x7f)
		return E_INVALID;
	if (insert)
		{
		len = strlen(string + *string_offset);
		if (len >= max_length)
			return E_INVALID;
		memmove(string + (*string_offset + 1),string + *string_offset,len);
		if (*string_offset + len + 1 >= max_length)
			string[max_length] = '\0';
		else
			string[*string_offset + len + 1] = '\0';
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		return E_VALID;
		}
	else
		{
		string[*string_offset] = (char)keystroke;
		++*string_offset;
		++*offset;
		}
	return E_VALID;
	}


/* color handling functions */

char *parse_color(int color)
	{
	static char buffer[31];

	buffer[0] = '\0';
	if (color & BLINK)
		strcat(buffer,"Blinking ");
	switch (color & 0xf)
		{
		case BLACK:
			strcat(buffer,"Black");
			break;
		case BLUE:
			strcat(buffer,"Blue");
			break;
		case GREEN:
			strcat(buffer,"Green");
			break;
		case CYAN:
			strcat(buffer,"Cyan");
			break;
		case RED:
			strcat(buffer,"Red");
			break;
		case MAGENTA:
			strcat(buffer,"Magenta");
			break;
		case BROWN:
			strcat(buffer,"Brown");
			break;
		case LT_GRAY:
			strcat(buffer,"Light Gray");
			break;
		case GRAY:
			strcat(buffer,"Gray");
			break;
		case LT_BLUE:
			strcat(buffer,"Light Blue");
			break;
		case LT_GREEN:
			strcat(buffer,"Light Green");
			break;
		case LT_CYAN:
			strcat(buffer,"Light Cyan");
			break;
		case LT_RED:
			strcat(buffer,"Light Red");
			break;
		case LT_MAGENTA:
			strcat(buffer,"Light Magenta");
			break;
		case YELLOW:
			strcat(buffer,"Yellow");
			break;
		case WHITE:
			strcat(buffer,"White");
			break;
		}
	switch (color & 0x70)
		{
		case 0:
			strcat(buffer," on Black");
			break;
		case ON_BLUE:
			strcat(buffer," on Blue");
			break;
		case ON_GREEN:
			strcat(buffer," on Green");
			break;
		case ON_CYAN:
			strcat(buffer," on Cyan");
			break;
		case ON_RED:
			strcat(buffer," on Red");
			break;
		case ON_MAGENTA:
			strcat(buffer," on Magenta");
			break;
		case ON_YELLOW:
			strcat(buffer," on Yellow");
			break;
		case ON_WHITE:
			strcat(buffer," on White");
			break;
		}
	return buffer;
	}



int select_color(char *title,int *pcolor)
	{
	struct window *wndw;
	char buffer[80];
	char save_help[41];
	int save[20];
	int rtn = 0;
	int quit = 0;
	int fore;
	int back;
	int key;

	if (wndw = open_window(0x41c,0x174f,WHITE,3))
		{
		strcpy(save_help,query_help_topic());
	   	register_help("color menu");
		set_cursor_type(HIDDEN);

		sprintf(buffer," Select Color for %s ",title);
		prntcenter(0x41c,50,WHITE | BLINK,buffer);
		prntcenter(0x171c,50,WHITE," Press Enter to select, ESC to abort ");
		for (back = 0; back < 16; back++)
			{
			for (fore = 0; fore < 16; fore++)
				prntnomov(((0x6 + back) << 8) + (fore * 3 + 0x1e),3,(back << 4) + fore," x ");
			}
		fore = *pcolor & 0xf;
		back = ((int)*pcolor >> 4) & 0xf;
		do
			{
			_vbr(save,((0x5 + back) << 8) + (fore * 3 + 0x1d),5);
			_vbr(save + 5,((0x6 + back) << 8) + (fore * 3 + 0x1d),5);
			_vbr(save + 10,((0x7 + back) << 8) + (fore * 3 + 0x1d),5);
			drawbox(((0x5 + back) << 8) + (fore * 3 + 0x1d),((0x7 + back) << 8) + (fore * 3 + 0x21),WHITE,0);
			key = read_keyboard(1,CLOCK_POS,color[0]);
			_vbw(save,((0x5 + back) << 8) + (fore * 3 + 0x1d),5);
			_vbw(save + 5,((0x6 + back) << 8) + (fore * 3 + 0x1d),5);
			_vbw(save + 10,((0x7 + back) << 8) + (fore * 3 + 0x1d),5);
			switch (key)
				{
				case UpArrow:
					if (back)
						--back;
					else
						back = 15;
					break;
				case DownArrow:
					if (back < 15)
						++back;
					else
						back = 0;
					break;
				case LeftArrow:
					if (fore)
						--fore;
					else
						fore = 15;
					break;
				case RightArrow:
					if (fore < 15)
						++fore;
					else
						fore = 0;
					break;
				case TAB:
					rtn = E_FORE;
					quit = 1;
					break;
				case STAB:
					rtn = E_BACK;
					quit = 1;
					break;
				case CR:
					if (fore == (back & 7))
						{
						error(NOTICE,"You cannot select both background and foreground colors to be the same!",1,CLOCK_POS,color[0]);
						rtn = 0;
						}
					else
						{
						*pcolor = (back << 4) + fore;
						rtn = E_EXIT;
						quit = 1;
						}
					break;
				case ESC:
				case 0x110:
					rtn = E_QUIT;
					quit = 1;
					break;
				case CEnd:
				case 0x12d:
					rtn = E_END;
					quit = 1;
					break;
				case PgUp:
					rtn = E_PGUP;
					quit = 1;
					break;
				case PgDn:
					rtn = E_PGDN;
					quit = 1;
					break;
				default:
					beep();
					break;
				}
			}
		while (!quit);
		set_cursor_type(LINE);
	   	register_help(save_help);
		close_window(wndw);
		}
	return rtn;
	}




