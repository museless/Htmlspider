/*---------------------------------------------
 *     modification time: 2016-08-12 00:50:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Nine part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Urlbug main
 *       Part Five:  Initialization
 *       Part Six:   Running preparation
 *       Part Seven: Entrance
 *       Part Eight: Urlbug mainly work
 *
 *---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spuglobal.h"
#include "spurlb.h"


/*---------------------------------------------
 *        Part Two: Local function          
-*---------------------------------------------*/

/* Part Four */
static  void    ubug_command_analyst(int nPara, char **pComm);

/* Part Five */
static  int     mainly_init(void);
static  bool    ubug_init_source(void);
static  int     ubug_init_dbuf(BUFF **pBuff);
static  void    ubug_init_weblist(void);
static  bool    ubug_init_urllist(char *urlStr, WEB *webStu);
static  WEBIN  *ubug_list_entity_set(MSLROW data_row);
static  void    ubug_set_ubset(void);

/* Part Six */
static  void    ubug_pthread_entrance(void *parameters);
static  int     ubug_pthread_apply_for_resource(WEBIN *web_info);

/* Part Seven */
static  void    ubug_main_entrance(void);
static  void    ubug_text_abstract_cont(WEBIN *abPoint);

/* Part Eight */
static  void    ubug_job(WEBIN *wPoint);


/*---------------------------------------------
 *          Part Three: Define
-*---------------------------------------------*/

#define ubug_init_ubset(fInit, fEnt, fSt, fStf, fDway, rTime) { \
    urlRunSet.ubs_init = fInit; \
    urlRunSet.ubs_fent = fEnt; \
    urlRunSet.ubs_fst = fSt; \
    urlRunSet.ubs_fstf = fStf; \
    urlRunSet.ubs_dway = fDway; \
    urlRunSet.ubs_rtime = rTime; \
}

#define ubug_init_ubset_way(fun_catch, fun_locate, fun_creat) {\
    urlRunSet.ubs_catch = fun_catch; \
    urlRunSet.ubs_locate = fun_locate; \
    urlRunSet.ubs_creat = fun_creat; \
}


/*---------------------------------------------
 *          Part Four: Urlbug main
 *
 *          1. main
 *          2. ubug_command_analyst
 *
-*---------------------------------------------*/

/*-----main-----*/
int main(int argc, char **argv)
{
    log_start(&messageLog, "/var/log/urlcatcher", LOG_LOCAL2);

    ubug_command_analyst(argc, argv);

    if (mainly_init() && ubug_init_source()) {
        ubug_init_signal();
        ubug_init_database();
        ubug_init_weblist();
        ubug_init_network();

        ubug_main_entrance();
    }

    mgc_all_clean(&objGc);

    exit(FRET_Z);
}


/*-----ubug_command_analyst-----*/
static void ubug_command_analyst(int nPara, char **pComm)
{
    int     config_flags = 0;
    char    ch;

    while ((ch = getopt(nPara, pComm, "c:h")) != FRET_N) {
        switch (ch) {
            case 'c':   
                mc_load_config("urlbug", optarg);
                config_flags = 1;
                break;

            case 'h':
                ubug_print_help();
                exit(FUN_RUN_OK);    

            default:
                printf("Urlbug---> wrong command: %c\n \
                    \rUrlbug--->please try \"-h\"\n\n", ch);
                exit(FRET_Z);
        }
    }

    if (config_flags != 1) {
        printf("Urlbug---> must set [-c]\n");
        exit(FUN_RUN_FAIL);    
    }

    ubug_set_ubset();
}


/*---------------------------------------------
 *          Part Five: Initialization
 *
 *          1. mainly_init
 *          2. ubug_init_source
 *          4. ubug_init_dbuf
 *          5. ubug_init_weblist
 *          6. ubug_init_urllist
 *          7. ubug_set_ubset
 *          8. ubug_list_entity_set 
 *
-*----------------------------------------------*/

/*-----mainly_init-----*/
int mainly_init(void)
{
    if (!frame("Urlbug"))
        return  FRET_Z;

    mato_init(writeStoreLock, 1);

    if (!mc_conf_read("urlbug_max_ulen", CONF_NUM, &urlMaxLen, sizeof(int))) {
        setmsg(LM19, "urlbug_max_ulen");
        return  FRET_Z;
    }

    if (urlMaxLen < MIN_URL_LEN || urlMaxLen > MAX_URL_LEN) {
        setmsg(LM27, urlMaxLen);
        return  FRET_Z;
    }

    return  FUN_RUN_OK;
}


