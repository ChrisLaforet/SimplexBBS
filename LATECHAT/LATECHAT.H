/* 1. Constants and Macros */

#define MAJOR_VERSION				1
#define MINOR_VERSION				1


#define getrandom(min,max) ((rand() % (int)((max) - (min))) + (min) + 1)
#define BackSpace send_string ("\b \b");

#define   MeLocate(X,Y)  fprintf(stderr,"\033[%d;%dH",Y,X)   /* Cursor positioner */
#define   CLS            send_string("\033[2J")           /* Clears screen     */
#define   CRLF           send_string("\n")
#define   PushSpot       send_string ("[s");
#define   PopSpot        send_string ("[u");
#define   ClrLine        send_string ("[K");

/* ============ Normal (DIM) colors ======================== */
#define   Blue           send_string("\033[0;34m")
#define   Cyan           send_string("\033[0;36m")
#define   Green          send_string("\033[0;32m")
#define   Red            send_string("\033[0;31m")
#define   Magenta        send_string("\033[0;35m")
#define   Yellow         send_string("\033[0;33m")
#define   White          send_string("\033[0;37m")


/* ============ Bright Colors  ============================= */
#define   BriteBlue      send_string("\033[1;34m")
#define   BriteCyan      send_string("\033[1;36m")
#define   BriteGreen     send_string("\033[1;32m")
#define   BriteRed       send_string("\033[1;31m")
#define   BriteMagenta   send_string("\033[1;35m")
#define   BriteYellow    send_string("\033[1;33m")
#define   BriteWhite     send_string("\033[1;37m")


#define Com1 			0
#define Com2 			1
#define Local			0
#define Remote 			1

#define SysopLine   	3
#define RemoteLine   	15
#define BottomLine      25
#define MAXLINE			9
#define MAXWIDTH		78


/* 2. Structures and Unions */

struct finfo
	{
	int finfo_size;
	char finfo_fossil;
	char finfo_rev;
	char far *fininfo_id;
	int finfo_inputsize;
	int finfo_input;
	int finfo_outputsize;
	int finfo_output;
	char finfo_width;
	char finfo_height;
	char finfo_baud;
	};



/* 3. Global Variable Declarations */

extern int Port;
extern int baud;
extern int Remaining;


/* 4. External Function Declarations */

extern void Chat(void);
extern void DoSwitch(unsigned int BothHit,int *X,int *Y,int StartY,char *LineBuf);
extern int init_fossil(int port);
#ifdef PROTECTED
	extern int setup_fossil(int handle);
#endif
extern void deinit_fossil(int port);
extern int set_baud(int port,int baud);
extern int get_status(int port);
extern void raise_dtr(int port);
extern void lower_dtr(int port);
extern void flush_output(int port);
extern void purge_output_buffer(int port);
extern void purge_input_buffer(int port);
extern void enable_flowctrl(int port,int cts,int xon_xmit,int xon_recv);
extern int _pascal check_cd(int port);
extern void _pascal output_wait(int port,char character);
extern int _pascal output_nowait(int port,char character);
extern int _pascal read_input(int port);
extern int _pascal peek_input(int port);
extern int _pascal read_kb(void);
extern int _pascal peek_kb(void);
extern int _pascal check_output(int port);
extern void _pascal purge_input(int port);
extern int get_kb(void);
extern int _pascal recv_char(int timeout);
extern int get_char(void);
extern void hangup(void);
extern void get_reason(char *string);
extern void _pascal purge_output(int port);
extern void _pascal send_char_fast(char character);
extern void _pascal send_char(char character);
extern int send_string(char *string);
extern int send_buffer(char *buf,int buflen);
extern void Locate(int x,int y);

extern void sleep(int time);
