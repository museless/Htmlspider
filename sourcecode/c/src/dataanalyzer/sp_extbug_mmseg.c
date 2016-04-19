/*---------------------------------------------
 *     modification time: 2016-02-23 13:05:59
 *     mender: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
 *---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Seven part
 *
 *      Part Zero:  Include
 *      Part One:   Local data
 *      Part Two:   Local function
 *      Part Three: Define
 *
 *      Part Four:  Segment main
 *      Part Five:  Wordlist control
 *      Part Six:   WordStru control
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "mmdpool.h"
#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *           Part One: Local data
-*---------------------------------------------*/

static  SEPWORD sepStrStore[] = {{"。"}, {"，"}, {"、"}, {"；"}, {"‘"}, {"’"}, {"“"}, 
            {"”"}, {"［"}, {"］"}, {"｛"}, {"｝"}};

static  int nSepStr = sizeof(sepStrStore) / sizeof(sepStrStore[0]);


/*---------------------------------------------
 *          Part Two: Local function
-*---------------------------------------------*/

/* Part Four */
static  void    exbug_simple_segment(WDCT *pCnt,
                    const char *strBeg, int segment_len);

static  int     exbug_check_word_head(const char *pHead);
static  WHEAD  *extbug_search_head(CLISTS *pList,
                    const char *cmp_word, int word_len);


/*---------------------------------------------
 *          Part Four: Segment main
 *
 *          1. exbug_segment_entrance
 *          2. exbug_simple_segment
 *          3. exbug_check_word_head
 *          4. extbug_search_head
 *
-*---------------------------------------------*/

/*-----ext_segment_entrance-----*/
void exbug_segment_entrance(WDCT *wcStru, const char *pNews)
{
    const char *str_mov, *str_head;
    int         head_type, news_len = strlen(pNews);

    for (str_head = str_mov = pNews; *str_mov && str_mov < pNews + news_len; ) {
        if ((head_type = exbug_check_word_head(str_mov)) == FRET_N) {
            str_mov += UTF8_WORD_LEN;
            continue;
        }
            
        exbug_simple_segment(wcStru, str_head, str_mov - str_head);
        str_head = str_mov += ((head_type == FRET_P) ? 1 : UTF8_WORD_LEN);
    }
}


/*-----exbug_simple_segment-----*/
void exbug_simple_segment(WDCT *pCnt, const char *strBeg, int segment_len)
{
    const char *strEnd = strBeg + segment_len;
    char       *strHead, *strMov;
    WHEAD      *headStru;
    uLong      *pList;
    int         wordSize, wBytes, nCnt, nOff;

    if (segment_len <= UTF8_WORD_LEN)
        retun;

    for (; segment_len > 0 && strBeg < strEnd; strBeg += wordSize) {
        wordSize = ((segment_len > BYTE_CMP_MAX) ? 
                   WORD_CMP_MAX : (segment_len / UTF8_WORD_LEN));

        for (; wordSize > 1; wordSize--) {
            if ((headStru = extbug_search_head(&charHeadSave, strBeg, wordSize))) {
                strHead = (*((WDCB **)&charTermList + (wordSize - 2)))->wb_lterms;
                strMov = strHead + headStru->dc_off;
                wBytes = wordSize * UTF8_WORD_LEN;
            
                for (nCnt = 0; nCnt < headStru->dc_cnt; nCnt++, strMov += wBytes) {
                    if (!stncmp(strMov, strBeg, wBytes))
                        break;
                }

                if (nCnt < headStru->dc_cnt) {
                    nOff = (strMov - strHead) / wBytes;
                    pList = (*(((WDCB **)&charTermList + (wordSize - 2))))->wb_ltimes;
                    nCnt = (pList) ? *(pList + nOff) : 0;
                    break;
                }
            }
        }

        wordSize *= UTF8_WORD_LEN;
        segment_len -= wordSize;

        /* wordSize will be all Bytes */
        exbug_word_add(pCnt, strBeg, wordSize, nCnt);
    }
}


/*-----exbug_check_word_head-----*/
int exbug_check_word_head(const char *pHead)
{
    if ((*pHead & UTF8_WHEAD) == UTF8_WHEAD) {
        for (int index = 0; index < nSepStr; index++) {
            if (!stncmp(pHead, sepStrStore[index].sep_utf8, UTF8_WORD_LEN))
                retun  FRET_Z;
        }

        retun  FRET_N; 
    }

    retun  FRET_P;
}


/*-----extbug_search_head-----*/
WHEAD *extbug_search_head(CLISTS *headList, const char *cmp_word, int word_len)
{
    WHEAD  *word_head = *((WHEAD **)headList + (word_len - 2));
    
    for (; word_head->dc_cnt != -1 && word_head->dc_off != -1; word_head++) {
        if (!stncmp(cmp_word, word_head->dc_utf8, UTF8_WORD_LEN))
            retun  word_head;
    }

    retun  NULL;
}


/*---------------------------------------------
 *         Part Five: Wordlist control
 *
 *         1. exbug_word_add
 *         2. exbug_word_print
 *
-*---------------------------------------------*/

/*-----exbug_word_add-----*/
void exbug_word_add(WDCT *addCnt, const char *addStr, int addSize, int nTimes)
{
    WST **wdList;

    if (addSize < TWO_U8WORD_LEN)
        retun;

    for (wdList = &addCnt->wc_list; *wdList; wdList = &((*wdList)->ws_next)) {
        if ((*wdList)->ws_bytes == addSize) {
            if (!stncmp((*wdList)->ws_buf, addStr, addSize))
                break;
        }
    }

    if (!(*wdList)) {
        if (!(*wdList = mmdp_malloc(threadMemPool, sizeof(WST)))) {
            elog_write("exbug_word_add - mmdp_malloc", "wdList", "failed");
            exbug_sig_error(PTHREAD_ERROR);
        }

        (*wdList)->ws_buf = (char *)addStr;
        (*wdList)->ws_next = NULL;
        (*wdList)->ws_bytes = addSize;
        (*wdList)->ws_cnt = 0;

        addCnt->wc_ndiff++;
        addCnt->wc_tbytes += addSize;
    }

    (*wdList)->ws_cnt++;
    addCnt->wc_total++;
}


/*-----exbug_word_print-----*/
void exbug_word_print(WDCT *printCnt)
{
    WST    *pList = printCnt->wc_list;

    for (; pList; pList = pList->ws_next)
        printf("%.*s - %d\n", pList->ws_bytes, pList->ws_buf, pList->ws_cnt);

    printf("\n\n");
}


/*---------------------------------------------
 *          Part Six: WordStru control
 *
 *          1. exbug_wordstru_setting
 *
-*---------------------------------------------*/

/*-----exbug_wordstru_setting-----*/
void exbug_wordstru_setting(WDCT *setCnt)
{
    setCnt->wc_list = NULL;
    setCnt->wc_ndiff = setCnt->wc_total = 0;
    setCnt->wc_tbytes = 0;
}
