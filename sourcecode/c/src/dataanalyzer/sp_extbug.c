/*---------------------------------------------
 *     modification time: 2016-02-17 09:25:59
 *     mender: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
 *---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Eleven part
 *
 *      Part Zero:  Include
 *      Part One:   Local data
 *      Part Two:   Local function
 *      Part Three: Define
 *
 *      Part Four:  Extbug main
 *      Part Five:  Initialization
 *      Part Six:   Dictionary control
 *      Part Seven: Exbug entrance
 *      Part Eight: Message part
 *      Part Nine:  Misc part
 *      Part Ten:   Extbug work
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "mipc.h"
#include "mmdpool.h"
#include "mmdperr.h"

#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *          Part One: Local data
-*---------------------------------------------*/

static  int fileIndexBuf[WORD_LEN_LIMIT];


/*---------------------------------------------
 *          Part Two: Local function
-*---------------------------------------------*/

/* Part Four */
static  void    exbug_command_analyst(int nPara, char **paraList);

/* Part Five */
static  int     mainly_init(void);
static  void    exbug_data_init(void);
static  void    exbug_timbuf_init(char *pTime);
static  int     exbug_mempool_init(void);
static  int     exbug_read_config(void); 
static  void    exbug_final_init(void);

static  int     exbug_up_init(void);

/* Part Six */
static  int     exbug_dictionary_load(char *fConf, char *savConf, char *fName, CLISTS *pTerm, CLISTS *charHead);
static  int     exbug_findex_load(char *finPath);
static  int     exbug_index_load(WHEAD **cStru, char *iName, int nTerms);
static  int     exbug_terms_load(WDCB **termStru, char *termFile, int nOff);

/* Part Seven */
static  void    exbug_keyword_job(void);
static  void    exbug_create_pthread(NCONT *pPara);
static  void   *exbug_memory_malloc(int nSize);

/* Part Nine */

/* Part Ten */
static  void    *exbug_pthread_entrance(void *pPara);


/*------------------------------------------
    Part Three: Define
--------------------------------------------*/

#define exbug_runset_init(exbent, minit, exinit, mwrt, mfwrt, ctab, runmode, upfun, mname) { \
    exbRunSet.emod_entry = exbent; \
    exbRunSet.emod_init = minit; \
    exbRunSet.emod_exinit = exinit; \
    exbRunSet.emod_wrt = mwrt; \
    exbRunSet.emod_fwrt = mfwrt; \
    exbRunSet.emod_tchange = ctab; \
    exbRunSet.emod_rmode = runmode; \
    exbRunSet.emod_up = upfun; \
    exbRunSet.emod_maname = mname; \
}


/*------------------------------------------
    Part Four: Extbug main

    1. main
    2. exbug_command_analyst

--------------------------------------------*/

/*-----main-----*/
int main(int argc, char *argv[])
{
    exbug_command_analyst(argc, argv);

    if (mainly_init() && exbug_database_init() && exbug_mempool_init()) {
        if (exbug_paper_num_read() && exbug_dictionary_load("extbug_noun_findex_path",
        "extbug_noun_path", "noun", &charTermList, &charHeadSave) == FUN_RUN_OK) {
            if (exbRunSet.emod_init && exbRunSet.emod_init() == FUN_RUN_OK) {
                exbug_final_init();
                exbRunSet.emod_entry();
            }
        }
    }

    mgc_all_clean(exbGarCol);

    return  FUN_RUN_OK;
}


/*-----exbug_command_analyst-----*/
void exbug_command_analyst(int nPara, char **paraList)
{
    int     conf_flags = 0, time_flags = 0;
    char    ch;

    while ((ch = getopt(nPara, paraList, "htc:"))) {
        switch (ch) {
            case 'h':
                exbug_print_help();
                mc_conf_unload();
                exit(FUN_RUN_OK);
            
            case 'c':
                mc_load_config("Extbug", optarg);
                conf_flags = 1;

            case 't':
                exbug_timbuf_init(paraList[++nCir]);
                time_flags = 1;
                break;

            default:
                printf("Extbug---> wrong command: %c\n", ch);
                mc_conf_unload();
                exit(FUN_RUN_END);
        }
    }
    
    if (!conf_flags) {
        printf("Extbug---> must set [-c]\n");
		exit(FUN_RUN_FAIL);    
    }
    
    if (!time_flags)
        exbug_timbuf_init(NULL);
}


