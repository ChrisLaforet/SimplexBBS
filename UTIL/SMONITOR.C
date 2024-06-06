/* smonitor.c
**
** Copyright (c) 1991, Christopher Laforet
** All Rights Reserved
**
** Started: 28 February 1991
**
** Revision Information: $Logfile$
**                       $Date$
**                       $Revision$
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <setjmp.h>
#define INCL_KBD 
#define INCL_DOSPROCESS
#define INCL_DOSFILEMGR
#define INCL_DOSDEVICES
#define INCL_DOSSEMAPHORES
#include <os2.h>




/* ----------------------- Structures for IOCTL ------------------------
**            Taken from MS's OS/2 toolkit -- missing in IBM's!
*/

struct _dcbinfo	
	{
	USHORT usWriteTimeout;
	USHORT usReadTimeout;
	BYTE fbCtlHndShake;
	BYTE fbFlowReplace;
	BYTE fbTimeout;
	BYTE bErrorReplacementChar;
	BYTE bBreakReplacementChar;
	BYTE bXONChar;
	BYTE bXOFFChar;
	};


struct _linecontrol
	{
	BYTE bDataBits;
	BYTE bParity;
	BYTE bStopBits;
	BYTE fTransBreak;
	};


struct _rxqueue
	{
	USHORT cch;
	USHORT cb;
	};


struct _modemstatus
	{
	BYTE fbModemOn;
	BYTE fbModemOff;
	};


/* -------------------- End of Structures for IOCTL -------------------- */



#define KB_STACKSIZE			2048
#define COM_STACKSIZE			2048
#define MAX_PORT				8


HFILE hComm[MAX_PORT] =
	{
	(HFILE)-1,
	(HFILE)-1,
	(HFILE)-1,
	(HFILE)-1,
	(HFILE)-1,
	(HFILE)-1,
	(HFILE)-1,
	(HFILE)-1,
	};
volatile int kb_hit = 0;
long kbaccess_sem = 0L;
int kbthread = 1;
TID kb_tid;
int kb_stack[KB_STACKSIZE / sizeof(int)];		/* word-align the stack */
volatile int incom_waiting = 0;
long incomaccess_sem = 0L;
int incomthread = 1;
TID incom_tid;
int incom_stack[COM_STACKSIZE / sizeof(int)];	/* word-align the stack */
int com_port = 0;
int reset_port = 0;
struct _dcbinfo reset_di;
int port;



void monitor_kb(void far *dummy)
	{
	KBDKEYINFO ki;

	DosSetPrty(PRTYS_THREAD,PRTYC_TIMECRITICAL,PRTYD_MINIMUM,kb_tid);
	while (kbthread)
		{
		if (!kb_hit)
			{
			DosSemRequest(&kbaccess_sem,SEM_INDEFINITE_WAIT);
			KbdPeek(&ki,0);
			if (ki.fbStatus & 0x40)		/* final character in */
				kb_hit = 1;
			else 
				kb_hit = 0;
			DosSemClear(&kbaccess_sem);
			}
		DosSleep(30L);
		}
	_endthread();
	}



int start_kbthread(void)
	{
	if ((kb_tid = _beginthread(monitor_kb,kb_stack,KB_STACKSIZE,NULL)) == -1)
		return 0;
	return 1;
	}



void monitor_incom(void far *dummy)
	{
	struct _rxqueue rq;

	DosSetPrty(PRTYS_THREAD,PRTYC_TIMECRITICAL,PRTYD_MINIMUM,incom_tid);
	while (incomthread)
		{
		DosSemRequest(&incomaccess_sem,SEM_INDEFINITE_WAIT);
		DosDevIOCtl(&rq,0L,0x68,1,hComm[com_port]);
		if (rq.cch)
			incom_waiting = 1;
		else 
			incom_waiting = 0;
		DosSemClear(&incomaccess_sem);
		DosSleep(50L);
		}
	_endthread();
	}



int start_comthread(void)
	{
	if ((incom_tid = _beginthread(monitor_incom,incom_stack,COM_STACKSIZE,NULL)) == -1)
		return 0;
	return 1;
	}



