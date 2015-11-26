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

	       1. ubug_html_download
           2. ubug_handle_httpreq
	       3. ubug_send_httpreq
           4. ubug_handle_http_retcode
           5. ubug_update_latest_time

--------------------------------------------*/

/*-----ubug_html_download-----*/
int ubug_html_download(WEBIN *wInfo)
{
	int	    cont_offset, byte_read;
    int     redirect_times = 0;

    for (; redirect_times < MAX_DIRECT_TIMES; redirect_times++) {
        if (sp_net_sock_init(wInfo) == FRET_N) {
            elog_write(
            "ubug_html_download - sp_net_sock_init",
            FUNCTION_STR, HERROR_STR);

            return  FRET_N;
        }
    }

    byte_read = sp_net_sock_read(
                wInfo->w_sock, wInfo->w_conbuf + cont_offset,
                WMP_PAGESIZE - cont_offset, UBUG_NREAD, 
                ubugPingInfo.p_time.tv_sec, ubugPingInfo.p_time.tv_usec);

    cont_offset += (byte_read == FUN_RUN_FAIL) ? 0 : byte_read;
    close(wInfo->w_sock);

	return	cont_offset;
}


/*-----ubug_handle_httpreq-----*/
int ubug_handle_httpreq(WEBIN *wInfo)
{
	char	urlStr[URL_LEN], rwData[HTTP_RECALL];
	WEB    *urlInfo = &wInfo->w_ubuf;

    sprintf(
    urlStr, "%s%s%s%s", MATCH_HTTP,
    urlInfo->web_host, urlInfo->web_path, urlInfo->web_file);

    int     datLen = HTTP_RECALL;
    int     http_ret_code = 
            sp_http_interact(urlInfo, wInfo->w_sock, rwData, &datLen);

    
    char   *string_point;
    int     string_size;

    if (!(string_point = strstr(rwData, "\r\n\r\n"))) {
        elog_write("ubug_send_httpreq - httpret", urlStr, "httpret not complete");
        return	FUN_RUN_FAIL;
    }

    strcpy(wInfo->w_conbuf, string_point);
    datLen -= (string_point - rwData);

    string_size = datLen;
    string_point = sp_http_compare_latest(
                   wInfo->w_latest, string_point, &string_size);

    if (!string_point) {
        sprintf(wInfo->w_latest, "%.*s", string_size, string_point);
        ubug_update_latest_time(wInfo, urlStr);
    }

	return	datLen;
}


/*-----ubug_handle_http_retcode-----*/
static int ubug_handle_http_retcode(
           char *http_buff, int buff_size, 
           int http_ret_code, WEBIN *web_info, const char *url)
{
    if (http_ret_code == FUN_RUN_END) {
        elog_write(
        "ubug_send_httpreq - sp_http_interact", url, HERROR_STR);

        return	FRET_N;
	}

    if (http_ret_code == RESP_PERM_MOVE || http_ret_code == RESP_TEMP_MOVE)
        return  ubug_handle_http_30x(http_buff, buff_size, web_info);

	if (http_ret_code != RESP_CONNECT_OK) {
		sprintf((char *)&http_ret_code, "%d", http_ret_code);
		elog_write("cannot download website", url, (char *)&http_ret_code);

		return	FRET_N;
	}

    return  FRET_P;
}


/*-----ubug_handle_http_30x-----*/
int ubug_handle_http_30x(char *http_buff, int buff_size, WEBIN *web_info)
{
    int     url_len = buff_size;
    char   *new_url = sp_http_header_locate(MATCH_LOCA, http_buff, &url_len);

    if (!new_url || sp_url_seperate(new_url, url_len, &web_info->w_ubuf) != FRET_P)
        return  FRET_N;

    return  FRET_UNIQUE;
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


