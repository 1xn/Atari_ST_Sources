/*
* SNDBIND.H	Low level Sound function bindings for use with "C"
*		and the added Sound XBIOS calls.
*
*		Mike Schmal		7/7/92
*
*
* Updates:
* 7/13/92 MS  Added buffptr function.
*
*/

#define	INQUIRE		-1

#define	LTATTEN		0
#define	RTATTEN		1
#define	LTGAIN		2
#define	RTGAIN		3
#define	ADDERIN		4
#define	ADCINPUT	5
#define	SETPRESCALE	6

#define	ADC			3
#define	EXTINP		2
#define	DSPXMIT		1
#define	DMAPLAY		0

#define	DAC			0x8
#define	EXTOUT		0x4
#define	DSPRECV		0x2
#define	DMAREC		0x1

#define	STEREO8		0
#define	STEREO16	1
#define	MONO8		2

#define	ENABLE		1
#define	TRISTATE	0

#define	PLAY_ENABLE		0x1
#define	PLAY_REPEAT		0x2
#define	RECORD_ENABLE	0x4
#define	RECORD_REPEAT	0x8

#define PLAY		0
#define	RECORD		1

#define	CLK_25M		0
#define	CLK_EXT		1
#define	CLK_32M		2

#define NO_SHAKE	1
#define HANDSHAKE	0

#define	CLK50K		1
#define	CLK33K		2
#define	CLK25K		3
#define	CLK20K		4
#define	CLK16K		5
#define	CLK12K		7
#define	CLK10K		9
#define	CLK8K		11

#define	ACT_CLK50K	49170
#define	ACT_CLK33K	33880
#define	ACT_CLK25K	24585
#define	ACT_CLK20K	20770
#define	ACT_CLK16K	16490
#define	ACT_CLK12K	12292
#define	ACT_CLK10K	9834
#define	ACT_CLK8K	8195

/*
 *  SOUND XBIOS Errors.
 */
#define	SNDNOTLOCK	-128
#define	SNDLOCKED	-129


/* SOUND trap calling routine. */

extern	long	xbios();

#define	locksnd()				(long) xbios(0x80)
#define	unlocksnd()				(void) xbios(0x81)
#define	soundcmd(a,b)			(int)  xbios(0x82,a,b)
#define	setbuffer(a,b,c)		(void) xbios(0x83,a,b,c)
#define	setmode(a)				(void) xbios(0x84,a)
#define	settrack(a,b)			(void) xbios(0x85,a,b)
#define	setmontrack(a)			(void) xbios(0x86,a)
#define	setinterrupt(a,b)		(void) xbios(0x87,a,b)
#define	buffoper(a)				(void) xbios(0x88,a)
#define	dsptristate(a,b)		(void) xbios(0x89,a,b)
#define	gpio(a,b)				(void) xbios(0x8A,a,b)
#define	devconnect(a,b,c,d,e)	(void) xbios(0x8B,a,b,c,d,e)
#define	sndstatus(a)			(void) xbios(0x8C,a)
#define	buffptr(a)				(void) xbios(0x8D,a)