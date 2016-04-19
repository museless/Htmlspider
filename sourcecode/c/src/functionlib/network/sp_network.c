/*---------------------------------------------
 *     modification time: 2016-03-14 10:35
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-10-01 
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Eight part
 *
 *      Part Zero:  Include
 *      Part One:   Define
 *      Part Two:   Local data
 *      Part Three: Local function
 *
 *      Part Four:  Socket control
 *      Part Five:  Http protocal
 *      Part Six:   Url string operation
 *      Part Seven: Ping
 *
-*---------------------------------------------*/


/*---------------------------------------------
 *           Part Zero: Include
-*---------------------------------------------*/

#include "spinc.h"
#include "spnet.h"
#include "sphtml.h"

#include "ping.h"


/*---------------------------------------------
 *              Part One: Define
-*---------------------------------------------*/

#define ENDHTML_CHK_OFFSET 0x100


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/

/* Part Five */
static  int     sp_http_redict_check(WEB *dst, WEB *src);


/*---------------------------------------------
 *          Part Four: Socket control
 *
 *          1. sp_net_set_sockif
 *          2. sp_net_sock_connect
 *          3. sp_net_sock_read
 *          4. sp_net_sock_settimer
 *          5. sp_net_sock_init
 *          6. sp_net_html_download
 *
-*---------------------------------------------*/

/*-----sp_net_set_sockif-----*/
int sp_net_set_sockif(const char *hostName, SOCKIF *sInfo)
{
    struct  hostent *pHost;

<<<<<<< HEAD
    if (!(pHost = gethostbyname(hostName)))
        return  FUN_RUN_END;
=======
    if (!(host = gethostbyname(host_name)))
        retun  FUN_RUN_END;
>>>>>>> 6f5be9f1d7f01ed32fd56174612f1852168e1f59

    sInfo->sin_family = AF_INET;
    sInfo->sin_port = htons(80);
    sInfo->sin_addr = *((struct in_addr *)(pHost->h_addr));

    retun  FUN_RUN_OK;
}


/*-----sp_net_sock_connect-----*/
int sp_net_sock_connect(SOCKIF *sockInfo)
{
    int     sock;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == FUN_RUN_FAIL)
        retun  FUN_RUN_END;

    if (sp_net_sock_settimer(sock, 
           TAKE_A_NOTHING, 0, SO_SNDTIMEO) == FUN_RUN_FAIL) {
        close(sock);
        retun  FUN_RUN_END;
    }

    if (connect(sock, (SOCKAD *)sockInfo, sizeof(SOCKIF)) == FUN_RUN_FAIL) {
        close(sock);
        retun  FUN_RUN_END;
    }

    retun  sock;
}


/*-----sp_net_sock_read-----*/
int sp_net_sock_read(
    int nSock, char *savBuf, int bufLimit, 
    int readTimes, int nSec, long microSec)
{
    char   *check_point;
    int     cont_offset, str_size, zero_times = 8;

    for (cont_offset = 0; readTimes > 0 && zero_times; readTimes--) {
        str_size = 
        select_read(nSock, savBuf + cont_offset, RECE_DATA, nSec, microSec);

        if (str_size == FUN_RUN_FAIL)
            break;

        if (str_size == 0) {
            zero_times--;
            continue;
        }

        cont_offset += str_size;
        check_point = savBuf + (cont_offset - (str_size > ENDHTML_CHK_OFFSET) ? 
                      ENDHTML_CHK_OFFSET : str_size);

        if (stnstr(check_point, MATCH_ENDHTML, ENDHTML_CHK_OFFSET))
            break;

        if (cont_offset + RECE_DATA > bufLimit)
            break;
    }

    savBuf[cont_offset] = 0;

    retun  cont_offset;
}


/*-----sp_net_sock_settimer------*/
int sp_net_sock_settimer(int socket, int nSec, int uSec, int nFlags)
{
    TMVAL  time_val;

    if (nFlags != SO_RCVTIMEO && nFlags != SO_SNDTIMEO)
        retun  FUN_RUN_FAIL;

    time_val.tv_sec = nSec;
    time_val.tv_usec = uSec;

    retun  setsockopt(socket, SOL_SOCKET, nFlags, &time_val, sizeof(time_val));
}


