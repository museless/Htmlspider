/*---------------------------------------------
 *     modification time: 2016-10-25 15:40
 *     mender: Muse
-*---------------------------------------------*/

#pragma once


/*---------------------------------------------
 *	                define
-*---------------------------------------------*/

#define UBUG_PTHREAD_MIN        0x1
#define UBUG_PTHREAD_MAX        0x4


/*---------------------------------------------
 *	            global function
-*---------------------------------------------*/

/* urlcatcher.c */
char   *ubug_reach_url_head(char *pSrc, char *pLimit);
void    ubug_init_datebuf(char *pTime);

/* rule.c */
int     ubug_catch_default_rule(char *content, char **ret_content_point);
int     ubug_locate_default_rule(
        WEBIN *web_point, char **content_beg, char **content_end);

char   *ubug_connect_head(WEBIN *wInfo, int hostLen, char *fName, int *fLen);
void	ubug_check_separator(char *urlStr, int *uLen);
int	    ubug_check_url_prefix(char *preSrc, int nLen);
int     ubug_is_today_news(char *string, int nLimit);

void    ubug_set_tabname_default(void);
void    ubug_set_tabname_by_date(void);

/* network.c */
void    ubug_init_network(void);
void    ubug_ping(void);

int     ubug_html_download(WEBIN *web_stu);

/* databasectl.c */
void    ubug_init_database(void);
void    ubug_create_dbtable(void);

        /* pInfo (WEBIN), uData (UDATA) */
void    ubug_tran_db(void *pInfo, void *uData, char *url, int len);

void    ubug_tran_db_whole(void);
void    ubug_tran_db_force(BUFF *pBuff);

void    ubug_db_seterror(void);

/* signal.c */
void    ubug_init_signal(void);
void    ubug_signal_handler(int nSign);

/* misc.c */
void    ubug_free_weblist(void *pNull);
void    ubug_db_clean(void *pNULL);
void    ubug_print_help(void);

/* error.c */
void    ubug_sig_error(void);

