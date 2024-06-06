/* s_sig.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 11 June 1990
**
** Revision Information: $Logfile:   G:/simplex/smail/vcs/s_sig.c_v  $
**                       $Date:   25 Oct 1992 14:18:26  $
**                       $Revision:   1.12  $
**
*/



#include <stdio.h>
#include <ctype.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef PROTECTED
	#define INCL_DOSDATETIME
	#include <os2.h>
#endif
#include "smail.h"



#define docrc16(value,data) 		(unsigned int)(crc16[((value >> 8) ^ (unsigned int)data) & 0xff] ^ (value << 8))



struct sigh **sighead = NULL;
int cur_sighead = 0;
int max_sighead = 0;
int sig_days = 0;



unsigned int crc16[256] =		/* the CCITT 16-bit CRC precalculated values by CML */
	{
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
		0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
		0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
		0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
		0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
		0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
		0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
		0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
		0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
		0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
		0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
		0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
		0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
		0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
		0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
		0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
		0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
		0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
		0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
		0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
		0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
		0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
		0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
		0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
		0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
		0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
		0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
		0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
		0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
		0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
		0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
	};



/* Signature file layout:
**
** Signiture file header, 21 chars with nul-terminated string.
**                        1 int with total number of echo tags.
**
** 1 long offset to start of echo tag chain.
**
** Echo tag chain:
**
** 1 long offset to start of NEXT echo tag chain.
** 1 char length of echo tag string.
** length * sizeof(char) for echo tag string, not NUL terminated.
** 1 int for number of signatures in this block.
** number * sizeof(struct sig) for signatures in block.
** 1 int for number of sigs in next block (non-0).
** 1 long for offset to next block (non-0).
**
**    This repeats until all zeros:
** 
**       number * sizeof(struct sig) for signatures in block.
**       1 int for number of sigs in next block (non-0).
**       1 long for offset to next block (non-0).
**      
** 
*/        



void make_sigmask(int area,char *pattern,long *prev,long *last,FILE *fd)
	{
	struct sig tsig;
	long offset = 0L;
	long prevoffset;
	long lastoffset;
	int total;
	int count;
	int byte;
	int bit;
	int quit = 0;

	lastoffset = ftell(fd);
	do
		{
		prevoffset = lastoffset;
		lastoffset = ftell(fd);
		if (fread(&total,sizeof(int),1,fd))
			{
			if (fread(&offset,sizeof(long),1,fd))
				{
				if (offset)
					{
					fseek(fd,offset,SEEK_SET);
					for (count = 0; count < total && !quit; count++)
						{
						if (!fread(&tsig,sizeof(struct sig),1,fd))
							quit = 1;
						else
							{
							byte = tsig.sig_crc1 >> 6;	/* divide by 64 to find byte byte bit is in */
							bit = (tsig.sig_crc1 & 0x3e) >> 3;	/* the bit we are interested in */
							pattern[byte & 0x3ff] |= (unsigned char)(1 << bit);
							++areas[area]->area_sigs;
							}
						}
					}
				else
					quit = 1;		/* logical end */
				}
			else
				quit = 1;
			}
		else
			quit = 1;
		}
	while (!quit);
	*prev = prevoffset;
	*last = lastoffset;
	}



