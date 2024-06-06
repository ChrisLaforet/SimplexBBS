/* s_fossil.c
**
** Copyright (c) 1989, Christopher Laforet
** All Rights Reserved
**
** Started: 23 November 1989
**
** Revision Information: $Logfile:   D:/mine/bbs/vcs/s_fossil.c_v  $
**                       $Date:   10 Dec 1990 02:23:56  $
**                       $Revision:   1.13  $
**
*/


#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#ifdef PROTECTED
	#define INCL_KBD 
	#define INCL_DOSFILEMGR
	#define INCL_DOSDEVICES
	#define INCL_DOSSEMAPHORES
	#define INCL_DOSPROCESS
	#include <os2.h>
#else
	#include <dos.h>
#endif
#include "latechat.h"


#ifdef PROTECTED



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


HFILE hComm = -1;
struct _dcbinfo save_di;


int setup_fossil(int handle)		/* if handle is passed to BBS */
	{
	struct _dcbinfo di;
	struct _modemstatus ms;
	int error;

	if (hComm == -1)
		{
		hComm = handle;

		DosDevIOCtl(&save_di,0L,0x73,1,hComm);
		memcpy(&di,&save_di,sizeof(struct _dcbinfo));
		di.usWriteTimeout = 0;
		di.usReadTimeout = 0;
		di.fbCtlHndShake = (BYTE)1;		/* dtr control */
		di.fbCtlHndShake |= (BYTE)0x8;	/* enable CTS control */
		di.fbFlowReplace = (BYTE)0x40;	/* flip RTS Control mode on */
		di.fbTimeout &= 0xfa;			/* kill no write timeout and wait read timeout -- LEAVE hardware buffering */
		di.fbTimeout |= (BYTE)2; 		/* read timeout */
		DosDevIOCtl(0L,&di,0x53,1,hComm);

		ms.fbModemOn = 0x2;			/* flip RTS on */
		ms.fbModemOff = 0xff;
		DosDevIOCtl(&error,&ms,0x46,1,hComm);		/* set modem control */
		}
	return 1;
	}



void deinit_fossil(int port)
	{
	if (hComm != -1)
		{
		DosDevIOCtl(0L,&save_di,0x53,1,hComm);
		hComm = -1;
		}
	}



int set_baud(int port,int baud)
	{
	struct _linecontrol lc;

	if (DosDevIOCtl(0L,&baud,0x41,1,hComm))
		return 0;
	lc.bDataBits = 8;
	lc.bParity = 0;
	lc.bStopBits = 0;
	if (DosDevIOCtl(0L,&lc,0x42,1,hComm))
		return 0;
	return 1;
	}



int get_status(int port)
	{
	USHORT flags;
	int rtn = 8;

	DosDevIOCtl(&flags,0L,0x72,1,hComm);
	if (flags & 0x1)		/* char received */
		rtn |= 0x100;
	if (flags & 0x4)		/* last char sent */
		rtn |= 0x4000;
	DosDevIOCtl(&flags,0L,0x67,1,hComm);
	if (flags & 0x80)		/* DCD on? */
		rtn |= 0x80;
	return rtn;
	}



void raise_dtr(int port)
	{
	struct _modemstatus ms;
	USHORT error;

	ms.fbModemOn = 0x1;			/* DTR on */
	ms.fbModemOff = 0xff;
	DosDevIOCtl(&error,&ms,0x46,1,hComm);
	}



void lower_dtr(int port)
	{
	struct _modemstatus ms;
	USHORT error;

	ms.fbModemOn = 0x0;
	ms.fbModemOff = 0xfe;			/* DTR off */
	DosDevIOCtl(&error,&ms,0x46,1,hComm);
	}



