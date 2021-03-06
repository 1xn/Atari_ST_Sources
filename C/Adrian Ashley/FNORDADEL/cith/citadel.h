/*
 * citadel.h -- global function and variable declarations for citadel.tos.
 *		Generated by `mkptypes citmain\*.c' (minus cfg.c.) and by
 *		painstaking effort...
 *
 * 90Aug28 AA	Created.
 */

#ifndef _CITADEL_H
#define _CITADEL_H

#if defined(__STDC__) || defined(__cplusplus)
# define _P(s) s
#else
# define _P(s) ()
#endif


/* citmain\archive.c */
int sendARchar _P((int c));
int ARsetup _P((char *file));
int sendARend _P((void));

/* citmain\calllog.c */
void logMessage _P((char val, char *str, char sig));

/* citmain\ctdl.c */

/* citmain\doenter.c */
void doEnter _P((int prefix, char cmd));

/* citmain\door.c */
void initdoor _P((void));
void dodoor _P((void));

/* citmain\doread.c */
int rwProtocol _P((char cp));
int initWC _P((int mode));
void doRead _P((int prefix, int hack, char cmd));

/* citmain\dosysop.c */
int doSysop _P((void));

/* citmain\driver.c */
void setmodem _P((int interactive));
void fixmodem _P((void));
void setBaud _P((int x));
int gotcarrier _P((void));
void modemClose _P((void));
void modemOpen _P((void));
void mflush _P((void));

/* citmain\floor.c */
int doFloor _P((char c));
int gotoFloor _P((char genNumber));
void listFloor _P((short mask));
void lFloor _P((short mask));
int findFloor _P((char gen));

/* citmain\format.c */
char *printword _P((register char *word));
void mformat _P((register char *string));
void mprintf _P((char *format, ...));

/* citmain\holdmsg.c */
int puthold _P((int idx));
int gethold _P((int idx));
void killhold _P((int idx));
int chkhold _P((int idx));

/* citmain\hothelp.c */
int hothelp _P((char *filename));
int blurb _P((char *name, int impervious));
int dobanner _P((void));
void menu _P((char *name));
int help _P((char *name, int impervious));

/* citmain\login.c */
int getpwlog _P((LABEL pw, struct logBuffer *p));
int login _P((char prefix));
void setlog _P((void));

/* citmain\misc.c */
void getNormStr _P((char *prompt, char *s, int size, int doEcho));
void givePrompt _P((void));
long asknumber _P((char *prompt, long bottom, long top, int def));
long getNumber _P((char *prompt, long bottom, long top));
int whereis _P((char *, char *, int, int));
void setclock _P((void));
void showcfg _P((void));
void config _P((char what));
int ingestFile _P((char *name));
int _getstring _P((char *string, int i, int size, int escape, int visible));
int getString _P((char *prompt, char *string, int size, char escape, int visible));
int typeWC _P((FILE *fd));
int download _P((struct dirList *fn));
int typefile _P((struct dirList *p));
int wildcard _P((int (*fn)(struct dirList *), char *pattern, 
	int (*preamble)(int, struct dirList *)));
void upload _P((char WCmode));
char *plural _P((char *msg, long number));
void showdays _P((char mask, int oldstyle));
int dateok _P((time_t time));
int dl_not_ok _P((long time, long size));
void dlstat _P((char *fname, long time, long size));
void whazzit _P((void));
char *uname _P((void));
void initCitadel _P((void));
void exitCitadel _P((int status));

/* citmain\modem.c */
int BBSCharReady _P((void));
unsigned iChar _P((void));
void mputchar _P((char c));
void connect _P((int line_echo, int mapCR, int local_echo));
char modIn _P((void));
void ringSysop _P((void));

/* citmain\msg.c */
int mAbort _P((void));
int printdraft _P((void));
int permission _P((int complain));
int addressee _P((int netflag));
void promote _P((void));
int entermesg _P((int protocol));
int heldmesg _P((int protocol));
int localmesg _P((int protocol));
int nettedmesg _P((int protocol));
int pick1mesg _P((long id));
int read1mesg _P((int msgNo, int canbackup));
int msgbrk _P((register long lim));
void showMessages _P((int which, int reverse));

/* citmain\netcall.c */
int netWCstartup _P((char *from));
int caller _P((void));
void mastermode _P((int reversed));
void readNegMail _P((void));
int netcommand _P((int cmd, ...));