void load_sigfile(char *sigfile)
	{
	struct sh tsh;
	struct sf *sfptr;
	struct sigh *sighptr;
	unsigned char tlen;
	char buffer[120];
	long offset;
	int invalid = 0;
	int count;
	int found;
	int area;
	FILE *fd;

	sprintf(buffer,"%s%s",bbspath,sigfile);
	if (fd = fopen(buffer,"r+b"))
		{
		if (fread(&tsh,sizeof(struct sh),1,fd))
			{
			if (!strcmp(tsh.sh_signature,"SMAIL SIGNATURE FILE"))
				{
				fprintf(stderr,"Loading signature file %s\n",sigfile);
				++sig_days;

				if (!fread(&offset,sizeof(long),1,fd))		/* grab the first jump to first signature group */
					offset = (long)sizeof(struct sh) + (long)sizeof(long);

				while (offset)
					{
					if (fseek(fd,offset,SEEK_SET))
						break;
					offset = 0L;
					if (!fread(&offset,sizeof(long),1,fd))
						break;
					if (!fread(&tlen,sizeof(char),1,fd))
						break;
					if (tlen && (int)tlen < sizeof(buffer))
						{
						if (fread(buffer,(int)tlen * sizeof(char),1,fd))
							{
							buffer[(int)tlen] = '\0';
							fprintf(stderr,"\rLoading: \"%s\"....          \r",buffer);

							for (count = 0, found = 0; count < cur_areas; count++)
								{
								if (!stricmp(areas[count]->area_name,buffer))
									{
									area = count;
									found = 1;
									break;
									}
								}
							if (found)
								{
								for (count = 0, found = 0; count < cur_sighead; count++)
									{
									if (area == sighead[count]->sigh_area)
										{
										found = 1;
										sighptr = sighead[count];
										break;
										}
									}

								if (!found)
									{
									if (cur_sighead >= max_sighead)
										{
										if (!(sighead = realloc(sighead,(max_sighead += 25) * sizeof(struct sigh *))))
											{
											fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for signatures.\n");
											exit(1);
											}
										}
									if (!(sighead[cur_sighead] = malloc(sizeof(struct sigh))))
										{
										fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for signatures.\n");
										exit(1);
										}
									sighead[cur_sighead]->sigh_area = area;
									memset(sighead[count]->sigh_mask,0,1024);
									sighead[cur_sighead]->sigh_files = NULL;
									sighead[cur_sighead]->sigh_curfiles = 0;
									sighead[cur_sighead]->sigh_maxfiles = 0;
									sighptr = sighead[cur_sighead];
									++cur_sighead;
									}

								if (sighptr->sigh_curfiles >= sighptr->sigh_maxfiles)
									{
									if (!(sighptr->sigh_files = realloc(sighptr->sigh_files,(sighptr->sigh_maxfiles += 10) * sizeof(struct sf *))))
										{
										fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for signatures.\n");
										exit(1);
										}
									}

								if (!(sighptr->sigh_files[sighptr->sigh_curfiles] = malloc(sizeof(struct sf))))
									{
									fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for signatures.\n");
									exit(1);
									}
								sfptr = sighptr->sigh_files[sighptr->sigh_curfiles];

								if (!(sfptr->sf_file = malloc((strlen(sigfile) + 1) * sizeof(char))))
									{
									fprintf(stderr,"Fatal Error: Out of memory...unable to allocate for signatures.\n");
									exit(1);
									}
								strcpy(sfptr->sf_file,sigfile);
								sfptr->sf_start = ftell(fd);
								make_sigmask(area,sighptr->sigh_mask,&sfptr->sf_prev,&sfptr->sf_end,fd);
								++sighptr->sigh_curfiles;
								}
							}
						else
							break;
						}
					}
				}
			else
				invalid = 1;
			}
		else
			invalid = 1;
		fclose(fd);

		if (invalid)
			{
			fprintf(stderr,"Error: Invalid signature file \"%s\" - Deleting.\n",sigfile);
			unlink(buffer);
			}
		else 
			fprintf(stderr,"\r                                   \r",buffer);
		}
	else
		fprintf(stderr,"Error: Unable to open possible signature file \"%s\".\n",sigfile);
	}



void load_signatures(void)
	{
	struct fi tfi;
	char buffer[120];
	int rtn;

	sprintf(buffer,"%ssmail*.sig",bbspath);

	rtn = get_firstfile(&tfi,buffer);
	while (rtn)
		{
		load_sigfile(tfi.fi_name);
		rtn = get_nextfile(&tfi);
		}
	}