void enable_flowctrl(int port,int cts,int xon_xmit,int xon_recv)
	{
	struct _dcbinfo di;

	di.fbCtlHndShake = (BYTE)1;		/* DTR control */
	di.fbFlowReplace = (BYTE)0;
	if (cts)
		{
		di.fbCtlHndShake |= 0x8;		/* CTS handshake */
		di.fbFlowReplace |= 0x80;		/* RTS handshake */
		}
	else 
		di.fbFlowReplace |= 0x40;		/* RTS control */
	if (xon_xmit)
		di.fbFlowReplace |= 0x1;		/* XON/XOFF automatic input flow control */
	if (xon_recv)
		di.fbFlowReplace |= 0x2;		/* XON/XOFF automatic output flow control */
	if (xon_xmit || xon_recv)
		{
		di.bXONChar = (BYTE)0x11;
		di.bXOFFChar = (BYTE)0x13;
		}
	DosDevIOCtl(0L,&di,0x53,1,hComm);
	}



void flush_output(int port)
	{

	}



void purge_output_buffer(int port)		/* clears pending bytes in output buffer */
	{
	DosDevIOCtl(0L,0L,0x2,0xb,hComm);		/* dev flush output */
	}



void purge_input_buffer(int port)		/* clears pending bytes in input buffer */
	{
	DosDevIOCtl(0L,0L,0x1,0xb,hComm);		/* dev flush input */
	}



int pascal check_cd(int port)
	{
	USHORT flags;

	DosDevIOCtl(&flags,0L,0x67,1,hComm);
	if (flags & 0x80)		/* DCD on? */
		return 1;
	return 0;
	}



void pascal output_wait(int port,char character)
	{
	USHORT written;

	do
		{
		DosWrite(hComm,&character,1,&written);
		if (!written)
			DosSleep(0L);
		else if (!check_cd)
			exit(1);
		}
	while (!written);
	}



int pascal output_nowait(int port,char character)
	{
	USHORT written;

	DosWrite(hComm,&character,1,&written);
	if (written)
		return 1;
	return 0;
	}



int pascal check_output(int port)
	{
	struct _rxqueue rq;

	DosDevIOCtl(&rq,0L,0x69,1,hComm);
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
		DosRead(hComm,&character,1,&read);
		if (!read)
			DosSleep(1L);
		}
	while (!read);
	return (int)character;
	}



int pascal peek_input(int port)
	{
	struct _rxqueue rq;

	DosDevIOCtl(&rq,0L,0x68,1,hComm);
	if (rq.cch)
		return 0;
	return 0xffff;
	}



int pascal read_kb(void)
	{
	KBDKEYINFO ki;
	int rtn;
	
	KbdCharIn(&ki,IO_WAIT,0);
	if ((ki.chChar == 0) || (ki.chChar == 0xe0))
		rtn = ki.chScan << 8;
	else
		rtn = ki.chChar;
	return rtn;
	}



int pascal peek_kb(void)
	{
	KBDKEYINFO ki;

	KbdPeek(&ki,0);
	if (ki.fbStatus & 0x40)		/* final character in */
		return 0;
	return 0xffff;
	}


#else


int init_fossil(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.x.bx = 0;
	registers.h.ah = 0x4;
	int86(0x14,&registers,&registers);
	if (registers.x.ax != 0x1954)
		return 0;		/* no fossil installed */
	if (registers.h.bl < 0x1b)
		return 0;		/* minimum function needed is 1b hex - information */
	return (int)registers.h.bl;		/* return maximum func number supported */
	}



void deinit_fossil(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0x5;
	int86(0x14,&registers,&registers);
	}



int set_baud(int port,int baud)
	{
	union REGS registers;

	switch ((unsigned int)baud)
		{
		case 300:
			registers.h.al = 2;
			break;
		case 1200:
			registers.h.al = 4;
			break;
		case 2400:
			registers.h.al = 5;
			break;
		case 4800:
			registers.h.al = 6;
			break;
		case 9600:
			registers.h.al = 7;
			break;
		case 19200:
			registers.h.al = 0;
			break;
		case 38400:
			registers.h.al = 1;
			break;
		default:
			return 0;
			break;
		}
	registers.h.al <<= 5;
	registers.h.al |= 0x3;		/* set lower bits to N-8-1 (00011) */
	registers.x.dx = port;
	registers.h.ah = 0x0;
	int86(0x14,&registers,&registers);
	return registers.x.ax;		/* return status */
	}



