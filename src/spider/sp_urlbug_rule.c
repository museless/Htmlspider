/*------------------------------------------
	Source file content Eleven part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:  URL catch rule

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
	    Part Four: URL catch rule

        1. ubug_catch_normal_rule

--------------------------------------------*/

/*-----ubug_catch_normal_rule------*/
int ubug_catch_normal_rule(char *content, char **ret_content_point)
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
