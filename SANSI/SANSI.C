/* sansi.c
**
** Copyright (c) 1990, Christopher Laforet
** All Rights Reserved
**
** Started: 4 January 1990
**
** Revision Information: $Logfile:   G:/simplex/sansi/vcs/sansi.c_v  $
**                       $Date:   25 Oct 1992 14:12:40  $
**                       $Revision:   1.2  $
**
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sansi.h"


FILE *yyfd;
FILE *ansifd;
FILE *asciifd;


int main(int argc,char *argv[])
	{
	unsigned char buffer[100];
	unsigned char drive[_MAX_DRIVE];
	unsigned char dir[_MAX_DIR];
	unsigned char fname[_MAX_FNAME];
	unsigned char ext[_MAX_EXT];
	int count;

	fprintf(stderr,"SANSI (v %u.%02u of %s): SIMPLEX BBS ANSI Text File Compiler\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1990, Christopher Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");
	if (argc < 2)
		{
		printf("Usage is: SANSI textfile[.ext]\n\n");

		printf("SANSI keywords are:\n\n");
		for (count = 0; count < KEYWORDS; count++)
			printf("    %s\t%s\n",keywords[count].key_ansionly ? "" : keywords[count].key_sequence,keywords[count].key_word);

		printf("\n");
		return 1;
		}
	_splitpath(argv[1],drive,dir,fname,ext);
	if (!stricmp(ext,".ANS") || !stricmp(ext,".ASC"))
		{
		printf("Fatal Error: Source file cannot have an ANS or ASC extension.\n\n");
		return 1;
		}
	if (!(yyfd = fopen(argv[1],"rb")))
		{
		printf("Fatal Error: Unable to open source file.\n\n");
		return 1;
		}
	_makepath(buffer,drive,dir,fname,"ANS");			 
	if (!(ansifd = fopen(buffer,"wb")))
		{
		printf("Fatal Error: Unable to create ansi output file.\n\n");
		return 1;
		}
	_makepath(buffer,drive,dir,fname,"ASC");			 
	if (!(asciifd = fopen(buffer,"wb")))
		{
		printf("Fatal Error: Unable to create ascii output file.\n\n");
		return 1;
		}
	_makepath(buffer,"","",fname,ext);
	printf("Compiling %s:\n",buffer);
	compile();
	fclose(yyfd);
	fclose(ansifd);
	fclose(asciifd);
	return(0);
	}






