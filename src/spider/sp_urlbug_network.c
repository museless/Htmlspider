/*------------------------------------------
	Source file content Six part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:  Ping network
    Part Five:  Network event 

--------------------------------------------*/

/*------------------------------------------
            Part Zero: Include
--------------------------------------------*/

#include "spinc.h"
#include "sphtml.h"
#include "spdb.h"
#include "spnet.h"
#include "spmpool.h"
#include "spmsg.h"
#include "spuglobal.h"
#include "spurlb.h"


/*------------------------------------------
	       Part One: Local data
--------------------------------------------*/


/*------------------------------------------
	     Part Two: Local function
--------------------------------------------*/

/* Part Four */
static  void    ubug_ping_default_init(SPPING *ping_info);

/* Part Five */
static  int     ubug_handle_httpreq(WEBIN *web_info);
static  void    ubug_update_latest_time(WEBIN *wInfo, char *url_string);


/*------------------------------------------
	        Part Three: Define
--------------------------------------------*/


/*------------------------------------------
	    Part Four: Ping network

        1. ubug_init_pinginfo
        2. ubug_ping
        3. ubug_ping_default_init

--------------------------------------------*/

/*-----ubug_init_pinginfo-----*/
void ubug_init_pinginfo(void)
{
	if (mc_conf_read(
        "ping_host", CONF_STR, 
        ubugPingInfo.p_host, SMALL_BUF) == FUN_RUN_FAIL)
		mc_conf_print_err("ping_host");

    if (mc_conf_read(
        "ping_packet", CONF_NUM,
        &ubugPingInfo.p_packnum, sizeof(int)) == FUN_RUN_FAIL)
        mc_conf_print_err("ping_packet");
}


/*-----ubug_ping-----*/
void ubug_ping(void)
{
    long    time_recv;

    ubug_ping_default_init(&ubugPingInfo);
    time_recv = sp_net_speed_ping(ubugPingInfo.p_host, ubugPingInfo.p_packnum);

    if (time_recv < FUN_RUN_OK) {
        printf("Urlbug---> zero pack to ping\n");

        ubugPingInfo.p_time.tv_sec = 0;
        ubugPingInfo.p_time.tv_usec = 80000;
        return;
    }

    ubugPingInfo.p_time.tv_sec = time_recv / MICSEC_PER_SEC;
    ubugPingInfo.p_time.tv_usec = (time_recv % MICSEC_PER_SEC) * 2.5;

    printf(
    "Urlbug---> current network speed: %lu ms\n",
    ubugPingInfo.p_time.tv_usec);
}


/*-----ubug_ping_default_init-----*/
static void ubug_ping_default_init(SPPING *ping_info)
{
    if (ping_info->p_host[0] == 0 || ping_info->p_packnum == 0) {
        sprintf(ping_info->p_host, DEFAULT_PING_HOST);
        ping_info->p_packnum = DEFAULT_PING_PNUM;
    }
}


/*------------------------------------------
           Part Five: Network event

	       1. ubug_download_website
           2. ubug_handle_httpreq
	       3. ubug_send_httpreq
           4. ubug_catch_http_retcode
           4. ubug_update_latest_time

--------------------------------------------*/

/*-----ubug_download_website-----*/
int ubug_download_website(WEBIN *wInfo)
{
	int	cont_offset, byte_read;

	if (!(wInfo->w_sock = sp_net_sock_connect(&wInfo->w_sockif))) {
		printf("Urlbug---> sp_net_sock_connect: %s%s\n%s\n",
		wInfo->w_ubuf.web_host, wInfo->w_ubuf.web_file, strerror(errno));
		return	FUN_RUN_END;
	}

    if ((cont_offset = ubug_handle_httpreq(wInfo)) == FUN_RUN_FAIL) {
		close(sock);
		return	FUN_RUN_FAIL;
	}

    byte_read = sp_net_sock_read(
                sock, wInfo->w_conbuf + cont_offset,
                WMP_PAGESIZE - cont_offset, UBUG_NREAD, 
                ubugPingInfo.p_time.tv_sec, ubugPingInfo.p_time.tv_usec);

    cont_offset += (byte_read == FUN_RUN_FAIL) ? 0 : byte_read;
    close(sock);

	return	wInfo->w_sock;
}


