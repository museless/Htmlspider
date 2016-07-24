/*---------------------------------------------
 *     modification time: 2016-02-18 09:25:59
 *     mender: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
 *---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Six part
 *
 *      Part Zero:  Include
 *      Part One:   Local data
 *      Part Two:   Local function
 *      Part Three: Define
 *
 *      Part Four:  Extract part
 *      Part Five:  Dictionary control
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "mmdpool.h"
#include "mmdperr.h"

#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *           Part One: Local data
-*---------------------------------------------*/

static int  fileIndexBuf[WORD_LEN_LIMIT]; 


/*---------------------------------------------
 *          Part Four: Extract part
 *
 *          1. exbug_extract_keyword
 *          2. exbug_update_terms
 *          3. exbug_data_sync
 *
-*---------------------------------------------*/

/*-----exbug_extract_keyword-----*/
void exbug_extract_keyword(WDCT *extDic)
{
    return;
}


/*-----exbug_update_terms-----*/
void exbug_update_terms(WDCT *upList, const char *pInd)
{
    WST    *pList = upList->wc_list;
    char   *update_string;
    int     nOff, nCnt;

    update_string = mmdp_malloc(threadMemPool,
            (upList->wc_tbytes + (upList->wc_ndiff * PER_WD_MORE)) + 1);

    if (!update_string) {
        elog_write("exbug_update_terms - mmdp_malloc", FUNCTION_STR, ERROR_STR);
        exbug_sig_error(PTHREAD_ERROR);
    }

    for (nOff = nCnt = 0; pList && nCnt < upMaxTerms; nCnt++) {
        if (nOff + pList->ws_bytes > keywordListSize)
            break; 

        nOff += sprintf(&update_string[nOff], TERMS_FORMAT, 
                    pList->ws_bytes, pList->ws_buf);

        pList = pList->ws_next;
    }

    mato_lock(dicDbLock);

    if (!buff_size_enough(extSaveBuf, nOff + UPKEY_OTH_MAX)) {
        if (mysql_real_query(&dbKeysHandler, 
                buff_place_start(extSaveBuf), buff_now_size(extSaveBuf))) {
            if (mysql_error_log(&dbKeysHandler, dbKeysName, 
                    "exbug_update_terms - mysql_real_query") != FUN_RUN_OK)
                exbug_sig_error(PTHREAD_ERROR);
        }

        buff_stru_make_empty(extSaveBuf);
    }

    buff_size_add(extSaveBuf, (buff_stru_empty(extSaveBuf) ? 
        sprintf(buff_place_start(extSaveBuf), INSERT_KEYWD, 
            tblKeysName, pInd, nOff, update_string, upList->wc_ndiff) :
        sprintf(buff_place_end(extSaveBuf), INSERT_KW_NEXT, 
            pInd, nOff, update_string, upList->wc_ndiff)));

    mato_unlock(dicDbLock);
}


/*-----exbug_data_sync-----*/
void exbug_data_sync(void)
{
    mato_lock(dicDbLock);

    if (buff_check_exist(extSaveBuf)) {
        if (!buff_stru_empty(extSaveBuf)) {
            if (mysql_real_query(&dbKeysHandler,
                    buff_place_start(extSaveBuf), buff_now_size(extSaveBuf)))
                elog_write("exbug_data_sync", 
                    FUNCTION_STR, (char *)mysql_error(&dbKeysHandler));

            buff_stru_make_empty(extSaveBuf);
        }
    }

    mato_unlock(dicDbLock);
}


/*---------------------------------------------
 *        Part Five: Dictionary control
 *
 *        1. exbug_dictionary_load
 *        2. exbug_findex_load
 *        3. exbug_index_load
 *        4. exbug_terms_load
 *
-*---------------------------------------------*/