/*---------------------------------------------
 *          Part Five: Initialization
 *
 *          1. mainly_init
 *          2. exbug_data_init
 *          3. exbug_timbuf_init
 *          4. exbug_mempool_init
 *          5. exbug_read_config
 *
 *          6. exbug_final_init
 *
 *          7. exbug_up_init
 *          8. exbug_tran_init
 *
-*---------------------------------------------*/

/*-----mainly_init-----*/
static int mainly_init(void)
{
    exbug_runset_init(
    exbug_keyword_job, exbug_up_init, 
    NULL, NULL, NULL, exbug_create_keyword_table, 
    exbug_extract_keyword, exbug_update_terms, MASK_EXT);

    if (!sp_normal_init(
        "Extbug", &exbGarCol, (MSGSET **)&extbugMsgSet, 
        exbug_msg_init, "extbug_err_locate", exbugIpcFd))
        return  FUN_RUN_END;

    /* mgc one init */
    if (mgc_one_init(&extResCol, (gcfun)mysql_free_result, TRY_MORE) == MGC_FAILED) {
        extbug_perror("mainly_init - mgc_one_init", errno);
        return  FUN_RUN_END;
    }

    /* atomic type parameter init */
    mato_init(&pthreadCtlLock, 0);
    mato_init(&freeCtlLock, 0);
    mato_init(&nPaperLock, 1);

    if (exbug_read_config() == FRET_Z)
        return  FRET_Z;

    exbug_data_init();

    sprintf(sqlSeleCom, GET_NEWS_CONT, tblNewsName, exbRunSet.emod_maname, nExbugPthead);

    return  exbug_signal_init();
}


/*-----exbug_data_init-----*/
static void exbug_data_init(void)
{
    memset(&charTermList, 0, sizeof(CLISTS));
    memset(&charHeadSave, 0, sizeof(CLISTS));
}


/*-----exbug_timbuf_init-----*/
static void exbug_timbuf_init(char *pTime)
{
    TMS    *extTime = time_str_extract(pTime);

    sprintf(
    tblNewsName, "N%d%02d%02d", 
    extTime->tm_year, extTime->tm_mon, extTime->tm_mday);

    sprintf(
    tblKeysName, "K%d%02d%02d",
    extTime->tm_year, extTime->tm_mon, extTime->tm_mday);
}


/*-----exbug_mempool_init-----*/
static int exbug_mempool_init(void)
{
    if ((threadMemPool = mmdp_create(upMaxTerms * PER_WORD_MAX)) == NULL) {
        extbug_perror("exbug_mempool_init - mmdp_create thread", errno);
        return  FUN_RUN_END;    
    }

    if (mgc_add(exbGarCol, threadMemPool, (gcfun)mmdp_free_all) == MGC_FAILED)
        extbug_perror("exbug_mempool_init - mgc_add - threadMemPool", errno);

    if ((procMemPool = mmdp_create(PROC_MP_SIZE)) == NULL) {
        extbug_perror("exbug_mempool_init - mmdp_create proc", errno);
        return  FUN_RUN_END;    
    }

    if (mgc_add(exbGarCol, procMemPool, (gcfun)mmdp_free_all) == MGC_FAILED)
        extbug_perror("exbug_mempool_init - mgc_add - procMemPool", errno);

    return  FUN_RUN_OK;
}


/*-----exbug_read_config-----*/
int exbug_read_config(void)
{
    char    path_string[PATH_LEN];

    if (mc_conf_read(
        "extbug_pthread_num", CONF_NUM, &nExbugPthead, sizeof(int)) == FRET_N) {
        mc_conf_print_err("extbug_pthread_num");
        return  FRET_Z;
    }

    if (mc_conf_read(
        "extbug_update_max", CONF_NUM, &upMaxTerms, sizeof(int)) == FRET_N) {
        mc_conf_print_err("extbug_update_max");
        return  FRET_Z;
    }

    upMaxTerms = ((upMaxTerms > MAX_UP_TERMS) ? MAX_UP_TERMS : upMaxTerms) + 1;

    if (mc_conf_read(
        "extbug_sem_key_file", CONF_STR, path_string, PATH_LEN) == FRET_N) {
        mc_conf_print_err("extbug_sem_key_file");
        return  FRET_Z;
    }

    if (!(ebSemControl = msem_create(path_string, nExbugPthead, PROJ_PTH_CTL))) {
        extbug_perror("exbug_read_config - msem_create", errno);
        return  FRET_Z;
    }

    if (mgc_add(exbGarCol, ebSemControl, (gcfun)msem_destroy) == MGC_FAILED)
        extbug_perror("exbug_read_config - mgc_add - sem", errno);

    return  FRET_P;
}


