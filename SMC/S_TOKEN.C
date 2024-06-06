/* s_token.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 December 1990
**
** Revision Information: $Logfile:   G:/simplex/smc/vcs/s_token.c_v  $
**                       $Date:   25 Oct 1992 14:25:36  $
**                       $Revision:   1.7  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "smc.h"



typedef enum
	{
	_START,
	_INCOMMENT,
	_INSQUOTE,
	_INDQUOTE,
	_INCONSTANT,
	_INIDENT,

	_COMMENT,
	_STRING,
	_CONSTANT,
	_IDENT,
	_SYMBOL,
	_OTHER,

	_EOFILE,
	_BAD
	} state;


static char curchar;		 /* current character */
static char nextchar;		 /* succeeding character */
static char *ptr = NULL;
static int yygettokenfirst = 1;
char yyline[LINE_LEN];
char yytext[LINE_LEN];
FILE *yyfile;
int line = 0;



static char yygetch()	  /* get next char from yyin */
	{
	if (!ptr || *ptr == '\n' || !*ptr)
		{
		if ((ptr = fgets(yyline,LINE_LEN,yyfile)) != NULL)
			{
			++line;
			nextchar = *(ptr + 1);
			return (curchar = *ptr);
			}
		else		/* at EOFILE */
			return (curchar = nextchar = '\0');
		}
	if (ptr == NULL)	  /* at EOFILE */
		return (curchar = nextchar = '\0');
	nextchar = *(ptr + 2);
	return (curchar = *++ptr);
	}



int yygettoken(void)	  /* get next token from input stream */
	{
	struct def *defptr;
	char buffer[LINE_LEN];
	state curstate;
	long tlong;
	int done;
	int bad;
	int ret = 0;

	if (yygettokenfirst)
		{
		yygetch();
		yygettokenfirst = 0;
		}
	do
		{
		curstate = _START;
		yytext[0] = '\0';
		bad = 0;
		done = 0;
		while (!done)
			{
			switch (curstate)
				{
				case _START:
					if (isspace(curchar))
						;		/* do nothing ... just keep scanning input */
					else if (curchar == '\0')		/* EOFILE */
						{
						curstate = _EOFILE;
						done++;
						}
					else if (curchar == '"')
						curstate = _INDQUOTE;
					else if (isalpha(curchar))	  /* beginning of identifier */
						{
						curstate = _INIDENT;
						strncat(yytext,&curchar,1);
						}
					else if (curchar == '/' && nextchar == '*') 	   /* start comment */
						{
						curstate = _INCOMMENT;
						yygetch();
						}
					else if (isdigit(curchar))
						{
						curstate = _INCONSTANT;
						strncat(yytext,&curchar,1);
						}
					else
						{
						curstate = _OTHER;
						strncat(yytext,&curchar,1);
						yygetch();
						done++;
						}
					break;
				case _INDQUOTE:
					if (curchar == '"')
						{
						curstate = _STRING;
						strncat(yytext,"\0",1);
						yygetch();
						done++;
						}
					else if (curchar == '\\' && nextchar == '"')
						{
						strncat(yytext,&nextchar,1);
						yygetch();
						}
					else if (curchar == '\0' || curchar == '\n')
						{
						curstate = _BAD;
						done++;
						}
					else
						strncat(yytext,&curchar,1);
					break;
				case _INCOMMENT:
					if (curchar == '*' && nextchar == '/')		   /* end of comment */
						{
						yygetch();
						curstate = _START;		/* should be _COMMENT but what use is it? */
						}
					else if (curchar == '/' && nextchar == '*')
						error("Illegal nesting of comments",WARNING);
					else if (curchar == '\0')
						error("Unexpected end of input",FATAL);
					break;
				case _INCONSTANT:
					if (isdigit(curchar))
						strncat(yytext,&curchar,1);
					else
						{
						curstate = _CONSTANT;
						done++;
						}
					break;
				case _INIDENT:
					if (isalnum(curchar))
						strncat(yytext,&curchar,1);
					else
						{
						curstate = _IDENT;
						done++;
						}
					break;
				}
			if (!done)
				yygetch();
			}
		switch (curstate)
			{
			case _BAD:
			case _OTHER:
				sprintf(buffer,"Invalid token \"%s\"",yytext);
				error(buffer,WARNING);
				bad = 1;
                break;
			case _EOFILE:
				ret = EOFILE;
				break;
			case _IDENT:
				if (!stricmp(yytext,"menu"))
					ret = MENU;
				else if (!stricmp(yytext,"prompt"))
					ret = PROMPT;
				else if (!stricmp(yytext,"line"))
					ret = LINE;
				else if (!stricmp(yytext,"color"))
					ret = COLOR;
				else if (!stricmp(yytext,"hilite"))
					ret = HILITE;
				else if (!stricmp(yytext,"key"))
					ret = KEY;
				else if (!stricmp(yytext,"type"))
					ret = TYPE;
				else if (!stricmp(yytext,"auto"))
					ret = AUTO;
				else if (!stricmp(yytext,"expert"))
					ret = EXPERT;
				else if (!stricmp(yytext,"data"))
					ret = DATA;
				else if (!stricmp(yytext,"priv"))
					ret = PRIV;
				else if (!stricmp(yytext,"flags"))
					ret = FLAGS;
				else if (!stricmp(yytext,"define"))
					ret = DEFINE;
				else
					{
					if (defptr = get_define(yytext))
						{
						strcpy(yytext,defptr->def_define);
						ret = defptr->def_type;
						}
					else 
						ret = IDENT;
					}
				break;
			case _STRING:
				ret = STRING;
				break;
			case _CONSTANT:
				tlong = atol(yytext);
				if (tlong > 0xffffL)
					{
					sprintf(buffer,"Integer \"%s\" is greater than 65535: truncating",yytext);
					error(buffer,WARNING);
					}
				ret = CONST;
				break;
			case _COMMENT:
				ret = COMMENT;
				break;
			}
		}
	while (bad);
	return ret;
	}
