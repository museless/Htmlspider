#ifndef	_SPINC_H
#define	_SPINC_H


/*---------------------------------------------
 *	                include
-*---------------------------------------------*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <unistd.h>

#include <pthread.h>

#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdarg.h>

#include <signal.h>

#include <time.h>
#include <fcntl.h>

#include <getopt.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/types.h>

#include <iconv.h>
#include <errno.h>

#include "sperr.h"

#include "confparser.h"
#include "mato.h"
#include "mgc.h"


/*---------------------------------------------
 *	                define
-*---------------------------------------------*/

#define	FUN_RUN_OK	    1
#define	FUN_RUN_END	    0
#define	FUN_RUN_FAIL	-1

#define FRET_P          1
#define FRET_Z          0
#define FRET_N          -1
#define FRET_UNIQUE     -2

#define	FRET_VAL	    FUN_RUN_OK
#define	FRET_NULL	    FUN_RUN_END

#define	OWN_NAME_LEN	0x9

#define	CHARSET_LEN     0x10

#define	HEAD_LEN	    0x30

#define	LATEST_LEN	    0x40
#define	PATH_LEN	    0x80

#define	UHOST_LEN	    0x100
#define	UPATH_LEN	    0xC0
#define	UFILE_LEN	    0x40
#define	URL_LEN		    (UHOST_LEN + UPATH_LEN + UFILE_LEN)

#define	RECE_DATA	    0x1000
#define	HTTP_RECALL	    0x400 

#define	DATE_CMODE	    0x7

#define	SMALL_BUF	    0x10
#define	MIDDLE_BUF	    0x30
#define	BIG_BUF		    0x100
#define	HUGE_BUF	    0x1000

#define	NAMBUF_LEN      0x100
#define	FNAME_LEN	    0x40
#define	LOGBUF_LEN	    0x20000
#define	LOGBASE_LEN	    0x20
#define	DATEBUF_LEN	    0x8

#define	NPAPER_LEN	    0x40

#define FILE_OPERATION  (O_RDWR | O_CREAT | O_TRUNC)
#define	FILE_AUTHOR     (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/* sem project id */
#define	PROJ_PTH_CTL	0x0
#define	PROJ_SHM_CTL	0x1

/* elog */
#define	EMPTY_OBJ_STR   "Empty"
#define	FUNCTION_STR	"Function"
#define	THREAD_STR	    "Thread"
#define	ERROR_STR	    strerror(errno)
#define	HERROR_STR	    ((char *)hstrerror(h_errno))
#define	MYERR_STR(pSql)	((char *)mysql_error(pSql))

/* sleep time: sec */
#define	TAKE_A_NO       0x0
#define	TAKE_A_SEC      0x1
#define	TAKE_A_NOTHING  0x2
#define	TAKE_A_EYECLOSE 0x4
#define	TAKE_A_SHNAP    0x8
#define	TAKE_A_NAP      0x10
#define	TAKE_A_REST     0x20
#define	TAKE_A_SLEEP    0x40
#define	TAKE_A_LONGSLP  0x80
#define	TAKE_A_LLSLP    0x12C
#define TAKE_A_WHILE    0x258

/* sleep time: microsec */
#define	MICSEC_PER_SEC	1000000
#define	MILLISEC_500	500000
#define MICSEC_500000   500000
#define	TINY_USEC	    0x120000

/* time transfer */
#define MICSEC_PER_SEC  1000000
#define MILSEC_PER_SEC  1000


/*---------------------------------------------
 *                  typedef
-*---------------------------------------------*/

typedef pthread_t       pth_t;
typedef pthread_attr_t  pattr_t;

typedef unsigned char   uChar;
typedef unsigned int    uInt;
typedef unsigned long   uLong;
typedef unsigned short  uShort;

typedef struct	stat    STAT;
typedef struct	tm      TMS;
typedef struct	timeval TMVAL;

typedef struct	buff    BUFF;
typedef struct  url_dat UDATA;

typedef void	*(*mafun)(void *, int);
typedef void	(*mafree)(void *);
typedef void	(*sigfun)(int);


/*---------------------------------------------
 *                  struct
-*--------------------------------------------*/

struct	buff {
	void   *b_start;
	
	int     b_cap;
	int     b_size;
};

struct  url_dat {
    int     ud_poff;        /* path offset */
    int     ud_foff;        /* file offset */
};


/*---------------------------------------------
 *	             extern data
-*--------------------------------------------*/

extern	char	kerNameBuf[], ubNameBuf[];
extern	char	tbNameBuf[], ebNameBuf[];


/*---------------------------------------------
 *	            global function
-*---------------------------------------------*/


/* sp_misc.c */
void	sp_stop(void);
void	sp_stop_str(void);

/* sp_html.c */
char   *sp_html_tag_range_locate(const char *html_string, int len);
char   *sp_html_get_tag_string(char *tag_start, int *tag_len);
char   *sp_html_code_location(const char *html_text, 
            int text_len, const char *html_code);

/* sp_pfun.c */
int	    readn(int rFd, void *rBuf, size_t bCount);
int	    writen(int wFd, void *wBuf, size_t bCount);

char   *strnstr(char *findBuf, const char *needStr, int nLimit);
char   *strnchr(char *findBuf, char needCh, int nLimit);
int     select_read(int nSock, char *readBuf, int nRead, int nSec, long microSec);
char   *strchrb(char *strBeg, int nLimit, char fCh);
int	    atoin(char *datBuf, int nLen);
int	    read_all_file(char **pStore, char *ofName, int readOff);
int	    examine_empty_string(char *exaStr);

TMS	   *time_str_extract(char *timStr);
int     count_url_layer(const char *str_url);

void    url_layer_extract(UDATA *pData, char *urlStr, int uLen);
int     count_enter_num(const char *string);


/* fun libary */

/* dboper/dboper.c */
int	    mysql_string_exist_check(void *chkSql, char *chkCom);
void   *mysql_return_result(void *sql_handle, const char *sql_string, ...);
int     mysql_simple_connect(
        void *sql_handle, const char *database_name, const char *host, int port);

int     mysql_creat_table(void *sql_handle, const char *creat_string, ...);
int     mysql_error_log(void *sql_handle, char *db_name, char *error_string);

/* sp_bufoper.c */

/* 1 means empty
   0 means no empty
  -1 means failed */
int	    buff_stru_empty(BUFF *pBuff);
/* 1 means enough 
   0 means no enough
  -1 means failed */
int	    buff_size_enough(BUFF *pBuff, int nCheck);
int	    buff_check_exist(BUFF *cBuff);
void	buff_stru_make_empty(BUFF *pBuff);
int	    buff_now_size(BUFF *pBuff);
char   *buff_place_locate(BUFF *pBuff, int nSize);
char   *buff_place_end(BUFF *pBuff);
char   *buff_place_start(BUFF *pBuff);
void	buff_size_add(BUFF *pBuff, int addSize);

BUFF   *buff_stru_init(int nMalloc);
void	buff_stru_free_all(void *bufStru);

char   *buff_stru_strstr(BUFF *strBuf, char *needStr);

/* sp_elog.c */
int	    elog_init(char *confStr);
int	    elog_write(char *errStr, char *objStr1, char *objStr2);
void	elog_write_force(void);
void	elog_destroy(void);

#endif
