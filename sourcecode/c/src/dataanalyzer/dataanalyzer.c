/*---------------------------------------------
 *     modification time: 2016-07-31 11:05:00
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
static  bool    exbug_prepare(void);

/* Part Five */
static  bool    mainly_init(void);
static  void    exbug_data_init(void);
static  void    exbug_tblname_set(char *time_string);
static  bool    exbug_mempool_init(void);
static  bool    exbug_read_config(void); 

static  bool    exbug_work_setting(void);

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
 *          3. exbug_prepare 
 *
-*---------------------------------------------*/

/*-----main-----*/
int main(int argc, char *argv[])
{
    exbug_option_analyst(argc, argv);

    if (exbug_prepare())
        exbRunSet.emod_entry();

    mgc_all_clean(&objGc);

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


/*-----exbug_prepare-----*/
bool exbug_prepare(void)
{
    if (!mainly_init() || !exbug_database_init() || !exbug_mempool_init())
        return  false;

    if (exbug_dictionary_load("extbug_noun_findex_path", "extbug_noun_path",
            "noun", &charTermList, &charHeadSave) == FUN_RUN_OK) {
        if (exbRunSet.emod_init && exbRunSet.emod_init())
            return  true;
    }

    return  false;
}


/*---------------------------------------------
 *          Part Five: Initialization
 *
 *          1. mainly_init
 *          2. exbug_data_init
 *          3. exbug_tblname_set
 *          4. exbug_mempool_init
 *          5. exbug_read_config
 *          6. exbug_work_setting
 *
-*---------------------------------------------*/

/*-----mainly_init-----*/
bool mainly_init(void)
{
    exbug_runset_init(exbug_keyword_job, exbug_work_setting, 
        NULL, NULL, NULL, exbug_create_keyword_table, 
        exbug_extract_keyword, exbug_update_terms, MASK_EXT);

    if (!frame("Extbug"))
        return  false;

    /* atomic type parameter init */
    mato_init(pthreadCtlLock, 0);
    mato_init(freeCtlLock, 0);
    mato_one(nPaperLock);

    if (!exbug_read_config())
        return  false;

    exbug_data_init();

    sprintf(sqlSeleCom, GET_NEWS_CONT,
        tblNewsName, exbRunSet.emod_maname, nExbugPthead);

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
bool exbug_mempool_init(void)
{
    if (!(threadMemPool = mmdp_create(upMaxTerms * PER_WORD_MAX))) {
        setmsg(LM17);
        return  false;
    }

    if (!mgc_add(&objGc, threadMemPool, (gcfun)mmdp_free_all))
        setmsg(LM5, "thread mempool");

    if (!(procMemPool = mmdp_create(PROC_MP_SIZE))) {
        setmsg(LM17, "proc mempool");
        return  false;
    }

    if (!mgc_add(&objGc, procMemPool, (gcfun)mmdp_free_all))
        setmsg(LM5, "proc mempool");

    return  true;
}


/*-----exbug_read_config-----*/
bool exbug_read_config(void)
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
        return  false;
    }

    upMaxTerms = ((upMaxTerms > MAX_UP_TERMS) ? MAX_UP_TERMS : upMaxTerms) + 1;

    if (!(ebSemControl = msem_create(path_string, nExbugPthead, PROJ_PTH_CTL))) {
        setmsg(LM29);
        return  false;
    }
    
    if (!mgc_add(&objGc, ebSemControl, (gcfun)msem_destroy))
        setmsg(LM5, "sem");

    return  true;
}


/*-----exbug_work_setting-----*/
bool exbug_work_setting(void)
{
    if (exbug_module_database_init() == FUN_RUN_FAIL)
        return  false;

    if (!mgc_add(&objGc, GC_DEFOBJ, (gcfun)exbug_database_close))
        setmsg(LM5, "database close");

    if (!(extSaveBuf = buff_stru_init(SQL_HHCOM_LEN))) {
        setmsg(LM12);
        return  false;
    }

    if (!mgc_add(&objGc, extSaveBuf, buff_stru_free_all))
        setmsg(LM5, "extSaveBuf");

    return  true;
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

    while (true) {
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
        setmsg(LM14);
        exbug_sig_quit(PROC_ERROR);
    }
}


/*-----exbug_memory_malloc-----*/
void *exbug_memory_malloc(int nSize)
{
    void   *addr = mmdp_malloc(threadMemPool, nSize);

    if (!addr) {
        setmsg(LM15);
        exbug_sig_quit(PROC_ERROR);
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
