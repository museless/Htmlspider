/*------------------------------------------
	Source file content Eleven part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:  Urlbug main
	Part Five:	Initialization
	Part Six:	Running preparation
	Part Seven:	Entrance
	Part Eight:	Urlbug mainly work
	Part Nine:	Message pluging

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

static	int	    nCatchNum;


/*------------------------------------------
	     Part Two: Local function
--------------------------------------------*/

/* Part Four */
static	void	ubug_command_analyst(int nPara, char **pComm);

/* Part Five */
static	int	    mainly_init(void);
static	int	    ubug_init_source_pool(void);
static	void	ubug_init_datebuf(char *pTime);
static  int     ubug_init_dbuf(BUFF **pBuff);
static	void	ubug_init_weblist(void);
static	int	    ubug_init_urllist(char *urlStr, WEB *webStu);
static  int     ubug_url_count_nlayer(char *strPath);

/* Part Six */
static	void	ubug_create_pthread(WEBIN *webNode);
static	void	ubug_pthread_entrance(void *nParameter);

/* Part Seven */
static	void	ubug_main_entrance(void);
static	void	ubug_text_abstract_cont(WEBIN *abPoint);
static	void	ubug_text_store_content(WEBIN *stPoint);

/* Part Eight */
static	void	ubug_job(WEBIN *wPoint);
static	char   *ubug_connect_head(WEBIN *wInfo, int hostLen, char *fName, int *fLen);
static	int	    ubug_get_pattern(const char *pHost);
static	void	ubug_check_separator(char *urlStr, int *uLen);
static	int	    ubug_check_url_prefix(char *preSrc, int nLen);


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


/*------------------------------------------
	    Part Four: Urlbug main

        1. main
        2. ubug_command_analyst

--------------------------------------------*/

/*-----main-----*/
int main(int argc, char *argv[])
{
	ubug_command_analyst(argc, argv);

	if(mainly_init() && ubug_init_source_pool()) {
		ubug_init_signal();
		ubug_init_database();
		ubug_init_weblist();
	    ubug_init_pinginfo();

		ubug_main_entrance();
	}

	mgc_all_clean(urlGarCol);

	exit(FUN_RUN_END);
}


/*-----ubug_command_analyst-----*/
static void ubug_command_analyst(int nPara, char **pComm)
{
	int	nCir, cOff, tFlags = 0;

	for(cOff = -1, nCir = 1; nCir < nPara; nCir++) {
		if (!strcmp(pComm[nCir], "--help") || !strcmp(pComm[nCir], "-h")) {
			ubug_print_help();
			exit(FUN_RUN_OK);

		} else if (!strcmp(pComm[nCir], "--write_db") || !strcmp(pComm[nCir], "-d")) {
			ubug_init_ubset(
            NULL, ubug_text_abstract_cont, ubug_tran_db, ubug_tran_db_whole,
			ubug_download_website, RUN_PERN);

		} else if (!strcmp(pComm[nCir], "--down_website") || !strcmp(pComm[nCir], "-l")) {
			ubug_init_ubset(
            NULL, ubug_text_store_content, NULL, NULL,
			ubug_download_website, RUN_ONCE);

		} else if (!strcmp(pComm[nCir], "-f")) {
			procCommuFd = atoi(pComm[++nCir]);

		} else if (!strcmp(pComm[nCir], "-t")) {
			ubug_init_datebuf(pComm[++nCir]); tFlags = 1;

		} else if (!strcmp(pComm[nCir], "-c")) {
			cOff = ++nCir;

		} else {
			printf("Urlbug---> wrong command: %s\n \
			\r--->please try \"-h\" or \"--help\"\n\n", pComm[nCir]);
			exit(FUN_RUN_END);
		}
	}

    const   char   *def_conf_path = "/MuseSp/conf/urlbug.cnf";

	if (mc_conf_load(
       "Urlbug", ((cOff == -1) ?  def_conf_path: pComm[cOff])) == FUN_RUN_FAIL) {
		printf("Urlbug---> load configure failed\n");
		ubug_perror("ubug_command_analyst - mc_conf_load", errno);
		mc_conf_unload();
		exit(FUN_RUN_FAIL);
	}

	if (urlRunSet.ubs_fent == NULL) {
		ubug_init_ubset(
        NULL, ubug_text_abstract_cont, ubug_tran_db, ubug_tran_db_whole,
		ubug_download_website, RUN_PERN);
	}

	if (!tFlags)
		ubug_init_datebuf(NULL);
}


