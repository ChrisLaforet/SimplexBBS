/* s_outb.c
**
** Copyright (c) 1990, Chris Laforet Software/Chris Laforet
** All Rights Reserved
**
** Started: 18 November 1990
**
** Revision Information: $Logfile:   G:/simplex/smp/vcs/s_outb.c_v  $
**                       $Date:   25 Oct 1992 14:26:30  $
**                       $Revision:   1.10  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <direct.h>
#include <io.h>
#include "smp.h"



struct olist **olist = NULL;
int cur_olist = 0;
int max_olist = 0;



void add_outbound(int type,int zone,char *filename)
	{
	char buffer[10];
 	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	int count;
	int net;
	int node;

	_splitpath(filename,buffer,buffer,fname,ext);
	if (strlen(fname) != 8)
		return;					/* don't waste your time!  Can't be a valid file. */
	sprintf(buffer,"%4.4s",fname);
	sscanf(buffer,"%x",&net);
	sscanf(fname + 4,"%x",&node);

	if (cur_olist >= max_olist)
		{
		if (!(olist = realloc(olist,(max_olist += 10) * sizeof(struct olist *))))
			{
			for (count = 0; count < cur_actions; count++)	/* free memory before crapping out */
				free(actions[count]);
			free(actions);
			printf("Fatal Error:  Out of memory to allocate outbound list structures.\n");
			exit(1);
			}
		}

	if (!(olist[cur_olist] = (struct olist *)malloc(sizeof(struct olist))))
		{
		for (count = 0; count < cur_actions; count++)	/* free memory before crapping out */
			free(actions[count]);
		free(actions);
		for (count = 0; count < cur_olist; count++)
			{
			free(olist[count]->olist_file);
			free(olist[count]);
			}
		free(olist);
		printf("Fatal Error:  Out of memory to allocate outbound list structures.\n");
		exit(1);
		}

	if (!(olist[cur_olist]->olist_file = malloc((strlen(filename) + 1) * sizeof(char))))
		{
		for (count = 0; count < cur_actions; count++)	/* free memory before crapping out */
			free(actions[count]);
		free(actions);
		for (count = 0; count < cur_olist; count++)
			{
			free(olist[count]->olist_file);
			free(olist[count]);
			}
		free(olist[cur_olist]);		/* and free the one we just allocated! */
		free(olist);
		printf("Fatal Error:  Out of memory to allocate outbound list structures.\n");
		exit(1);
		}

	olist[cur_olist]->olist_type = type;
	olist[cur_olist]->olist_dzone = zone;
	olist[cur_olist]->olist_dnet = net;
	olist[cur_olist]->olist_dnode = node;
	strcpy(olist[cur_olist]->olist_file,filename);
	++cur_olist;
	}



