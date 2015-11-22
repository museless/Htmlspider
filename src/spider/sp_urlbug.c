/*------------------------------------------
	Source file content Nine part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:  Urlbug main
	Part Five:	Initialization
	Part Six:	Running preparation
	Part Seven:	Entrance
	Part Eight:	Urlbug mainly work

--------------------------------------------*/

/*------------------------------------------
	        Part Zero: Include
--------------------------------------------*/

#include "sp.h"

#include "mipc.h"
#include "mpctl.h"

#include "spuglobal.h"
#include "spurlb.h"


/*------------------------------------------
	        Part One: Local data
--------------------------------------------*/


/*------------------------------------------
	      Part Two: Local function          
--------------------------------------------*/

/* Part Four */
static	void	ubug_command_analyst(int nPara, char **pComm);

/* Part Five */
static	int	    mainly_init(void);
static	int	    ubug_init_source_pool(void);
static  int     ubug_init_dbuf(BUFF **pBuff);
static	void	ubug_init_weblist(void);
static	int	    ubug_init_urllist(char *urlStr, WEB *webStu);
static  WEBIN  *ubug_list_entity_set(MSLROW data_row);
static  void    ubug_load_config(const char *config_path);
static  void    ubug_set_ubset(const char *way_option);

/* Part Six */
static	void	ubug_create_pthread(WEBIN *webNode);
static	void	ubug_pthread_entrance(void *nParameter);

/* Part Seven */
static	void	ubug_main_entrance(void);
static	void	ubug_text_abstract_cont(WEBIN *abPoint);

/* Part Eight */
static	void	ubug_job(WEBIN *wPoint);


/*------------------------------------------
	        Part Three: Define
--------------------------------------------*/

#define	ubug_init_ubset(fInit, fEnt, fSt, fStf, fDway, rTime) { \
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


/*------------------------------------------
	    Part Four: Urlbug main

        1. main
        2. ubug_command_analyst

--------------------------------------------*/

/*-----main-----*/
int main(int argc, char **argv)
{
	ubug_command_analyst(argc, argv);

	if(mainly_init() && ubug_init_source_pool()) {
		ubug_init_signal();
		ubug_init_database();
		ubug_init_weblist();
	    ubug_init_pinginfo();

        sp_stop_str();

		ubug_main_entrance();
	}

	mgc_all_clean(urlGarCol);

	exit(FUN_RUN_END);
}


/*-----ubug_command_analyst-----*/
static void ubug_command_analyst(int nPara, char **pComm)
{
	int	    config_flags = 0;
    char    ch;

    while ((ch = getopt(nPara, pComm, "c:d:h")) != FUN_RUN_FAIL) {
        switch (ch) {
            case 'c':	
                ubug_load_config(optarg);
                config_flags = 1;
                break;

            case 'h':
                ubug_print_help();
                exit(FUN_RUN_OK);    
        
            case 'd':
                ubug_set_ubset(optarg);
                break;
            
            case 'f':
                procCommuFd = atoi(optarg);
                break;

            default:
			    printf(
                "Urlbug---> wrong command: %c\n \
			    \r--->please try \"-h\" or \"--help\"\n\n", ch);
			    exit(FUN_RUN_END);
        }
    }

    if (urlRunSet.ubs_fent == NULL || !config_flags) {
        printf("Urlbug---> must set [-d] and [-c]\n");
		exit(FUN_RUN_FAIL);    
    }
}


/*------------------------------------------
	    Part Five: Initialization

        1. mainly_init
        2. ubug_init_source_pool
        4. ubug_init_dbuf
        5. ubug_init_weblist
        6. ubug_init_urllist
        7. ubug_load_config
        8. ubug_set_ubset
        9. ubug_list_entity_set 

--------------------------------------------*/

