
#include <stdio.h>
#include <stdlib.h>


#define TEMPNAME				"filetrim.$$$"



main (int argc, char *argv[])
	{
    FILE *DataFile, *DataFile2;
    char newbuf[256];
    int Count=0, Step=0, MaxLines = 400;

	if (argc < 2)
    	{
	    printf ("\r\n\n Usage:  FileTrim [filename] [lines_left] \n\n");
    	exit (1);
	    }

	MaxLines = atoi (argv [2]);
	if (DataFile = fopen (argv[1], "r"))            /* if it's there...*/
    	{
    	printf ("\r\n");
    	while (fgets ( newbuf, 128, DataFile) != NULL)
        	{
        	Count++;
        	printf ( "\rFileTrim:  %4i ",Count);
        	}

    	if (Count <= MaxLines)
        	{
        	printf ("\rFILETRIM: Nothing to do -- file already that small (%u).  Yawn.\r\n",Count);
        	exit (1);
        	}
    	printf ("\r%u lines found - trimming off %i ... ", Count, Count - MaxLines);

		fseek(DataFile,0L,SEEK_SET);

    	for (Step = 1; Step <= Count - MaxLines; Step++)
        	fgets (newbuf, 128, DataFile);

    	if (!(DataFile2 = fopen (TEMPNAME, "w")))
			{
	    	printf ("\r\nFILETRIM:  Unable to create temp file. \r\n\n", argv [1]);
			return 1;
			}
	   	while (fgets (newbuf, 256, DataFile))
	       	fprintf (DataFile2,"%s",newbuf);

    	fclose (DataFile2);
    	fclose (DataFile);
	   	remove(argv[1]);
    	rename(TEMPNAME,argv [1]);
    	printf("Done!\r\n");
    	}
	else
    	{
    	printf ( "\r\nFILETRIM:  Target file %s not found. \r\n\n", argv [1]);
		return 1;
    	}
	return 0;
	}