int init_fossil(int port)
	{
	USHORT action;
	UCHAR szBuffer[5];
	struct _dcbinfo di;

	if (port >= MAX_PORT)
		return 0;
	if (hComm[port] == -1)
		{
		sprintf(szBuffer,"COM%u",port + 1);
		if (DosOpen(szBuffer,&hComm[port],&action,0L,FILE_NORMAL,FILE_OPEN,OPEN_ACCESS_READWRITE | OPEN_SHARE_DENYREADWRITE,0L))
			return 0;				/* failed port open */
		DosDevIOCtl(&di,0L,0x73,1,hComm[port]);
		di.usWriteTimeout = 0;
		di.usReadTimeout = 0;
		di.fbCtlHndShake = (BYTE)1;		/* dtr control */
		di.fbFlowReplace = (BYTE)0;
		di.fbTimeout = (BYTE)2; 		/* read timeout */
		DosDevIOCtl(0L,&di,0x53,1,hComm[port]);
		com_port = port;
		}
	return 1;
	}



void raise_dtr(int port)
	{
	struct _modemstatus ms;
	USHORT error;

	ms.fbModemOn = 0x2;			/* RTS on -- does DTR separately because of bug in 1.21 */
	ms.fbModemOff = 0xff;
	DosDevIOCtl(&error,&ms,0x46,1,hComm[port]);		/* set modem control */
	ms.fbModemOn = 0x1;			/* DTR on */
	ms.fbModemOff = 0xff;
	DosDevIOCtl(&error,&ms,0x46,1,hComm[port]);
	}



void lower_dtr(int port)
	{
	struct _modemstatus ms;
	USHORT error;

	ms.fbModemOn = 0x0;
	ms.fbModemOff = 0xfe;			/* DTR off */
	DosDevIOCtl(&error,&ms,0x46,1,hComm[port]);
	}



void enable_flowctrl(int port,int cts,int xon_xmit,int xon_recv)
	{
	struct _dcbinfo di;

	DosDevIOCtl(&di,0L,0x73,1,hComm[port]);
	di.fbCtlHndShake = (BYTE)1;				/* DTR control */
	di.fbFlowReplace = (BYTE)0;
	if (cts)
		{
		di.fbCtlHndShake |= 0x4;		/* CTS handshake */
		di.fbFlowReplace |= 0x80;		/* RTS handshake */
		}
	if (xon_xmit)
		di.fbFlowReplace |= 0x1;		/* XON/XOFF automatic input flow control */
	if (xon_recv)
		di.fbFlowReplace |= 0x2;		/* XON/XOFF automatic output flow control */
	if (xon_xmit || xon_recv)
		{
		di.bXONChar = (BYTE)0x11;
		di.bXOFFChar = (BYTE)0x13;
		}
	DosDevIOCtl(0L,&di,0x53,1,hComm[port]);
	}



int setup_fossil(int port,int handle)		/* if handle is passed to BBS */
	{
	struct _dcbinfo di;

	if (port >= MAX_PORT)
		return 0;
	if (hComm[port] == -1)
		{
		hComm[port] = handle;
		DosDevIOCtl(&di,0L,0x73,1,hComm[port]);
		memcpy(&reset_di,&di,sizeof(struct _dcbinfo));
		reset_port = 1;

		di.usWriteTimeout = 0;
		di.usReadTimeout = 0;
		di.fbCtlHndShake = (BYTE)1;		/* dtr control */
		di.fbFlowReplace = (BYTE)0;
		di.fbTimeout = (BYTE)2; 		/* read timeout */
		DosDevIOCtl(0L,&di,0x53,1,hComm[port]);
		com_port = port;
		}
	return 1;
	}



void reinit_fossil(int port)		/* reinits when BBS is reentered  */
	{
	struct _dcbinfo di;

	if (port >= MAX_PORT)
		return;
	if (hComm[port] != -1)
		{
		DosDevIOCtl(&di,0L,0x73,1,hComm[port]);
		di.usWriteTimeout = 0;
		di.usReadTimeout = 0;
		di.fbCtlHndShake = (BYTE)1;		/* enable dtr control */
		di.fbFlowReplace = (BYTE)0;
		di.fbTimeout = (BYTE)2; 		/* read timeout */
		DosDevIOCtl(0L,&di,0x53,1,hComm[port]);
		}
	}