int get_status(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0x3;
	int86(0x14,&registers,&registers);
	return registers.x.ax;
	}



void raise_dtr(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.al = 1;	
	registers.h.ah = 0x6;
	int86(0x14,&registers,&registers);
	}



void lower_dtr(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.al = 0;	
	registers.h.ah = 0x6;
	int86(0x14,&registers,&registers);
	}



void flush_output(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0x8;
	int86(0x14,&registers,&registers);
	}



void purge_output_buffer(int port)		/* clears pending bytes in output buffer */
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0x9;
	int86(0x14,&registers,&registers);
	}



void purge_input_buffer(int port)		/* clears pending bytes in input buffer */
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0xa;
	int86(0x14,&registers,&registers);
	}



void enable_flowctrl(int port,int cts,int xon_xmit,int xon_recv)
	{
	union REGS registers;

	registers.h.al = 0;
	if (cts)
		registers.h.al |= 0x2;
	if (xon_xmit)
		registers.h.al |= 0x1;
	if (xon_recv)
		registers.h.al |= 0x8;
	registers.x.dx = port;
	registers.h.ah = 0xf;
	int86(0x14,&registers,&registers);
	}



int pascal check_cd(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0x3;
	int86(0x14,&registers,&registers);
	if (registers.h.al & 0x80)
		return 1;
	return 0;		/* no carrier */
	}



void pascal output_wait(int port,char character)
	{
	union REGS registers;

	registers.h.al = character;
	registers.x.dx = port;
	registers.h.ah = 0x1;
	int86(0x14,&registers,&registers);
	}



int pascal output_nowait(int port,char character)
	{
	union REGS registers;

	registers.h.al = character;
	registers.x.dx = port;
	registers.h.ah = 0xb;
	int86(0x14,&registers,&registers);
	return registers.x.ax;			/* 0 if fail */
	}



int pascal read_input(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0x2;
	int86(0x14,&registers,&registers);
	return (int)registers.h.al;			/* character */
	}



int pascal peek_input(int port)
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0xc;
	int86(0x14,&registers,&registers);
	return registers.x.ax;			/* -1 (0xffff) if no character, else character */
	}



int pascal peek_protocol_input(int port)		/* in DOS same as peek_input() */
	{
	union REGS registers;

	registers.x.dx = port;
	registers.h.ah = 0xc;
	int86(0x14,&registers,&registers);
	return registers.x.ax;			/* -1 (0xffff) if no character, else character */
	}



int pascal read_kb(void)
	{
	union REGS registers;

	registers.h.ah = 0xe;
	int86(0x14,&registers,&registers);
	if (registers.x.ax & 0xff)
		return registers.x.ax & 0xff;	/* return the character w/o code */
	return registers.x.ax;				/* IBM scan-code */
	}



int pascal peek_kb(void)
	{
	union REGS registers;

	registers.h.ah = 0xd;
	int86(0x14,&registers,&registers);
	return registers.x.ax;			/* 0xffff if no character, else IBM scan-code */
	}



int pascal check_output(int port)
	{
	union REGS registers;
	struct SREGS segments;
	struct finfo info;
	struct finfo *tinfo;

	tinfo = &info;
    info.finfo_size = sizeof(struct finfo);
	registers.x.dx = port;
	registers.x.cx = sizeof(struct finfo);
	segments.es = FP_SEG(tinfo);
	registers.x.di = FP_OFF(tinfo);
	registers.h.ah = 0x1b;
	int86x(0x14,&registers,&registers,&segments);
	if (info.finfo_outputsize - info.finfo_output <= 1)
		return 0;
	return 1;
	}

#endif
