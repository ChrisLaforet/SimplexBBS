/* fforward.c
**
** Copyright (c) 1989, Christopher Laforet
** All Rights Reserved
**
** Started: 10 July 1989
**
** Revision Information:
**
** $Logfile:   E:/quickbbs/programs/fforward/vcs/fforward.c_v  $
** $Date:   10 Aug 1989 14:52:00  $
** $Revision:   0.5  $
**
*/


#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <io.h>
#ifdef PROTECTED
	#define INCL_DOSFILEMGR
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "fforward.h"



FILE *logfd;


int main(int argc,char *argv[])
	{
	char *cptr;
	char config[100];
	char buffer[100];
	char temp[20];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	int count;
	int kount;
	char flow_file[100];
	int delete_flow;
	int rtn;
	int character;
	int action = 0;
	FILE *fd;
#ifdef PROTECTED
	HDIR hdir;
	FILEFINDBUF find;
	int search;
#else
	struct find_t find;
#endif

	fprintf(stderr,"FFORWARD (v %u.%02u of %s): A FidoNet File Forwarding Utility\n",MAJOR_VERSION,MINOR_VERSION,__DATE__);
	fprintf(stderr,"Copyright (c) 1989,90, Chris Laforet Software.  All Rights Reserved.\n\n");
	if (cptr = getenv("FFORWARD"))
		{
		_splitpath(cptr,drive,dir,fname,ext);
		_makepath(config,drive,dir,"FFORWARD","CFG");
		}
	else
		strcpy(config,"FFORWARD.CFG");
	read_config(config);

	if (!(logfd = fopen(logfile,"r+b")))
		{
		if (!(logfd = fopen(logfile,"w+b")))
			{
			fprintf(stderr,"\a\a\aFatal Error: Unable to open log %s!\n\n",logfile);
			exit(-1);
			}
		else
			{
			fprintf(logfd,"FFORWARD Log File\r\n");
			fprintf(logfd,"-----------------\r\n\r\n");
			log("Created new log file!");
			}
		}

	/* if clean flag is set, delete all unneeded holding files */
	if (clean_flag)
		clean();
	/* forward files (FLO) */
	for (count = 0; count < cur_actions; count++)
		{
		if (actions[count]->act_type == FORWARD)
			{
			sprintf(temp,"%04x%04x.FLO",actions[count]->act_net,actions[count]->act_node);
			_makepath(flow_file,"",outbound,temp,"");
			delete_flow = 0;
			if (!(fd = fopen(flow_file,"r+b")))
				{
				if (!(fd = fopen(flow_file,"wb")))
					{
					sprintf(buffer,"Unable to open FLO file (%s)!",temp);
					log(buffer);
					}
				else
					delete_flow = 1;		/* delete if un-used */
				}
			if (fd)
				{
				_makepath(buffer,"",inbound,actions[count]->act_name,"");
#ifdef PROTECTED
				search = 1;
				hdir = HDIR_SYSTEM;
				rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
				while (!rtn)
					{
					if (!copy(find.achName,inbound,holdpath))
						{
						delete_flow = 0;
						_makepath(buffer,"",holdpath,find.achName,"");
						fseek(fd,0L,SEEK_END);
						if (ftell(fd))
							{
							fseek(fd,-1L,SEEK_CUR);
							character = fgetc(fd);
							fseek(fd,0L,SEEK_END);
							if (character != '\r' && character != '\n')
								fputc('\n',fd);
							}
						fprintf(fd,"%s\n",buffer);

						sprintf(buffer,"Forwarding %s to node %u/%u.",find.achName,actions[count]->act_net,actions[count]->act_node);
						log(buffer);
						action = 1;
						}
					else
						{
		   				for (kount = 0; kount < cur_actions; kount++)
							{
							if (actions[kount]->act_type == KILL)
								{
								if (!stricmp(actions[count]->act_name,actions[kount]->act_name))
									{
									sprintf(buffer,"Prohibiting deletion of %s!",actions[kount]->act_name);
									log(buffer);
									actions[kount]->act_type = 0;
									}
								}
							}
						}
					search = 1;
					rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
					}
#else
				rtn = _dos_findfirst(buffer,_A_ARCH,&find);
				while (!rtn)
					{
					if (!copy(find.name,inbound,holdpath))
						{
						delete_flow = 0;
						_makepath(buffer,"",holdpath,find.name,"");
						fseek(fd,0L,SEEK_END);
						if (ftell(fd))
							{
							fseek(fd,-1L,SEEK_CUR);
							character = fgetc(fd);
							fseek(fd,0L,SEEK_END);
							if (character != '\r' && character != '\n')
								fputc('\r',fd);
							}
						fprintf(fd,"%s\n",buffer);

						sprintf(buffer,"Forwarding %s to node %u/%u.",find.name,actions[count]->act_net,actions[count]->act_node);
						log(buffer);
						action = 1;
						}
					else
						{
		   				for (kount = 0; kount < cur_actions; kount++)
							{
							if (actions[kount]->act_type == KILL)
								{
								if (!stricmp(actions[count]->act_name,actions[kount]->act_name))
									{
									sprintf(buffer,"Prohibiting deletion of %s!",actions[kount]->act_name);
									log(buffer);
									actions[kount]->act_type = 0;
									}
								}
							}
						}
					rtn = _dos_findnext(&find);
					}
#endif
				fclose(fd);
				if (delete_flow)
					unlink(flow_file);
				}
			}
		}

	/* forward continuous files (CLO) */
	for (count = 0; count < cur_actions; count++)
		{
		if (actions[count]->act_type == CRASH)
			{
			sprintf(temp,"%04x%04x.CLO",actions[count]->act_net,actions[count]->act_node);
			_makepath(flow_file,"",outbound,temp,"");
			delete_flow = 0;
			if (!(fd = fopen(flow_file,"r+b")))
				{
				if (!(fd = fopen(flow_file,"wb")))
					{
					sprintf(buffer,"Unable to open CLO file (%s)!",temp);
					log(buffer);
					}
				else
					delete_flow = 1;		/* delete if un-used */
				}
			if (fd)
				{
				_makepath(buffer,"",inbound,actions[count]->act_name,"");
#ifdef PROTECTED
				search = 1;
				hdir = HDIR_SYSTEM;
				rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
				while (!rtn)
					{
					if (!copy(find.achName,inbound,holdpath))
						{
						delete_flow = 0;
						_makepath(buffer,"",holdpath,find.achName,"");
						fseek(fd,0L,SEEK_END);
						if (ftell(fd))
							{
							fseek(fd,-1L,SEEK_CUR);
							character = fgetc(fd);
							fseek(fd,0L,SEEK_END);
							if (character != '\r' && character != '\n')
								fputc('\r',fd);
							}
						fprintf(fd,"%s\n",buffer);

						sprintf(buffer,"Forwarding %s to node %u/%u.",find.achName,actions[count]->act_net,actions[count]->act_node);
						log(buffer);
						action = 1;
						}
					else
						{
		   				for (kount = 0; kount < cur_actions; kount++)
							{
							if (actions[kount]->act_type == KILL)
								{
								if (!stricmp(actions[count]->act_name,actions[kount]->act_name))
									{
									sprintf(buffer,"Prohibiting deletion of %s!",actions[kount]->act_name);
									log(buffer);
									actions[kount]->act_type = 0;
									}
								}
							}
						}
					search = 1;
					rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
					}
#else
				rtn = _dos_findfirst(buffer,_A_ARCH,&find);
				while (!rtn)
					{
					if (!copy(find.name,inbound,holdpath))
						{
						delete_flow = 0;
						_makepath(buffer,"",holdpath,find.name,"");
						fseek(fd,0L,SEEK_END);
						if (ftell(fd))
							{
							fseek(fd,-1L,SEEK_CUR);
							character = fgetc(fd);
							fseek(fd,0L,SEEK_END);
							if (character != '\r' && character != '\n')
								fputc('\r',fd);
							}
						fprintf(fd,"%s\n",buffer);

						sprintf(buffer,"Forwarding %s to node %u/%u.",find.name,actions[count]->act_net,actions[count]->act_node);
						log(buffer);
						action = 1;
						}
					else
						{
		   				for (kount = 0; kount < cur_actions; kount++)
							{
							if (actions[kount]->act_type == KILL)
								{
								if (!stricmp(actions[count]->act_name,actions[kount]->act_name))
									{
									sprintf(buffer,"Prohibiting deletion of %s!",actions[kount]->act_name);
									log(buffer);
									actions[kount]->act_type = 0;
									}
								}
							}
						}
					rtn = _dos_findnext(&find);
					}
#endif
				fclose(fd);
				if (delete_flow)
					unlink(flow_file);
				}
			}
		}

	/* forward hold files (HLO) */
	for (count = 0; count < cur_actions; count++)
		{
		if (actions[count]->act_type == HOLD)
			{
			sprintf(temp,"%04x%04x.HLO",actions[count]->act_net,actions[count]->act_node);
			_makepath(flow_file,"",outbound,temp,"");
			delete_flow = 0;
			if (!(fd = fopen(flow_file,"r+b")))
				{
				if (!(fd = fopen(flow_file,"wb")))
					{
					sprintf(buffer,"Unable to open HLO file (%s)!",temp);
					log(buffer);
					}
				else
					delete_flow = 1;		/* delete if un-used */
				}
			if (fd)
				{
				_makepath(buffer,"",inbound,actions[count]->act_name,"");
#ifdef PROTECTED
				search = 1;
				hdir = HDIR_SYSTEM;
				rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
				while (!rtn)
					{
					if (!copy(find.achName,inbound,holdpath))
						{
						delete_flow = 0;
						_makepath(buffer,"",holdpath,find.achName,"");
						fseek(fd,0L,SEEK_END);
						if (ftell(fd))
							{
							fseek(fd,-1L,SEEK_CUR);
							character = fgetc(fd);
							fseek(fd,0L,SEEK_END);
							if (character != '\r' && character != '\n')
								fputc('\r',fd);
							}
						fprintf(fd,"%s\n",buffer);

						sprintf(buffer,"Forwarding %s to node %u/%u.",find.achName,actions[count]->act_net,actions[count]->act_node);
						log(buffer);
						action = 1;
						}
					else
						{
		   				for (kount = 0; kount < cur_actions; kount++)
							{
							if (actions[kount]->act_type == KILL)
								{
								if (!stricmp(actions[count]->act_name,actions[kount]->act_name))
									{
									sprintf(buffer,"Prohibiting deletion of %s!",actions[kount]->act_name);
									log(buffer);
									actions[kount]->act_type = 0;
									}
								}
							}
						}
					search = 1;
					rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
					}
#else
				rtn = _dos_findfirst(buffer,_A_ARCH,&find);
				while (!rtn)
					{
					if (!copy(find.name,inbound,holdpath))
						{
						delete_flow = 0;
						_makepath(buffer,"",holdpath,find.name,"");
						fseek(fd,0L,SEEK_END);
						if (ftell(fd))
							{
							fseek(fd,-1L,SEEK_CUR);
							character = fgetc(fd);
							fseek(fd,0L,SEEK_END);
							if (character != '\r' && character != '\n')
								fputc('\r',fd);
							}
						fprintf(fd,"%s\n",buffer);

						sprintf(buffer,"Forwarding %s to node %u/%u.",find.name,actions[count]->act_net,actions[count]->act_node);
						log(buffer);
						action = 1;
						}
					else
						{
		   				for (kount = 0; kount < cur_actions; kount++)
							{
							if (actions[kount]->act_type == KILL)
								{
								if (!stricmp(actions[count]->act_name,actions[kount]->act_name))
									{
									sprintf(buffer,"Prohibiting deletion of %s!",actions[kount]->act_name);
									log(buffer);
									actions[kount]->act_type = 0;
									}
								}
							}
						}
					rtn = _dos_findnext(&find);
					}
#endif
				fclose(fd);
				if (delete_flow)
					unlink(flow_file);
				}
			}
		}

	for (count = 0; count < cur_actions; count++)
		{
		if (actions[count]->act_type == MOVE)
			{
			_makepath(buffer,"",inbound,actions[count]->act_name,"");
#ifdef PROTECTED
			search = 1;
			hdir = HDIR_SYSTEM;
			rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
			while (!rtn)
				{
				if (copy(find.achName,inbound,movepath))
					{
		   			for (kount = 0; kount < cur_actions; kount++)
						{
						if (actions[kount]->act_type == KILL)
							{
							if (!stricmp(actions[count]->act_name,actions[kount]->act_name))
								{
								sprintf(buffer,"Prohibiting deletion of %s!",actions[kount]->act_name);
								log(buffer);
								actions[kount]->act_type = 0;
								}
							}
						}
					}
				else
					{
					sprintf(buffer,"Moving %s to file holding directory.",find.achName);
					log(buffer);
					action = 1;
					}
				search = 1;
				rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
				}
#else
			rtn = _dos_findfirst(buffer,_A_ARCH,&find);
			while (!rtn)
				{
				if (copy(find.name,inbound,movepath))
					{
		   			for (kount = 0; kount < cur_actions; kount++)
						{
						if (actions[kount]->act_type == KILL)
							{
							if (!stricmp(actions[count]->act_name,actions[kount]->act_name))
								{
								sprintf(buffer,"Prohibiting deletion of %s!",actions[kount]->act_name);
								log(buffer);
								actions[kount]->act_type = 0;
								}
							}
						}
					}
				else
					{
					sprintf(buffer,"Moving %s to file holding directory.",find.name);
					log(buffer);
					action = 1;
					}
				rtn = _dos_findnext(&find);
				}
#endif
			}
		}
	for (count = 0; count < cur_actions; count++)
		{
		if (actions[count]->act_type == KILL)
			{
			_makepath(buffer,"",inbound,actions[count]->act_name,"");
#ifdef PROTECTED
			search = 1;
			hdir = HDIR_SYSTEM;
			rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
			while (!rtn)
				{
				sprintf(buffer,"Deleting %s from inbound directory.",find.achName);
				log(buffer);

				_makepath(buffer,"",inbound,find.achName,"");
				if (unlink(buffer))
					{
					sprintf(buffer,"*Error* Unable to delete %s",find.achName);
					log(buffer);
					}
				action = 1;

				search = 1;
				rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
				}
#else
			rtn = _dos_findfirst(buffer,_A_ARCH,&find);
			while (!rtn)
				{
				sprintf(buffer,"Deleting %s from inbound directory.",find.name);
				log(buffer);

				_makepath(buffer,"",inbound,find.name,"");
				if (unlink(buffer))
					{
					sprintf(buffer,"*Error* Unable to delete %s",find.name);
					log(buffer);
					}
				action = 1;

				rtn = _dos_findnext(&find);
				}
#endif
			}
		}
	if (!action)
		log("Nothing done!");
	fclose(logfd);
	return(0);
	}