void check_sigfile(char *sigfile,int tmin,int tmax)
	{
	struct sh tsh;
	char buffer[120];
	int date;
	int kill = 0;
	FILE *fd;

	sprintf(buffer,"%s%s",bbspath,sigfile);
	if (fd = fopen(buffer,"r+b"))
		{
		if (fread(&tsh,sizeof(struct sh),1,fd))
			{
			if (!strcmp(tsh.sh_signature,"SMAIL SIGNATURE FILE"))
				{
				date = atoi(sigfile + 5);
				if (tmin > tmax)
					{
					tmax += 366;
					date += 366;
					}
				if (date <= tmin || date > tmax)
					kill = 1;
				}
			}
		fclose(fd);
		}
	if (kill)
		{
		fprintf(stderr,"\rKilling %s....",sigfile);
		unlink(buffer);
		}
	}



void clean_signatures(int days)
	{
	struct fi tfi;
	char buffer[120];
    DATE_T today;
	DATE_T prev;
	int date;
	int count;
	int rtn;

	fprintf(stderr,"Cleaning signatures older than %d day%s....\n",days,days != 1 ? "s" : "");
	load_datetime(&date,(TIME_T *)&count);
	today = date_to_julian(date);
	prev = today;
	for (count = 0; count < days; count++)
		prev = prev_julian(prev);
	today &= 0x1ff;
	prev &= 0x1ff;

	sprintf(buffer,"%ssmail*.sig",bbspath);

	rtn = get_firstfile(&tfi,buffer);
	while (rtn)
		{
		check_sigfile(tfi.fi_name,(int)prev,(int)today);
		rtn = get_nextfile(&tfi);
		}

	fprintf(stderr,"\rFinished cleaning signatures...\n");
	}



