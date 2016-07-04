/*---------------------------------------------
 *     modification time: 2016-04-22 22:55:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-11-23 23:34:41
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Seven part
 *
 *      Part Zero:  Include
 *      Part One:   Local data
 *      Part Two:   Local function
 *      Part Three: Define
 *
 *      Part Four:  URL catch rule
 *      Part Five:  URL check rules
 *      Part Six:   URL table name
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spuglobal.h"
#include "spurlb.h"


/*---------------------------------------------
*            Part One: Local data
-*---------------------------------------------*/

static  char    timComMode[DATE_CMODE][SMALL_BUF];


/*---------------------------------------------
*          Part Two: Local function
-*---------------------------------------------*/

/* Part Six */
static  void    set_date_mode(TMS *date);


/*---------------------------------------------
 *        Part Four: URL catch rule
 *
 *        1. ubug_catch_default_rule
 *        2. ubug_locate_default_rule
 *
-*---------------------------------------------*/

/*-----ubug_catch_default_rule------*/
int ubug_catch_default_rule(char *content, char **ret_content_point)
{
    char   *content_begin, *content_end;
    int     content_len;     

    if (!(content_begin = strstr(content, MATCH_HREF)))
        return  FRET_N;

    content_begin += MHREF_LEN;

    if (!(content_end = strchr(content_begin, '"')))
        return  FRET_N;

    *ret_content_point = content_end;

    if (strnstr(content_begin, "htm", content_end - content_begin)) { 
        content_begin = ubug_reach_url_head(content_begin, content_end);

        if (!content_begin)
            return  FRET_Z;

        if ((content_len = content_end - content_begin) >= urlMaxLen ||
           ubug_is_today_news(content_begin, content_len) == FRET_N)
            return  FRET_Z;

        return  content_len;
    }

    return  FRET_Z;
}


/*-----ubug_locate_default_rule-----*/
int ubug_locate_default_rule(
    WEBIN *web_point, char **content_beg, char **content_end)
{
    (*content_beg) = web_point->w_conbuf;
    (*content_end) = (*content_beg) + web_point->w_size;

    return  FUN_RUN_OK;
}


/*---------------------------------------------
 *        Part Five: URL check rules
 *
 *        1. ubug_check_separator
 *        2. ubug_connect_head
 *        3. ubug_check_url_prefix
 *        4. ubug_reach_url_head
 *        5. ubug_is_today_news
 *
-*---------------------------------------------*/

/*-----ubug_check_separator-----*/
void ubug_check_separator(char *urlStr, int *uLen)
{
    char   *pBeg, *pEnter;
    int     nDec;

    if ((pBeg = strnchr(urlStr, '\r', *uLen)) ||
        (pBeg = strnchr(urlStr, '\n', *uLen))) {
        nDec = ((*pBeg == '\r') ? MLINK_LEN : 1);
        *uLen -= nDec;

        /* only one "\r\n" existed */
        if (!(pEnter = strnchr(pBeg + nDec, *pBeg, *uLen - (pBeg - urlStr)))) {
            strncpy(pBeg, pBeg + nDec, *uLen - (pBeg - urlStr));

        } else {
            strncpy(pBeg, pEnter + nDec, *uLen - (pEnter - urlStr));
            *uLen -= (pEnter - pBeg);
        }
    }
}


