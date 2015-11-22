/*------------------------------------------
	Source file content Eleven part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:  URL catch rule - default
    Part Five:  URL check rules
    Part Six:   URL table name 

--------------------------------------------*/

/*------------------------------------------
	        Part Zero: Include
--------------------------------------------*/

#include "spinc.h"
#include "spnet.h"
#include "spdb.h"
#include "sphtml.h"

#include "spmpool.h"
#include "spmsg.h"

#include "spuglobal.h"
#include "spurlb.h"


/*------------------------------------------
	 Part Four: URL catch rule - default

        1. ubug_catch_default_rule
        2. ubug_locate_default_rule

--------------------------------------------*/

/*-----ubug_catch_default_rule------*/
int ubug_catch_default_rule(char *content, char **ret_content_point)
{
    char   *content_begin, *content_end;
    int     content_len;     

    if (!(content_begin = strstr(content, MATCH_HREF)))
        return  FUN_RUN_FAIL;

    content_begin += MHREF_LEN;

    if (!(content_end = strchr(content_begin, '"')))
        return  FUN_RUN_FAIL;

    *ret_content_point = content_end;

    if (!strncmp(content_end - MHTML_LEN, MATCH_HTML, MHTML_LEN) ||
        !strncmp(content_end - MSHTML_LEN, MATCH_SHTML, MSHTML_LEN) ||
        !strncmp(content_end - MHTM_LEN, MATCH_HTM, MHTM_LEN)) {
        if (!(content_begin =
              ubug_reach_url_head(content_begin, content_end)))
            return  FUN_RUN_END;

        if ((content_len = content_end - content_begin) >= urlMaxLen)
            return  FUN_RUN_END;

        return  content_len;
    }

    return  FUN_RUN_END;
}


/*-----ubug_locate_default_rule-----*/
int ubug_locate_default_rule(
    WEBIN *web_point, char **content_beg, char **content_end)
{
    (*content_beg) = web_point->w_conbuf;
    (*content_end) = (*content_beg) + web_point->w_size;

    return  FUN_RUN_OK;
}


/*-----ubug_catch_csto_rule-----*/
int ubug_catch_csto_rule(char *content, char **ret_content_point)
{
    return  FUN_RUN_OK;
}


/*-----ubug_locate_csto_rule-----*/
int ubug_locate_csto_rule(
    WEBIN *web_point, char **content_beg, char **content_end)
{
    return  FUN_RUN_OK;
}


/*------------------------------------------
	    Part Five: URL check rules

        1. ubug_get_pattern
        2. ubug_check_separator
        3. ubug_connect_head
        4. ubug_check_url_prefix
        5. ubug_reach_url_head
        6. ubug_url_count_nlayer

--------------------------------------------*/

/*-----ubug_get_pattern-----*/
int ubug_get_pattern(const char *host_string)
{
	int     count;

    for (count = 0; ; count++) {
		if (strstr(host_string, perWebPatt[count]))
			return	count;

        if (!strcmp(perWebPatt[count], "ending"))
            break;
	}

	return	FUN_RUN_FAIL;
}


/*-----ubug_check_separator-----*/
void ubug_check_separator(char *urlStr, int *uLen)
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

	return	wInfo->w_url;
}


/*-----ubug_check_url_prefix-----*/
int ubug_check_url_prefix(char *preSrc, int nLen)
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


/*-----ubug_url_count_nlayer-----*/
int ubug_url_count_nlayer(char *str_url)
{
    char   *pStr;
    int     nLayer = 1;

    if (!str_url)
        return  0;

    if (!strcmp(str_url, "/"))
        return  nLayer;

    for (pStr = str_url + 1; pStr; pStr++) {
        if (*pStr == '/') {
            if (*(pStr + 1) == '\0')
                break;

            nLayer++;
        }
    }

    return  nLayer;
}


/*------------------------------------------
	     Part Six: URL table name

         1. ubug_set_tabname_default

--------------------------------------------*/

/*-----ubug_set_tabname_default-----*/
void ubug_set_tabname_default(void)
{
    sprintf(urlTabName, "%s", "t_all_url");
}
