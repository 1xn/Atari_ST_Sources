/*
 *      transprt.h          (c) Peter Rottengatter  1996
 *                              perot@pallas.amp.uni-hannover.de
 *
 *      Include this file to use functions from STinG.
 *      i.e.: tcp, udp, etc ...
 */

/* Some minor changes for Sozobon C made by Thothy - 1999 */

#ifndef STING_TRANSPRT_H
#define STING_TRANSPRT_H

#ifdef __TURBOC__
#define CDECL1
#define CDECL2 cdecl
#endif

#ifdef SOZOBON
#define CDECL1 cdecl
#define CDECL2
#endif

#ifdef __GNUC__
#define CDECL1
#define CDECL2
#endif

/*--------------------------------------------------------------------------*/


/*
 *   Data types used throughout STinG
 */

typedef          char  int8;        /*   Signed  8 bit (char)        */
typedef unsigned char  uint8;       /* Unsigned  8 bit (byte, octet) */
typedef          short int16;       /*   Signed 16 bit (int)         */
typedef unsigned short uint16;      /* Unsigned 16 bit (word)        */
typedef          long  int32;       /*   Signed 32 bit               */
typedef unsigned long  uint32;      /* Unsigned 32 bit (longword)    */


#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif



/*--------------------------------------------------------------------------*/


/*
 *   Driver access structure / functions
 */

#define MAGIC   "STiKmagic"                 /* Magic for DRV_LIST.magic     */
#define CJTAG   "STiK"


typedef struct drv_header {                 /* Header part of TPL structure */
    char *module;           /* Specific string that can be searched for     */
    char *author;           /* Any string                                   */
    char *version;          /* Format `00.00' Version:Revision              */
 } DRV_HDR;


typedef struct drv_list
{
 char            magic[10];             /* Magic string, (see MAGIC) */
 CDECL1 DRV_HDR * CDECL2 (*get_dftab)(char *x);  /* Get Driver Function Table */
 CDECL1 int16 CDECL2 (*ETM_exec)(char *y);   /* Execute a STinG module    */
 void            *cfg;                  /* Config structure          */
 void            *sting_basepage;       /* STinG basepage address    */
} DRV_LIST;

extern DRV_LIST *drivers;


#define get_dftab(x)    (*drivers->get_dftab)(x)
#define ETM_exec(x)     (*drivers->ETM_exec)(x)



/*--------------------------------------------------------------------------*/


#define TRANSPORT_DRIVER    "TRANSPORT_TCPIP"
#define TCP_DRIVER_VERSION  "01.00"


/*
 *   TCP and UDP port escape flags
 */

#define  TCP_ACTIVE        0x0000     /* Initiate active connection         */
#define  TCP_PASSIVE       0xffff     /* Initiate passive connection        */
#define  UDP_EXTEND        0x0000     /* Extended addressing scheme         */


/*
 *   TCP connection states
 */

#define  TCLOSED       0    /* No connection.  Null, void, absent, ...      */
#define  TLISTEN       1    /* Wait for remote request                      */
#define  TSYN_SENT     2    /* Connect request sent, await matching request */
#define  TSYN_RECV     3    /* Wait for connection ack                      */
#define  TESTABLISH    4    /* Connection established, handshake completed  */
#define  TFIN_WAIT1    5    /* Await termination request or ack             */
#define  TFIN_WAIT2    6    /* Await termination request                    */
#define  TCLOSE_WAIT   7    /* Await termination request from local user    */
#define  TCLOSING      8    /* Await termination ack from remote TCP        */
#define  TLAST_ACK     9    /* Await ack of terminate request sent          */
#define  TTIME_WAIT   10    /* Delay, ensures remote has received term' ack */


/*
 *   Error return values
 */