char cbuffer[10000];


int copy(char *file,char *from,char *to)
	{
	char buffer[200];
	char buffer1[100];
	char buffer2[100];
	int len;
	int err = 0;
#ifdef PROTECTED
	FILESTATUS filestatus;
#else
	int check;
	int date;
	int time;
#endif
	FILE *ffd;
	FILE *tfd;

	_makepath(buffer1,"",from,file,"");
	_makepath(buffer2,"",to,file,"");
	if (!(ffd = fopen(buffer1,"rb")))
		{
		sprintf(buffer,"Unable to open source %s!",buffer1);
		log(buffer);
		err = 1;
		}
	if (!(tfd = fopen(buffer2,"wb")))
		{
		sprintf(buffer,"Unable to open destination %s!",buffer2);
		log(buffer);
		err = 1;
		}
	while (len = read(fileno(ffd),cbuffer,sizeof(cbuffer)))
		{
		if (write(fileno(tfd),cbuffer,len) == -1)
			{
			sprintf(buffer,"Error copying %s to %s!",file,to);
			log(buffer);
			err = 1;
			break;
			}
		}
	if (!err)
		{
#ifdef PROTECTED
		DosQFileInfo(fileno(ffd),1,(BYTE *)&filestatus,sizeof(FILESTATUS));
		DosSetFileInfo(fileno(tfd),1,(BYTE *)&filestatus,sizeof(FILESTATUS));
#else
		_dos_getftime(fileno(ffd),(unsigned *)&date,(unsigned *)&time);
		_dos_setftime(fileno(tfd),date,time);
#endif
		}
	fclose(ffd);
	fclose(tfd);
	if (err)
		unlink(buffer2);
	return(err);
	}