/*-----ubug_init_source-----*/
bool ubug_init_source(void)
{
    sigset_t    set;
    int32_t     pthread_num;

    /* urlbug pthread num read */
    if (!mc_conf_read("urlbug_pthread_num", CONF_NUM, &pthread_num, sizeof(int))) {
        setmsg(LM19, "urlbug_pthread_num");
        return  false;
    }

    if (pthread_num > UBUG_PTHREAD_MAX || pthread_num < UBUG_PTHREAD_MIN) {
        setmsg(LM21, pthread_num);
        return  false;
    }

    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGSEGV);

    /* muse thread pool init */
    if (!mpc_create(&ubugThreadPool, pthread_num, SIG_BLOCK, &set)) {
        setmsg(LM10);
        return  false;
    }

    if ((contStorePool = wmpool_create(pthread_num, WMP_PAGESIZE)) == NULL) {
        setmsg(LM11, "contStorePool");
        return  false;
    }

    if (!mgc_add(&objGc, contStorePool, wmpool_destroy))
        setmsg(LM5, "contStorePool");

    if ((urlStorePool = wmpool_create(pthread_num, NAMBUF_LEN)) == NULL) {
        setmsg(LM11, "urlStorePool");
        return  false;
    }

    if (!mgc_add(&objGc, urlStorePool, wmpool_destroy))
        setmsg(LM5, "urlStorePool");

    return  true;
}


/*-----ubug_init_dbuf-----*/
int ubug_init_dbuf(BUFF **pBuff)
{
    if (!((*pBuff) = buff_stru_init(SQL_GCOM_LEN)))
        return  FRET_Z;

    if (!mgc_add(&objGc, (*pBuff), buff_stru_free_all))
        setmsg(LM5, "Buffstru");

    return  FUN_RUN_OK;
}


/*-----ubug_init_weblist-----*/
void ubug_init_weblist(void)
{
    char    tabname[MIDDLE_BUF];

    if (!mc_conf_read("urls_store_table_name", CONF_STR, tabname, MIDDLE_BUF)) {
        setmsg(LM5, "urls_store_table_name");
        ubug_sig_error();
    }

    WEBIN **pList = &urlSaveList;
    MSLRES *data_result = mysql_return_result(&urlDataBase, GET_DIRECTORY, tabname);

    if (!data_result)
        ubug_db_seterror();

    MSLROW  data_row;

    while ((data_row = mysql_fetch_row(data_result))) {
        if (!(*pList = ubug_list_entity_set(data_row)))
            continue;

        pList = &((*pList)->w_next);
        (*pList) = NULL;
    }

    mysql_free_result(data_result);

    (*pList) = NULL;

    if (!mgc_add(&objGc, GC_DEFOBJ, ubug_free_weblist))
        setmsg(LM5, "weblist free");
}


/*-----ubug_list_entity_set-----*/
WEBIN *ubug_list_entity_set(MSLROW data_row)
{
    WEBIN   *list_point;

    if (!(list_point = malloc(sizeof(WEBIN)))) {
        ubug_sig_error();
    }

    if (!ubug_init_urllist(data_row[0], &((list_point)->w_ubuf)))
        ubug_sig_error();

    (list_point)->w_latest[0] = 0;

    if (data_row[1])
        sprintf((list_point)->w_latest, data_row[1]);

    list_point->w_latestcnt = list_point->w_conbufsize = 
    list_point->w_urlbufsize = list_point->w_contoffset = 0;

    if (!ubug_init_dbuf(&((list_point)->w_buff))) {
        setmsg(LM3);
        ubug_sig_error();
    }

    return  list_point;
}


/*-----ubug_init_urllist-----*/
bool ubug_init_urllist(char *urlStr, WEB *webStu)
{
    if (sp_url_seperate(urlStr, strlen(urlStr), webStu) == FRET_Z) {
        setmsg(LM23, urlStr);
        return  false;
    }

    return  true;
}