void add_signature(int area,struct sig *sigptr)
	{
	struct sigh *sighptr;
	struct sf *sfptr;
	struct sh tsh;
	unsigned char tlen;
	char buffer[120];
	char fname[20];
	long offset;
	long last;
	long prev;
	long end;
	unsigned int today;
	DATE_T tdate;
	int added = 0;
	int found = 0;
	int ok = 0;
	int count;
	int kount;
	int tval;
	int byte;
	int bit;
	FILE *fd;

	load_datetime(&tdate,(TIME_T *)&count);
	today = date_to_julian(tdate);
	sprintf(fname,"smail%03d.sig",today & 0x1ff);
	for (count = 0; count < cur_sighead && !found; count++)
		{
		for (kount = 0; kount < sighead[count]->sigh_curfiles; kount++)
			{
			if (!stricmp(sighead[count]->sigh_files[kount]->sf_file,fname))
				{
				found = 1;
				break;
				}
			}
		}
	sprintf(buffer,"%s%s",bbspath,fname);
	if (found)
		{
		if (fd = fopen(buffer,"r+b"))
			{
			if (!fread(&tsh,sizeof(struct sh),1,fd))
				{
				printf("Error: Invalid signature in signature file.\n");
				fclose(fd);
				fd = NULL;
				}
			else if (strcmp(tsh.sh_signature,"SMAIL SIGNATURE FILE"))
				{
				printf("Error: Invalid signature in signature file.\n");
				fclose(fd);
				fd = NULL;
				}
			}
		}
	if (!fd || !found)
		{
		if (fd = fopen(buffer,"w+b"))
			{
			strcpy(tsh.sh_signature,"SMAIL SIGNATURE FILE");
			tsh.sh_areas = 1;
			if (!fwrite(&tsh,sizeof(struct sh),1,fd))
				{
				printf("Error: Unable to write header to file \"%s\"!\n",fname);
				fclose(fd);
				unlink(buffer);
				fd = NULL;
				}
			offset = 0L;
			if (!fwrite(&offset,sizeof(long),1,fd))
				{
				printf("Error: Unable to write header to file \"%s\"!\n",fname);
				fclose(fd);
				unlink(buffer);
				fd = NULL;
				}
			}
		}

	if (fd)
		{
		if (found)
			{
			found = 0;
			for (count = 0; count < cur_sighead; count++)
				{
				for (kount = 0; kount < sighead[count]->sigh_curfiles; kount++)
					{
					if (area == sighead[count]->sigh_area)
						{
						if (!stricmp(sighead[count]->sigh_files[kount]->sf_file,fname))
							{
							sighptr = sighead[count];
							sfptr = sighptr->sigh_files[kount];
							found = 1;
							break;
							}
						}
					}
				}
			}

		fseek(fd,0L,SEEK_END);
		end = ftell(fd);
		if (found)
			{
			if (sfptr->sf_end + (long)(sizeof(int) + sizeof(long)) == end)	/* are we lucky, i.e. the last entry is from our current area! */
				{
				offset = 0L;
				if (fwrite(&offset,sizeof(long),1,fd))
					{
					fseek(fd,sfptr->sf_end,SEEK_SET);
					fwrite(sigptr,sizeof(struct sig),1,fd);
					tval = 0;
					fwrite(&tval,sizeof(int),1,fd);
					fseek(fd,sfptr->sf_prev,SEEK_SET);
					fread(&tval,sizeof(int),1,fd);
					++tval;
					fseek(fd,sfptr->sf_prev,SEEK_SET);
					fwrite(&tval,sizeof(int),1,fd);
					fseek(fd,0L,SEEK_END);
					sfptr->sf_end = ftell(fd) - (long)(sizeof(int) + sizeof(long));
					added = 1;
					}
				else
					chsize(fileno(fd),end);		/* skip it since we cannot add the requiste bytes! */
				}
			else
				{
				if (fwrite(sigptr,sizeof(struct sig),1,fd))
					{
					tval = 0;
 					if (fwrite(&tval,sizeof(int),1,fd))
						{
						offset = 0L;
						if (fwrite(&offset,sizeof(long),1,fd))
							{
							fseek(fd,sfptr->sf_end,SEEK_SET);
							tval = 1;
							fwrite(&tval,sizeof(int),1,fd);		/* point to the new addition */
							fwrite(&end,sizeof(long),1,fd);
							sfptr->sf_prev = sfptr->sf_end;
							sfptr->sf_end = end + (long)(sizeof(struct sig));
							added = 1;
							ok = 1;
							}
						}
					}
				if (!ok)
					chsize(fileno(fd),end);		/* skip it since we cannot add the requiste bytes! */
				}
			}
		else
			{
			++sig_days;
			fseek(fd,(long)sizeof(struct sh),SEEK_SET);
			offset = ftell(fd);
			do
				{
				last = offset;
				if (fread(&offset,sizeof(long),1,fd) && offset)
					{
					if (fseek(fd,offset,SEEK_SET))
						offset = 0L;
					}
				else
					offset = 0L;
				}
			while (offset);

			fseek(fd,end,SEEK_SET);
			offset = 0L;
			if (fwrite(&offset,sizeof(long),1,fd))
				{
				tlen = (char)(int)strlen(areas[area]->area_name);
				if (fwrite(&tlen,sizeof(char),1,fd))
					{
					if (fwrite(areas[area]->area_name,(int)tlen * sizeof(char),1,fd))
						{
						tval = 1;
						prev = ftell(fd);
						if (fwrite(&tval,sizeof(int),1,fd))
							{
							offset = ftell(fd) + (long)sizeof(long);
							if (fwrite(&offset,sizeof(long),1,fd))
								{
								if (fwrite(sigptr,sizeof(struct sig),1,fd))
									{
									tval = 0;
									if (fwrite(&tval,sizeof(int),1,fd))
										{
										offset = 0L;
										if (fwrite(&offset,sizeof(long),1,fd))
											{
											fseek(fd,last,SEEK_SET);
											fwrite(&end,sizeof(long),1,fd);		/* point to new area header */

											for (count = 0, found = 0; count < cur_sighead; count++)
												{
												if (area == sighead[count]->sigh_area)
													{
													found = 1;
													break;
													}
												}

											if (!found)
												{
												if (cur_sighead >= max_sighead)
													{
													if (!(sighead = realloc(sighead,(max_sighead += 25) * sizeof(struct sigh *))))
														{
														fprintf(stderr,"Error: Out of memory...unable to allocate for signatures.\n");
														exit(1);
														}
													}
												if (!(sighead[cur_sighead] = malloc(sizeof(struct sigh))))
													{
													fprintf(stderr,"Error: Out of memory...unable to allocate for signatures.\n");
													exit(1);
													}
												sighead[cur_sighead]->sigh_area = area;
												memset(sighead[count]->sigh_mask,0,1024);
												sighead[cur_sighead]->sigh_files = NULL;
												sighead[cur_sighead]->sigh_curfiles = 0;
												sighead[cur_sighead]->sigh_maxfiles = 0;
												sighptr = sighead[cur_sighead];
												count = cur_sighead;
												++cur_sighead;
												}
											else
												sighptr = sighead[count];

											if (sighptr->sigh_curfiles >= sighptr->sigh_maxfiles)
												{
												if (!(sighptr->sigh_files = realloc(sighptr->sigh_files,(sighead[count]->sigh_maxfiles += 10) * sizeof(struct sf *))))
													{
													fprintf(stderr,"Error: Out of memory...unable to allocate for signatures.\n");
													exit(1);
													}
												}

											if (!(sighptr->sigh_files[sighptr->sigh_curfiles] = malloc(sizeof(struct sf))))
												{
												fprintf(stderr,"Error: Out of memory...unable to allocate for signatures.\n");
												exit(1);
												}

											sfptr = sighptr->sigh_files[sighptr->sigh_curfiles];
											if (!(sfptr->sf_file = malloc((strlen(fname) + 1) * sizeof(char))))
												{
												fprintf(stderr,"Error: Out of memory...unable to allocate for signatures.\n");
												exit(1);
												}
											strcpy(sfptr->sf_file,fname);
											sfptr->sf_start = prev;
											sfptr->sf_prev = prev;
											fseek(fd,0L,SEEK_END);
											sfptr->sf_end = ftell(fd) - (long)(sizeof(int) + sizeof(long));
											++sighptr->sigh_curfiles;

											added = 1;
											ok = 1;

											++tsh.sh_areas;			/* adjust header */
											fseek(fd,0L,SEEK_SET);
											fwrite(&tsh,sizeof(struct sh),1,fd);
											}
										}
									}
								}
							}
						}
					}
				}
			if (!ok)
				chsize(fileno(fd),end);		/* skip it since we cannot add the requiste bytes! */
			}

		if (added)
			{
			byte = sigptr->sig_crc1 >> 6;		/* divide by 64 to find byte byte bit is in */
			bit = (sigptr->sig_crc1 & 0x3e) >> 3;	/* the bit we are interested in */
			sighptr->sigh_mask[byte & 0x3ff] |= (unsigned char)(1 << bit);
			++areas[area]->area_sigs;
			}
		fclose(fd);
		}
	else
		printf("Error: Unable to open/create signature file \"%s\"!\n",fname);
	}