/*-----mainly_init-----*/
static int mainly_init(void)
{
	if (!sp_normal_init(
         "Urlbug", &urlGarCol, &urlMsgSet,
         ubug_msg_init, "urlbug_err_log", procCommuFd))
		return	FUN_RUN_END;

	mato_init(&writeStoreLock, 1);

    /* max url len read, size of table U20xxxxxx Url field */
	if (mc_conf_read(
        "urlbug_max_ulen", CONF_NUM,
        &urlMaxLen, sizeof(int)) == FUN_RUN_FAIL) {
	    mc_conf_print_err("urlbug_max_ulen");
		return	FUN_RUN_END;
	}

	if (urlMaxLen < MIN_URL_LEN || urlMaxLen > MAX_URL_LEN) {
		printf("Urlbug---> url max size is underlimit: %d\n", urlMaxLen);
		return	FUN_RUN_END;
	}

	return	FUN_RUN_OK;
}


/*-----ubug_init_source_pool-----*/
static int ubug_init_source_pool(void)
{
    int pthread_num;

	/* urlbug pthread num read */
	if (mc_conf_read(
        "urlbug_pthread_num",
        CONF_NUM, &pthread_num, sizeof(int)) == FUN_RUN_FAIL) {
		mc_conf_print_err("urlbug_pthread_num");
		return	FUN_RUN_END;
	}

	if (pthread_num > UBUG_PTHREAD_MAX || pthread_num < UBUG_PTHREAD_MIN) {
		printf("Urlbug---> pthread num is underlimit: %d\n", pthread_num);
		return	FUN_RUN_END;
	}

	/* muse thread pool init */
	if (!(ubugThreadPool = mpc_create(pthread_num))) {
		ubug_perror("ubug_init_source_pool - mpc_create", errno);
		return	FUN_RUN_END;
	}

	if ((contStorePool = wmpool_create(pthread_num, WMP_PAGESIZE)) == NULL) {
		ubug_perror("wmpool_create - contStorePool", errno);
		return	FUN_RUN_END;
	}

	if (mgc_add(urlGarCol, contStorePool, wmpool_destroy) == MGC_FAILED)
		ubug_perror("ubug_init_source_pool - mgc_add - contStorePool", errno);

	if ((urlStorePool = wmpool_create(pthread_num, NAMBUF_LEN)) == NULL) {
		ubug_perror("wmpool_create - urlStorePool", errno);
		return	FUN_RUN_END;
	}

	if (mgc_add(urlGarCol, urlStorePool, wmpool_destroy) == MGC_FAILED)
		ubug_perror("ubug_init_source_pool - mgc_add - urlStorePool", errno);

	return	FUN_RUN_OK;
}


/*-----ubug_init_dbuf-----*/
static int ubug_init_dbuf(BUFF **pBuff)
{
	if (((*pBuff) = buff_stru_init(SQL_GCOM_LEN)) == NULL) {
		ubug_perror("ubug_init_dbuf - buff_stru_init", errno);
		return  FUN_RUN_END;
	}

	if (mgc_add(urlGarCol, (*pBuff), buff_stru_free_all) == MGC_FAILED)
		ubug_perror("ubug_init_dbuf - mgc_add", errno);

        return  FUN_RUN_OK;
}


/*-----ubug_init_weblist-----*/
static void ubug_init_weblist(void)
{
    char    url_store_table_name[MIDDLE_BUF];

    if (mc_conf_read(
        "urls_store_table_name", CONF_STR,
        url_store_table_name, MIDDLE_BUF) == FUN_RUN_FAIL) {
        mc_conf_print_err("urls_store_table_name");
		ubug_sig_error();
    }

    WEBIN **pList = &urlSaveList;
	MSLROW	allRow;
    MSLRES *data_result= 
            mysql_return_result(&urlDataBase, GET_DIRECTORY, url_store_table_name);

    if (!data_result) {
		if (ubug_dberr(
            &urlDataBase, "ubug_init_weblist - mysql_return_result") != FUN_RUN_OK)
			ubug_sig_error();
	}

	while ((allRow = mysql_fetch_row(data_result))) {
        if (!(*pList = ubug_list_entity_set(allRow)))
            continue;

		pList = &((*pList)->w_next);
		(*pList) = NULL;
	}

	mysql_free_result(data_result);

	(*pList) = NULL;

	if(mgc_add(urlGarCol, NULL_POINT, ubug_free_weblist) == MGC_FAILED)
		ubug_perror("ubug_init_weblist - mgc_add", errno);
}