/*------------------------------------------
	    Part Five: Initialization

        1. mainly_init
        2. ubug_init_source_pool
        3. ubug_init_datebuf
        4. ubug_init_dbuf
        5. ubug_init_weblist
        6. ubug_init_urllist
        7. ubug_url_count_nlayer

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


/*-----ubug_init_datebuf-----*/
static void ubug_init_datebuf(char *pTime)
{
	TMS    *nowTime;
	char	strYear[5];

	nowTime = time_str_extract(pTime);

	sprintf(strYear, "%d", nowTime->tm_year);
	sprintf(urlTabName, "U%s%02d%02d", strYear, nowTime->tm_mon, nowTime->tm_mday);
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
	MSLRES	*allRes;
	MSLROW	allRow;
	WEBIN	**pList = &urlSaveList;

	if (mysql_query(&urlDataBase, GET_DIRECTORY) != FUN_RUN_END) {
		if (ubug_dberr(&urlDataBase, "ubug_init_weblist - mysql_query - GET_DIRECTORY") != FUN_RUN_OK)
			ubug_sig_error();
	}

	if (!(allRes = mysql_store_result(&urlDataBase))) {
		printf("Urlbug---> ubug_init_weblist - mysql_store_result - no web\n");

		if (ubug_dberr(&urlDataBase, "ubug_init_weblist - mysql_store_result") != FUN_RUN_OK)
			ubug_sig_error();
	}

	while ((allRow = mysql_fetch_row(allRes))) {
		if ((*pList = malloc(sizeof(WEBIN))) == NULL) {
			elog_write("ubug_init_weblist - malloc",
                        FUNCTION_STR, ERROR_STR);
			ubug_sig_error();
		}

		if(!ubug_init_urllist(allRow[0], &((*pList)->w_ubuf)))
			ubug_sig_error();

		if (allRow[1]) {
			(*pList)->w_latest[strlen(allRow[1])] = 0;
			strcpy((*pList)->w_latest, allRow[1]);
		}

		(*pList)->w_latestcnt = 0;

        if (!ubug_init_dbuf(&((*pList)->w_buff))) {
                elog_write("ubug_init_weblist - ubug_init_dbuf", FUNCTION_STR, ERROR_STR);
                ubug_sig_error();
        }

		if (!sp_net_set_sockif((*pList)->w_ubuf.web_host, &(*pList)->w_sockif)) {
			elog_write("ubug_init_weblist - sp_net_set_sockif", FUNCTION_STR, ERROR_STR);
			free(*pList);
			continue;
		}

		pList = &((*pList)->w_next);
		(*pList) = NULL;
	}

	mysql_free_result(allRes);

	(*pList) = NULL;

	if(mgc_add(urlGarCol, NULL_POINT, ubug_free_weblist) == MGC_FAILED)
		ubug_perror("ubug_init_weblist - mgc_add", errno);
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

        //printf("%s - %s - %s - %d\n", webStu->web_host, webStu->web_path,
        //webStu->web_file, webStu->web_nlayer);

        return	FUN_RUN_OK;
    }

	printf("Urlbug---> ubug_init_urllist - strange url: %s\n", urlStr);

	return	FUN_RUN_END;
}


