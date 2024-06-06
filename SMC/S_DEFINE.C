/* s_define.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 3 December 1990
**
** Revision Information: $Logfile:   G:/simplex/smc/vcs/s_define.c_v  $
**                       $Date:   25 Oct 1992 14:25:34  $
**                       $Revision:   1.7  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smc.h"



struct def **defs = NULL;
int cur_defs = 0;
int max_defs = 0;



void add_define(int type,char *name,char *define)
	{
	if (cur_defs >= max_defs)
		{
		if (!(defs = realloc(defs,(max_defs += 10) * sizeof(struct def *))))
			error("Out of memory for loading define",FATAL);
		}
	if (!(defs[cur_defs] = malloc(sizeof(struct def))))
		error("Out of memory for loading define",FATAL);
	if (!(defs[cur_defs]->def_name = malloc((strlen(name) + 1) * sizeof(char))))
		error("Out of memory for loading define",FATAL);
	strcpy(defs[cur_defs]->def_name,name);
	if (!(defs[cur_defs]->def_define = malloc((strlen(define) + 1) * sizeof(char))))
		error("Out of memory for loading define",FATAL);
	strcpy(defs[cur_defs]->def_define,define);
	defs[cur_defs]->def_type = type;
	++cur_defs;
	}



struct def *get_define(char *name)
	{
	int count;

	for (count = 0; count < cur_defs; count++)
		{
		if (!stricmp(defs[count]->def_name,name))
		 	return defs[count];
		}
	return NULL;
	}



void free_define(void)
	{
	int count;

	if (max_defs)
		{
		for (count = 0; count < cur_defs; count++)
			{
			free(defs[count]->def_name);
			free(defs[count]->def_define);
			free(defs[count]);
			}
		free(defs);
		}
	}