/*-----ubug_list_entity_set-----*/
static WEBIN *ubug_list_entity_set(MSLROW data_row)
{
    WEBIN   *list_point;

    if ((list_point = malloc(sizeof(WEBIN))) == NULL) {
        elog_write(
        "ubug_list_entity_set - malloc", FUNCTION_STR, ERROR_STR);
        ubug_sig_error();
    }

    if (!ubug_init_urllist(data_row[0], &((list_point)->w_ubuf)))
	    ubug_sig_error();

    if (data_row[1]) {
	    (list_point)->w_latest[strlen(data_row[1])] = 0;
	    strcpy((list_point)->w_latest, data_row[1]);
	}

    (list_point)->w_latestcnt = 0;

    if (!ubug_init_dbuf(&((list_point)->w_buff))) {
        elog_write(
        "ubug_list_entity_set - ubug_init_dbuf", FUNCTION_STR, ERROR_STR);

        ubug_sig_error();
    }

    if (!sp_net_set_sockif(
        (list_point)->w_ubuf.web_host, &(list_point)->w_sockif)) {
        elog_write(
        "ubug_list_entity_set - sp_net_set_sockif", FUNCTION_STR, ERROR_STR);

		free(list_point);
		return  NULL;
	}

    return  list_point;
}


/*-----ubug_init_urllist-----*/
static int ubug_init_urllist(char *urlStr, WEB *webStu)
{
    char   *pSlash;
	int	    nCir, nBack, urlLen, nOff = 0;

	bzero(webStu, sizeof(WEB));

	if (strncmp(urlStr, MATCH_HTTP, MHTTP_LEN)) {
		printf("Urlbug---> ubug_init_urllist - lack of http: %s\n", urlStr);
		return	FUN_RUN_END;
	}

	webStu->web_port = HTTP_PORT;
	urlLen = strlen(urlStr += MHTTP_LEN);

    if ((pSlash = strnchr(urlStr, '/', urlLen))) {
        nCir = pSlash - urlStr;
        sprintf(webStu->web_host, "%.*s", nCir, urlStr);

        if ((pSlash = strchrb(urlStr, urlLen, '/'))) {
            if ((nBack = pSlash - urlStr) != nCir)
                nOff = sprintf(
                       webStu->web_file, "%.*s", 
                       urlLen - nBack - 1, &urlStr[nBack + 1]);

            webStu->web_file[nOff] = 0;
        }

        sprintf(webStu->web_path, "%.*s", nBack - nCir + 1, &urlStr[nCir]);
        webStu->web_nlayer = ubug_url_count_nlayer(webStu->web_path);

        printf(
        "Host: %s - Path: %s - File: %s - Layer num: %d\n", 
        webStu->web_host, webStu->web_path,
        webStu->web_file, webStu->web_nlayer);

        return	FUN_RUN_OK;
    }

	printf("Urlbug---> ubug_init_urllist - strange url: %s\n", urlStr);

	return	FUN_RUN_END;
}


/*-----ubug_load_config-----*/
static void ubug_load_config(const char *config_path)
{
    if (mc_conf_load("Urlbug", config_path) == FUN_RUN_FAIL) {
        printf("Urlbug---> load configure failed\n");
        ubug_perror("ubug_load_config - mc_conf_load", errno);
        mc_conf_unload();
        exit(FUN_RUN_FAIL);
    }
}


/*-----ubug_set_ubset-----*/
static void ubug_set_ubset(const char *way_option)
{
    ubug_init_ubset(
    NULL, ubug_text_abstract_cont, ubug_tran_db,
    ubug_tran_db_whole, ubug_download_website, RUN_PERN);

    if (!strcmp(way_option, "normal")) {
        ubug_init_ubset_way(
        ubug_catch_default_rule, 
        ubug_locate_default_rule, ubug_set_tabname_default);

    } else if (!strcmp(way_option, "csto")) {
        ubug_init_ubset_way(
        ubug_catch_csto_rule, ubug_locate_csto_rule, ubug_set_tabname_default);

    } else {
        printf("Urlbug---> -d requires an argument");
        exit(FUN_RUN_FAIL);
    }

    urlRunSet.ubs_creat();
}