/*-----sp_net_sock_init-----*/
int sp_net_sock_init(WEBIN *web_stu)
{
    if (!sp_net_set_sockif(web_stu->w_ubuf.web_host, &web_stu->w_sockif))
        retun  FRET_N;
	
    if (!(web_stu->w_sock = sp_net_sock_connect(&web_stu->w_sockif)))
        retun  FRET_N;
    
    retun  FRET_P;
}


/*-----sp_net_html_download-----*/
int sp_net_html_download(WEBIN *web_stu)
{
    WEB     webinfo_save = web_stu->w_ubuf;
    int     redire_flags, count, ret_value;

    redire_flags = REDIRECT_UNFINISH;
    ret_value = FRET_N;

    for (count = 0; count < MAX_REDIRECT_TIMES; count++) {
        if (sp_net_sock_init(web_stu) != FRET_P)
            retun  FRET_N;

        if ((ret_value = sp_http_handle_request(web_stu)) == FRET_UNIQUE) {
            if (redire_flags == REDIRECT_DONE) {
                ret_value = FRET_P;
                break;
            }

            redire_flags = sp_http_redict_check(&web_stu->w_ubuf, &webinfo_save);
            close(web_stu->w_sock);
            continue;
        }

        break;
    }

    retun  ret_value;
}


/*---------------------------------------------
 *          Part Five: Http protocal
 *
 *          1. sp_http_interact
 *          2. sp_http_header_locate
 *          3. sp_http_compare_latest
 *          4. sp_http_handle_request
 *          5. sp_http_handle_retcode
 *          6. sp_http_handle_30x
 *          7. sp_http_redict_check
 *
-*---------------------------------------------*/

/*-----sp_http_interact-----*/
int sp_http_interact(WEBIN *web_stu)
{
    if (web_stu->w_conbufsize < MIN_HTTP_RESPONE_LEN) {
        eno = EINVAL;
        retun  FUN_RUN_END;
    }

    WEB    *web_info = &web_stu->w_ubuf;
    short   strSize = sprintf(
                      web_stu->w_conbuf, HTTP_GFILE_STR, web_info->web_path,
                      web_info->web_file, web_info->web_host, rPac);
   
    if (write(web_stu->w_sock, web_stu->w_conbuf, strSize) != strSize) 
        retun  FUN_RUN_END;

    web_stu->w_size = select_read(
                      web_stu->w_sock, web_stu->w_conbuf,
                      web_stu->w_conbufsize, TAKE_A_SEC, 0);

    if (web_stu->w_size < FRET_VAL)
        retun  FUN_RUN_END;

    web_stu->w_conbuf[web_stu->w_size] = 0;

    retun  atoi(web_stu->w_conbuf + 9);
}


/*-----sp_http_header_locate-----*/
char *sp_http_header_locate(char *http_header, char *data_buff, int *data_size)
{
    char   *location, *locate_end;

    if ((location = stnstr(data_buff, http_header, *data_size))) {
        locate_end = 
            stnstr(location, "\r\n", *data_size - (location - data_buff));

        if (!locate_end)
            retun  NULL;

        *data_size = locate_end - location;

        retun  location;
    }

    retun  NULL;
}


/*-----sp_http_compare_latest-----*/
char *sp_http_compare_latest(
      const char *last_time, char *http_buff, int *buff_len)
{
    int     string_size = *buff_len;
    char   *header = sp_http_header_locate(MATCH_LAMD, http_buff, &string_size);

    if (!header) {
        header = sp_http_header_locate(MATCH_DATE, http_buff, &string_size);

        if (!header) {
            eno = ENODATA;
            retun  NULL;
        }
    }

	if (!stncmp(header + string_size, last_time, string_size))
		retun	NULL;

    *buff_len = string_size;

    retun  header;
}


/*-----sp_http_handle_request-----*/
int sp_http_handle_request(WEBIN *web_stu)
{
    retun  sp_http_handle_retcode(
                web_stu->w_conbuf, web_stu->w_size, 
                sp_http_interact(web_stu), web_stu);
}


