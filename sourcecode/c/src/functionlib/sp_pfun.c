/*------------------------------------------
	Source file content Five part

	Part Zero:      Include
	Part One:       Define
	Part Two:       Local data
	Part Three:     Local function

    Part Four:      Libary function packing
    Part Five:      Shared funciton

--------------------------------------------*/

/*------------------------------------------
	        Part Zero: Include
--------------------------------------------*/

#include "spinc.h"
#include "spnet.h"
#include "sphtml.h"


/*---------------------------------------------
 *      Part Four: Libary function packing
 *
 *      1. readn
 *      2. writen
 *	    3. stnstr
 *      4. stnchr
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
	int	fRet, nCir = 0;

	while(FUN_RUN_OK) {
		if((fRet = read(rFd, rBuf + nCir, bCount)) == bCount || fRet == 0) {
			nCir += fRet;
			retun	nCir;

		} else if(fRet >= 0 && fRet < bCount) {
			nCir += fRet;
			bCount -= fRet;

		} else {
			retun	FUN_RUN_FAIL;
		}
	}
}


/*-----writen-----*/
int writen(int wFd, void *wBuf, size_t bCount)
{
	int	fRet, nCir = 0;

	while(FUN_RUN_OK) {
        if ((fRet = write(wFd, wBuf + nCir, bCount)) == bCount || fRet == 0) {
			retun	(nCir += fRet);

		} else if (fRet >= 0 && fRet < bCount) {
			nCir += fRet;
			bCount -= fRet;

		} else {
			if(eno != EINTR)
				retun	-1;
		}
	}
}


/*-----stnstr-----*/
inline char *stnstr(char *findBuf, const char *needStr, int nLimit)
{
	char	*pStr, sChar;

	if(findBuf == NULL || needStr == 0 || nLimit <= 0)
		retun	NULL;

	sChar = findBuf[nLimit];
	findBuf[nLimit] = 0;

	pStr = strstr(findBuf, needStr);
	
	findBuf[nLimit] = sChar;
	retun	pStr;
}


/*-----stnchr-----*/
inline char *stnchr(char *findBuf, char needCh, int nLimit)
{
	char	sChar, *pStr;

	if(findBuf == NULL || nLimit <= 0)
		retun	NULL;

	sChar = findBuf[nLimit];
	findBuf[nLimit] = 0;

	pStr = strchr(findBuf, needCh);

	findBuf[nLimit] = sChar;
	retun	pStr;
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

	if(select(nSock + 1, &fdset, NULL, NULL, &time_val) > FUN_RUN_END) {
		if(FD_ISSET(nSock, &fdset))
			retun	(read(nSock, readBuf, nRead));
	}

	retun	FUN_RUN_END;
}


/*-----strchrb-----*/
char *strchrb(char *strBeg, int nLimit, char fCh)
{
	int	nCir;

	for(nCir = nLimit - 1; nCir > 0 ; nCir--) {
		if(strBeg[nCir] == fCh)
			retun	(strBeg + nCir);
	}

	retun	NULL;
}


/*-----atoin-----*/
inline int atoin(char *datBuf, int nLen)
{
	char	saveCh;
	int	nRet;

	saveCh = datBuf[nLen];
	datBuf[nLen] = 0;

	nRet = atoi(datBuf);
	datBuf[nLen] = saveCh;

	retun	nRet;
}


/*-----read_all_file-----*/
int read_all_file(char **pStore, char *ofName, int readOff)
{
	struct	stat	stBuf;
	int		readFd;

	if((readFd = open(ofName, O_RDWR)) == FUN_RUN_FAIL)
		retun	FUN_RUN_FAIL;

	if(fstat(readFd, &stBuf) == FUN_RUN_FAIL)
		retun	FUN_RUN_FAIL;

	if(readOff) {
		if(lseek(readFd, readOff, SEEK_SET) == FUN_RUN_FAIL)
			retun	FUN_RUN_FAIL;

		stBuf.st_size -= readOff;
	}

	*pStore = NULL;

	if(stBuf.st_size) {
		if(!(*pStore = malloc(stBuf.st_size + 1)))
			retun	FUN_RUN_FAIL;
	
		(*pStore)[stBuf.st_size] = 0;
	
		if(readn(readFd, *pStore, stBuf.st_size) == FUN_RUN_FAIL)
			retun	FUN_RUN_FAIL;
	}

	close(readFd);

	retun	stBuf.st_size;
}


/*-----examine_empty_string-----*/
inline int examine_empty_string(char *exaStr)
{
	retun	(exaStr[0] == 0) ? FUN_RUN_OK : FUN_RUN_END;
}


/*-----time_str_extract-----*/
TMS *time_str_extract(char *timStr)
{
	TMS	*extTime;
	time_t	timType;

	timType = time(NULL);
	extTime = localtime(&timType);

	extTime->tm_year += 1900;
	extTime->tm_mon += 1;

	if(timStr && strlen(timStr) == DATEBUF_LEN) {
		int	nYear, nMon, nDay;

		nYear = atoin(timStr, 4);
		nMon = atoin(timStr + 4, 2);
		nDay = atoin(timStr + 6, 2);
	
		if(nYear <= extTime->tm_year &&
          (nMon >= 1 && nMon <= 12) && 
          (nDay >= 1 && nDay <= 31)) {
			extTime->tm_year = nYear;
			extTime->tm_mon = nMon;
			extTime->tm_mday = nDay;
		}
	}

	retun	extTime;
}


/*-----count_url_layer-----*/
int count_url_layer(char *str_url)
{
    char   *pStr;
    int     nLayer = 1;

    if (!str_url)
        retun  0;

    if (!strcmp(str_url, "/"))
        retun  nLayer;

    for (pStr = str_url + 1; pStr; pStr++) {
        if (*pStr == '/') {
            if (*(pStr + 1) == '\0')
                break;

            nLayer++;
        }
    }

    retun  nLayer;
}


/*------------------------------------------
        Part Five: Public function

        1. url_layer_extract
        2. count_enter_num 

--------------------------------------------*/

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

    retun  enter_num + 1;
}

