/*---------------------------------------------
 *     modification time: 2016-07-31 17:10:00
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

#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *           Part One: Local data
-*---------------------------------------------*/

static int  fileIndexBuf[WORD_LEN_LIMIT]; 


/*---------------------------------------------
 *          Part Two: Local function
-*---------------------------------------------*/

static bool exbug_findex_load(char *finPath);


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
        setmsg(LM15);
        exbug_sig_quit(PTHREAD_ERROR);
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
                buff_place_start(extSaveBuf), buff_now_size(extSaveBuf)))
            exbug_db_seterror(&dbKeysHandler, dbKeysName, PTHREAD_ERROR);

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

    if (buff_check_exist(extSaveBuf) && !buff_stru_empty(extSaveBuf)) {
        if (mysql_real_query(&dbKeysHandler,
                buff_place_start(extSaveBuf), buff_now_size(extSaveBuf)))
            setmsg(LM22, (char *)mysql_error(&dbKeysHandler));

        buff_stru_make_empty(extSaveBuf);
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

    if (!mc_conf_read(fConf, CONF_STR, dicPath, PATH_LEN)) {
        setmsg(LM0, fConf);
        return  FUN_RUN_FAIL;
    }

    if (!exbug_findex_load(dicPath)) {
        setmsg(LM30);
        return  FUN_RUN_FAIL;
    }

    if (!mc_conf_read(savConf, CONF_STR, dicPath, PATH_LEN)) {
        setmsg(LM0, savConf);
        return  FUN_RUN_FAIL;
    }

    for (int nCnt = 0; nCnt < WORD_LEN_LIMIT; nCnt++) {
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
bool exbug_findex_load(char *finPath)
{
    char   *fiPoint, *pMov;
    int    *inBuf = fileIndexBuf;

    if (read_all_file(&fiPoint, finPath, 0) == FUN_RUN_FAIL)
        return  false;

    for (pMov = fiPoint; *pMov; pMov++, inBuf++) {
        *inBuf = atoi(pMov);
        
        if (!(pMov = strchr(pMov, '\n')))
            break;
    }

    free(fiPoint);

    return  true;
}


/*-----exbug_index_load-----*/
int exbug_index_load(WHEAD **cStru, char *iName, int nTerms)
{
    WHEAD  *cNext;
    char   *iStore, *iMov;
    int32_t term_size = ((nTerms + 1) * sizeof(WHEAD));

    if (!(cNext = *cStru = mmdp_malloc(procMemPool, term_size))) {
        setmsg(LM15);
        return  FUN_RUN_END;
    }

    if (read_all_file(&iStore, iName, 0) == FUN_RUN_FAIL)
        return  FUN_RUN_END;

    for (iMov = iStore; *iMov; iMov++, cNext++) {
        strncpy(cNext->dc_utf8, iMov, UTF8_WORD_LEN);
        iMov += (UTF8_WORD_LEN + 1);

        cNext->dc_off = atoi(iMov);

        if (!(iMov = strchr(iMov, '\t'))) {
            setmsg(LM0, "missing tab");
            return  FUN_RUN_END;
        }

        cNext->dc_cnt = atoi(++iMov);

        if (!(iMov = strchr(iMov, '\n'))) {
            setmsg(LM0, "missing enter");
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
    if (!(*termStru = mmdp_malloc(procMemPool, sizeof(WDCB)))) {
        setmsg(LM15);
        return  FUN_RUN_END;
    }

    memset(*termStru, 0, sizeof(WDCB));

    if (read_all_file(&((*termStru)->wb_lterms), termFile, 0) == FRET_N) {
        setmsg(LM31);
        return  FUN_RUN_END;
    }

    if (!mgc_add(&objGc, ((*termStru)->wb_lterms), free))
        setmsg(LM5, "terms");

    if (exbRunSet.emod_exinit) {
        if (!exbRunSet.emod_exinit(*termStru, nOff))
            return  FUN_RUN_END;
    }

    return  FUN_RUN_OK;
}

