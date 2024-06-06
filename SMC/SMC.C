/* smc.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 December 1990
**
** Revision Information: $Logfile:   G:/simplex/smc/vcs/smc.c_v  $
**                       $Date:   25 Oct 1992 14:25:30  $
**                       $Revision:   1.7  $
**
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "smc.h"



int main(int argc,char *argv[])
	{
	char buffer[120];
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	int which = 0;
	int count;
	char *cptr;
	char *filename = NULL;

	fprintf(stderr,"SMC (v %d.%02d of %s): Simplex Menu Compiler and Decompiler.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1990-91, Chris Laforet Software and Chris Laforet.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (argc >= 2)
		{
		for (count = 1; count < argc; count++)
			{
			cptr = argv[count];
			if (*cptr == '/' || *cptr == '-')
				{
				++cptr;
				if (*cptr == 'd' || *cptr == 'D')
					which = 1;
				else
					printf("Unknown option \"%s\" on command line.\n",cptr);
				}
			else if (!filename)
				filename = argv[count];
			}

		if (!filename)
			error("No filename provided",FATAL);
		else
			{
			strlwr(filename);
			_splitpath(filename,drive,path,fname,ext);
			if (which)
				{
				strcpy(ext,"mnu");
				_makepath(buffer,drive,path,fname,ext);
				if (yyfile = fopen(buffer,"rb"))
					decompile(fname);
				else 
					{
					sprintf(buffer,"Unable to open menu file \"%s.smc\"",fname);
					error(buffer,FATAL);
					}
				}
			else
				{
				strcpy(ext,"smc");
				_makepath(buffer,drive,path,fname,ext);
				if (yyfile = fopen(buffer,"r"))
					{
					printf("Compiling \"%s.smc\":\n",fname);
					_makepath(buffer,drive,path,"","");
					if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
						strcat(buffer,P_SSEP);
					parse_file(buffer);
					free_define();
					}
				else
					{
					sprintf(buffer,"Unable to open source file \"%s.smc\"",fname);
					error(buffer,FATAL);
					}
				}
			}
		}
	else
		{
		printf("Usage is: \"SMC source.smc\" to compile menu source to menus,\n");
		printf("      or: \"SMC /d menu.mnu\" to decompile menu file to source.\n\n");
		printf("Listing of Menu Types:\n---------------------\n\n");
		for (count = 0; count < NUM_MENUS; count++)
			printf("  Type %u: %s\n",mtype[count].m_type,mtype[count].m_descrip);
		printf("\n");
		}
	return 0;
	}