/*-----ubug_url_count_nlayer-----*/
static int ubug_url_count_nlayer(char *strPath)
{
    char   *pStr;
    int     nLayer = 1;

    if (!strPath)
        return  0;

    if (!strcmp(strPath, "/"))
        return  nLayer;

    for (pStr = strPath + 1; pStr; pStr++) {
        if (*pStr == '/') {
            if (*(pStr + 1) == '\0')
                break;

            nLayer++;
        }
    }

    return  nLayer;
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
        3. ubug_text_store_content

--------------------------------------------*/

/*-----ubug_main_entrance-----*/
static void ubug_main_entrance(void)
{
	WEBIN	*webPoint;

	do {
        ubug_ping();

		for (nCatchNum = 0, webPoint = urlSaveList; webPoint != NULL;
             webPoint = webPoint->w_next) {
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


/*-----ubug_text_store_content-----*/
static void ubug_text_store_content(WEBIN *stPoint)
{

}


/*------------------------------------------
	    Part Eight: Urlbug mainly work

        1. ubug_job
        2. ubug_get_pattern
        3. ubug_check_separator
        4. ubug_connect_head
        5. ubug_check_url_prefix
        6. ubug_reach_url_head

--------------------------------------------*/

/*-----ubug_job-----*/
static void ubug_job(WEBIN *wPoint)
{
    char   *pHttp, *pBegin, *pSign;
    UDATA   uData;
    int     getLen, hostLen = strlen(wPoint->w_ubuf.web_host);

	pHttp = pBegin = wPoint->w_conbuf;

	for (; pBegin && pBegin < wPoint->w_conbuf + wPoint->w_size; pBegin = pSign) {
		if ((getLen = ubug_catch_normal_rule(pBegin, &pSign)) == FUN_RUN_FAIL)
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

        url_layer_extract(&uData, pHttp, getLen);

        urlRunSet.ubs_fst(wPoint, &uData, pHttp, getLen);
        nCatchNum++;
	}
}


/*-----ubug_get_pattern-----*/
static int ubug_get_pattern(const char *pHost)
{
	int     count;

    for (count = 0; ; count++) {
		if (strstr(pHost, perWebPatt[count]))
			return	count;

        if (!strcmp(perWebPatt[count], "ending"))
            break;
	}

	return	FUN_RUN_FAIL;
}


/*-----ubug_check_separator-----*/
static void ubug_check_separator(char *urlStr, int *uLen)
{
	char   *pBeg, *pEnter;
    int     nDec;

	if((pBeg = strnchr(urlStr, '\r', *uLen)) || (pBeg = strnchr(urlStr, '\n', *uLen))) {
        nDec = ((*pBeg == '\r') ? MLINK_LEN : 1);
        *uLen -= nDec;

		/* only one "\r\n" existed */
		if((pEnter = strnchr(pBeg + nDec, *pBeg, *uLen - (pBeg - urlStr))) == NULL) {
			strncpy(pBeg, pBeg + nDec, *uLen - (pBeg - urlStr));

		} else {
			strncpy(pBeg, pEnter + nDec, *uLen - (pEnter - urlStr));
			*uLen -= (pEnter - pBeg);
		}
	}
}


/*-----ubug_connect_head-----*/
static char *ubug_connect_head(WEBIN *wInfo, int hostLen, char *fName, int *fLen)
{
	WEB    *wPt = &wInfo->w_ubuf;
	char   *puBuf, *pEnd;
	int     nLayer, pathLen;

	memset(wInfo->w_url, 0, NAMBUF_LEN);
	strcpy(wInfo->w_url, MATCH_HTTP);
	strncpy(wInfo->w_url + MHTTP_LEN, wPt->web_host, hostLen);
	puBuf = wInfo->w_url + MHTTP_LEN + hostLen;

	/* directory resolving */
	for (nLayer = wPt->web_nlayer; *fName == '.'; nLayer--) {
		if (strncmp(fName, "../", 3)) {
			if (!strncmp(fName, "./", 2)) {
				fName += 2, (*fLen) -= 2;	
				break;
			}
		}

		fName += 3, (*fLen) -= 3;
	}

	/* three conditions, full layer, no layer, less layer */
	if (nLayer == wPt->web_nlayer) {
		pathLen = sprintf(puBuf, "%s", wPt->web_path);

	} else if (nLayer == 0) {
		pathLen = sprintf(puBuf, "/");

	} else {
		/* means how much '/' should pass */
		nLayer += 1;

		for (pEnd = wPt->web_path; nLayer; pEnd++) {
			if (*pEnd == '/')
				nLayer--;
		}

		pathLen = sprintf(puBuf, "%.*s",
                (unsigned int)(pEnd -  wPt->web_path), wPt->web_path);
	}

	strncpy(puBuf + pathLen, fName, *fLen);
	*fLen += (hostLen + MHTTP_LEN + pathLen);

	return	wInfo->w_url;
}


/*-----ubug_check_url_prefix-----*/
static int ubug_check_url_prefix(char *preSrc, int nLen)
{
	int     count;

	preSrc += MHTTP_LEN;

	for (count = 0; ; count++) {
        if (!forbitStrList[count].fb_len)
            break;

		if (strnstr(preSrc, forbitStrList[count].fb_str, nLen))
			return	FUN_RUN_OK;
	}

	return	FUN_RUN_END;
}


/*-----ubug_reach_url_head-----*/
char *ubug_reach_url_head(char *pSrc, char *pLimit)
{
	for(; pSrc < pLimit; pSrc++) {
		if (isalnum(*pSrc) || *pSrc == '/' || *pSrc == '.')
			return	pSrc;
	}

	return	NULL;
}


/*------------------------------------------
        Part Nine: Message pluging

	    1. ubug_time_change
	    2. ubug_send_message

--------------------------------------------*/

/*-----ubug_time_change-----*/
void ubug_time_change(void)
{
	mpc_thread_wait(ubugThreadPool);

	urlRunSet.ubs_fstf();
	ubug_init_datebuf(NULL);
	ubug_create_dbtable();
}


/*-----ubug_send_message-----*/
int ubug_send_message(int msgFd)
{
	PMSG	sendMsg;

	if(nCatchNum) {
		memset(&sendMsg, 0, MSG_LEN);
		sp_msg_fill_stru(&sendMsg, PART_TEXTBUG, KEEP_WORKING);

		if(sp_msg_write(msgFd, &sendMsg) == FUN_RUN_FAIL) {
			elog_write("ubug_send_message - sp_msg_write", FUNCTION_STR, ERROR_STR);
			return	FUN_RUN_FAIL;
		}
	}

	return	FUN_RUN_OK;
}