void log(char *string)
	{
	char date[10];
	char time[10];

	_strdate(date);
	_strtime(time);
	fseek(logfd,0L,SEEK_END);
	fprintf(logfd,"FForward [%s at %s] %s\r\n",date,time,string);
	fprintf(stderr,"%s\n",string);
	}



struct clean **cleans = NULL;
int max_cleans = 0;
int cur_cleans = 0;


void clean(void)
	{
	int count;
	char *cptr;
	char buffer[100];
	char buffer1[100];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
#ifdef PROTECTED
	HDIR hdir;
	FILEFINDBUF find;
	int search;
#else
	struct find_t find;
#endif
	int rtn;
	FILE *fd;

	_makepath(buffer,"",holdpath,"*","*");
#ifdef PROTECTED
	search = 1;
	hdir = HDIR_SYSTEM;
	rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
	while (!rtn)
		{
		if (cur_cleans >= max_cleans)
			{
			if (!(cleans = realloc(cleans,(max_cleans += 10) * sizeof(struct clean *))))
				{
				log("Out of memory for cleaning!");
				fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
				exit(-1);
				}
			}
		if (!(cleans[cur_cleans] = calloc(1,sizeof(struct clean))))
			{
			log("Out of memory for cleaning!");
			fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
			exit(-1);
			}
		if (!(cptr = calloc(strlen(find.achName) + 1,sizeof(char))))
			{
			log("Out of memory for cleaning!");
			fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
			exit(-1);
			}
		strcpy(cptr,find.achName);
		cleans[cur_cleans]->clean_name = cptr;
		cleans[cur_cleans]->clean_flag = 0;
		++cur_cleans;
		search = 1;
		rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
		}
#else
	rtn = _dos_findfirst(buffer,_A_ARCH,&find);
	while (!rtn)
		{
		if (cur_cleans >= max_cleans)
			{
			if (!(cleans = realloc(cleans,(max_cleans += 10) * sizeof(struct clean *))))
				{
				log("Out of memory for cleaning!");
				fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
				exit(-1);
				}
			}
		if (!(cleans[cur_cleans] = calloc(1,sizeof(struct clean))))
			{
			log("Out of memory for cleaning!");
			fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
			exit(-1);
			}
		if (!(cptr = calloc(strlen(find.name) + 1,sizeof(char))))
			{
			log("Out of memory for cleaning!");
			fprintf(stderr,"\a\a\aFatal Error: Out of memory!\n\n");
			exit(-1);
			}
		strcpy(cptr,find.name);
		cleans[cur_cleans]->clean_name = cptr;
		cleans[cur_cleans]->clean_flag = 0;
		++cur_cleans;
		rtn = _dos_findnext(&find);
		}
#endif
	if (cur_cleans)
		{
		_makepath(buffer,"",outbound,"*","FLO");
#ifdef PROTECTED
		search = 1;
		hdir = HDIR_SYSTEM;
		rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
		while (!rtn)
			{
			_makepath(buffer,"",outbound,find.achName,"");
			if (fd = fopen(buffer,"r"))
				{
				while (fgets(buffer,sizeof(buffer),fd))
					{
					cptr = buffer + strlen(buffer);
					while (*cptr <= ' ' && cptr != buffer)
						--cptr;
					if (*cptr > ' ')
						++cptr;
					*cptr = '\0';

					cptr = buffer;
					while (*cptr == '#' || *cptr == '^')
						++cptr;
					_splitpath(cptr,drive,dir,fname,ext);
					_makepath(buffer,drive,dir,fname,ext);
					for (count = 0; count < cur_cleans; count++)
						{
						if (!cleans[count]->clean_flag)
							{
							_makepath(buffer1,"",holdpath,cleans[count]->clean_name,"");
							strupr(buffer);
							strupr(buffer1);
							if (!stricmp(buffer,buffer1))
								{
								cleans[count]->clean_flag = 1;		/* keep this file */
								break;
								}
							}
						}
					}
				}
			else
				{
				log("Unable to open a flow file.  Clean process stopped!");
				return;
				}
			search = 1;
			rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
			}
#else
		rtn = _dos_findfirst(buffer,_A_ARCH,&find);
		while (!rtn)
			{
			_makepath(buffer,"",outbound,find.name,"");
			if (fd = fopen(buffer,"r"))
				{
				while (fgets(buffer,sizeof(buffer),fd))
					{
					cptr = buffer + strlen(buffer);
					while (*cptr <= ' ' && cptr != buffer)
						--cptr;
					if (*cptr > ' ')
						++cptr;
					*cptr = '\0';

					cptr = buffer;
					while (*cptr == '#' || *cptr == '^')
						++cptr;
					_splitpath(cptr,drive,dir,fname,ext);
					_makepath(buffer,drive,dir,fname,ext);
					for (count = 0; count < cur_cleans; count++)
						{
						if (!cleans[count]->clean_flag)
							{
							_makepath(buffer1,"",holdpath,cleans[count]->clean_name,"");
							strupr(buffer);
							strupr(buffer1);
							if (!stricmp(buffer,buffer1))
								{
								cleans[count]->clean_flag = 1;		/* keep this file */
								break;
								}
							}
						}
					}
				}
			else
				{
				log("Unable to open a flow file.  Clean process stopped!");
				return;
				}
			rtn = _dos_findnext(&find);
			}
#endif

		_makepath(buffer,"",outbound,"*","CLO");

#ifdef PROTECTED
		search = 1;
		hdir = HDIR_SYSTEM;
		rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
		while (!rtn)
			{
			_makepath(buffer,"",outbound,find.achName,"");
			if (fd = fopen(buffer,"r"))
				{
				while (fgets(buffer,sizeof(buffer),fd))
					{
					cptr = buffer + strlen(buffer);
					while (*cptr <= ' ' && cptr != buffer)
						--cptr;
					if (*cptr > ' ')
						++cptr;
					*cptr = '\0';

					cptr = buffer;
					while (*cptr == '#' || *cptr == '^')
						++cptr;
					_splitpath(cptr,drive,dir,fname,ext);
					_makepath(buffer,drive,dir,fname,ext);
					for (count = 0; count < cur_cleans; count++)
						{
						if (!cleans[count]->clean_flag)
							{
							_makepath(buffer1,"",holdpath,cleans[count]->clean_name,"");
							strupr(buffer);
							strupr(buffer1);
							if (!stricmp(buffer,buffer1))
								{
								cleans[count]->clean_flag = 1;		/* keep this file */
								break;
								}
							}
						}
					}
				}
			else
				{
				log("Unable to open a flow file.  Clean process stopped!");
				return;
				}
			search = 1;
			rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
			}
#else
		rtn = _dos_findfirst(buffer,_A_ARCH,&find);
		while (!rtn)
			{
			_makepath(buffer,"",outbound,find.name,"");
			if (fd = fopen(buffer,"r"))
				{
				while (fgets(buffer,sizeof(buffer),fd))
					{
					cptr = buffer + strlen(buffer);
					while (*cptr <= ' ' && cptr != buffer)
						--cptr;
					if (*cptr > ' ')
						++cptr;
					*cptr = '\0';

					cptr = buffer;
					while (*cptr == '#' || *cptr == '^')
						++cptr;
					_splitpath(cptr,drive,dir,fname,ext);
					_makepath(buffer,drive,dir,fname,ext);
					for (count = 0; count < cur_cleans; count++)
						{
						if (!cleans[count]->clean_flag)
							{
							_makepath(buffer1,"",holdpath,cleans[count]->clean_name,"");
							strupr(buffer);
							strupr(buffer1);
							if (!stricmp(buffer,buffer1))
								{
								cleans[count]->clean_flag = 1;		/* keep this file */
								break;
								}
							}
						}
					}
				}
			else
				{
				log("Unable to open a flow file.  Clean process stopped!");
				return;
				}
			rtn = _dos_findnext(&find);
			}
#endif

		_makepath(buffer,"",outbound,"*","HLO");
#ifdef PROTECTED
		search = 1;
		hdir = HDIR_SYSTEM;
		rtn = DosFindFirst(buffer,&hdir,FILE_NORMAL | FILE_ARCHIVED,&find,sizeof(FILEFINDBUF),&search,0L);
		while (!rtn)
			{
			_makepath(buffer,"",outbound,find.achName,"");
			if (fd = fopen(buffer,"r"))
				{
				while (fgets(buffer,sizeof(buffer),fd))
					{
					cptr = buffer + strlen(buffer);
					while (*cptr <= ' ' && cptr != buffer)
						--cptr;
					if (*cptr > ' ')
						++cptr;
					*cptr = '\0';

					cptr = buffer;
					while (*cptr == '#' || *cptr == '^')
						++cptr;
					_splitpath(cptr,drive,dir,fname,ext);
					_makepath(buffer,drive,dir,fname,ext);
					for (count = 0; count < cur_cleans; count++)
						{
						if (!cleans[count]->clean_flag)
							{
							_makepath(buffer1,"",holdpath,cleans[count]->clean_name,"");
							strupr(buffer);
							strupr(buffer1);
							if (!stricmp(buffer,buffer1))
								{
								cleans[count]->clean_flag = 1;		/* keep this file */
								break;
								}
							}
						}
					}
				}
			else
				{
				log("Unable to open a flow file.  Clean process stopped!");
				return;
				}
			search = 1;
			rtn = DosFindNext(hdir,&find,sizeof(FILEFINDBUF),&search);
			}
#else
		rtn = _dos_findfirst(buffer,_A_ARCH,&find);
		while (!rtn)
			{
			_makepath(buffer,"",outbound,find.name,"");
			if (fd = fopen(buffer,"r"))
				{
				while (fgets(buffer,sizeof(buffer),fd))
					{
					cptr = buffer + strlen(buffer);
					while (*cptr <= ' ' && cptr != buffer)
						--cptr;
					if (*cptr > ' ')
						++cptr;
					*cptr = '\0';

					cptr = buffer;
					while (*cptr == '#' || *cptr == '^')
						++cptr;
					_splitpath(cptr,drive,dir,fname,ext);
					_makepath(buffer,drive,dir,fname,ext);
					for (count = 0; count < cur_cleans; count++)
						{
						if (!cleans[count]->clean_flag)
							{
							_makepath(buffer1,"",holdpath,cleans[count]->clean_name,"");
							strupr(buffer);
							strupr(buffer1);
							if (!stricmp(buffer,buffer1))
								{
								cleans[count]->clean_flag = 1;		/* keep this file */
								break;
								}
							}
						}
					}
				}
			else
				{
				log("Unable to open a flow file.  Clean process stopped!");
				return;
				}
			rtn = _dos_findnext(&find);
			}
#endif

		for (count = 0; count < cur_cleans; count++)
			{
			if (!cleans[count]->clean_flag)
				{
				sprintf(buffer,"Cleaning out %s!",cleans[count]->clean_name);
				log(buffer);

				_makepath(buffer,"",holdpath,cleans[count]->clean_name,"");
				unlink(buffer);
				}
			}
		}
	else
		log("No files in holding area.  Clean process stopped!");
	}

