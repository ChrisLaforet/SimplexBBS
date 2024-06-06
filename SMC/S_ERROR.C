/* s_error.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 2 December 1990
**
** Revision Information: $Logfile:   G:/simplex/smc/vcs/s_error.c_v  $
**                       $Date:   25 Oct 1992 14:25:34  $
**                       $Revision:   1.7  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smc.h"



void error(char *message,int strategy)
	{
	char buffer[15];

   	sprintf(buffer,"Line %u",line);
	switch (strategy)
		{
		case WARNING:
			printf("Warning (%s): %s!\n",buffer,message);
			break;
		case FATAL:
			printf("Fatal Error (%s): %s!\n",buffer,message);
			exit(-1);
			break;
		default:
			error("Internal Error in Compiler!",FATAL);
			break;
		}
	}



