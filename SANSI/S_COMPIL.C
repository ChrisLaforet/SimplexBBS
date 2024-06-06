/* s_compil.c
**
** Copyright (c) 1990, Christopher Laforet
** All Rights Reserved
**
** Started: 4 January 1990
**
** Revision Information: $Logfile:   G:/simplex/sansi/vcs/s_compil.c_v  $
**                       $Date:   25 Oct 1992 14:12:40  $
**                       $Revision:   1.2  $
**
*/


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "sansi.h"



struct key keywords[KEYWORDS] =
	{
		{"name","`A",0},
		{"first","`B",0},
		{"last","`C",0},
		{"citystate","`D",0},
		{"homephone","`E",0},
		{"dataphone","`F",0},
		{"password","`G",0},
		{"priv","`H",0},
		{"flags","`I",0},
		{"alias1","`@",0},
		{"alias2","`#",0},
		{"alias3","`%",0},
		{"alias4","`&",0},

		{"baud","`M",0},
		{"ansi","`N",0},
		{"more","`O",0},
		{"clear","`P",0},
		{"editor","`Q",0},
		{"expert","`R",0},
		{"pagelen","`S",0},
		{"credit","`T",0},

		{"minbaud","`U",0},
		{"ansibaud","`V",0},
		{"yellstart","`W",0},
		{"yellstop","`X",0},
		{"dlstart","`Y",0},
		{"dlstop","`Z",0},

		{"firstdate","`a",0},
		{"lastdate","`b",0},
		{"lasttime","`c",0},
		{"totaltime","`d",0},
		{"timeused","`e",0},
		{"timeleft","`f",0},
		{"timeon","`g",0},
		{"ul","`h",0},
		{"ulkb","`i",0},
		{"dl","`j",0},
		{"dlkb","`k",0},
		{"calls","`l",0},
		{"msgread","`m",0},
		{"msgsent","`n",0},

		{"day","`p",0},
		{"shortday","`q",0},
		{"date","`r",0},
		{"time","`s",0},
		{"yells","`t",0},
		{"users","`u",0},
		{"lastuser","`v",0},
		{"bbsname","`w",0},
		{"sysopname","`x",0},
		{"minbaud","`U",0},
		{"ansibaud","`V",0},
		{"yellstart","`W",0},
		{"yellstop","`X",0},
		{"dlstart","`Y",0},
		{"dlstop","`Z",0},

		{"moreoff","`0",0},
		{"moreon","`1",0},
		{"stopoff","`2",0},
		{"stopon","`3",0},
		{"enter","`4",0},
		{"cls","`5",1},
		{"bell","`6",0},
		{"bs","`7",0},

		{"black","\x1b[0;30m",1},
		{"red","\x1b[0;31m",1},
		{"green","\x1b[0;32m",1},
		{"brown","\x1b[0;33m",1},
		{"blue","\x1b[0;34m",1},
		{"magenta","\x1b[0;35m",1},
		{"cyan","\x1b[0;36m",1},
		{"white","\x1b[0;37m",1},
		{"lightgray","\x1b[0;37m",1},

		{"lightblack","\x1b[0;30;1m",1},
		{"gray","\x1b[0;30;1m",1},
		{"lightred","\x1b[0;31;1m",1},
		{"lightgreen","\x1b[0;32;1m",1},
		{"lightbrown","\x1b[0;33;1m",1},
		{"yellow","\x1b[0;33;1m",1},
		{"lightblue","\x1b[0;34;1m",1},
		{"lightmagenta","\x1b[0;35;1m",1},
		{"lightcyan","\x1b[0;36;1m",1},
		{"lightwhite","\x1b[0;37;1m",1},

		{"onblack","\x1b[40m",1},
		{"onred","\x1b[41m",1},
		{"ongreen","\x1b[42m",1},
		{"onbrown","\x1b[43m",1},
		{"onblue","\x1b[44m",1},
		{"onmagenta","\x1b[45m",1},
		{"oncyan","\x1b[46m",1},
		{"onwhite","\x1b[47m",1},

		{"blink","\x1b[5m",1},

		{"erasetoeol","\x1b[K",1},
		{"erasetoeos","\x1b[0J",1},
		{"erasefromtos","\x1b[1J",1},
		{"erasescreen","\x1b[2J",1},

		{"cursorup","\x1b[1A",1},
		{"cursordown","\x1b[1B",1},
		{"cursorleft","\x1b[1D",1},
		{"cursorright","\x1b[1C",1},
		{"home","\x1b[H",1},
	};


unsigned short yylineno = 0;
unsigned char cur_char;
unsigned char next_char;
unsigned char yyline[513];



struct key *translate_keyword(unsigned char *keyword)
	{
	short count;
	struct key *rtn = NULL;

	for (count = 0; count < KEYWORDS; count++)
		{						   
		if (!stricmp(keyword,keywords[count].key_word))
			{
			rtn = &keywords[count];
			break;
			}
		}
	return(rtn);
	}



void yygetchar(void)
	{
	static short first_flag = 1;
	static unsigned char *cptr;

	if (!cptr || first_flag)
		{
		first_flag = 0;
		if (cptr = fgets(yyline,sizeof(yyline),yyfd))
			{
			cur_char = *cptr++;
			next_char = *cptr;
			++yylineno;
			}
		else
			cur_char = next_char = '\0';
		}
	else
		{
		if (cptr == yyline)
			++yylineno;
		cur_char = *cptr++;
		if (!cur_char)
			{
			if (cptr = fgets(yyline,sizeof(yyline),yyfd))
				next_char = *cptr;
			else
				next_char = '\0';
			}
		else
			next_char = *cptr;
		}
	}



void compile(void)
	{
	short in_keywords = 0;
	short quit = 0;
	unsigned char keyword[513];
	unsigned char *cptr;
	struct key *key;

	yygetchar();
	do
		{
		if (!cur_char)
			{
			if (!next_char)
				{
				if (in_keywords)
					printf("Error (line %u): Unexpected EOF while in keyword braces!\n",yylineno);
				quit = 1;
				}
			else
				yygetchar();
			}
		else if (!in_keywords)
			{
			if (cur_char == '{')
				{
				if (next_char != '{')
					in_keywords = 1;
				else
					{
					fprintf(ansifd,"{");
					fprintf(asciifd,"{");
					yygetchar();
					}
				}
			else
				{
				fprintf(ansifd,"%c",cur_char);
				fprintf(asciifd,"%c",cur_char);
				}
			yygetchar();
			}
		else
			{
			if (cur_char == '}')
				{
				in_keywords = 0;
				yygetchar();
				}
			else if (cur_char == '{')
				{
				printf("Warning (line %u): Ignoring unexpected \"{\" within keyword braces!\n");
				yygetchar();
				}
			else
				{
				if (isalpha(cur_char))
					{
					cptr = keyword;
					while (isalnum(cur_char))
						{
						*cptr++ = cur_char;
						yygetchar();
						}
					*cptr = '\0';
					key = translate_keyword(keyword);
					if (key)
						{
						fprintf(ansifd,"%s",key->key_sequence);
						if (!key->key_ansionly)
							fprintf(asciifd,"%s",key->key_sequence);
						}
					 else
						printf("Error (line %u): Invalid keyword \"%s\" provided!\n",yylineno,keyword);
					}
				else
					yygetchar();
				}
			}
		}
	while (!quit);
	}



