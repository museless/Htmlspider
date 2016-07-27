/*---------------------------------------------
 *      Source file content Seven part
 *
 *      Part Zero:  Include
 *      Part One:   Define
 *      Part Two:   Local data
 *      Part Three: Local function
 *
 *      Part Four:  Libary function packing
 *      Part Five:  Shared funciton
 *      Part Six:   Log message
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "spinc.h"
#include "spnet.h"
#include "sphtml.h"


/*---------------------------------------------
 *      Part Four: Libary function packing
 *
 *      1. readn
 *      2. writen
 *      3. strnstr
 *      4. strnchr
 *      5. select_read
 *      6. strchrb
 *      7. atoin
 *      8. read_all_file
 *      9. examine_empty_string
 *      10. count_url_layer
 *
-*---------------------------------------------*/

/*-----readn-----*/
int readn(int rFd, void *rBuf, size_t bCount)
{
    int fRet, nCir = 0;

    while(FUN_RUN_OK) {
        if ((fRet = read(rFd, rBuf + nCir, bCount)) == bCount || fRet == 0) {
            nCir += fRet;
            return  nCir;

        } else if (fRet >= 0 && fRet < bCount) {
            nCir += fRet;
            bCount -= fRet;

        } else {
            return  FUN_RUN_FAIL;
        }
    }
}


/*-----writen-----*/
int writen(int wFd, void *wBuf, size_t bCount)
{
    int fRet, nCir = 0;

    while(FUN_RUN_OK) {
        if ((fRet = write(wFd, wBuf + nCir, bCount)) == bCount || fRet == 0) {
            return  (nCir += fRet);

        } else if (fRet >= 0 && fRet < bCount) {
            nCir += fRet;
            bCount -= fRet;

        } else {
            if (errno != EINTR)
                return  -1;
        }
    }
}


/*-----strnstr-----*/
char *strnstr(char *findBuf, const char *needStr, int nLimit)
{
    char    *pStr, sChar;

    if (findBuf == NULL || needStr == 0 || nLimit <= 0)
        return  NULL;

    sChar = findBuf[nLimit];
    findBuf[nLimit] = 0;

    pStr = strstr(findBuf, needStr);
    
    findBuf[nLimit] = sChar;

    return  pStr;
}


/*-----strnchr-----*/
inline char *strnchr(char *findBuf, char needCh, int nLimit)
{
    char    sChar, *pStr;

    if (findBuf == NULL || nLimit <= 0)
        return  NULL;

    sChar = findBuf[nLimit];
    findBuf[nLimit] = 0;

    pStr = strchr(findBuf, needCh);

    findBuf[nLimit] = sChar;
    return  pStr;
}


/*-----select_read-----*/
int select_read(int nSock, char *readBuf, int nRead, int nSec, long microSec)
{
    TMVAL   time_val;
    fd_set  fdset;

    time_val.tv_sec = nSec;
    time_val.tv_usec = microSec;

    FD_ZERO(&fdset);
    FD_SET(nSock, &fdset);

    if (select(nSock + 1, &fdset, NULL, NULL, &time_val) > FUN_RUN_END) {
        if (FD_ISSET(nSock, &fdset))
            return  (read(nSock, readBuf, nRead));
    }

    return  FUN_RUN_END;
}


/*-----strchrb-----*/
char *strchrb(char *strBeg, int nLimit, char fCh)
{
    for (int nCir = nLimit - 1; nCir > 0 ; nCir--) {
        if (strBeg[nCir] == fCh)
            return  (strBeg + nCir);
    }

    return  NULL;
}


/*-----atoin-----*/
int atoin(char *datBuf, int nLen)
{
    char    saveCh;
    int     nRet;

    saveCh = datBuf[nLen];
    datBuf[nLen] = 0;

    nRet = atoi(datBuf);
    datBuf[nLen] = saveCh;

    return  nRet;
}


/*-----read_all_file-----*/
int read_all_file(char **pStore, char *ofName, int readOff)
{
    struct  stat    stBuf;
    int     readFd;

    if ((readFd = open(ofName, O_RDWR)) == FUN_RUN_FAIL)
        return  FUN_RUN_FAIL;

    if (fstat(readFd, &stBuf) == FUN_RUN_FAIL)
        return  FUN_RUN_FAIL;

    if (readOff) {
        if (lseek(readFd, readOff, SEEK_SET) == FUN_RUN_FAIL)
            return  FUN_RUN_FAIL;

        stBuf.st_size -= readOff;
    }

    *pStore = NULL;

    if (stBuf.st_size) {
        if (!(*pStore = malloc(stBuf.st_size + 1)))
            return  FUN_RUN_FAIL;
    
        (*pStore)[stBuf.st_size] = 0;
    
        if (readn(readFd, *pStore, stBuf.st_size) == FUN_RUN_FAIL)
            return  FUN_RUN_FAIL;
    }

    close(readFd);

    return  stBuf.st_size;
}


/*-----examine_empty_string-----*/
inline int examine_empty_string(char *exaStr)
{
    return  (exaStr[0] == 0) ? FUN_RUN_OK : FUN_RUN_END;
}


/*-----time_str_extract-----*/
TMS *time_str_extract(char *timStr)
{
    TMS    *extTime;
    time_t  timType;

    timType = time(NULL);
    extTime = localtime(&timType);

    extTime->tm_year += 1900;
    extTime->tm_mon += 1;

    if (timStr && strlen(timStr) == DATEBUF_LEN) {
        int nYear, nMon, nDay;

        nYear = atoin(timStr, 4);
        nMon = atoin(timStr + 4, 2);
        nDay = atoin(timStr + 6, 2);
    
        if (nYear <= extTime->tm_year &&
           (nMon >= 1 && nMon <= 12) && (nDay >= 1 && nDay <= 31)) {
            extTime->tm_year = nYear;
            extTime->tm_mon = nMon;
            extTime->tm_mday = nDay;
        }
    }

    return  extTime;
}


/*-----count_url_layer-----*/
int count_url_layer(const char *str_url)
{
    int     nLayer = 1;

    if (!str_url)
        return  0;

    if (!strcmp(str_url, "/"))
        return  nLayer;

    for (const char *pStr = str_url + 1; pStr; pStr++) {
        if (*pStr == '/') {
            if (*(pStr + 1) == '\0')
                break;

            nLayer++;
        }
    }

    return  nLayer;
}


/*---------------------------------------------
 *         Part Five: Public function
 *
 *         1. url_layer_extract
 *         2. count_enter_num 
 *
-*---------------------------------------------*/

/*-----url_layer_extract-----*/
void url_layer_extract(UDATA *pData, char *urlStr, int uLen)
{
    char    *pStr;

    /* get path off */
    pStr = strchr(urlStr + MHTTP_LEN, '/');
    pData->ud_poff = pStr - urlStr;

    /* get file off */
    for (uLen--; uLen; uLen--) {
        if (urlStr[uLen] == '/')
            break;
    }

    pData->ud_foff = uLen + 1;
}


/*-----count_enter_num-----*/
int count_enter_num(const char *string)
{
    int     enter_num = 0;

    while ((string = strchr(string, '\n')))
        enter_num++, string++;

    return  enter_num + 1;
}


/*---------------------------------------------
 *         Part Six: Log message
 *
 *         1. set_error
 *         2. set_
 *
-*---------------------------------------------*/
