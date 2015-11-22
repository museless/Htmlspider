#ifndef	_SPNET_H
#define	_SPNET_H


/*---------------------
        include
-----------------------*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


/*---------------------
        define
-----------------------*/

#define	MIN_URL_LEN             0x80
#define MAX_URL_LEN             0x100

#define	LATEST_UPGRADE_LIMIT    0x10

#define	DBURL_ID_LEN            0x20    /* must be the same as spdb.h - URL_ID_LEN */
#define	WEB_TITLE_LEN           0x100
#define	WEB_CONTEXT_LEN         0x4000
#define	WEB_INDEX_LEN           0x30
#define	WEB_TIME_LEN            0x6
#define	WEB_DATE_LEN            0x8
#define	WEB_EXTRA_LEN           0x20

#define	WEB_COUNT_LEN           (WEB_EXTRA_LEN + WEB_INDEX_LEN + WEB_TIME_LEN + WEB_DATE_LEN + 3 * sizeof(int))

#define	HTTP_GFILE_STR          "GET %s%s HTTP/1.1\r\nHost: %s\r\n%s"

/* server respone */
#define	RESP_CONNECT_OK         200
#define	RESP_CONNECT_ACCEPT     202

#define	RESP_PERM_MOVE          301
#define	RESP_TEMP_MOVE          302

#define	RESP_SERV_UNAVAIABLE    503

/* server port */
#define HTTP_PORT               80

/* ping data */
#define DEFAULT_PING_HOST       "www.baidu.com"
#define DEFAULT_PING_PNUM       0x8


/*---------------------
        typedef
-----------------------*/

typedef struct  sp_ping_info    SPPING;
typedef	struct	web_if		    WEBIN;
typedef	struct	web		        WEB;
typedef	struct	web_txt		    TEXT;

typedef struct  sockaddr        SOCKAD;
typedef	struct	sockaddr_in	    SOCKIF;

typedef	int     (*fcset)(unsigned char *);


/*---------------------
        struct
-----------------------*/

struct sp_ping_info {
    char    p_host[SMALL_BUF];
    TMVAL   p_time;
    int     p_packnum;
};

struct	web {
	char	web_host[UHOST_LEN];
	char	web_path[UPATH_LEN];
	char	web_file[UFILE_LEN];
        
	int	    web_port;
    int     web_nlayer;             /* path's layer */
};

struct	web_if {
	WEBIN  *w_next;
	BUFF   *w_buff;		            /* store url */

	char   *w_url;			        /* store url (a temp value) */
	char   *w_conbuf;		        /* store website content */

	SOCKIF	w_sockif;
	WEB	    w_ubuf;

	char	w_latest[LATEST_LEN];	/* use for saving latest time */
	int	    w_latestcnt;		    /* when equal to LATEST_UPGRADE_LIMIT, update data to mysql */

	int	    w_pattern;		        /* pattern for current url */
	int	    w_size;			        /* size of webpage */
};

struct	web_txt {
	char   *wt_pool;
	int	    wt_plsize;

	int	    wt_gsize;		/* size of get content */
	int	    wt_coff;		/* content offset */
	int	    wt_csize;		/* size of real content */

	fcset	wt_csetfun;		/* hook the charset transform function */
	int     wt_cinc;		/* per word size at curren charset */

	char	wt_url[URL_LEN];
	char	wt_id[DBURL_ID_LEN];
	char	wt_charset[CHARSET_LEN];

	char	wt_index[WEB_INDEX_LEN];
	char	wt_title[WEB_TITLE_LEN];
	char	wt_time[WEB_TIME_LEN];

    int     wt_poff;
    int     wt_foff;

	uChar	wt_pattern;
	uChar	wt_tlen;
};


/*---------------------
     extern data
-----------------------*/

extern	char	*rPac;


/*---------------------
     global function
-----------------------*/

/* sp_network.c */
int     sp_net_set_sockif(const char *hostName, SOCKIF *sInfo);
int     sp_net_sock_connect(SOCKIF *sockInfo);
int     sp_net_sock_read(int nSock, char *savBuf, int bufLimit,
                         int readTimes, int nSec, long microSec);

int     sp_http_interact(WEB *wbStru, int nSock, char *strBuf, int bufSize);
long    sp_net_speed_ping(const char *ping_host, int num_pack);
#endif