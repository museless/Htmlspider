/*---------------------------------------------
 *     modification time: 2016-07-31 16:05:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/


#include "sp.h"
#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *                Global data
-*---------------------------------------------*/

Muselog  messageLog;
Gc       objGc;
BUFF	*extSaveBuf;

void	*threadMemPool, *procMemPool;	/* DMPH */
void	*ebSemControl;			        /* MSEM */

EXBSET	exbRunSet;
MYSQL	dbNewsHandler, dbDicHandler, dbKeysHandler;
CLISTS	charTermList, charHeadSave;

char	sqlSeleCom[SQL_LICOM_LEN];

char	tblNewsName[SQL_TABNAME_LEN], dbNewsName[SQL_DBNAME_LEN];
char	tblWordName[SQL_TABNAME_LEN], dbDicName[SQL_DBNAME_LEN];
char	tblKeysName[SQL_TABNAME_LEN], dbKeysName[SQL_DBNAME_LEN];

MATOS	pthreadCtlLock, freeCtlLock, nPaperLock, dicDbLock;

uLong	tPaperNum;
int32_t	nExbugPthead, upMaxTerms, keywordListSize;