void reset_fossil(int port)		/* reinits when BBS is exited when port handle was passed  */
	{
	if (port >= MAX_PORT)
		return;
	kbthread = 0;
	incomthread = 0;
	DosSleep(500L);
	if (reset_port && hComm[port] != -1)
		DosDevIOCtl(0L,&reset_di,0x53,1,hComm[port]);
	}



void deinit_fossil(int port)
	{
	struct _dcbinfo di;

	if (port >= MAX_PORT)
		return;
	if (hComm[port] != (HFILE)-1)
		{
		kbthread = 0;
		incomthread = 0;
		DosSleep(250L);

		if (hComm[port] != -1)
			{
			DosDevIOCtl(&di,0L,0x73,1,hComm[port]);
			di.usWriteTimeout = 0;
			di.usReadTimeout = 0;
			di.fbCtlHndShake = (BYTE)1;		/* enable dtr control */
			di.fbFlowReplace = (BYTE)0x40;	/* enable rts */
			di.fbTimeout = (BYTE)2; 		/* read timeout */
			DosDevIOCtl(0L,&di,0x53,1,hComm[port]);
			}
		raise_dtr(port);

		DosClose(hComm[port]);
		hComm[port] = (HFILE)-1;
		}
	}



int set_baud(int port,int baud)
	{
	struct _linecontrol lc;

	if (DosDevIOCtl(0L,&baud,0x41,1,hComm[port]))
		return 0;
	lc.bDataBits = 8;
	lc.bParity = 0;
	lc.bStopBits = 0;
	if (DosDevIOCtl(0L,&lc,0x42,1,hComm[port]))
		return 0;
	return 1;
	}



int get_status(int port)
	{
	USHORT flags;
	int rtn = 8;

	DosDevIOCtl(&flags,0L,0x72,1,hComm[port]);		/* get comm event */
	if (flags & 0x1)		/* char received */
		rtn |= 0x100;
	if (flags & 0x4)		/* last char sent */
		rtn |= 0x4000;
	DosDevIOCtl(&flags,0L,0x67,1,hComm[port]);		/* get modem input */
	if (flags & 0x80)		/* DCD on? */
		rtn |= 0x80;
	return rtn;
	}



void flush_output(int port)
	{

	}



void purge_output_buffer(int port)		/* clears pending bytes in output buffer */
	{
	DosDevIOCtl(0L,0L,0x2,0xb,hComm[port]);		/* dev flush output */
	}



void purge_input_buffer(int port)		/* clears pending bytes in input buffer */
	{
	DosSemRequest(&incomaccess_sem,SEM_INDEFINITE_WAIT);
	DosDevIOCtl(0L,0L,0x1,0xb,hComm[port]);		/* dev flush input */
	incom_waiting = 0;
	DosSemClear(&incomaccess_sem);
	}



int pascal check_cd(int port)
	{
	USHORT flags;

	DosDevIOCtl(&flags,0L,0x67,1,hComm[port]);		/* get modem input */
	if (flags & 0x80)		/* DCD on? */
		return 1;
	return 0;
	}



void pascal output_wait(int port,char character)
	{
	USHORT written;

	do
		{
		DosWrite(hComm[port],&character,1,&written);
		if (!written)
			DosSleep(0L);
		}
	while (!written);
	}



int pascal output_nowait(int port,char character)
	{
	USHORT written;

	DosWrite(hComm[port],&character,1,&written);
	if (written)
		return 1;
	return 0;
	}



int pascal check_output(int port)
	{
	struct _rxqueue rq;

	DosDevIOCtl(&rq,0L,0x69,1,hComm[port]);
	if (rq.cch <= 1)
		return 0;
	return 1;
	}



int pascal read_input(int port)
	{
	USHORT read;
	UCHAR character;

	do
		{
		DosRead(hComm[port],&character,1,&read);
		if (!read)
			DosSleep(1L);
		}
	while (!read);
	if (!DosSemRequest(&incomaccess_sem,SEM_IMMEDIATE_RETURN))
		{
		incom_waiting = 0;
		DosSemClear(&incomaccess_sem);
		}
	return (int)character;
	}



int pascal peek_input(int port)
	{
	if (incom_waiting)
		return 0;
	return -1;
	}