void load_outbound(void)
	{
	struct ph tph;
	struct fi tfi;
	char buffer[100];
	char *cptr;
	int count;
	int kount;
	int rtn;
	FILE *fd;

	printf("Scanning outbound directories....\n");
	if (zone)
		{
		for (kount = 0; kount < 255; kount++)
			{
			if (outbound[kount])
				{
				cptr = get_outbound(kount);
				for (count = LEFT; count <= DIRECT; count++)		/* Boy, won't we be in trouble if these constants are changed! */
					{
					strcpy(buffer,cptr);
					switch (count)
						{
						case LEFT:
							strcat(buffer,"*.n?o");
							break;
						case HOLD:
							strcat(buffer,"*.hlo");
							break;
						case ROUTE:
							strcat(buffer,"*.flo");
							break;
						case DIRECT:
							strcat(buffer,"*.clo");
							break;
						}

					rtn = get_firstfile(&tfi,buffer);
					while (rtn)
						{
						if (tfi.fi_size >= (long)sizeof(struct ph))
							{
							strcpy(buffer,cptr);
							strcat(buffer,tfi.fi_name);
							if (fd = fopen(buffer,"rb"))
								{
								if (fread(&tph,sizeof(struct ph),1,fd) && tph.ph_sig1 == 2 && tph.ph_sig2 == 0)
									add_outbound(count,kount,tfi.fi_name);
								fclose(fd);
								}
							}
// add_outbound(count,kount,tfi.fi_name);
						rtn = get_nextfile(&tfi);
						}
					get_closefile();		/* close directory handle if needed */

					strcpy(buffer,cptr);
					switch (count)
						{
						case LEFT:
							strcat(buffer,"*.n?t");
							break;
						case HOLD:
							strcat(buffer,"*.hut");
							break;
						case ROUTE:
							strcat(buffer,"*.out");
							break;
						case DIRECT:
							strcat(buffer,"*.cut");
							break;
						}

					rtn = get_firstfile(&tfi,buffer);
					while (rtn)
						{
						add_outbound(count,kount,tfi.fi_name);
						rtn = get_nextfile(&tfi);
						}
					get_closefile();		/* close directory handle if needed */
					}
				}
			}
		}
	else
		{
		for (count = LEFT; count <= DIRECT; count++)		/* Boy, won't we be in trouble if these constants are changed! */
			{
			strcpy(buffer,cfg.cfg_outboundpath);
			if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
				strcat(buffer,P_SSEP);
			switch (count)
				{
				case LEFT:
					strcat(buffer,"*.n?o");
					break;
				case HOLD:
					strcat(buffer,"*.hlo");
					break;
				case ROUTE:
					strcat(buffer,"*.flo");
					break;
				case DIRECT:
					strcat(buffer,"*.clo");
					break;
				}

			rtn = get_firstfile(&tfi,buffer);
			while (rtn)
				{
				add_outbound(count,0,tfi.fi_name);
				rtn = get_nextfile(&tfi);
				}
			get_closefile();		/* close directory handle if needed */

			strcpy(buffer,cfg.cfg_outboundpath);
			if (buffer[0] && buffer[strlen(buffer) - 1] != P_CSEP)
				strcat(buffer,P_SSEP);
			switch (count)
				{
				case LEFT:
					strcat(buffer,"*.n?t");
					break;
				case HOLD:
					strcat(buffer,"*.hut");
					break;
				case ROUTE:
					strcat(buffer,"*.out");
					break;
				case DIRECT:
					strcat(buffer,"*.cut");
					break;
				}

			rtn = get_firstfile(&tfi,buffer);
			while (rtn)
				{
				add_outbound(count,0,tfi.fi_name);
				rtn = get_nextfile(&tfi);
				}
			get_closefile();		/* close directory handle if needed */

			}
		}
	}



void dump_outbound(void)
	{
	char *cptr;
	int count;

	printf("The following is the outbound list:\n\n");
	if (!cur_olist)
		printf("\tThere were no outbound files found.\n");
	else
		{
		for (count = 0; count < cur_olist; count++)
			{
			switch (olist[count]->olist_type)
				{
				case LEFT:
					cptr = "Left";
					break;
				case HOLD:
					cptr = "Hold";
					break;
				case ROUTE:
					cptr = "Route";
					break;
				case DIRECT:
					cptr = "Direct";
					break;
				default:
					cptr = "Unknown";
					break;
				}
			printf("\tFile: %s to %u:%u/%u of type \"%s\"\n",olist[count]->olist_file,olist[count]->olist_dzone,olist[count]->olist_dnet,olist[count]->olist_dnode,cptr);
			}
		}
	printf("\n");
	}