struct sig *check_signature(int area,char *to,char *from,char *subject,char *body,int bodylen)
	{
	static struct sig tsig;
	struct sig tsig1;
	unsigned char mask;
	char buffer[120];
	char *cptr;
	long offset;
	int skip = 0;
	int total;
	int count;
	int kount;
	int counter;
	int quit;
	int byte;
	int bit;
	int match = 0;
	FILE *fd;

	tsig.sig_crc1 = 0xffff;		/* precondition CRC's */
	tsig.sig_crc2 = 0xffff;		/* precondition CRC's */
	cptr = body;
	for (count = 0; count < bodylen; count++)
		{
		if (*cptr == '\x1')		/* ^A kludge - skip to end of line */
			skip = 1;
		else if (*cptr == '\r' || *cptr == '\n' || *cptr == '\x8d')
			skip = 0;
		if (!skip && !isspace(*cptr))
			tsig.sig_crc1 = docrc16(tsig.sig_crc1,*cptr);
		++cptr;
		}
	cptr = to;
	while (*cptr)
		{
		tsig.sig_crc2 = docrc16(tsig.sig_crc2,*cptr);
		++cptr;
		}
	cptr = from;
	while (*cptr)
		{
		tsig.sig_crc2 = docrc16(tsig.sig_crc2,*cptr);
		++cptr;
		}
	cptr = subject;
	while (*cptr)
		{
		tsig.sig_crc2 = docrc16(tsig.sig_crc2,*cptr);
		++cptr;
		}

	byte = tsig.sig_crc1 >> 6;	/* divide by 64 to find byte byte bit is in */
	bit = (tsig.sig_crc1 & 0x3e) >> 3;	/* the bit we are interested in */
	mask = (unsigned char)(1 << bit);

	for (count = 0; count < cur_sighead; count++)
		{
		if (area == sighead[count]->sigh_area)
			{
			if (sighead[count]->sigh_mask[byte & 0x3ff] & mask)
				{
				for (kount = 0; kount < sighead[count]->sigh_curfiles; kount++)
					{
					sprintf(buffer,"%s%s",bbspath,sighead[count]->sigh_files[kount]->sf_file);
					if (fd = fopen(buffer,"r+b"))
						{
						quit = 0;
						fseek(fd,sighead[count]->sigh_files[kount]->sf_start,SEEK_SET);
						do
							{
							if (fread(&total,sizeof(int),1,fd))
								{
								if (fread(&offset,sizeof(long),1,fd))
									{
									fseek(fd,offset,SEEK_SET);
									for (counter = 0; counter < total && !quit; counter++)
										{
										if (!fread(&tsig1,sizeof(struct sig),1,fd))
											quit = 1;
										else if (!(tsig1.sig_crc1 ^ tsig.sig_crc1))		/* if values are equal, XORing will return 0 */
											{
						 					if (!(tsig1.sig_crc2 ^ tsig.sig_crc2))		/* if values are equal, XORing will return 0 */
												{
												match = 1;
												quit = 1;
												}
											}
										}
									}
								else
									quit = 1;
								}
							else
								quit = 1;
							}
						while (!quit);
						fclose(fd);
						}
					}
				}
			break;
			}
		}
	if (match)
		return NULL;
	return &tsig;
	}