#define  E_NORMAL         0     /* No error occured ...                     */
#define  E_OBUFFULL      -1     /* Output buffer is full                    */
#define  E_NODATA        -2     /* No data available                        */
#define  E_EOF           -3     /* EOF from remote                          */
#define  E_RRESET        -4     /* Reset received from remote               */
#define  E_UA            -5     /* Unacceptable packet received, reset      */
#define  E_NOMEM         -6     /* Something failed due to lack of memory   */
#define  E_REFUSE        -7     /* Connection refused by remote             */
#define  E_BADSYN        -8     /* A SYN was received in the window         */
#define  E_BADHANDLE     -9     /* Bad connection handle used.              */
#define  E_LISTEN        -10    /* The connection is in LISTEN state        */
#define  E_NOCCB         -11    /* No free CCB's available                  */
#define  E_NOCONNECTION  -12    /* No connection matches this packet (TCP)  */
#define  E_CONNECTFAIL   -13    /* Failure to connect to remote port (TCP)  */
#define  E_BADCLOSE      -14    /* Invalid TCP_close() requested            */
#define  E_USERTIMEOUT   -15    /* A user function timed out                */
#define  E_CNTIMEOUT     -16    /* A connection timed out                   */
#define  E_CANTRESOLVE   -17    /* Can't resolve the hostname               */
#define  E_BADDNAME      -18    /* Domain name or dotted dec. bad format    */
#define  E_LOSTCARRIER   -19    /* The modem disconnected                   */
#define  E_NOHOSTNAME    -20    /* Hostname does not exist                  */
#define  E_DNSWORKLIMIT  -21    /* Resolver Work limit reached              */
#define  E_NONAMESERVER  -22    /* No nameservers could be found for query  */
#define  E_DNSBADFORMAT  -23    /* Bad format of DS query                   */
#define  E_UNREACHABLE   -24    /* Destination unreachable                  */
#define  E_DNSNOADDR     -25    /* No address records exist for host        */
#define  E_NOROUTINE     -26    /* Routine unavailable                      */
#define  E_LOCKED        -27    /* Locked by another application            */
#define  E_FRAGMENT      -28    /* Error during fragmentation               */
#define  E_TTLEXCEED     -29    /* Time To Live of an IP packet exceeded    */
#define  E_PARAMETER     -30    /* Problem with a parameter                 */
#define  E_BIGBUF        -31    /* Input buffer is too small for data       */
#define  E_LASTERROR      31    /* ABS of last error code in this list      */



/*--------------------------------------------------------------------------*/


/*
 *   IP packet header.
 */

typedef  struct ip_header {
    unsigned  version   : 4;    /* IP Version                               */
    unsigned  hd_len    : 4;    /* Internet Header Length                   */
    unsigned  tos       : 8;    /* Type of Service                          */
    uint16    length;           /* Total of all header, options and data    */
    uint16    ident;            /* Identification for fragmentation         */
    unsigned  reserved  : 1;    /* Reserved : Must be zero                  */
    unsigned  dont_frg  : 1;    /* Don't fragment flag                      */
    unsigned  more_frg  : 1;    /* More fragments flag                      */
    unsigned  frag_ofst : 13;   /* Fragment offset                          */
    uint8     ttl;              /* Time to live                             */
    uint8     protocol;         /* Protocol                                 */
    uint16    hdr_chksum;       /* Header checksum                          */
    uint32    ip_src;           /* Source IP address                        */
    uint32    ip_dest;          /* Destination IP address                   */
 } IP_HDR;


/*
 *   Internal IP packet representation.
 */

typedef  struct ip_packet {
    IP_HDR    hdr;              /* Header of IP packet                      */
    void      *options;         /* Options data block                       */
    int16     opt_length;       /* Length of options data block             */
    void      *pkt_data;        /* IP packet data block                     */
    int16     pkt_length;       /* Length of IP packet data block           */
    uint32    timeout;          /* Timeout of packet life                   */
    uint32    ip_gateway;       /* Gateway for forwarding this packet       */
    void      *recvd;           /* Receiving port                           */
    struct ip_packet  *next;    /* Next IP packet in IP packet queue        */
 } IP_DGRAM;