/*-----exbug_dictionary_load-----*/
int exbug_dictionary_load(char *fConf, char *savConf,
        char *fName, CLISTS *pTerm, CLISTS *charHead)
{
    WDCB  **pContent;
    WHEAD **pHead;
    char    nameBuf[PATH_LEN];
    char    dicPath[PATH_LEN];
    int     nCnt;

    if (mc_conf_read(fConf, CONF_STR, dicPath, PATH_LEN) == FUN_RUN_FAIL) {
        mc_conf_print_err(fConf);
        return  FUN_RUN_FAIL;
    }

    if (exbug_findex_load(dicPath) == FUN_RUN_FAIL) {
        printf("Extbug---> exbug_findex_load failed\n");
        return  FUN_RUN_FAIL;
    }

    if (mc_conf_read(savConf, CONF_STR, dicPath, PATH_LEN) == FUN_RUN_FAIL) {
        mc_conf_print_err(savConf);
        return  FUN_RUN_FAIL;
    }

    for (nCnt = 0; nCnt < WORD_LEN_LIMIT; nCnt++) {
        sprintf(nameBuf, "%s/%s%d", dicPath, fName, nCnt + 2);

        pContent = ((WDCB **)pTerm + nCnt);

        if (fileIndexBuf[nCnt]) {
            if (!exbug_terms_load(pContent, nameBuf, nCnt))
                return  FUN_RUN_FAIL;
        }

        sprintf(nameBuf, "%s/Index%d", dicPath, nCnt + 2);
        pHead = (WHEAD **)charHead + nCnt;

        if (fileIndexBuf[nCnt]) {
            if (!exbug_index_load(pHead, nameBuf, fileIndexBuf[nCnt]))
                return  FUN_RUN_FAIL;
        }
    }

    return  FUN_RUN_OK;
}


/*-----exbug_findex_load-----*/
int exbug_findex_load(char *finPath)
{
    char   *fiPoint, *pMov;
    int    *inBuf = fileIndexBuf;

    if (read_all_file(&fiPoint, finPath, 0) == FUN_RUN_FAIL)
        return  FUN_RUN_FAIL;

    for (pMov = fiPoint; *pMov; pMov++, inBuf++) {
        *inBuf = atoi(pMov);
        
        if ((pMov = strchr(pMov, '\n')) == NULL)
            break;
    }

    free(fiPoint);

    return  FUN_RUN_OK;
}


/*-----exbug_index_load-----*/
int exbug_index_load(WHEAD **cStru, char *iName, int nTerms)
{
    WHEAD   *cNext;
    char    *iStore, *iMov;
    int      term_size = ((nTerms + 1) * sizeof(WHEAD));

    if ((cNext = *cStru = mmdp_malloc(procMemPool, term_size)) == NULL) {
        exbug_perror("exbug_index_load - mmdp_malloc", errno);
        return  FUN_RUN_END;
    }

    if (read_all_file(&iStore, iName, 0) == FUN_RUN_FAIL)
        return  FUN_RUN_END;

    for (iMov = iStore; *iMov; iMov++, cNext++) {
        strncpy(cNext->dc_utf8, iMov, UTF8_WORD_LEN);
        iMov += (UTF8_WORD_LEN + 1);

        cNext->dc_off = atoi(iMov);

        if ((iMov = strchr(iMov, '\t')) == NULL) {
            exbug_perror("exbug_index_load - strchr - miss tab", errno);
            return  FUN_RUN_END;
        }

        cNext->dc_cnt = atoi(++iMov);

        if ((iMov = strchr(iMov, '\n')) == NULL) {
            exbug_perror("exbug_index_load - strchr - miss enter", errno);
            return  FUN_RUN_END;
        }
    }

    free(iStore);

    cNext->dc_cnt = cNext->dc_off = -1;

    return  FUN_RUN_OK;
}


/*-----exbug_terms_load-----*/
int exbug_terms_load(WDCB **termStru, char *termFile, int nOff)
{
    if ((*termStru = mmdp_malloc(procMemPool, sizeof(WDCB))) == NULL) {
        exbug_perror("exbug_terms_load - mmdp_malloc", errno);
        return  FUN_RUN_END;
    }

    memset(*termStru, 0, sizeof(WDCB));

    if (read_all_file(&((*termStru)->wb_lterms), termFile, 0) == FRET_N)
        return  FUN_RUN_END;

    if (mgc_add(exbGarCol, ((*termStru)->wb_lterms), free) != FUN_RUN_OK)
        printf("Extbug---> exbug_terms_load - mgc_add failed\n");

    if (exbRunSet.emod_exinit) {
        if (!exbRunSet.emod_exinit(*termStru, nOff))
            return  FUN_RUN_END;
    }

    return  FUN_RUN_OK;
}

