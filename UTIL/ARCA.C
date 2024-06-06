/* arca.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 24 May 1990
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <process.h>



#define MAJOR_VERSION			1
#define MINOR_VERSION			0


char **args;


int main(int argc,char *argv[])
	{
	int rtn;
	int delete = 0;
	int count;
	int kount;

	fprintf(stderr,"ARCA (v %d.%02d of %s): An ARCA replacement for OS/2.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1990, Chris Laforet Software.  All Rights Reserved.\n\n");
	if (argc > 3)
		{
		if (!(args = calloc(100,sizeof(char *))))
			{
			fprintf(stderr,"Error: Unable to allocate memory.\n");
			return 1;
			}
		args[0] = "ARC2.EXE";
		for (count = 1, kount = 2; count < argc; count++)
			{
			if (argv[count][0] == '-' || argv[count][0] == '/')
				{
				if (argv[count][1] == 'D' || argv[count][1] == 'd')
					delete = 1;
				else
					fprintf(stderr,"Invalid argument %s.\n",argv[count]);
				}
			else
				args[kount++] = argv[count];
			}
		if (kount < 4)
			goto usage;
		if (delete)
			args[1] = "m5";
		else 
			args[1] = "a5";
		args[kount] = NULL;
		if ((rtn = spawnvp(P_WAIT,args[0],args)) == -1)
			return 1;
		else
			return rtn;
		}
	else
		{
usage:
		fprintf(stderr,"This program calls ARC2.EXE from System Enhancements Associates.\n");
		fprintf(stderr,"ARC2.EXE must be located along the path.\n\n\n");
		fprintf(stderr,"Usage is: ARCA archive file [file...] [/D]\n\n");
		fprintf(stderr,"\twhere   archive   is the pathname to the file to create.\n");
		fprintf(stderr,"\t        file      is the pathname to the file to add.\n");
		fprintf(stderr,"\t        /D or /d  indicates to delete the files as they are added.\n\n");
		return 1;
		}
	return 0;
	}