/*
 *   Input queue structures
 */

typedef struct ndb {        /* Network Data Block.  For data delivery       */
    char        *ptr;       /* Pointer to base of block. (For KRfree();)    */
    char        *ndata;     /* Pointer to next data to deliver              */
    uint16      len;        /* Length of remaining data                     */
    struct ndb  *next;      /* Next NDB in chain or NULL                    */
 } NDB;


/*
 *   Addressing information block.
 */

typedef  struct cab {
    uint16      lport;      /* TCP local  port     (ie: local machine)      */
    uint16      rport;      /* TCP remote port     (ie: remote machine)     */
    uint32      rhost;      /* TCP remote IP addr  (ie: remote machine)     */
    uint32      lhost;      /* TCP local  IP addr  (ie: local machine)      */
 } CAB;


/*
 *   Connection information block
 */

typedef struct cib {        /* Connection Information Block                 */
    uint16      protocol;   /* TCP or UDP or ... 0 means CIB is not in use  */
    CAB         address;    /* Adress information                           */
    uint16      status;     /* Net status. 0 means normal                   */
 } CIB;


/*
 *   Values for protocol field
 */

#define P_ICMP     1        /* IP assigned number for ICMP                  */
#define P_TCP      6        /* IP assigned number for TCP                   */
#define P_UDP     17        /* IP assigned number for UDP                   */



/*--------------------------------------------------------------------------*/


/*
 *   Transport structure / functions
 */

typedef  struct tpl
{
 char *       module;      /* Specific string that can be searched for     */
 char *       author;      /* Any string                                   */
 char *       version;     /* Format `00.00' Version:Revision              */
 CDECL1 void * CDECL2 (* KRmalloc) (int32 a);
 CDECL1 void   CDECL2 (* KRfree) (void *b);
 CDECL1 int32  CDECL2 (* KRgetfree) (int16 c);
 CDECL1 void * CDECL2 (* KRrealloc) (void *d, int32 e);
 CDECL1 char * CDECL2 (* get_err_text) (int16);
 CDECL1 char * CDECL2 (* getvstr) (char *);
 CDECL1 int16  CDECL2 (* carrier_detect) (void);
 CDECL1 int16  CDECL2 (* TCP_open) (uint32, uint16, uint16, uint16);
 CDECL1 int16  CDECL2 (* TCP_close) (int16, int16);
 CDECL1 int16  CDECL2 (* TCP_send) (int16, void *, int16);
 CDECL1 int16  CDECL2 (* TCP_wait_state) (int16, int16, int16);
 CDECL1 int16  CDECL2 (* TCP_ack_wait) (int16, int16);
 CDECL1 int16  CDECL2 (* UDP_open) (uint32, uint16);
 CDECL1 int16  CDECL2 (* UDP_close) (int16);
 CDECL1 int16  CDECL2 (* UDP_send) (int16, void *, int16);
 CDECL1 int16  CDECL2 (* CNkick) (int16);
 CDECL1 int16  CDECL2 (* CNbyte_count) (int16);
 CDECL1 int16  CDECL2 (* CNget_char) (int16);
 CDECL1 NDB *  CDECL2 (* CNget_NDB) (int16);
 CDECL1 int16  CDECL2 (* CNget_block) (int16, void *, int16);
 CDECL1 void   CDECL2 (* housekeep) (void);
 CDECL1 int16  CDECL2 (* resolve) (char *, char **, uint32 *, int16);
 CDECL1 void   CDECL2 (* ser_disable) (void);
 CDECL1 void   CDECL2 (* ser_enable) (void);
 CDECL1 int16  CDECL2 (* set_flag) (int16);
 CDECL1 void   CDECL2 (* clear_flag) (int16);
 CDECL1 CIB *  CDECL2 (* CNgetinfo) (int16);
 CDECL1 int16  CDECL2 (* on_port) (char *);
 CDECL1 void   CDECL2 (* off_port) (char *);
 CDECL1 int16  CDECL2 (* setvstr) (char *, char *);
 CDECL1 int16  CDECL2 (* query_port) (char *);
 CDECL1 int16  CDECL2 (* CNgets) (int16, char *, int16, char);
 CDECL1 int16  CDECL2 (* ICMP_send) (uint32, uint8, uint8, void *, uint16);
 CDECL1 int16  CDECL2 (* ICMP_handler) (/*__CDECL*/int16  (*fun)(IP_DGRAM *), int16);
 CDECL1 void   CDECL2 (* ICMP_discard) (IP_DGRAM *);
} TPL;