void free_signatures(void)
	{
	int count;
	int kount;

	if (max_sighead)
		{
		for (count = 0; count < cur_sighead; count++)
			{
			for (kount = 0; kount < sighead[count]->sigh_curfiles; kount++)
				{
				free(sighead[count]->sigh_files[kount]->sf_file);
				free(sighead[count]->sigh_files[kount]);
				}
			free(sighead[count]);
			}
		free(sighead);
		sighead = NULL;
		cur_sighead = 0;
		max_sighead = 0;
		}
	}



void report_traffic(void)
	{
	unsigned long tlong;
	char buffer[10];
	char date[9];
	char time[9];
	int count;

	if (sig_days)
		{
		_strdate(date);
		_strtime(time);
		printf("    SMAIL (v %d.%02d) Echomail Traffic Report for Past %d Days.\n",MAJOR_VERSION,MINOR_VERSION,sig_days);
		printf("    Generated on %s at %s\n\n",date,time);
		printf("    Area Tag                             Total Msgs  Msgs/Day\n");
		printf("    -----------------------------------  ----------  --------\n");
		for (count = 0; count < cur_areas; count++)
			{
			tlong = areas[count]->area_sigs * 100L / (long)sig_days;
			sprintf(buffer,"%lu.%02lu",tlong / 100L,tlong % 100L);
			printf("    %-35.35s  %8lu    %8.8s\n",areas[count]->area_name,areas[count]->area_sigs,buffer);
			}
		printf("\n\n");
		}
	}

