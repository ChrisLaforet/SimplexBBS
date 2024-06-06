/* sfac.c
**
** Copyright (c) 1992, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 11 June 1992
**
** Revision Information: $Logfile:   G:/simplex/sfac/vcs/sfac.c_v  $
**                       $Date:   25 Oct 1992 14:16:34  $
**                       $Revision:   1.0  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <signal.h>
#include <ctype.h>
#include <io.h>
#include "sfac.h"



char workbuffer[512];
char buffer[512];
char filesbbs[100];
char bbspath[100];
char uploader[41];



void signal_trap(int signo)
	{
	/* this function does nothing except receive
	** calls from SIGINT and SIGBREAK.  This is
	** done this way since not all compilers handle
	** SIG_IGN correctly.
	*/
	}



int main(int argc,char *argv[])
	{
	struct file tfile;
	struct fe tfe;
	struct fi tfi;
	char buffer1[40];
	char *cptr;
	char *cptr1;
	long lastpos;
	DATE_T today;
	int use_date = 0;
	int wild_flag;
	int found = 0;
	int kill_file = 0;
	int direction = 1;
	int count;
	int error = 0;
	int area = 0;
	FILE *fd;
	FILE *sfd;
	FILE *dfd;

	fprintf(stderr,"SFAC (Version %d.%02d of %s): Simplex File Area Listing Compiler.\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1992, Christopher Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");

	if (argc < 2)
		{
		fprintf(stderr,"Usage is: SFAC [-options] FileAreaNum\n");
		fprintf(stderr,"   where options can be:\n");
		fprintf(stderr,"          -d               use current date as upload date.\n"); 
		fprintf(stderr,"          -k               kill \"files.bbs\" after transferring data.\n");
		fprintf(stderr,"          -uuploader_name  set uploader name to \"uploader name\".\n");
		fprintf(stderr,"   otherwise:\n");
		fprintf(stderr,"          -f               create a files.bbs listing from filelist.bbs.\n"); 
		fprintf(stderr,"          FileAreaNum = file area to convert.\n\n"); 
		}
	else
		{
		uploader[0] = '\0';
		for (count = 1; count < argc; count++)
			{
			if (argv[count][0] == '/' || argv[count][0] == '-')
				{
				switch (argv[count][1])
					{
					case 'd':
					case 'D':
						use_date = 1;
						today = get_cdate();
						fprintf(stderr,"Notice: Using today's date as upload date.\n");
						break;

					case 'k':
					case 'K':
						kill_file = 1;
						fprintf(stderr,"Notice: Killing \"files.bbs\" after data transfer.\n");
						break;

					case 'u':
					case 'U':
						if (argv[count][2])
							{
							cptr = argv[count] + 2;
							cptr1 = uploader;
							while (*cptr)
								{
								if (*cptr == '_')
									*cptr1++ = ' ';
								else 
									*cptr1++ = *cptr;
								if (cptr1 >= (uploader + 39))
									break;
								++cptr;
								}
							*cptr1 = (char)'\0';

							fprintf(stderr,"Notice: Setting uploader to \"%s\".\n",uploader);
							}
						break;

					case 'F':
					case 'f':
						fprintf(stderr,"Notice: Translating \"filelist.bbs\" to  \"files.bbs\".\n");
						direction = 0;
						break;

					default:
						fprintf(stderr,"Warning: Invalid argument \"%s\" on command line.\n",argv[count]);
						break;

					}
				}
			else if (isdigit(argv[count][0]))
				area = atoi(argv[count]);
			else
				fprintf(stderr,"Warning: Invalid argument \"%s\" on command line.\n",argv[count]);
			}

		if (area)
			{
			if (cptr = getenv("SIMPLEX"))		/* now for our path */
				strcpy(buffer,cptr);
			else
				getcwd(buffer,sizeof(buffer));
			if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
				strcat(buffer,P_SSEP);
			sprintf(bbspath,buffer);

			signal(SIGINT,signal_trap);				/* first, down with Ctrl-C */
			signal(SIGBREAK,signal_trap);			/* then down with Ctrl-Break */

			strcat(buffer,"filearea.bbs");
			if (fd = fopen(buffer,"rb"))
				{
				while (fread(&tfile,sizeof(struct file),1,fd))
					{
					if (tfile.file_number == area)
						{
						printf("Preparing to convert area %d: \"%s\"....\n",tfile.file_number,tfile.file_areaname);
						found = 1;

						if (direction)
							{
							if (tfile.file_descname[0])				/* load and show the file header */
								strcpy(filesbbs,tfile.file_descname);
							else 
								strcpy(filesbbs,tfile.file_pathname);
							if (filesbbs[0])
								{
								if (filesbbs[strlen(filesbbs) - 1] != P_CSEP)
									strcat(filesbbs,P_SSEP);
								}
							strcat(filesbbs,"files.bbs");

							if (sfd = fopen(filesbbs,"r+b"))
								{
								printf("Opened \"files.bbs\"...\n");
								if (tfile.file_descname[0])				/* load and show the file header */
									strcpy(buffer,tfile.file_descname);
								else 
									strcpy(buffer,tfile.file_pathname);
								if (buffer[0])
									{
									if (buffer[strlen(buffer) - 1] != P_CSEP)
										strcat(buffer,P_SSEP);
									}
								strcat(buffer,"filelist.bbs");

								if (!(dfd = fopen(buffer,"r+b")))
									dfd = fopen(buffer,"wb");

								if (dfd)
									{
									printf("Opened \"filelist.bbs\"...\n");
									fseek(dfd,0L,SEEK_END);

									lastpos = ftell(dfd);		/* hang on to this position */

									while (!error && fgets(workbuffer,sizeof(workbuffer),sfd))
										{
										cptr = buffer;
										cptr1 = workbuffer;
										while (*cptr1)
											{
											if (*cptr1 != (char)'\x1a')
												*cptr++ = *cptr1++;
											else
												{
												*cptr++ = ' ';
												++cptr1;
												}
											}
										*cptr = (char)'\0';

										if (!isspace(buffer[0]) && buffer[0] != '-' && buffer[0] != '*' && buffer[0] != '?')
											{
											cptr = buffer;
											cptr1 = buffer1;
											wild_flag = 0;
											while (*cptr && !isspace(*cptr) && (cptr1 - buffer1) < 15)
												{
												if (*cptr == '*' || *cptr == '?')
													wild_flag = 1;
												*cptr1++ = *cptr++;
												}
											*cptr1 = '\0';
											if (!wild_flag && strlen(buffer1) <= 12)
												{
												while (*cptr && isspace(*cptr))
													++cptr;
												strupr(buffer1);

												strcpy(workbuffer,tfile.file_pathname);		/* get a template for testing */
												if (workbuffer[0])
													{
													if (workbuffer[strlen(workbuffer) - 1] != P_CSEP)
														strcat(workbuffer,P_SSEP);
													}
												strcat(workbuffer,buffer1);

												while (*cptr && isspace(*cptr))		/* strip spaces and tabs */
													++cptr;
												cptr1 = cptr;
												while (*cptr1 && *cptr1 >= 0x20)		/* end this with first control character! */
													++cptr1;
												*cptr1 = (char)'\0';

												if (get_firstfile(&tfi,workbuffer))
													{
													memset(&tfe,0,sizeof(struct fe));
													strcpy(tfe.fe_name,buffer1);
													strncpy(tfe.fe_descrip,cptr,256);
													tfe.fe_location = tfile.file_number;
													tfe.fe_priv = tfile.file_priv;
													tfe.fe_flags = tfile.file_flags;
													if (use_date)
														tfe.fe_uldate = (unsigned int)today;
													else 
														tfe.fe_uldate = (unsigned int)tfi.fi_date;
													strcpy(tfe.fe_uploader,uploader);

													if (!fwrite(&tfe,sizeof(struct fe),1,dfd))
														{
														fprintf(stderr,"Fatal Error: Unable to write to \"filelist.bbs\" - restoring file to original size!");

														fseek(dfd,lastpos,SEEK_SET);
														chsize(fileno(dfd),lastpos);

														error = 1;
														}
													}
												get_closefile();
												}
											}
										}
									fclose(dfd);
									}
								else
									{
									fprintf(stderr,"Fatal Error: Unable to open/create \"filelist.bbs\"!\n");
									return 1;
									}

								fclose(sfd);

								if (kill_file && !error)
									unlink(filesbbs);
								}
							else
								{
								fprintf(stderr,"Fatal Error: Unable to find/open \"files.bbs\"!\n");
								return 1;
								}
							}
						else			/* !direction */
							{
							if (tfile.file_descname[0])				/* load and show the file header */
								strcpy(filesbbs,tfile.file_descname);
							else 
								strcpy(filesbbs,tfile.file_pathname);
							if (filesbbs[0])
								{
								if (filesbbs[strlen(filesbbs) - 1] != P_CSEP)
									strcat(filesbbs,P_SSEP);
								}
							strcat(filesbbs,"filelist.bbs");

							if (sfd = fopen(filesbbs,"rb"))
								{
								printf("Opened \"filelist.bbs\"...\n");

								if (tfile.file_descname[0])				/* load and show the file header */
									strcpy(buffer,tfile.file_descname);
								else 
									strcpy(buffer,tfile.file_pathname);
								if (buffer[0])
									{
									if (buffer[strlen(buffer) - 1] != P_CSEP)
										strcat(buffer,P_SSEP);
									}
								strcat(buffer,"files.bbs");

								if (!(dfd = fopen(buffer,"r+b")))
									dfd = fopen(buffer,"wb");

								if (dfd)
									{
									printf("Opened \"files.bbs\"...\n");

									fseek(dfd,0L,SEEK_END);
									while (fread(&tfe,sizeof(struct fe),1,sfd))
										fprintf(dfd,"%s %s\r\n",tfe.fe_name,tfe.fe_descrip);

									fclose(sfd);
									}
								else
									{
									fprintf(stderr,"Fatal Error: Unable to find/open \"files.bbs\"!\n");
									return 1;
									}
								fclose(dfd);
								}
							else
								{
								fprintf(stderr,"Fatal Error: Unable to find/open \"filelist.bbs\"!\n");
								return 1;
								}
							}

						break;
						}
					}

				fclose(fd);

				if (!found)
					{
					fprintf(stderr,"Fatal Error: Unable to find area # %u!\n",area);
					return 1;
					}

				}
			else
				{
				fprintf(stderr,"Fatal Error: Unable to find/open \"filearea.bbs\"!\n");
				return 1;
				}
			}
		else
			{
			fprintf(stderr,"Fatal Error: No area number provided on command line!\n");
			return 1;
			}
		}

	if (error)
		return 1;
	return 0;
	}



