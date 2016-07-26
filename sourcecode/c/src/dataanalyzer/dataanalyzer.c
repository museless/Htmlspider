/*---------------------------------------------
 *     modification time: 2016-05-16 19:00:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

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
 *      Part Seven: Exbug entrance
 *      Part Eight: Misc part
 *      Part Nine:  Extbug work
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


/*---------------------------------------------
 *          Part Two: Local function
-*---------------------------------------------*/

/* Part Four */
static  void    exbug_option_analyst(int nPara, char **paraList);
static  void    exbug_access(void);

/* Part Five */
static  int     mainly_init(void);
static  void    exbug_data_init(void);
static  void    exbug_tblname_set(char *time_string);
static  int     exbug_mempool_init(void);
static  int     exbug_read_config(void); 
static  void    exbug_work_prepare(void);

static  int     exbug_work_setting(void);

/* Part Seven */
static  void    exbug_keyword_job(void);
static  void    exbug_create_pthread(NCONT *pPara);
static  void   *exbug_memory_malloc(int nSize);

/* Part Ten */
static  void    *exbug_pthread_entrance(void *pPara);


/*---------------------------------------------
 *            Part Three: Define
-*---------------------------------------------*/

#define exbug_runset_init(exbent, minit, exinit, \
            mwrt, mfwrt, ctab, runmode, upfun, mname) { \
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


/*---------------------------------------------
 *          Part Four: Extbug main
 *
 *          1. main
 *          2. exbug_option_analyst
 *          3. exbug_access 
 *
-*---------------------------------------------*/

/*-----main-----*/
int main(int argc, char *argv[])
{
    exbug_option_analyst(argc, argv);
    exbug_access();

    mgc_all_clean(&exbGarCol);

    return  FUN_RUN_OK;
}


/*-----exbug_option_analyst-----*/
void exbug_option_analyst(int nPara, char **paraList)
{
    int     conf_flags = 0, time_flags = 0;
    char    ch;

    while ((ch = getopt(nPara, paraList, "ht:c:")) != FRET_N) {
        switch (ch) {
            case 'h':
                exbug_print_help();
                mc_conf_unload();
                exit(FUN_RUN_OK);
            
            case 'c':
                mc_load_config("Extbug", optarg);
                conf_flags = 1;
                break;

            case 't':
                exbug_tblname_set(optarg);
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
        exbug_tblname_set(NULL);
}


/*-----exbug_access-----*/
void exbug_access(void)
{
    if (mainly_init() == FRET_Z) 
        return;

    if (exbug_database_init() == FRET_Z || exbug_mempool_init() == FRET_Z)
        return;

    if (exbug_dictionary_load("extbug_noun_findex_path", "extbug_noun_path",
            "noun", &charTermList, &charHeadSave) == FUN_RUN_OK) {
        if (exbRunSet.emod_init && exbRunSet.emod_init() == FUN_RUN_OK) {
            exbug_work_prepare();
            exbRunSet.emod_entry();
        }
    }
}


/*---------------------------------------------
 *          Part Five: Initialization
 *
 *          1. mainly_init
 *          2. exbug_data_init
 *          3. exbug_tblname_set
 *          4. exbug_mempool_init
 *          5. exbug_read_config
 *          6. exbug_work_prepare
 *          7. exbug_work_setting
 *
-*---------------------------------------------*/

/*-----mainly_init-----*/
static int mainly_init(void)
{
    exbug_runset_init(exbug_keyword_job, exbug_work_setting, 
        NULL, NULL, NULL, exbug_create_keyword_table, 
        exbug_extract_keyword, exbug_update_terms, MASK_EXT);

    if (!sp_normal_init("Extbug", &exbGarCol, 
        (MSGSET **)&extbugMsgSet, 
        exbug_msg_init, "extbug_err_locate", exbugIpcFd))
        return  FUN_RUN_END;

    /* atomic type parameter init */
    mato_init(pthreadCtlLock, 0);
    mato_init(freeCtlLock, 0);
    mato_init(nPaperLock, 1);

    if (exbug_read_config() == FRET_Z)
        return  FRET_Z;

    exbug_data_init();

    sprintf(sqlSeleCom, GET_NEWS_CONT, tblNewsName, exbRunSet.emod_maname, nExbugPthead);

    return  exbug_signal_init();
}


/*-----exbug_data_init-----*/
void exbug_data_init(void)
{
    memset(&charTermList, 0, sizeof(CLISTS));
    memset(&charHeadSave, 0, sizeof(CLISTS));
}


/*-----exbug_tblname_set-----*/
void exbug_tblname_set(char *time_string)
{
    TMS    *extTime = time_str_extract(time_string);

    sprintf(tblNewsName, "N%d%02d%02d", 
        extTime->tm_year, extTime->tm_mon, extTime->tm_mday);

    sprintf(tblKeysName, "K%d%02d%02d",
        extTime->tm_year, extTime->tm_mon, extTime->tm_mday);
}


/*-----exbug_mempool_init-----*/
static int exbug_mempool_init(void)
{
    if ((threadMemPool = mmdp_create(upMaxTerms * PER_WORD_MAX)) == NULL) {
        exbug_perror("exbug_mempool_init - mmdp_create thread", errno);
        return  FUN_RUN_END;    
    }

    if (!mgc_add(&exbGarCol, threadMemPool, (gcfun)mmdp_free_all))
        exbug_perror("exbug_mempool_init - mgc_add - threadMemPool", errno);

    if ((procMemPool = mmdp_create(PROC_MP_SIZE)) == NULL) {
        exbug_perror("exbug_mempool_init - mmdp_create proc", errno);
        return  FUN_RUN_END;    
    }

    if (!mgc_add(&exbGarCol, procMemPool, (gcfun)mmdp_free_all))
        exbug_perror("exbug_mempool_init - mgc_add - procMemPool", errno);

    return  FUN_RUN_OK;
}


/*-----exbug_read_config-----*/
int exbug_read_config(void)
{
    char        path_string[PATH_LEN];
    PerConfData read[] = {
        {"extbug_pthread_num", CONF_NUM, &nExbugPthead, sizeof(int), "0"},
        {"extbug_update_max", CONF_NUM, &upMaxTerms, sizeof(int), "0"},
        {"extbug_sem_key_file", CONF_STR, path_string, PATH_LEN, ""},
        {"keyword_buff_size", CONF_NUM, &keywordListSize, sizeof(int), "2048"},
    };

    mc_conf_read_list(read, sizeof(read) / sizeof(read[0]));

    if (nExbugPthead == 0 || upMaxTerms == 0 || path_string[0] == 0) {
        printf("[extbug_pthread_num] | [extbug_update_max] | "
               "[extbug_sem_key_file] must set\n");
        return  FRET_Z;
    }

    upMaxTerms = ((upMaxTerms > MAX_UP_TERMS) ? MAX_UP_TERMS : upMaxTerms) + 1;

    if (!(ebSemControl = msem_create(path_string, nExbugPthead, PROJ_PTH_CTL))) {
        exbug_perror("exbug_read_config - msem_create", errno);
        return  FRET_Z;
    }
    
    if (!mgc_add(&exbGarCol, ebSemControl, (gcfun)msem_destroy))
        exbug_perror("exbug_read_config - mgc_add - sem", errno);

    return  FRET_P;
}


/*-----exbug_work_prepare-----*/
void exbug_work_prepare(void)
{
    if (!mgc_add(&exbGarCol, GC_DEFOBJ, (gcfun)exbug_database_close))
        exbug_perror("exbug_work_prepare - mgc_add - dbclose", errno);
}


/*-----exbug_work_setting-----*/
int exbug_work_setting(void)
{
    if (exbug_module_database_init() == FUN_RUN_FAIL)
        return  FUN_RUN_FAIL;

    if ((extSaveBuf = buff_stru_init(SQL_HHCOM_LEN)) == NULL) {
        elog_write(
        "exbug_work_setting - buff_stru_init", FUNCTION_STR, ERROR_STR);

        return  FUN_RUN_FAIL;
    }

    if (!mgc_add(&exbGarCol, extSaveBuf, buff_stru_free_all))
        elog_write("exbug_work_setting - mgc_add - extSaveBuf", 
            FUNCTION_STR, ERROR_STR);

    return  FUN_RUN_OK;
}


/*---------------------------------------------
 *          Part Seven: Exbug entrance
 *
 *          1. exbug_keyword_job
 *          2. exbug_create_pthread
 *          3. exbug_memory_malloc
 *
-*---------------------------------------------*/

/*----exbug_keyword_job-----*/
void exbug_keyword_job(void)
{
    MSLRES *newsRes;
    NCONT  *pContent;
    MSLROW  news_data;

    while (FRET_P) {
        if (!(newsRes = exbug_content_download())) {
            sleep(TAKE_A_EYECLOSE);
            continue;
        }

        while ((news_data = mysql_fetch_row(newsRes))) {
            msem_wait(ebSemControl);

            pContent = exbug_memory_malloc(sizeof(NCONT));

            pContent->nc_ind = (char *)news_data[0];
            pContent->nc_cont = (char *)news_data[1];

            exbug_create_pthread(pContent);
        }

        while (!mato_sub_and_test(freeCtlLock, 0))
            ;   /* nothing */

        mysql_free_result(newsRes);
        mmdp_reset_default(threadMemPool);
        exbug_data_sync();
    }
}


/*-----exbug_create_pthread-----*/
void exbug_create_pthread(NCONT *pPara)
{
    pth_t   thread_id;

    exbug_rewind_exmark(pPara->nc_ind, exbRunSet.emod_maname);
    mato_inc(freeCtlLock);

    if (pthread_create(&thread_id, NULL, exbug_pthread_entrance, (void *)pPara)) {
        elog_write("exbug_create_pthread - pthread_create", 
                FUNCTION_STR, ERROR_STR);
        exbug_sig_error(PROC_ERROR);
    }
}


/*-----exbug_memory_malloc-----*/
void *exbug_memory_malloc(int nSize)
{
    void   *addr;

    if (!(addr = mmdp_malloc(threadMemPool, nSize))) {
        elog_write("exbug_memory_malloc - mmdp_malloc", "threadMemPool", "failed");
        exbug_sig_error(PROC_ERROR);
    }

    return  addr;
}


/*---------------------------------------------
 *          Part Ten: Extbug work
 *
 *          1. exbug_pthread_entrance
 *
-*---------------------------------------------*/

/*-----exbug_pthread_entrance-----*/
void *exbug_pthread_entrance(void *pPara)
{
    WDCT    wCnt;

    mato_inc(pthreadCtlLock);

    pthread_detach(pthread_self());

    exbug_wordstru_setting(&wCnt);
    exbug_segment_entrance(&wCnt, ((NCONT *)pPara)->nc_cont);

    if (exbRunSet.emod_rmode) {
        exbRunSet.emod_rmode(&wCnt);

        if (exbRunSet.emod_up)
            exbRunSet.emod_up(&wCnt, ((NCONT *)pPara)->nc_ind);
    }

    mato_dec(pthreadCtlLock);
    mato_dec(freeCtlLock);
    msem_wake(ebSemControl);

    return  NULL;
}