void process_outbound(void)
	{
 	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	char next[_MAX_EXT];
	char buffer[100];
	char buffer1[100];
	char buffer2[20];
	char *cptr;
	char *cptr1;
	char *cptr2;
	int count;
	int kount;
	int counter;
	int err;
	int which;
	FILE *sfd;
	FILE *dfd;

	printf("Processing outbound files....\n");
	for (count = 0; count < cur_actions; count++)
		{
		for (kount = 0; kount < cur_olist; kount++)
			{
			if (actions[count]->act_zone == olist[kount]->olist_dzone || (unsigned int)actions[count]->act_zone == ALL)
				{
				if (actions[count]->act_net == olist[kount]->olist_dnet || (unsigned int)actions[count]->act_net == ALL)
					{
					if (actions[count]->act_node == olist[kount]->olist_dnode || (unsigned int)actions[count]->act_node == ALL)
						{
						strcpy(buffer1,olist[kount]->olist_file);
						_splitpath(buffer1,buffer,buffer,fname,ext);
						cptr = ext;
						next[0] = (char)'\0';
						if (*cptr == (char)'.')
							++cptr;

						cptr1 = get_outbound(olist[kount]->olist_dzone);
						strcpy(buffer,cptr1);		/* let's create directory if it doesn't exist! */
						if (buffer[0])
							{
							cptr2 = buffer + (strlen(buffer) - 1);
							if (*cptr2 == P_CSEP)
								*cptr2 = (char)'\0';
							if (access(buffer,0))
								{
								printf("Notice: Attempting to create outbound directory for zone %u\n",olist[kount]->olist_dzone);
								if (!mkdir(buffer))
									outbound[olist[kount]->olist_dzone] = (char)1;
								}
							}

						switch (actions[count]->act_type)
							{
							case LEAVEHOLD:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"nho");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"nht");
										which = 1;
										}
									if (next[0])
										{
										printf("LEAVEHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = LEFT;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying HOLD mail to LEAVE mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to LEAVEHOLD \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = LEFT;
												}
											}
										}
									}
								break;

							case LEAVEROUTE:
								if (olist[kount]->olist_type == ROUTE)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"nfo");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"not");
										which = 1;
										}
									if (next[0])
										{
										printf("LEAVEROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = LEFT;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying ROUTE mail to LEAVE mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to LEAVEROUTE \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = LEFT;
												}
											}
										}
									}
								break;

							case LEAVEDIRECT:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"nco");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"nct");
										which = 1;
										}
									if (next[0])
										{
										printf("LEAVEDIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = LEFT;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying DIRECT mail to LEAVE mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to LEAVEDIRECT \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = LEFT;
												}
											}
										}
									}
								break;

							case SENDHOLD:
								if (olist[kount]->olist_type == LEFT && (!stricmp(cptr,"nho") || !stricmp(cptr,"nht")))
									{
									if (!stricmp(cptr + 2,"o"))
										{
										strcpy(next,"hlo");
										which = 0;
										}
									else if (!stricmp(cptr + 2,"t"))
										{
										strcpy(next,"hut");
										which = 1;
										}
									if (next[0])
										{
										printf("SENDHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = HOLD;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying (SENDING) LEAVE mail to HOLD mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to SENDHOLD \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = HOLD;
												}
											}
										}
									}
								break;

							case SENDROUTE:
								if (olist[kount]->olist_type == LEFT && (!stricmp(cptr,"nfo") || !stricmp(cptr,"not")))
									{
									if (!stricmp(cptr + 2,"o"))
										{
										strcpy(next,"flo");
										which = 0;
										}
									else if (!stricmp(cptr + 2,"t"))
										{
										strcpy(next,"out");
										which = 1;
										}
									if (next[0])
										{
										printf("SENDROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = ROUTE;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying (SENDING) LEAVE mail to ROUTE mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to SENDROUTE \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = ROUTE;
												}
											}
										}
									}
								break;

							case SENDDIRECT:
								if (olist[kount]->olist_type == LEFT && (!stricmp(cptr,"nco") || !stricmp(cptr,"nct")))
									{
									if (!stricmp(cptr + 2,"o"))
										{
										strcpy(next,"clo");
										which = 0;
										}
									else if (!stricmp(cptr + 2,"t"))
										{
										strcpy(next,"cut");
										which = 1;
										}
									if (next[0])
										{
										printf("SENDDIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = DIRECT;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying (SENDING) LEAVE mail to DIRECT mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to SENDDIRECT \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = DIRECT;
												}
											}
										}
									}
								break;


							case DIRECTROUTE:
								if (olist[kount]->olist_type == DIRECT)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"flo");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"out");
										which = 1;
										}
									if (next[0])
										{
										printf("DIRECTROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = ROUTE;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying DIRECT mail to ROUTE mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to DIRECTROUTE \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = ROUTE;
												}
											}
										}
									}
								break;

							case DIRECTHOLD:
								if (olist[kount]->olist_type == DIRECT)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"hlo");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"hut");
										which = 1;
										}
									if (next[0])
										{
										printf("DIRECTHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = HOLD;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying DIRECT mail to HOLD mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to DIRECTHOLD \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = HOLD;
												}
											}
										}
									}
								break;

							case HOLDROUTE:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"flo");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"out");
										which = 1;
										}
									if (next[0])
										{
										printf("HOLDROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = ROUTE;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying HOLD mail to ROUTE mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to HOLDROUTE \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = ROUTE;
												}
											}
										}
									}
								break;

							case HOLDDIRECT:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"clo");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"cut");
										which = 1;
										}
									if (next[0])
										{
										printf("HOLDDIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = DIRECT;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying HOLD mail to DIRECT mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to DIRECTROUTE \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = DIRECT;
												}
											}
										}
									}
								break;

							case ROUTEHOLD:
								if (olist[kount]->olist_type == ROUTE)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"hlo");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"hut");
										which = 1;
										}
									if (next[0])
										{
										printf("ROUTEHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = HOLD;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying ROUTE mail to HOLD mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to ROUTEHOLD \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = HOLD;
												}
											}
										}
									}
								break;

							case ROUTEDIRECT:
								if (olist[kount]->olist_type == ROUTE)
									{
									if (!stricmp(cptr + 1,"lo"))
										{
										strcpy(next,"clo");
										which = 0;
										}
									else if (!stricmp(cptr + 1,"ut"))
										{
										strcpy(next,"cut");
										which = 1;
										}
									if (next[0])
										{
										printf("ROUTEDIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										_makepath(buffer,"",cptr1,fname,next);
										_makepath(buffer1,"",cptr1,fname,ext);
										if (!rename(buffer1,buffer))
											{
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											olist[kount]->olist_type = DIRECT;
											}
										else
											{
											err = 1;
											sfd = fopen(buffer1,"r+b");
											if (sfd)
												{
												dfd = fopen(buffer,"r+b");
												if (dfd)
													{
													printf("Copying ROUTE mail to DIRECT mail for %d;%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
													if (which)
														err = append_mail(sfd,dfd);
													else 
														err = append_outfile(sfd,dfd);
													fclose(dfd);
													}
												fclose(sfd);
												}
											if (err)
												printf("Error: Unable to ROUTEDIRECT \"%s\".\n",olist[kount]->olist_file);
											else
												{
												unlink(buffer1);
												_makepath(buffer,"","",fname,next);
												strcpy(olist[kount]->olist_file,buffer);
												olist[kount]->olist_type = DIRECT;
												}
											}
										}
									}
								break;


							case ARCHOLD:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ARCHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ARC,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ARCHOLD \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ZIPHOLD:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ZIPHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ZIP,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ZIPHOLD \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case LZHHOLD:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("LZHHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(LZH,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to LZHHOLD \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ARJHOLD:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ARJHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ARJ,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ARJHOLD \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ZOOHOLD:
								if (olist[kount]->olist_type == HOLD)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ZOOHOLD \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ZOO,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ZOOHOLD \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;

							case ARCROUTE:
								if (olist[kount]->olist_type == ROUTE)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ARCROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ARC,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ARCROUTE \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ZIPROUTE:
								if (olist[kount]->olist_type == ROUTE)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ZIPROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ZIP,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ZIPROUTE \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case LZHROUTE:
								if (olist[kount]->olist_type == ROUTE)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("LZHROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(LZH,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to LZHROUTE \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ARJROUTE:
								if (olist[kount]->olist_type == ROUTE)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ARJROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ARJ,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ARJROUTE \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ZOOROUTE:
								if (olist[kount]->olist_type == ROUTE)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ZOOROUTE \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ZOO,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ZOOROUTE \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;

							case ARCDIRECT:
								if (olist[kount]->olist_type == DIRECT)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ARCDIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ARC,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ARCDIRECT \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ZIPDIRECT:
								if (olist[kount]->olist_type == DIRECT)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ZIPDIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ZIP,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ZIPDIRECT \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case LZHDIRECT:
								if (olist[kount]->olist_type == DIRECT)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("LZHDIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(LZH,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to LZHDIRECT \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ARJDIRECT:
								if (olist[kount]->olist_type == DIRECT)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ARJDIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ARJ,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ARJDIRECT \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;
							case ZOODIRECT:
								if (olist[kount]->olist_type == DIRECT)
									{
									if (!stricmp(cptr + 1,"ut"))
										{
										printf("ZOODIRECT \"%s\" to %u:%u/%u\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode);
										if (archive(ZOO,olist[kount]->olist_file,buffer,olist[kount]->olist_dzone,olist[kount]->olist_dnet,olist[kount]->olist_dnode))
											{
											for (counter = 0; counter < cur_olist; counter++)
												{
												if (counter != kount)
													{
													if (!stricmp(buffer,olist[counter]->olist_file))	/* new archive file already exists! */
														{
														olist[kount]->olist_type = DELETED;
														break;
														}
													}
												if (counter >= cur_olist)		/* archive file doesn't exist?  Does now! */
													strcpy(olist[kount]->olist_file,buffer);
												}
											}
										else
											printf("Error: Unable to ZOODIRECT \"%s\".\n",olist[kount]->olist_file);
										}
									}
								break;

							case HOSTHOLD:
								if (olist[kount]->olist_type == HOLD)
									{
									printf("HOSTHOLD \"%s\" to %u:%u/0\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet);
									sprintf(buffer2,"%04x0000",olist[kount]->olist_dnet);
									_makepath(buffer,"",cptr1,buffer2,ext);
									_makepath(buffer1,"",cptr1,fname,ext);
									if (!rename(buffer1,buffer))
										{
										_makepath(buffer,"","",fname,next);
										strcpy(olist[kount]->olist_file,buffer);
										}
									else
										{
										err = 1;
										sfd = fopen(buffer1,"r+b");
										if (sfd)
											{
											dfd = fopen(buffer,"r+b");
											if (dfd)
												{
												printf("Copying (HOST)HOLD mail to HOLD mail for %d:%d/0\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet);
												if (which)
													err = append_mail(sfd,dfd);
												else 
													err = append_outfile(sfd,dfd);
												fclose(dfd);
												}
											fclose(sfd);
											}
										if (err)
											printf("Error: Unable to HOSTROUTE \"%s\".\n",olist[kount]->olist_file);
										else
											{
											unlink(buffer1);
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											}
										}
									}
								break;
							case HOSTROUTE:
								if (olist[kount]->olist_type == ROUTE)
									{
									printf("HOSTROUTE \"%s\" to %u:%u/0\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet);
									sprintf(buffer2,"%04x0000",olist[kount]->olist_dnet);
									_makepath(buffer,"",cptr1,buffer2,ext);
									_makepath(buffer1,"",cptr1,fname,ext);
									if (!rename(buffer1,buffer))
										{
										_makepath(buffer,"","",fname,next);
										strcpy(olist[kount]->olist_file,buffer);
										}
									else
										{
										err = 1;
										sfd = fopen(buffer1,"r+b");
										if (sfd)
											{
											dfd = fopen(buffer,"r+b");
											if (dfd)
												{
												printf("Copying ROUTE mail to ROUTE mail for %d:%d/%d\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet);
												if (which)
													err = append_mail(sfd,dfd);
												else 
													err = append_outfile(sfd,dfd);
												fclose(dfd);
												}
											fclose(sfd);
											}
										if (err)
											printf("Error: Unable to HOSTROUTE \"%s\".\n",olist[kount]->olist_file);
										else
											{
											unlink(buffer1);
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											}
										}
									}
								break;
							case HOSTDIRECT:
								if (olist[kount]->olist_type == DIRECT)
									{
									printf("HOSTDIRECT \"%s\" to %u:%u/0\n",olist[kount]->olist_file,olist[kount]->olist_dzone,olist[kount]->olist_dnet);
									sprintf(buffer2,"%04x0000",olist[kount]->olist_dnet);
									_makepath(buffer,"",cptr1,buffer2,ext);
									_makepath(buffer1,"",cptr1,fname,ext);
									if (!rename(buffer1,buffer))
										{
										_makepath(buffer,"","",fname,next);
										strcpy(olist[kount]->olist_file,buffer);
										}
									else
										{
										err = 1;
										sfd = fopen(buffer1,"r+b");
										if (sfd)
											{
											dfd = fopen(buffer,"r+b");
											if (dfd)
												{
												printf("Copying (HOST)DIRECT mail to DIRECT mail for %d:%d/0\n",olist[kount]->olist_dzone,olist[kount]->olist_dnet);
												if (which)
													err = append_mail(sfd,dfd);
												else 
													err = append_outfile(sfd,dfd);
												fclose(dfd);
												}
											fclose(sfd);
											}
										if (err)
											printf("Error: Unable to HOSTROUTE \"%s\".\n",olist[kount]->olist_file);
										else
											{
											unlink(buffer1);
											_makepath(buffer,"","",fname,next);
											strcpy(olist[kount]->olist_file,buffer);
											}
										}
									}
								break;
							}
						}
					}
				}
			}
		if (actions[count]->act_type == POLL)
			generate_poll(actions[count]->act_zone,actions[count]->act_net,actions[count]->act_node);
		}
	}