extern TPL *tpl;


/*
 *   Definitions of transport functions for direct use
 */

#define KRmalloc(x)                      (*tpl->KRmalloc)(x)
#define KRfree(x)                        (*tpl->KRfree)(x)
#define KRgetfree(x)                     (*tpl->KRgetfree)(x)
#define KRrealloc(x,y)                   (*tpl->KRrealloc)(x,y)
#define get_err_text(x)                  (*tpl->get_err_text)(x)
#define getvstr(x)                       (*tpl->getvstr)(x)
#define carrier_detect()                 (*tpl->carrier_detect)()
#define TCP_open(w,x,y,z)                (*tpl->TCP_open)(w,x,y,z)
#define TCP_close(x,y)                   (*tpl->TCP_close)(x,y)
#define TCP_send(x,y,z)                  (*tpl->TCP_send)(x,y,z)
#define TCP_wait_state(x,y,z)            (*tpl->TCP_wait_state)(x,y,z)
#define TCP_ack_wait(x,y)                (*tpl->TCP_ack_wait)(x,y)
#define UDP_open(x,y)                    (*tpl->UDP_open)(x,y)
#define UDP_close(x)                     (*tpl->UDP_close)(x)
#define UDP_send(x,y,z)                  (*tpl->UDP_send)(x,y,z)
#define CNkick(x)                        (*tpl->CNkick)(x)
#define CNbyte_count(x)                  (*tpl->CNbyte_count)(x)
#define CNget_char(x)                    (*tpl->CNget_char)(x)
#define CNget_NDB(x)                     (*tpl->CNget_NDB)(x)
#define CNget_block(x,y,z)               (*tpl->CNget_block)(x,y,z)
#define CNgetinfo(x)                     (*tpl->CNgetinfo)(x)
#define CNgets(w,x,y,z)                  (*tpl->CNgets)(w,x,y,z)
#define housekeep()                      (*tpl->housekeep)()
#define resolve(w,x,y,z)                 (*tpl->resolve)(w,x,y,z)
#define ser_disable()                    (*tpl->ser_disable)()
#define ser_enable()                     (*tpl->ser_enable)()
#define set_flag(x)                      (*tpl->set_flag)(x)
#define clear_flag(x)                    (*tpl->clear_flag)(x)
#define on_port(x)                       (*tpl->on_port)(x)
#define off_port(x)                      (*tpl->off_port)(x)
#define setvstr(x,y)                     (*tpl->setvstr)(x,y)
#define query_port(x)                    (*tpl->query_port)(x)
#define ICMP_send(v,w,x,y,z)             (*tpl->ICMP_send)(v,w,x,y,z)
#define ICMP_handler(x,y)                (*tpl->ICMP_handler)(x,y)
#define ICMP_discard(x)                  (*tpl->ICMP_discard)(x)



/*--------------------------------------------------------------------------*/


/*
 *   Handler flag values.
 */

#define  HNDLR_SET        0         /* Set new handler if space             */
#define  HNDLR_FORCE      1         /* Force new handler to be set          */
#define  HNDLR_REMOVE     2         /* Remove handler entry                 */
#define  HNDLR_QUERY      3         /* Inquire about handler entry          */



/*--------------------------------------------------------------------------*/


#endif /* STING_TRANSPRT_H */
