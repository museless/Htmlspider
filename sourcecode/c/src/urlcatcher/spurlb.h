#ifndef	_SPURLB_H
#define	_SPURLB_H


/*---------------------------
	       include
-----------------------------*/


/*---------------------------
	       define
-----------------------------*/

#define RUN_ONCE                0x0
#define RUN_PERN                0x1

#define UBUG_NREAD              0x80

#define UBUG_PTHREAD_MIN        0x1
#define UBUG_PTHREAD_MAX        0x4

#define UBUG_READ_SEC           0x0
#define UBUG_READ_MICROSEC      0x31000


/*---------------------------
	   global function
-----------------------------*/

/* sp_urlbug.c */
char   *ubug_reach_url_head(char *pSrc, char *pLimit);
void    ubug_init_datebuf(char *pTime);

/* sp_urlbug_rule.c */
int     ubug_catch_default_rule(char *content, char **ret_content_point);
int     ubug_locate_default_rule(
        WEBIN *web_point, char **content_beg, char **content_end);

char   *ubug_connect_head(WEBIN *wInfo, int hostLen, char *fName, int *fLen);
void	ubug_check_separator(char *urlStr, int *uLen);
int	    ubug_check_url_prefix(char *preSrc, int nLen);
int     ubug_is_today_news(char *string, int nLimit);

void    ubug_set_tabname_default(void);
void    ubug_set_tabname_by_date(void);

/* sp_urlbug_network.c */
void    ubug_init_pinginfo(void);
void    ubug_ping(void);

int     ubug_html_download(WEBIN *web_stu);

/* sp_urlbug_db.c */
void    ubug_init_database(void);
void    ubug_create_dbtable(void);

        /* pInfo (WEBIN), uData (UDATA) */
void    ubug_tran_db(void *pInfo, void *uData, char *pUrl, int uLen);

void    ubug_tran_db_whole(void);
void    ubug_tran_db_force(BUFF *pBuff);
int     ubug_tran_db_real(BUFF *pBuff);

int     ubug_dberr(MYSQL *sHandler, char *withStr);

/* sp_urlbug_disc.c */
void    ubug_print_help(void);

/* sp_urlbug_signal.c */
void    ubug_init_signal(void);
void    ubug_signal_handler(int nSign);

/* sp_urlbug_clean.c */
void    ubug_free_weblist(void *pNull);
void    ubug_db_clean(void *pNULL);

/* sp_urlbug_msg.c */
void   *ubug_msg_init(int msgFd);

/* sp_urlbug.err.c */
void    ubug_sig_error(void);
void    ubug_perror(char *errStr, int nErr);

#endif
