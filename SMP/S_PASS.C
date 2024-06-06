/* s_pass.c
**
** Copyright (c) 1993, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 28 September 1993
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "smp.h"



void read_passwords(FILE *fd)
	{
	char buffer[100];
	char passwd[9];
	char *cptr;
	char *cptr1;
	int tzone;
	int tnet;
	int tnode;

	fseek(fd,0L,SEEK_SET);
	while (fgets(buffer,sizeof(buffer),fd))
		{
		cptr = buffer;
		if (*cptr != ';')
			{
			while (*cptr && isspace(*cptr))		/* trim off leading whitespace */
				++cptr;

			if (*cptr)
				{
				cptr1 = cptr;
				while (*cptr && !isspace(*cptr))
					++cptr;
				if (*cptr)
					{
					*cptr++ = '\0';
					strncpy(passwd,cptr1,8);
					passwd[8] = (char)'\0';
					strupr(passwd);

					if (*cptr)
						{
						while (*cptr && isspace(*cptr))
							++cptr;
						if (*cptr)
							{
							tzone = zone;
							tnet = net;
							tnode = node;
							if (parse_address(cptr,&tzone,&tnet,&tnode))
								{
								if (cur_passwords >= max_passwords)
									{
									if (!(passwords = realloc(passwords,(max_passwords += 10) * sizeof(struct pp *))))
										{
										fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
										exit(2);
										}
									}
								if (!(passwords[cur_passwords] = malloc(sizeof(struct pp))))
									{
									fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
									exit(2);
									}
								if (!(passwords[cur_passwords]->pp_password = malloc(strlen(passwd) + 1)))
									{
									fprintf(stderr,"Fatal Error: Out of memory (%s, line %u)!\n",__FILE__,__LINE__);
									exit(2);
									}
								strcpy(passwords[cur_passwords]->pp_password,passwd);
								passwords[cur_passwords]->pp_zone = tzone;
								passwords[cur_passwords]->pp_net = tnet;
								passwords[cur_passwords]->pp_node = tnode;
								++cur_passwords;
								}
							}
						}
					}
				}
			}
		}
	}



