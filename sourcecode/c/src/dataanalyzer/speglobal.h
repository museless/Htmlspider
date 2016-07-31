/*---------------------------------------------
 *     modification time: 2016.07.31 16:50
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

#pragma once


/*---------------------------------------------
 *                  define
-*---------------------------------------------*/

#define PROC_ERROR      0x0
#define PTHREAD_ERROR   0x1


/*---------------------------------------------
 *                extern data
-*---------------------------------------------*/

extern Gc       objGc;
extern Muselog  messageLog;

extern BUFF    *extSaveBuf;

extern void    *threadMemPool, *procMemPool;   /* DMPH */
extern void    *ebSemControl;                  /* MSEM */

extern EXBSET   exbRunSet;
extern MYSQL    dbNewsHandler, dbDicHandler, dbKeysHandler;

extern CLISTS   charTermList, charHeadSave;

extern char     sqlSeleCom[];
extern char     tblNewsName[], dbNewsName[];
extern char     tblWordName[], dbDicName[];
extern char     tblKeysName[], dbKeysName[];

extern MATOS    pthreadCtlLock, freeCtlLock, nPaperLock, dicDbLock;

extern uLong    tPaperNum;
extern int32_t	nExbugPthead, upMaxTerms, keywordListSize;

