/*---------------------------------------------
 *     modification time: 2016-04-24 08:05:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: sp_urlbug_network.c 
 *     creation time: 2015-06-01
 *     author: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *        Source file content Six part
 *
 *          Part Zero:  Include
 *          Part One:   Local data
 *          Part Two:   Local function
 *          Part Three: Define
 *
 *          Part Four:  Ping network
 *          Part Five:  Network event 
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spuglobal.h"
#include "spurlb.h"


/*---------------------------------------------
 *          Part One: Local data
-*---------------------------------------------*/

static int  PerReadTimes; 


/*---------------------------------------------
 *         Part Two: Local function
-*---------------------------------------------*/

/* Part Five */
static  int     ubug_handle_httpreq(WEBIN *web_info);
static  void    ubug_update_latest_time(WEBIN *web_stu);


/*---------------------------------------------
 *          Part Four: Ping network
 *
 *          1. ubug_ping
 *
-*---------------------------------------------*/

/*-----ubug_ping-----*/
void ubug_ping(void)
{
    long    recv;

    recv = sp_net_speed_ping(ubugPingInfo.p_host, ubugPingInfo.p_packnum);

    if (recv < (ubugPingInfo.p_packnum >> 1)) {
        printf("Urlbug---> ping failed - use default\n");

        ubugPingInfo.p_time.tv_sec = 0;
        ubugPingInfo.p_time.tv_usec = 80000;
        return;
    }

    ubugPingInfo.p_time.tv_sec = recv / MICSEC_PER_SEC;
    ubugPingInfo.p_time.tv_usec = (recv % MICSEC_PER_SEC) * 4.0;

    printf("Urlbug---> network speed: %lums\n", recv);
}


/*---------------------------------------------
 *          Part Five: Network event
 *
 *          1. ubug_init_network
 *          2. ubug_html_download
 *          3. ubug_handle_httpreq
 *          4. ubug_update_latest_time
 *
-*---------------------------------------------*/

/*-----ubug_init_network-----*/
void ubug_init_network(void)
{
    PerConfData read[] = {
        {"ping_host", CONF_STR, ubugPingInfo.p_host, SMALL_BUF, "www.baidu.com"},
        {"ping_packet", CONF_NUM, &ubugPingInfo.p_packnum, sizeof(int), "80"},
        {"per_read_times", CONF_NUM, &PerReadTimes, sizeof(int), "128"},
    };

    mc_conf_read_list(read, sizeof(read) / sizeof(read[0]));

    ubugPingInfo.p_time.tv_sec = 0;
    ubugPingInfo.p_time.tv_usec = 100000;
}


/*-----ubug_html_download-----*/
int ubug_html_download(WEBIN *web_stu)
{
    int     cont_offset, byte_read;

    if (sp_net_html_download(web_stu) != FRET_P) {
        setmsg(LM2, HERROR_STR, web_stu->w_ubuf.web_host);
        close(web_stu->w_sock);
        return  FRET_Z;
    }

    if ((cont_offset = ubug_handle_httpreq(web_stu)) == FRET_N) {
        close(web_stu->w_sock);
        return  FRET_N;
    }

    byte_read = sp_net_sock_read(web_stu->w_sock, 
        web_stu->w_conbuf + cont_offset, WMP_PAGESIZE - cont_offset, 
        PerReadTimes, ubugPingInfo.p_time.tv_sec, ubugPingInfo.p_time.tv_usec);

    cont_offset += (byte_read == FRET_N) ? 0 : byte_read;

    close(web_stu->w_sock);

    return  cont_offset;
}


/*-----ubug_handle_httpreq-----*/
int ubug_handle_httpreq(WEBIN *web_stu)
{
    char   *string_point;

    web_stu->w_contoffset = 
        (string_point = strstr(web_stu->w_conbuf, "\r\n\r\n")) ?
         string_point - web_stu->w_conbuf : strlen(web_stu->w_conbuf);

    int     string_size = web_stu->w_contoffset;

    string_point = sp_http_compare_latest(web_stu->w_latest, 
            web_stu->w_conbuf, &string_size);

    if (string_point) {
        sprintf(web_stu->w_latest, "%.*s", string_size, string_point);
        ubug_update_latest_time(web_stu);
        return  web_stu->w_contoffset;
    }

    return  FRET_N;
}


/*-----ubug_update_latest_time-----*/
void ubug_update_latest_time(WEBIN *web_stu)
{
    char    sqlCom[SQL_SCOM_LEN]; 

    sprintf(sqlCom, UPDATE_LATEST, web_stu->w_latest, 
        web_stu->w_ubuf.web_host, web_stu->w_ubuf.web_path,
        web_stu->w_ubuf.web_file);

    while (!mato_dec_and_test(writeStoreLock))
        mato_inc(writeStoreLock);

    if (mysql_query(&urlDataBase, sqlCom) != FUN_RUN_END)
        ubug_db_seterror();

    mato_inc(writeStoreLock);
}