/*-----exbug_final_init-----*/
static void exbug_final_init(void)
{
    if (mgc_add(exbGarCol, NULL_POINT, (gcfun)exbug_database_close) == MGC_FAILED)
        extbug_perror("exbug_final_init - mgc_add - dbclose", errno);
}


/*-----exbug_up_init-----*/
static int exbug_up_init(void)
{
    if (exbug_module_database_init() == FUN_RUN_FAIL)
        return  FUN_RUN_FAIL;

    if ((extSaveBuf = buff_stru_init(SQL_HHCOM_LEN)) == NULL) {
        elog_write("exbug_up_init - buff_stru_init", FUNCTION_STR, ERROR_STR);
        return  FUN_RUN_FAIL;
    }

    if (mgc_add(exbGarCol, extSaveBuf, buff_stru_free_all) == MGC_FAILED)
        elog_write("exbug_up_init - mgc_add - extSaveBuf", FUNCTION_STR, ERROR_STR);

    return  FUN_RUN_OK;
}


/*------------------------------------------
    Part Six: Dictionary control

    1. exbug_dictionary_load
    2. exbug_findex_load
    3. exbug_index_load
    4. exbug_terms_load

--------------------------------------------*/

/*-----exbug_dictionary_load-----*/
int exbug_dictionary_load(
    char *fConf, char *savConf, char *fName, CLISTS *pTerm, CLISTS *charHead)
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

    for(nCnt = 0; nCnt < WORD_LEN_LIMIT; nCnt++) {
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
static int exbug_findex_load(char *finPath)
{
    char   *fiPoint, *pMov;
    int    *inBuf = fileIndexBuf;

    if (read_all_file(&fiPoint, finPath, 0) == FUN_RUN_FAIL)
        return  FUN_RUN_FAIL;

    for(pMov = fiPoint; *pMov; pMov++, inBuf++) {
        *inBuf = atoi(pMov);
        
        if ((pMov = strchr(pMov, '\n')) == NULL)
            break;
    }

    free(fiPoint);

    return  FUN_RUN_OK;
}


/*-----exbug_index_load-----*/
static int exbug_index_load(WHEAD **cStru, char *iName, int nTerms)
{
    WHEAD   *cNext;
    char    *iStore, *iMov;

    if ((cNext = *cStru = mmdp_malloc(procMemPool, ((nTerms + 1) * sizeof(WHEAD)))) == NULL) {
        extbug_perror("exbug_index_load - mmdp_malloc", errno);
        return  FUN_RUN_END;
    }

    if (read_all_file(&iStore, iName, 0) == FUN_RUN_FAIL)
        return  FUN_RUN_END;

    for(iMov = iStore; *iMov; iMov++, cNext++) {
        strncpy(cNext->dc_utf8, iMov, UTF8_WORD_LEN);
        iMov += (UTF8_WORD_LEN + 1);

        cNext->dc_off = atoi(iMov);

        if ((iMov = strchr(iMov, '\t')) == NULL) {
            extbug_perror("exbug_index_load - strchr - one", errno);
            return  FUN_RUN_END;
        }

        cNext->dc_cnt = atoi(++iMov);

        if ((iMov = strchr(iMov, '\n')) == NULL) {
            extbug_perror("exbug_index_load - strchr - two", errno);
            return  FUN_RUN_END;
        }
    }

    free(iStore);

    cNext->dc_cnt = cNext->dc_off = -1;

    return  FUN_RUN_OK;
}


/*-----exbug_terms_load-----*/
static int exbug_terms_load(WDCB **termStru, char *termFile, int nOff)
{
    if ((*termStru = mmdp_malloc(procMemPool, sizeof(WDCB))) == NULL) {
        extbug_perror("exbug_terms_load - mmdp_malloc", errno);
        return  FUN_RUN_END;
    }

    memset(*termStru, 0, sizeof(WDCB));

    if (read_all_file(&((*termStru)->wb_lterms), termFile, 0) == FUN_RUN_FAIL)
        return  FUN_RUN_END;

    if (mgc_add(exbGarCol, ((*termStru)->wb_lterms), free) != FUN_RUN_OK)
        printf("Extbug---> exbug_terms_load - mgc_add failed\n");

    if (exbRunSet.emod_exinit) {
        if (!exbRunSet.emod_exinit(*termStru, nOff))
            return  FUN_RUN_END;
    }

    return  FUN_RUN_OK;
}


/*------------------------------------------
    Part Seven: Exbug entrance

    1. exbug_keyword_job
    2. exbug_create_pthread
    3. exbug_memory_malloc

--------------------------------------------*/

/*----exbug_keyword_job-----*/
static void exbug_keyword_job(void)
{
    MSLRES  *newsRes;
    MSLROW  newsRow;
    NCONT   *pContent;

    while (FUN_RUN_OK) {
        if (!(newsRes = exbug_content_download())) {
            sleep(TAKE_A_EYECLOSE);
            continue;
        }

        mgc_one_add(&extResCol, newsRes);

        while ((newsRow = mysql_fetch_row(newsRes))) {
            msem_wait(ebSemControl);

            pContent = exbug_memory_malloc(sizeof(NCONT));

            pContent->nc_ind = (char *)newsRow[0];
            pContent->nc_cont = (char *)newsRow[1];

            exbug_create_pthread(pContent);
        }

        while (!mato_sub_and_test(&freeCtlLock, 0))
            ;   /* nothing */

        mgc_one_clean(&extResCol);
        mmdp_reset_default(threadMemPool);
        exbug_data_sync();

        //sp_stop();
    }
}


/*-----exbug_create_pthread-----*/
static void exbug_create_pthread(NCONT *pPara)
{
    pth_t   nPth;

    exbug_rewind_exmark(pPara->nc_ind, exbRunSet.emod_maname);
    mato_inc(&freeCtlLock);

    if (pthread_create(&nPth, NULL, exbug_pthread_entrance, (void *)pPara)) {
        elog_write("exbug_create_pthread - pthread_create", FUNCTION_STR, ERROR_STR);
        exbug_sig_error(PROC_ERROR);
    }
}


/*-----exbug_memory_malloc-----*/
static void *exbug_memory_malloc(int nSize)
{
    void    *pMalloc;

    if (!(pMalloc = mmdp_malloc(threadMemPool, nSize))) {
        elog_write("exbug_memory_malloc - mmdp_malloc", "threadMemPool", "failed");
        exbug_sig_error(PROC_ERROR);
    }

    return  pMalloc;
}


/*------------------------------------------
    Part Eight: Message part

    1. exbug_time_change
    2. exbug_keep_working

--------------------------------------------*/

/*-----exbug_time_change-----*/
void exbug_time_change(void)
{
    while (!mato_sub_and_test(&pthreadCtlLock, 0))
        ;   /* nothing */

    if (exbRunSet.emod_fwrt)
        exbRunSet.emod_fwrt();

    exbug_wait_arouse(sp_msg_frame_fd(extbugMsgSet), TAKE_A_REST);
    exbug_timbuf_init(NULL);

    if (exbRunSet.emod_tchange)
        exbRunSet.emod_tchange();

    return;
}


/*-----exbug_keep_working-----*/
void exbug_keep_working(void *pResult)
{
    if (!mysql_num_rows((MSLRES *)pResult))
        sleep(TAKE_A_EYECLOSE);
}


/*------------------------------------------
    Part Nine: Misc part

    1. exbug_data_sync

--------------------------------------------*/

/*-----exbug_data_sync-----*/
void exbug_data_sync(void)
{
    while (!mato_dec_and_test(&dicDbLock))
        mato_inc(&dicDbLock);

    if (buff_check_exist(extSaveBuf)) {
        if (!buff_stru_empty(extSaveBuf)) {
            if (mysql_real_query(&dbKeysHandler, buff_place_start(extSaveBuf), buff_now_size(extSaveBuf)))
                elog_write("exbug_data_sync", FUNCTION_STR, (char *)mysql_error(&dbKeysHandler));

            buff_stru_make_empty(extSaveBuf);
        }
    }

    mato_inc(&dicDbLock);
}


/*------------------------------------------
    Part Ten: Extbug work

    1. exbug_pthread_entrance

--------------------------------------------*/

/*-----exbug_pthread_entrance-----*/
static void *exbug_pthread_entrance(void *pPara)
{
    WDCT    wCnt;

    mato_inc(&pthreadCtlLock);

    pthread_detach(pthread_self());

    exbug_wordstru_setting(&wCnt);
    exbug_segment_entrance(&wCnt, ((NCONT *)pPara)->nc_cont);

    if (exbRunSet.emod_rmode) {
        exbRunSet.emod_rmode(&wCnt);

        if (exbRunSet.emod_up)
            exbRunSet.emod_up(&wCnt, ((NCONT *)pPara)->nc_ind);
    }

    mato_dec(&pthreadCtlLock);
    mato_dec(&freeCtlLock);
    msem_wake(ebSemControl);

    return  NULL;
}