/*-----ubug_handle_httpreq-----*/
static int ubug_handle_httpreq(WEBIN *web_info)
{
    return  FUN_RUN_OK;
}


/*-----ubug_send_httpreq-----*/
int ubug_send_httpreq(WEBIN *wInfo)
{
	char	urlStr[URL_LEN], rwData[HTTP_RECALL];
	WEB    *urlInfo = &wInfo->w_ubuf;
	char   *pChar, *pEnd;

    sprintf(
    urlStr, "%s%s%s%s", MATCH_HTTP,
    urlInfo->web_host, urlInfo->web_path, urlInfo->web_file);

    int     httpChk, datLen = HTTP_RECALL;

    if (!(httpChk = sp_http_interact(urlInfo, wInfo->w_sock, rwData, &datLen))) {
        elog_write(
        "ubug_send_httpreq - sp_http_interact", 
	    urlStr, HERROR_STR);

        return	FUN_RUN_FAIL;
	}

	if(httpChk != RESP_CONNECT_OK) {
        char    http_code_string[SMALL_BUF];

		sprintf(http_code_string, "%d", httpChk);
		elog_write("cannot download website", urlStr, http_code_string);
		return	FUN_RUN_FAIL;
	}

    if (!(pChar = strstr(rwData, "\r\n\r\n"))) {
        elog_write("ubug_send_httpreq - httpret", urlStr, "httpret wrong");
        return	FUN_RUN_FAIL;
    }

    strcpy(wInfo->w_conbuf, pChar);
    datLen -= (pChar - rwData);

	/* move offset "Last-Modified:" or "Date:" */
	if((pChar = strstr(rwData, MATCH_LAMD))) {
		pChar += MLAMD_LEN;

	} else if((pChar = strstr(rwData, MATCH_DATE))) {
		pChar += MDATE_LEN;
	}

    if (pChar == NULL)
        return  FUN_RUN_END;

	/* if there're no "\r\n" at the end, returing */
	if((pEnd = strstr(pChar, MATCH_LINKBRK)) == NULL)
		return	FUN_RUN_FAIL;

	if(!strncmp(pChar, wInfo->w_latest, pEnd - pChar))
		return	FUN_RUN_FAIL;

    sprintf(wInfo->w_latest, "%.*s", (int)(pEnd - pChar), pChar);
    ubug_update_latest_time(wInfo, urlStr);

	return	datLen;
}


/*-----ubug_handle_http_retcode-----*/
static int ubug_handle_http_recode(
           int sock, WEBIN *web_info, char *buff, int *buff_size)
{
    int     http_retcode, data_size = *buff_size;

    http_retcode = sp_http_interact(
                   &web_info->w_ubuf, web_info->w_sock, buff, &data_size);

    if (http_retcode != RESP_PERM_MOVE || http_retcode != RESP_TEMP_MOVE)
        http_retcode = sp_http_handle_30x(web_info, buff, *buff_size);

    return  http_retcode;
}


/*-----ubug_update_latest_time-----*/
static void ubug_update_latest_time(WEBIN *wInfo, char *url_string)
{
    char	sqlCom[SQL_SCOM_LEN]; 

    if (++wInfo->w_latestcnt == LATEST_UPGRADE_LIMIT) {
        sprintf(sqlCom, UPDATE_LATEST, wInfo->w_latest, url_string);
        wInfo->w_latestcnt = 0;

        while (!mato_dec_and_test(&writeStoreLock))
                mato_inc(&writeStoreLock);

        if (mysql_query(&urlDataBase, sqlCom) != FUN_RUN_END) {
                if (ubug_dberr(&urlDataBase, "send_httpreq - up latest") != FUN_RUN_OK)
                        ubug_sig_error();
        }

        mato_inc(&writeStoreLock);
	}
}