/*------------------------------------------
        Part Six: Running preparation

        1. ubug_create_pthread
        2. ubug_pthread_entrance

--------------------------------------------*/

/*-----ubug_create_pthread-----*/
static void ubug_create_pthread(WEBIN *webNode)
{
	if(!(webNode->w_conbuf = wmpool_malloc(contStorePool))) {
		ubug_perror("ubug_create_pthread - wmpool_malloc - contbuf", errno);
		ubug_sig_error();
	}

	if(!(webNode->w_url = wmpool_malloc(urlStorePool))) {
		ubug_perror("ubug_create_pthread - wmpool_malloc - url", errno);
		ubug_sig_error();
	}

	if((webNode->w_size = urlRunSet.ubs_dway(webNode)) > FUN_RUN_END) {
		if(mpc_thread_wake(ubugThreadPool, ubug_pthread_entrance, (void *)webNode))
			return;

		elog_write("ubug_create_pthread - mpc_thread_wake", FUNCTION_STR, ERROR_STR);
		ubug_sig_error();
	}

	wmpool_free(urlStorePool, webNode->w_url);
	wmpool_free(contStorePool, webNode->w_conbuf);
}


/*-----ubug_pthread_entrance-----*/
void ubug_pthread_entrance(void *nParameter)
{
	WEBIN	*webNode = (WEBIN *)nParameter;

	urlRunSet.ubs_fent(webNode);

	wmpool_free(contStorePool, webNode->w_conbuf);
	wmpool_free(urlStorePool, webNode->w_url);
}


/*------------------------------------------
	    Part Seven: Entrance

        1. ubug_main_entrance
        2. ubug_text_abstract_cont

--------------------------------------------*/

/*-----ubug_main_entrance-----*/
static void ubug_main_entrance(void)
{
	WEBIN	*webPoint;

	do {
        ubug_ping();

		for (urlCatchNum = 0, webPoint = urlSaveList;
             webPoint != NULL; webPoint = webPoint->w_next) {
	        ubug_create_pthread(webPoint);
		}

		urlRunSet.ubs_fstf();

		if(procCommuFd)
			sp_msg_frame_run(urlMsgSet, NULL);

		sleep(TAKE_A_REST);

	} while(urlRunSet.ubs_rtime);

	ubug_sig_error();
}


/*-----ubug_text_abstract_cont-----*/
static void ubug_text_abstract_cont(WEBIN *abPoint)
{
	abPoint->w_pattern = ubug_get_pattern(abPoint->w_ubuf.web_host);
	ubug_job(abPoint);
}


/*------------------------------------------
	    Part Eight: Urlbug mainly work

        1. ubug_job

--------------------------------------------*/

/*-----ubug_job-----*/
static void ubug_job(WEBIN *wPoint)
{
    char   *content_begin, *content_end;
    char   *pHttp, *pSign;
    int     getLen, hostLen = strlen(wPoint->w_ubuf.web_host);

    urlRunSet.ubs_locate(wPoint, &content_begin, &content_end);

	pHttp = content_begin;

	for (; content_begin && content_begin < content_end; content_begin = pSign) {
        if ((getLen = urlRunSet.ubs_catch(content_begin, &pSign)) == FUN_RUN_FAIL)
            return;

        if (getLen == FUN_RUN_END)
            continue;

        pHttp = pSign - getLen;

	    ubug_check_separator(pHttp, &getLen);

        if (strncmp(pHttp, MATCH_HTTP, MHTTP_LEN)) {
            if ((pHttp = ubug_connect_head(wPoint, hostLen, pHttp, &getLen)) == NULL)
                continue;
        }

        if (ubug_check_url_prefix(pHttp, getLen))
            continue;
    
        UDATA   url_data;

        url_layer_extract(&url_data, pHttp, getLen);

        urlRunSet.ubs_fst(wPoint, &url_data, pHttp, getLen);
        urlCatchNum++;
	}
}