int pascal peek_protocol_input(int port)
	{
	struct _rxqueue rq;

	DosDevIOCtl(&rq,0L,0x68,1,hComm[port]);
	if (rq.cch)
		return 0;
	return -1;
	}



int pascal read_kb(void)
	{
	KBDKEYINFO ki;
	int rtn;
	
	KbdCharIn(&ki,IO_WAIT,0);
	if ((ki.chChar == 0) || (ki.chChar == 0xe0))
		rtn = (int)(unsigned int)ki.chScan << 8;
	else
		rtn = (int)(unsigned int)ki.chChar;
	if (!DosSemRequest(&kbaccess_sem,SEM_IMMEDIATE_RETURN))
		{
		kb_hit = 0;
		DosSemClear(&kbaccess_sem);
		}
	return rtn;
	}



int pascal peek_kb(void)
	{
	if (kb_hit)
		return 0;
	return -1;
	}



void send_string(char *buf)
	{
	int buflen = strlen(buf);
	int current = 0;
	int sent;

	while (current < buflen)
		{
		sent = 0;
		do
			{
			if (!check_output(port))		/* transmitter almost empty */
				{
				output_wait(port,buf[current]);		/* out to comm port */
				putc(buf[current],stderr);
				++sent;
				}
			if (!(sent % 20))
				DosSleep(0L);		/* hand off a time slice - local mode would be a foreground session! */
			}
		while (!sent);
		++current;
		}
	}



int main(int argc,char *argv[])
	{
	char buf[10];
	int quit = 0;
	int key;

	fprintf(stderr,"SMONITOR: Simplex Null Modem Monitor.\n");
	fprintf(stderr,"Copyright (c) 1991, Chris Laforet and Chris Laforet Software.\n");
	fprintf(stderr,"All Rights Reserved.\n\n");
	if (argc < 2 || argc > 2)
		{
		fprintf(stderr,"Usage is: SMONITOR port\n");
		fprintf(stderr,"    where port is 0 for com1, 1 for com2, 2 for com3 and 3 for com4\n");
		}
	else
		{
		port = atoi(argv[1]);
		if (port >= 0 && port < MAX_PORT)
			{
			if (!init_fossil(port))
				{
				fprintf(stderr,"Unable to open com%d.\n",port + 1);
				return 1;
				}
			if (!start_kbthread())
				{
				fprintf(stderr,"Unable to start keyboard monitoring thread...Aborting!\n");
				return 1;
				}
			if (!start_comthread())
				{
				fprintf(stderr,"Unable to start com port monitoring thread...Aborting!\n");
				return 1;
				}
			raise_dtr(port);
			enable_flowctrl(port,1,0,0);		/* cts-rts and NO xon-xoff */
			if (!set_baud(port,19200))
				{
				fprintf(stderr,"Unable to set baud rate!\n");
				return 1;
				}

			send_string("SMONITOR: Press Ctrl-B for BBS or Ctrl-X to exit.\r\n");
			while (!quit)
				{
				key = -1;
				if (peek_input(port) != -1)
					key = read_input(port);
				else if (peek_kb() != -1)
					key = read_kb();

				if (key != -1)
					{
					if (key == 2)	/* ctrl-b */
						{
						sprintf(buf,"-P%u",hComm[port]);
						if (spawnlp(P_WAIT,"simplex2.exe","simplex2.exe","-B19200","-X",buf,NULL) == -1)
							fprintf(stderr,"Error while spawning Simplex2.exe\n");

						reinit_fossil(port);
						raise_dtr(port);
						enable_flowctrl(port,1,0,0);		/* cts-rts and NO xon-xoff */
						if (!set_baud(port,19200))
							{
							fprintf(stderr,"Unable to set baud rate!\n");
							return 1;
							}
						send_string("\r\nSMONITOR: Press Ctrl-B for BBS or Ctrl-X to exit.\r\n");
						}
					else if (key == 24)		/* ctrl-x */
						{
						send_string("SMONITOR is exiting.\r\n");
						quit = 1;
						}
					}
				else 
					DosSleep(250L);
				}
			}
		else
			fprintf(stderr,"Invalid port number.\n");
		}
	return 0;
	}
