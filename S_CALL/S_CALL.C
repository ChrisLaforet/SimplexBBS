/* s_call.c
**
** Copyright (c) 1994, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 24 July 1994
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/

#define INCL_DOSFILEMGR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <os2.h>


char **args = NULL;


int main(int argc,char *argv[])
	{
	int count;
	int rtn;

	fprintf(stderr,"S_CALL: Changes directory and then calls unarchiver.\n");
	fprintf(stderr,"Copyright (c) 1994, Chris Laforet Software.  All Rights Reserved.\n\n");

	if (argc > 3)
		{
		if (argv[1][1] == ':')
			{
			if (DosSetDefaultDisk((long)(toupper(argv[1][0]) - 'A' + 1)))
				{
				fprintf(stderr,"Error: unable to set drive.\n");
				return 1;
				}
			}
		if (argv[1][strlen(argv[1]) - 1] == '\\')
			argv[1][strlen(argv[1]) - 1] = '\0';
		if (DosSetCurrentDir(argv[1]))
			{
			fprintf(stderr,"Error: unable to set path.\n");
			return 1;
			}

		if (!(args = malloc((argc - 1) * sizeof(char *))))
			{
			fprintf(stderr,"Error: out of memory for argument list.\n");
			return 1;
			}
		for (count = 0; count < argc - 2; count++)
			{
			if (!(args[count] = malloc(strlen(argv[count + 2]) + 1)))
				{
				fprintf(stderr,"Error: out of memory for argument list.\n");
				return 1;
				}
			strcpy(args[count],argv[count + 2]);
			}
		args[argc - 1] = NULL;

		rtn = spawnvp(P_WAIT,args[0],args);			/* spawn the archiver */
		if (rtn == -1)
			{
			fprintf(stderr,"Error: Failed to spawn archiver (%s).\n",args[0]);
			return 1;
			}
		else
			{
			fprintf(stderr,"Archiver return = %u\n",rtn);
			return rtn;
			}
		}
	fprintf(stderr,"Usage is: S_CALL new_directory arc'er_name (arguments)\n");
	return 1;
	}
