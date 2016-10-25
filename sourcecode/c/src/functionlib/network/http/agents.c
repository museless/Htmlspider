/*---------------------------------------------
 *     modification time: 2016-10-25 17:32:31
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: agents.c
 *     creation time: 2016-10-25 17:32:31
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *                  Include 
-*---------------------------------------------*/

#include "spinc.h"


/*---------------------------------------------
 *                Global data 
-*---------------------------------------------*/

char    agentsList[] = {
    /* opera */
    "Opera/9.51 (Windows NT 5.2; U; en)",
    "Opera/9.51 (X11; Linux i686; U; Linux Mint; en)",

    "Opera/9.63 (Windows NT 6.0; U; cs) Presto/2.1.1",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 6.0; en) Opera 9.50",
    "Mozilla/4.0 (compatible; MSIE 6.0; X11; Linux i686; en) Opera 9.27",

    /* Lynx */
    "Lynx/2.8.8dev.3 libwww-FM/2.14 SSL-MM/1.4.1",
    "Lynx/2.8.6rel.5 libwww-FM/2.14 SSL-MM/1.4.1 OpenSSL/1.0.0a",
    "Lynx/2.8.5rel.1 libwww-FM/2.14 SSL-MM/1.4.1 OpenSSL/0.9.7m",
    "Lynx/2.8.7pre.5 libwww-FM/2.14 SSL-MM/1.4.1",

    /* Chrome */
    "Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:45.0)"
    " Gecko/20100101 Firefox/45.0",

    "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36"
    " (KHTML, like Gecko) Chrome/41.0.2224.3 Safari/537.36",

    "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.1"
    " (KHTML, like Gecko) Chrome/14.0.811.0 Safari/535.1",
};


/*---------------------------------------------
 *                 Function
-*---------------------------------------------*/

/*-----http_pick_agent-----*/
const char *http_pick_agent(void)
{
    return  agentsList[1];
}