/*-----ubug_set_ubset-----*/
void ubug_set_ubset(void)
{
    ubug_init_ubset(NULL, ubug_text_abstract_cont, ubug_tran_db,
        ubug_tran_db_whole, ubug_html_download, RUN_PERN);

    ubug_init_ubset_way(ubug_catch_default_rule, 
        ubug_locate_default_rule, ubug_set_tabname_by_date);

    urlRunSet.ubs_creat();
}


/*---------------------------------------------
 *      Part Six: Running preparation
 *
 *      1. ubug_pthread_entrance
 *      2. ubug_pthread_apply_for_resource
 *
-*---------------------------------------------*/

/*-----ubug_pthread_entrance-----*/
void ubug_pthread_entrance(void *parameters)
{
    WEBIN  *web_info = (WEBIN *)parameters;
    int     fun_ret;

    if ((fun_ret = ubug_pthread_apply_for_resource(web_info)) == FRET_N)
        return;

    if (fun_ret == FRET_P)
        urlRunSet.ubs_fent(web_info);

    wmpool_free(contStorePool, web_info->w_conbuf);
    wmpool_free(urlStorePool, web_info->w_url);
}


/*-----ubug_pthread_apply_for_resource-----*/
int ubug_pthread_apply_for_resource(WEBIN *web_info)
{
    if (!(web_info->w_conbuf = wmpool_malloc(contStorePool))) {
        setmsg(LM24, "contbuf");
        return  FRET_N; 
    }

    if (!(web_info->w_url = wmpool_malloc(urlStorePool))) {
        wmpool_free(contStorePool, web_info->w_conbuf);
        setmsg(LM24, "url store");
        return  FRET_N;
    }

    web_info->w_conbufsize = WMP_PAGESIZE;
    web_info->w_urlbufsize = NAMBUF_LEN;
    web_info->w_size = urlRunSet.ubs_dway(web_info);

    setmsg(LM25, web_info->w_ubuf.web_host, web_info->w_ubuf.web_path, 
        web_info->w_ubuf.web_file, web_info->w_size);

    return  (web_info->w_size > 0) ? FRET_P : FRET_Z;
}


/*---------------------------------------------
 *          Part Seven: Entrance
 *
 *          1. ubug_main_entrance
 *          2. ubug_text_abstract_cont
 *
-*---------------------------------------------*/

/*-----ubug_main_entrance-----*/
void ubug_main_entrance(void)
{
    WEBIN  *info;

    do {
        ubug_ping();
        urlCatchNum = 0;

        for (info = urlSaveList; info; info = info->w_next) {
            while (!mpc_thread_wake(&ubugThreadPool, ubug_pthread_entrance, info))
                ;   /* nothing */        
        }

        urlRunSet.ubs_fstf();
        sleep(20);

    } while (urlRunSet.ubs_rtime);

    ubug_sig_error();
}


/*-----ubug_text_abstract_cont-----*/
void ubug_text_abstract_cont(WEBIN *abPoint)
{
    ubug_job(abPoint);
}


/*---------------------------------------------
 *      Part Eight: Urlbug mainly work
 *
 *      1. ubug_job
 *
-*---------------------------------------------*/

/*-----ubug_job-----*/
static void ubug_job(WEBIN *wPoint)
{
    char   *content_begin, *content_end;
    char   *url_head, *url_end;
    int     url_catch_len, host_len = strlen(wPoint->w_ubuf.web_host);
    UDATA   url_data;

    urlRunSet.ubs_locate(wPoint, &content_begin, &content_end);

    for (; content_begin && content_begin < content_end; content_begin = url_end) {
        url_catch_len = urlRunSet.ubs_catch(content_begin, &url_end);

        if (url_catch_len == FRET_N)
            return;

        if (url_catch_len == FRET_Z)
            continue;

        url_head = url_end - url_catch_len;

        ubug_check_separator(url_head, &url_catch_len);

        if (strncmp(url_head, MATCH_HTTP, MHTTP_LEN)) {
            url_head = ubug_connect_head(wPoint, host_len, url_head, &url_catch_len);

            if (!url_head)
                continue;
        }

        if (ubug_check_url_prefix(url_head, url_catch_len))
            continue;
    
        url_layer_extract(&url_data, url_head, url_catch_len);

        urlRunSet.ubs_fst(wPoint, &url_data, url_head, url_catch_len);
        urlCatchNum++;
    }
}

