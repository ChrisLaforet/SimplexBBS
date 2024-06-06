/* sansi.h
**
** Copyright (c) 1990, Christopher Laforet
** All Rights Reserved
**
** Started: 4 January 1990
**
** Revision Information: $Logfile:   G:/simplex/sansi/vcs/sansi.h_v  $
**                       $Date:   25 Oct 1992 14:12:42  $
**                       $Revision:   1.2  $
**
*/


#define MAJOR_VERSION			1
#define MINOR_VERSION			3


#define KEYWORDS				101


struct key
	{
	unsigned char *key_word;
	unsigned char *key_sequence;
	unsigned short key_ansionly;
	};


extern FILE *yyfd;
extern FILE *ansifd;
extern FILE *asciifd;
extern struct key keywords[KEYWORDS];


extern void compile(void);
