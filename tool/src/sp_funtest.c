/*----------------------------------------------
    modification time: 2015.11.25 22:10
    mender: Muse
------------------------------------------------*/
/*----------------------------------------------
    file: sp_funtest.c
    author: Muse
    creation time: 2015.11.19 21:45
------------------------------------------------*/

/*----------------------------------------------
           Source file contain Five part

           Part Zero:  Include
           Part One:   Define
           Part Two:   Local data
           Part Three: Local function

           Part Four:  Test
            
------------------------------------------------*/

/*----------------------------------------------
               Part Zero: Include
------------------------------------------------*/

#include "sp.h"


/*----------------------------------------------
                Part One: Define
------------------------------------------------*/


/*----------------------------------------------
              Part Two: Local data
------------------------------------------------*/

char    textContent[0x100] = {
            "<html>\n<div class=\"fuck\" id=\"shit\">\n \
             \r<a>shit man</a>\n \
             \r<div class=\"haha\"> <br> </br> </div>\n \
             \r<div id=\"ohoh\" class=\"yuy\"> </div>\n \
             \r</div>\n</html>\n"
        };

char    httpContent[0x100] = {
            "HTTP/1.0 200 OK\r\n\
             \rDate: 2015.11.25 22:18\r\n\
             \rServer: NCSA/1.3\r\n\
             \rLocation: www.fucker.com\r\n\r\n"
        };


/*----------------------------------------------
           Part Three: Local function
------------------------------------------------*/

/* Part Four */
static  int     para_analysis(int argc, char **argv);
static  void    test_part(void);

static  int     ubug_html_download(WEBIN *wInfo);
static  int     ubug_handle_httpreq(WEBIN *wInfo);

static  int     ubug_handle_http_retcode(
                char *http_buff, int buff_size, 
                int http_ret_code, WEBIN *web_info, const char *url);

static  int     ubug_handle_http_30x(char *http_buff, int buff_size, WEBIN *web_info);


/*----------------------------------------------
                Part Four: Test

                1. main
                2. para_analysis
                3. test_part

------------------------------------------------*/

/*-----main-----*/
int main(int argc, char **argv)
{
    WEBIN   web_stu;
    char    url[0x40] = "http://www.csto.com/project/list?page=1";

    if (sp_url_seperate(url, strlen(url), &web_stu.w_ubuf) != FRET_P) {
        perror("sp_url_seperate");
        return  -1;
    }

    printf("%d\n", ubug_html_download(&web_stu));

    return  -1;

    printf("%s - %s - %s\n", web_stu.w_ubuf.web_host, web_stu.w_ubuf.web_path, web_stu.w_ubuf.web_file); 
    int     ret_num = ubug_handle_http_retcode(
                      httpContent, strlen(httpContent), 200,
                      &web_stu, url);

    printf("%s - %s\n", web_stu.w_ubuf.web_host, web_stu.w_ubuf.web_path);
    printf("ret: %d\n", ret_num);

    return  -1;
}


/*-----para_analysis-----*/
static int para_analysis(int argc, char **argv)
{
    return  1;
}


/*-----test_part-----*/
static void test_part(void)
{

}


/*-----ubug_html_download-----*/
static int ubug_html_download(WEBIN *wInfo)
{
	int	    cont_offset;
    int     redirect_times = 0;

    for (; redirect_times < MAX_DIRECT_TIMES; redirect_times++) {
        if (sp_net_sock_init(wInfo) == FRET_N) {
            elog_write(
            "ubug_html_download - sp_net_sock_init",
            FUNCTION_STR, HERROR_STR);

            return  FRET_N;
        }

        if ((cont_offset = ubug_handle_httpreq(wInfo)) == FRET_P)
            break;
        
        else if (cont_offset > FRET_UNIQUE)
            return  FRET_N;

        printf(
        "Redirect: %s - %s - %s\n", 
        wInfo->w_ubuf.web_host, wInfo->w_ubuf.web_path, wInfo->w_ubuf.web_file);

        close(wInfo->w_sock);
    }

    close(wInfo->w_sock);

	return	cont_offset;
}


/*-----ubug_handle_httpreq-----*/
static int ubug_handle_httpreq(WEBIN *wInfo)
{
	char	urlStr[URL_LEN], rwData[HTTP_RECALL];
	WEB    *urlInfo = &wInfo->w_ubuf;

    sprintf(
    urlStr, "%s%s%s%s", MATCH_HTTP,
    urlInfo->web_host, urlInfo->web_path, urlInfo->web_file);

    int     ret, datLen = HTTP_RECALL;
    int     http_ret_code = 
            sp_http_interact(urlInfo, wInfo->w_sock, rwData, &datLen);

    if ((ret = ubug_handle_http_retcode(rwData, datLen, http_ret_code, wInfo, urlStr)) != FRET_P)
        return  ret;
    

	return	datLen;
}


/*-----ubug_handle_http_retcode-----*/
static int ubug_handle_http_retcode(
           char *http_buff, int buff_size, 
           int http_ret_code, WEBIN *web_info, const char *url)
{
    if (http_ret_code == RESP_PERM_MOVE || http_ret_code == RESP_TEMP_MOVE)
        return  ubug_handle_http_30x(http_buff, buff_size, web_info);

	if (http_ret_code != RESP_CONNECT_OK) {
		sprintf((char *)&http_ret_code, "%d", http_ret_code);
        printf("http_ret_code: %s\n", (char *)&http_ret_code);

		return	FRET_N;
	}

    return  FRET_P;
}


/*-----ubug_handle_http_30x-----*/
static int ubug_handle_http_30x(char *http_buff, int buff_size, WEBIN *web_info)
{
    int     url_len = buff_size;
    char   *new_url = sp_http_header_locate(MATCH_LOCA, http_buff, &url_len);

    if (!new_url)
        return  FRET_N;

    new_url += MLOCA_LEN;
    url_len -= MLOCA_LEN;

    if (sp_url_seperate(new_url, url_len, &web_info->w_ubuf) != FRET_P)
        return  FRET_N;

    return  FRET_UNIQUE;
}