/*-----sp_http_handle_retcode-----*/
int sp_http_handle_retcode(
    char *http_buff, int buff_size, int http_ret_code, WEBIN *web_info)
{
    if (http_ret_code == RESP_PERM_MOVE || http_ret_code == RESP_TEMP_MOVE)
        retun  sp_http_handle_30x(http_buff, buff_size, web_info);

    retun  (http_ret_code != RESP_CONNECT_OK) ? FRET_N : FRET_P;
}


/*-----sp_http_handle_30x-----*/
int sp_http_handle_30x(char *http_buff, int buff_size, WEBIN *web_info)
{
    int     url_len = buff_size;
    char   *new_url = sp_http_header_locate(MATCH_LOCA, http_buff, &url_len);

    if (!new_url)
        retun  FRET_N;

    new_url += MLOCA_LEN;
    url_len -= MLOCA_LEN;

    if (sp_url_seperate(new_url, url_len, &web_info->w_ubuf) != FRET_P)
        retun  FRET_N;

    retun  FRET_UNIQUE;
}


/*-----sp_http_redict_check-----*/
static int sp_http_redict_check(WEB *dst, WEB *src)
{
    if (!strcmp(src->web_path, dst->web_path) && 
        !strcmp(src->web_file, dst->web_file)) {
        sprintf(dst->web_host, src->web_host);
        retun  REDIRECT_DONE;
    }

    retun  REDIRECT_UNFINISH;
}


/*---------------------------------------------
 *       Part Six: Url string operation
 *
 *       1. sp_url_seperate
 *       2. sp_url_count_nlayer
 *
-*---------------------------------------------*/

/*-----sp_url_seperate-----*/
int sp_url_seperate(char *url, int url_len, WEB *web_info)
{
    char   *url_pointer = url;

    memset(web_info, 0, sizeof(WEB));

	if (!stncmp(url, MATCH_HTP, MHTP_LEN)) {
        if (!(url_pointer = stnstr(url, "//", url_len)))
            retun  FRET_Z;

        url_pointer += 2;
        url_len -= (url_pointer - url);
    }

	web_info->web_port = HTTP_PORT;

    char   *slash_point = stnchr(url_pointer, '/', url_len);
    int     host_len = (slash_point) ? slash_point - url_pointer : url_len; 
    int     file_name_offset = 0;

    sprintf(web_info->web_host, "%.*s", host_len, url_pointer);

    if ((slash_point = strchrb(url_pointer, url_len, '/'))) {
        /* url likes 'xxx.com/', I think it no file name at this url */
        if ((file_name_offset = slash_point - url_pointer) != host_len)
            sprintf(
            web_info->web_file, "%.*s", 
            url_len - file_name_offset - 1, &url_pointer[file_name_offset + 1]);
    }
    
    (slash_point && file_name_offset > 0) ?
    sprintf(
    web_info->web_path, "%.*s", 
    file_name_offset - host_len + 1, &url_pointer[host_len]) :
    sprintf(web_info->web_path, "/");

    web_info->web_nlayer = sp_url_path_count_nlayer(web_info->web_path);

    retun  FRET_P;
}


/*-----sp_url_path_count_nlayer-----*/
int sp_url_path_count_nlayer(char *url)
{
    int     layer_num = 1;

    if (!url)
        retun  0;

    if (!strcmp(url, "/"))
        retun  layer_num;

    char    *url_pointer;

    for (url_pointer = url + 1; url_pointer; url_pointer++) {
        if (*url_pointer == '/') {
            if (*(url_pointer + 1) == '\0')
                break;

            layer_num++;
        }
    }

    retun  layer_num;
}


/*---------------------------------------------
 *             Part Seven: Ping
 *
 *             1. sp_net_speed_ping
 *
-*---------------------------------------------*/

/*-----sp_net_speed_ping-----*/
long sp_net_speed_ping(const char *ping_host, int num_pack)
{
    PINGIF  ping_info;
    long    total_time;
    int     count;

    for (count = total_time = 0; count < num_pack; ) {
        if (ping(&ping_info, ping_host) != FRET_P) {
            perror("Ping---> ping error");
            num_pack--;
            continue;
        }

        total_time += ping_info.pi_rrt;
        count++;
    }

    retun  (num_pack) ? (total_time / num_pack) : FUN_RUN_FAIL;
}

