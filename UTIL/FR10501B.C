/* fr10501b.c
**
** Copyright (c) 1991, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 27 December 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <string.h>



#define COMB_AREA					1250



struct ocomb				/* combined message board structure */
	{
	int comb_user;				/* user number */
	char comb_areas[COMB_AREA];
	};



struct comb				/* combined message board structure */
	{
	int comb_user;				/* user number */
	int comb_flags;
	char comb_areas[COMB_AREA];
	};



struct ocomb ocomb;
struct comb comb;



int main(int argc,char *argv[])
	{
	FILE *sfd;
	FILE *dfd;

	fprintf(stderr,"FR10501B: Converts Simplex Files from pre-version 1.05.02.\n");
	fprintf(stderr,"Copyright (c) 1991, Chris Laforet.  All Rights Reserved.\n\n");

	fprintf(stderr,"Converting MSGCOMB.BBS.\n");
	unlink("msgcomb.bak");
	if (rename("msgcomb.bbs","msgcomb.bak"))
		{
		fprintf(stderr,"Error: Unable to find/rename msgcomb.bbs....Exiting.\n");
		return 1;
		}
	if (!(sfd = fopen("msgcomb.bak","rb")))
		{
		rename("msgcomb.bak","msgcomb.bbs");
		fprintf(stderr,"Error: Unable to open old msgcomb.bbs....Exiting.\n");
		return 1;
		}
	if (!(dfd = fopen("msgcomb.bbs","wb")))
		{
		rename("msgcomb.bak","msgcomb.bbs");
		fprintf(stderr,"Error: Unable to create new msgcomb.bbs....Exiting.\n");
		return 1;
		}

	while (fread(&ocomb,sizeof(struct ocomb),1,sfd))
		{
		memset(&comb,0,sizeof(struct comb));

		comb.comb_user = ocomb.comb_user;
		memcpy(comb.comb_areas,ocomb.comb_areas,COMB_AREA * sizeof(char));

		if (!fwrite(&comb,sizeof(struct comb),1,dfd))
			{
			fclose(dfd);
			unlink("msgcomb.bbs");
			rename("msgcomb.bak","msgcomb.bbs");
			fprintf(stderr,"Error: Unable to write new msgcomb.bbs....Exiting.\n");
			return 1;
			}
		}


	fclose(dfd);
	fclose(sfd);

	fprintf(stderr,"\aFinished!\n\n");
	return 0;
	}
