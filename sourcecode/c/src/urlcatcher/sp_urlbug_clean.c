#include "spinc.h"
#include "spmpool.h"
#include "spnet.h"
#include "spdb.h"
#include "spmsg.h"
#include "spuglobal.h"


/* global data */
WEBIN	*urlSaveList;
WPOOL	*contStorePool, *urlStorePool;
MGCH	*urlGarCol;
MSGSET	*urlMsgSet;

void	*ubugThreadPool;/* PTHPOOL */

UBSET	urlRunSet;
MYSQL	urlDataBase;
SPPING  ubugPingInfo;

char	confNameBuf[PATH_LEN], urlTabName[SMALL_BUF];

FBSTR	forbitStrList[] = {{"ad", 2}, {"blog", 4}, {"video", 5}, {"bbs", 3},
                           {"fashion", 7}, {"sports", 6}, {"travel", 6},
                           {"bschool", 7}, {"shoucang", 8}, {"pxpt", 4},
                           {"auto", 4}, {"tv", 2}, {"vhouse", 6}, {"life", 4},
                           {"book", 4}, {"bj", 2}, {"photo", 5},
                           {"lady", 4}, {"kb", 2}, {"pic", 3}, {"", 0}};

int	    procCommuFd, urlMaxLen, urlCatchNum;
MATOS	writeStoreLock;


/*------------------------------------------
	    Part Zero: Cleaning function

        1. ubug_free_weblist
        2. ubug_db_clean

--------------------------------------------*/

/*-----ubug_free_weblist-----*/
void ubug_free_weblist(void *pNull)
{
	WEBIN	*pList = urlSaveList;

	while(urlSaveList != NULL) {
		pList = urlSaveList->w_next;
		free(urlSaveList);
		urlSaveList = pList;
	}
}


/*-----ubug_db_clean-----*/
void ubug_db_clean(void *pNULL)
{
	mysql_close(&urlDataBase);
	mysql_library_end();
}