/*-----ubug_connect_head-----*/
char *ubug_connect_head(WEBIN *wInfo, int hostLen, char *fName, int *fLen)
{
    WEB    *web_stu_point = &wInfo->w_ubuf;
    char   *p_buff, *pEnd;
    int     nLayer, pathLen;

    memset(wInfo->w_url, 0, NAMBUF_LEN);
    strcpy(wInfo->w_url, MATCH_HTTP);
    strncpy(wInfo->w_url + MHTTP_LEN, web_stu_point->web_host, hostLen);
    p_buff = wInfo->w_url + MHTTP_LEN + hostLen;

    /* directory resolving */
    for (nLayer = web_stu_point->web_nlayer; *fName == '.'; nLayer--) {
        if (strncmp(fName, "../", 3)) {
            if (!strncmp(fName, "./", 2)) {
                fName += 2, (*fLen) -= 2;   
                break;
            }
        }

        fName += 3, (*fLen) -= 3;
    }

    /* three conditions, full layer, no layer, less layer */
    if (nLayer == web_stu_point->web_nlayer) {
        pathLen = sprintf(p_buff, "%s", web_stu_point->web_path);

    } else if (nLayer == 0) {
        pathLen = sprintf(p_buff, "/");

    } else {
        /* means how much '/' should pass */
        nLayer += 1;

        for (pEnd = web_stu_point->web_path; nLayer; pEnd++) {
            if (*pEnd == '/')
                nLayer--;
        }

        pathLen = 
        sprintf(p_buff, "%.*s",
        (unsigned int)(pEnd -  web_stu_point->web_path),
        web_stu_point->web_path);
    }

    strncpy(p_buff + pathLen, fName, *fLen);
    *fLen += (hostLen + MHTTP_LEN + pathLen);

    return  wInfo->w_url;
}


/*-----ubug_check_url_prefix-----*/
int ubug_check_url_prefix(char *preSrc, int nLen)
{
    preSrc += MHTTP_LEN;

    for (int count = 0; forbitStrList[count].fb_len != 0; count++) {
        if (strnstr(preSrc, forbitStrList[count].fb_str, nLen))
            return  FUN_RUN_OK;
    }

    return  FUN_RUN_END;
}


/*-----ubug_reach_url_head-----*/
char *ubug_reach_url_head(char *pSrc, char *pLimit)
{
    for (; pSrc < pLimit; pSrc++) {
        if (isalnum(*pSrc) || *pSrc == '/' || *pSrc == '.')
            return  pSrc;
    }

    return  NULL;
}


/*-----ubug_is_today_news-----*/
int ubug_is_today_news(char *string, int nLimit)
{
    for (int nCir = 0; nCir < DATE_CMODE; nCir++) {
        if (strnstr(string, timComMode[nCir], nLimit))
            return  FRET_P;
    }

    return  FRET_N;
}


/*---------------------------------------------
 *        Part Six: URL table name
 *
 *        1. ubug_set_tabname_default
 *        2. ubug_set_tabname_by_date
 *        3. set_date_mode
 *
-*---------------------------------------------*/

/*-----ubug_set_tabname_default-----*/
void ubug_set_tabname_default(void)
{
    sprintf(urlTabName, "%s", "t_url");
}


/*-----ubug_set_tabname_by_date-----*/
void ubug_set_tabname_by_date(void)
{
    TMS    *current_date;

    current_date = time_str_extract(NULL);

    set_date_mode(current_date);

    sprintf(urlTabName, "U%04d%02d%02d", 
        current_date->tm_year, current_date->tm_mon, current_date->tm_mday);
}


/*-----set_date_mode-----*/
void set_date_mode(TMS *date)
{
    char    year[5];

    sprintf(year, "%d", date->tm_year);

    sprintf(timComMode[0], "%s/%02d%02d", &year[2], date->tm_mon, date->tm_mday);
    sprintf(timComMode[1], "%s%02d%02d", year, date->tm_mon, date->tm_mday);
    sprintf(timComMode[2], "%s-%02d-%02d", &year[2], date->tm_mon, date->tm_mday);
    sprintf(timComMode[3], "%s-%02d/%02d", year, date->tm_mon, date->tm_mday);
    sprintf(timComMode[4], "%s_%02d_%02d", year, date->tm_mon, date->tm_mday);
    sprintf(timComMode[5], "%s%02d/%02d", &year[2], date->tm_mon, date->tm_mday);
    sprintf(timComMode[6], "%s/%02d-%02d", year, date->tm_mon, date->tm_mday);
}



