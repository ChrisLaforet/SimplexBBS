/* s_decomp.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 3 December 1990
**
** Revision Information: $Logfile:   G:/simplex/smc/vcs/s_decomp.c_v  $
**                       $Date:   25 Oct 1992 14:25:32  $
**                       $Revision:   1.7  $
**
*/


#include <stdio.h>
#include <string.h>
#include "smc.h"



char *get_flags(int flags)
	{
	static char flaglist[17];
	char *cptr;
	int count;
	int val = 1;

	memset(flaglist,0,17);
	cptr = flaglist;
	for (count = 0; count < 16; count++)
		{
		if (flags & val)
			*cptr++ = (char)('A' + count);
		val <<= 1;
		}
	return flaglist;
	}



int xlat_flags(char *string)
	{
	char *cptr;
	int template = 0;

	cptr = string;
	while (*cptr)
		{
		switch (*cptr)
			{
			case 'A':
			case 'a':
				template |= UF_A;
				break;
			case 'B':
			case 'b':
				template |= UF_B;
				break;
			case 'C':
			case 'c':
				template |= UF_C;
				break;
			case 'D':
			case 'd':
				template |= UF_D;
				break;
			case 'E':
			case 'e':
				template |= UF_E;
				break;
			case 'F':
			case 'f':
				template |= UF_F;
				break;
			case 'G':
			case 'g':
				template |= UF_G;
				break;
			case 'H':
			case 'h':
				template |= UF_H;
				break;
			case 'I':
			case 'i':
				template |= UF_I;
				break;
			case 'J':
			case 'j':
				template |= UF_J;
				break;
			case 'K':
			case 'k':
				template |= UF_K;
				break;
			case 'L':
			case 'l':
				template |= UF_L;
				break;
			case 'M':
			case 'm':
				template |= UF_M;
				break;
			case 'N':
			case 'n':
				template |= UF_N;
				break;
			case 'O':
			case 'o':
				template |= UF_O;
				break;
			case 'P':
			case 'p':
				template |= UF_P;
				break;
			}
		++cptr;
		}
	return template;
	}



void decompile(char *fname)
	{
	struct prompt tprompt;
	struct menu tmenu;
	char *cptr;
	int count;

	if (!fread(&tprompt,sizeof(struct prompt),1,yyfile))
		error("Unable to read prompt header from file",FATAL);
	if (tprompt.prompt_sig != 0xa442)
		error("Invalid menu file",FATAL);
	printf("/* Decompilation of menu file %s.mnu\n",fname);
	printf("**\n");
	printf("** Achieved by SMC (v %d.%02d)\n",MAJOR_VERSION,MINOR_VERSION);
	printf("*/\n\n\n");

	printf("menu \"%s\"\n\n",fname);

	printf("  prompt \"%s\"\n",tprompt.prompt_string);
	printf("  \tcolor %u\thilite %u\n\n",tprompt.prompt_color,tprompt.prompt_hilite);

	while (fread(&tmenu,sizeof(struct menu),1,yyfile))
		{
		for (count = 0; count < NUM_MENUS; count++)
			{
			if (mtype[count].m_type == tmenu.menu_type)
				break;
			}
		printf("  /* Line %d> Type: %s */\n",tmenu.menu_number,count < NUM_MENUS ? mtype[count].m_descrip : "Unknown/Invalid");
		printf("  line \"");
		cptr = tmenu.menu_line;
		while (*cptr)
			{
			if (*cptr == '"')
				putc('\\',stdout);
			putc(*cptr,stdout);
			++cptr;
			}
		printf("\"\n");

		printf("  \ttype %u%s%s\n",tmenu.menu_type,tmenu.menu_auto ? "\tauto" : "",tmenu.menu_expert ? "\texpert" : "");
		if (tmenu.menu_key || tmenu.menu_data[0])
			{
			printf("  ");
			if (tmenu.menu_key)
				printf("\tkey \"%c\"",tmenu.menu_key);
			if (tmenu.menu_data[0])
				{
				printf("\tdata \"");
				cptr = tmenu.menu_data;
				while (*cptr)
					{
					if (*cptr == '"')
						putc('\\',stdout);
					putc(*cptr,stdout);
					++cptr;
					}
				printf("\"\n");
				}
			printf("\n");
			}
		printf("  \tpriv %u\tflags \"%s\"\n",tmenu.menu_priv,get_flags(tmenu.menu_flags));
		printf("  \tcolor %u\thilite %u\n",tmenu.menu_color,tmenu.menu_hilite);
		printf("\n");
		}
	}