/* citmain\neterror.c */
void neterror _P((int hup, char *format, ...));

/* citmain\netmain.c */
int increment _P((int c));
void readMail _P((char zap, void (*mailer )()));
void inMail _P((void));
void openNet _P((void));
void netmode _P((int length, int whichnet));
int checkpolling _P((void));
void pollnet _P((int which));
void closeNet _P((void));
int netTimeLeft _P((void));
int callout _P((int i));
int dialer _P((int i, int abort));
void OutOfNet _P((void));
int netAck _P((void));

/* citmain\netmisc.c */
int normID _P((register char *source, register char *dest));
int srchNetNm _P((char *name));
int srchNetId _P((char *forId));
void listnodes _P((int extended));
void netmenu _P((void));
int getSysName _P((char *prompt, char *system));
int netmesg _P((int slot));
void netPrintMsg _P((short loc, long id));
void sendXmh _P((void));
long sysRoomLeft _P((void));
int netchdir _P((char *path));

/* citmain\netrcv.c */
int issharing _P((int slot));
void nmcalled _P((void));
void called _P((void));
void slavemode _P((int reversed, int gotID));
char *netopt _P((char *optstr, char *proto));
void doSetup _P((void));
void doResults _P((void));

/* citmain\nfs.c */
void nfs_put _P((int place, int cmd, char *file, char *dir, char *room));
void nfs_process _P((void));

/* citmain\postmsg.c */
void _spool _P((FILE *f));
int postmail _P((int savemail));
void msgprintf _P((char *format, ...));
void note2Message _P((long id, int loc));
void msgToDisk _P((char *filename));
int storeMessage _P((struct logBuffer *who, int idx));
void aideMessage _P((int noteDeletedMessage));

/* citmain\room.c */
int roomExists _P((char *room));
int canEnter _P((int i, int enterifZ));
int nextroom _P((int mode));
int msgCount _P((register int brk));
int statroom _P((void));
void toroom _P((int roomno, int skipflag));
int gotoname _P((char *name));
void steproom _P((int expand, int forward));
void gotoroom _P((char *name, char mode));
int hasNew _P((int i));
int rvalid _P((int rmno, short mode));
void listRooms _P((short mode));
int partialExist _P((LABEL target));
void indexRooms _P((void));
void makeRoom _P((void));
void initialArchive _P((char *fn));
void getList _P((int (*fn)(char *), char *prompt));

/* citmain\roomedit.c */
void roomreport _P((char *buffer));
void whosnetting _P((void));
void editroom _P((void));

/* citmain\scandir.c */
int scandir _P((char *mask, struct dirList **list));
void freedir _P((struct dirList *list, int count));

/* citmain\statbar.c */
int makebar _P((void));
void killbar _P((void));
void stat_upd _P((void));

/* citmain\sysdep.c */
void getArea _P((struct aRoom *roomData));
void homeSpace _P((void));
int xchdir _P((char *path));
int mmesgbaud _P((void));
int scanbaud _P((void));
void crashout _P((char *msg, ...));
void xputs _P((char *s));
int xputc _P((int c));
void xprintf _P((char *format, ...));
void iprintf _P((char *format, ...));
void splitF _P((FILE *diskfile, char *format, ...));
void wcprintf _P((char *format, ...));
int set_time _P((struct tm *clk));
long dosexec _P((char *cmd, char *tail));
void systemInit _P((void));
void systemShutdown _P((void));

/* citmain\terminat.c */
void terminate _P((int disconnect, char flag));

/* citmain\xymodem.c */
int sendCchar _P((int c));
int sendCinit _P((void));
int sendCend _P((void));
int sendARinit _P((void));
int sendYhdr _P((char *name, long size));
int recXfile _P((int (*pc )(int )));
int beginWC _P((void));
int endWC _P((void));
int enterfile _P((int (*pc )(int ), char mode));

/* citmain\zaploop.c */
void init_zap _P((void));
void close_zap _P((void));
int notseen _P((void));

#undef _P

/* global variables for citadel.tos */

/* ctdl.c */
extern long	_stksize;
extern char	Abandon;	/* True when time to bring system down	*/
extern char	eventExit;	/* true when an event goes off		*/
extern char	dropDTR;	/* hang up phone when in console mode	*/
extern char	netymodem;
#ifdef ATARIST
extern char	multiTask;	/* run as a background multitasker?	*/
#endif
extern char	statbar;	/* disable the status bar		*/
extern char	restrict;	/* implement login restrictions?	*/
extern char	msgpurge;	/* implement msg purge after logoff?	*/
extern char	Debug;		/* normal debugging			*/
extern int	exitValue;
extern char	confirm[];
extern struct user *restlist;
extern int	numrestrict;
extern struct user *purgelist;
extern int	numpurge;

/* door.c */
extern struct doorway *doors;	/* doors defined in the system	*/
extern struct doorway *shell;	/* "outside" doorway...		*/
extern struct doorway *login_door;
extern struct doorway *logout_door;
extern struct doorway *newuser_door;

/* doread.c */
extern char WC;			/* WC mode rwProtocol returns		*/
extern char dPass;		/* for reading from a date		*/
extern time_t dAfterDate;	/* the date in standard form		*/
extern time_t dBeforeDate;	/* the date in standard form		*/
extern char dFormatted;		/* display formatted files		*/
extern int wantuser;

/* driver.c */
extern int byteRate;

/* floor.c */
extern char floorhook;

/* format.c */
extern int column;
extern char _nl_[];

/* holdmsg.c */
extern char holdtemplate[];

/* login.c */
extern char	loggedIn;	/* Global have-caller flag	*/
extern char	sameuser;
extern char	marktime[];	/* last login (for status lines)*/
extern int	badpw;
extern int	logfl;
extern short	oldcatChar;	/* Record cfg.catChar & cfg.catSector	*/
extern short	oldcatSector;	/* at user login.			*/
struct previnfo {
    LABEL name;			/* name of previous user	*/
    long flags;			/* flags from previous user	*/
};
extern struct previnfo prevuser;

/* misc.c */
extern FILE	*upfd;
extern int	masterCount;
extern char	*protocol[];

/* modem.c */
extern char justLostCarrier;	/* Modem <==> rooms connection		*/
extern char newCarrier;		/* Just got carrier			*/
extern char modStat;	/* Whether modem had carrier LAST time you checked. */
extern char haveCarrier;	/* set if DCD == YES			*/
extern char echo;		/* Either NEITHER, CALLER, or BOTH	*/
extern char usingWCprotocol;	/* True during WC protocol transfers	*/
extern char warned;
extern char sysRequest;		/* sysop wants to use the console	*/
extern char chatrequest;	/* user wants to chat			*/

/* msg.c */
extern char	heldMessage;
extern LABEL	oldTarget;
extern int	*msgsentered;	/* msgs entered per room this call */
extern char	*program;
extern char	outFlag;
extern int 	justLocals;	/* local messages only? */
extern int	singleMsg;	/* pause after each message for N/S */
extern LABEL	msguser;	/* display messages from/to this user */

/* netcall.c */
extern char checkNegMail;
extern char netmlcheck[];

/* neterror.c */
extern FILE *errfile;		/* error logfile		*/
extern PATHBUF logfile;		/* the name of ^^^		*/

/* netmain.c */
extern FILE    *netLog;
extern FILE    *debuglog;
extern char	logNetResults;
extern char	inNet;
extern char	sectBuf[];
extern int	counter;
extern int	rmtslot;
extern LABEL	rmtname;
extern LABEL	rmt_id;
extern struct alias *net_alias;
extern long netFin;		/* when the whole mess is done with	*/
extern char netDebug;		/* network-specific debugging flag	*/
extern int noKill;
extern int errCount;
extern struct nodeRoomsTab *sharedRooms;

/* netmisc.c */
extern char route_char;		/* routing code -- set by dumpNetRoom */

/* netrcv.c */
extern char *sr_sent;
extern char *sr_rcvd;
extern char processMail;
extern char netrmspool[];
extern char netmlspool[];

/* patchnum.c */
extern int PATCHNUM;

/* postmsg.c */
extern char Misvalid;
extern char Mlocal;
extern int  Mindex;
extern int  Mcost;

/* room.c */
extern int lastRoom;
extern int lastStack[];
extern struct Index *indices;
extern char remoteSysop;
extern char *on;
extern char *off;
extern char *no;
extern char *yes;
extern LABEL target;

/* xymodem.c */
extern LABEL rYfile;		/* file coming in from remote */
extern char WCError;		/* needed by other modules... */
extern char batchWC;		/* is this gonna be a batch transfer? */
extern int (*sendPFchar)(int);	/* send-a-char-by-protocol function ptr */

/* zaploop.c */
extern char checkloops;


#endif /* _CITADEL_H */
