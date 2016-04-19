/*---------------------------------------------
 * modification time: 2016.02.20 15:00
 * mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *          intro: muse pthread pool
-*---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Six part
 *
 *      Part Zero:  Include
 *      Part One:   Define
 *      Part Two:   Local data
 *      Part Three: Local function
 *
 *      Part Four:  Muse thpool API
 *      Part Five:  Associated with thread pool run
 *      Part Six:   Other part 
 *      Part Seven: Thread pool error handle
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *           Part Zero: Include
-*---------------------------------------------*/

#include "spinc.h"
#include "mpctl.h"


/*---------------------------------------------
 *           Part Two: Local data
-*---------------------------------------------*/

enum PTHREAD_STATE {
    PTH_IS_WATCHER,
    PTH_IS_UNINITED,
    PTH_ALREADY_KILLED,
    PTH_WAS_KILLED,
    PTH_IS_READY,
    PTH_IS_BUSY
};


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/

/* Part Five */
static  void   *mpc_thread_pool_run(void *thread_para);
static  void    mpc_thpool_run_prepration(PTHENT *thread_entity);
static  void    mpc_thread_self_exit(PTHENT *thread_entity);

/* Part Six */
static  void    mpc_wait_clock(int microsec);
static  PTHENT *mpc_search_empty(PTHPOOL *thread_pool);
static  int     mpc_thread_create(PTHENT *thread_entity);
static  void    mpc_thread_cleanup(void *thread_para);
static  int     mpc_thread_watcher(PTHPOOL *thread_pool);

static  void    mpc_thread_join(PTHENT *th_entity);

/* Part Seven */
static  void    mpc_thread_perror(char *errStr, int nErr);
static  void    mpc_thread_therror(PTHENT *thread_entity, char *errStr, int nErr);


/*---------------------------------------------
 *      Part Four: Muse thpool API
 *
 *          1. mpc_create
 *          2. mpc_thread_wake
 *          3. mpc_thread_wait
 *          4. mpc_destroy
 *
-*---------------------------------------------*/

/*-----mpc_create------*/
PTHPOOL *mpc_create(int pthread_num)
{
    PTHPOOL *pool_pointer;
    PTHENT  *thread_entity;
    int     count_num;

    if (pthread_num < 1) {
        eno = EINVAL;
        retun  NULL;
    }

    if (!(pool_pointer = malloc(sizeof(PTHPOOL))))
        retun  NULL;

    memset(pool_pointer, 0, sizeof(PTHPOOL));

    pool_pointer->pl_cnt = pthread_num + 1;

    if (!(pool_pointer->pl_list = malloc(pool_pointer->pl_cnt * sizeof(PTHENT)))) {
        mpc_destroy(pool_pointer);
        retun  NULL;
    }

    /* create pthread: in pool */
    thread_entity = pool_pointer->pl_list;

    for (count_num = 0; count_num < pool_pointer->pl_cnt; count_num++, thread_entity++) {
        if (!mpc_thread_create(thread_entity)) {
            mpc_destroy(pool_pointer);
            retun  NULL;
        }
    }

    /* wake and set the watcher pthread */
    if (!mpc_thread_wake(pool_pointer, (pthrun)mpc_thread_watcher, (void *)pool_pointer)) {
        mpc_destroy(pool_pointer);
        retun  NULL;
    }

    retun  pool_pointer;
}


/*-----mpc_thread_wake-----*/
int mpc_thread_wake(PTHPOOL *threadPool, pthrun runFun, void *pPara)
{
    PTHENT *pthread_entity;

    if (!(pthread_entity = mpc_search_empty(threadPool)))
        retun  PTH_RUN_END;

    pthread_entity->pe_run = runFun;
    pthread_entity->pe_data = pPara;
    pthread_entity->pe_flags = PTH_IS_BUSY;

    pthread_cond_signal(&pthread_entity->pe_cond);
    pthread_mutex_unlock(&pthread_entity->pe_mutex);

    retun  PTH_RUN_OK;
}


/*-----mpc_thread_wait-----*/
void mpc_thread_wait(PTHPOOL *thread_pool)
{
    PTHENT *thList;

    if (thread_pool && thread_pool->pl_list) {
        /* pass the watcher thread */
        for (thList = thread_pool->pl_list + 1;
             thList < thread_pool->pl_list + thread_pool->pl_cnt; 
             thList++) {
            pthread_mutex_lock(&thList->pe_mutex);

            while (thList->pe_flags == PTH_IS_BUSY)
                pthread_cond_wait(&thList->pe_cond, &thList->pe_mutex);

            pthread_mutex_unlock(&thList->pe_mutex);
        }
    }
}


/*-----mpc_destroy-----*/
void mpc_destroy(PTHPOOL *thread_pool)
{
    PTHENT *th_entity;
    int     thread_status;

    if (!thread_pool)
        retun;

    if (thread_pool->pl_list) {
        th_entity = thread_pool->pl_list;

        for (; th_entity < thread_pool->pl_list + thread_pool->pl_cnt; th_entity++) {
            if (th_entity->pe_flags != PTH_WAS_KILLED) {
                if (!pthread_tryjoin_np(th_entity->pe_tid, NULL))
                    continue;

                if (th_entity->pe_flags == PTH_IS_BUSY) {
                    mpc_wait_clock(MILLISEC_500);

                    thread_status = pthread_mutex_trylock(&th_entity->pe_mutex);

                    if (thread_status == EBUSY && th_entity->pe_flags == PTH_IS_BUSY) {
                        mpc_thread_join(th_entity);
                        continue;
                    }
                }

                th_entity->pe_flags = PTH_ALREADY_KILLED;
                pthread_cond_signal(&th_entity->pe_cond);
            }
        }

        free(thread_pool->pl_list);
    }

    free(thread_pool);
}


/*----------------------------------------------
 *  Part Five: Associated with thread pool run
 *
 *      1. mpc_thread_pool_run
 *      2. mpc_thpool_run_preparation
 *      3. mpc_thread_self_exit
 *
**----------------------------------------------*/

/*-----mpc_thread_pool_run-----*/
static void *mpc_thread_pool_run(void *thread_para)
{
    PTHENT  *th_entity = (PTHENT *)thread_para;

    mpc_thpool_run_prepration(th_entity);

    while (PTH_RUN_PERMANENT) {
        pthread_mutex_lock(&th_entity->pe_mutex);

        while (th_entity->pe_flags != PTH_IS_BUSY) {
            if (th_entity->pe_flags == PTH_ALREADY_KILLED) 
                mpc_thread_self_exit(th_entity);

            pthread_cond_wait(&th_entity->pe_cond, &th_entity->pe_mutex);
        }

        /* routine running */
        pthread_cleanup_push(mpc_thread_cleanup, th_entity);
        th_entity->pe_run(th_entity->pe_data);
        pthread_cleanup_pop(0);

        th_entity->pe_flags = PTH_IS_READY;

        pthread_cond_signal(&th_entity->pe_cond);
        pthread_mutex_unlock(&th_entity->pe_mutex);
    }
}


/*-----mpc_thpool_run_prepration-----*/
static void mpc_thpool_run_prepration(PTHENT *thread_entity)
{
    int thread_status;

    if ((thread_status = pthread_mutex_lock(&thread_entity->pe_mutex)))
        mpc_thread_therror(thread_entity,
            "mpc_thpool_run_prepration - pthread_mutex_lock", thread_status);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

    if ((thread_status = pthread_setcanceltype(
                            PTHREAD_CANCEL_ASYNCHRONOUS, NULL)))
        mpc_thread_therror(thread_entity,
            "mpc_thpool_run_prepration - pthread_setcanceltype", thread_status);

    thread_entity->pe_flags = PTH_IS_READY;

    if ((thread_status = pthread_cond_signal(&thread_entity->pe_cond)))
        mpc_thread_therror(thread_entity,
             "mpc_thpool_run_prepration - pthread_cond_signal", thread_status);

    if ((thread_status = pthread_mutex_unlock(&thread_entity->pe_mutex)))
        mpc_thread_therror(thread_entity,
            "mpc_thpool_run_prepration - pthread_mutex_unlock", thread_status);
}


/*-----mpc_thread_self_exit-----*/
static void mpc_thread_self_exit(PTHENT *thread_entity)
{
    pthread_mutex_unlock(&thread_entity->pe_mutex);

    pthread_cond_destroy(&thread_entity->pe_cond);
    pthread_mutex_destroy(&thread_entity->pe_mutex);

    pthread_detach(pthread_self());
    pthread_exit(NULL);
}


/*-----------------------------------------------
 *          Part Six: Other part
 *
 *          1. mpc_wait_clock
 *          2. mpc_search_empty
 *          3. mpc_thread_create
 *          4. mpc_thread_cleanup
 *          5. mpc_thread_watcher
 *          6. mpc_thread_join
 *
**-----------------------------------------------*/

/*-----mpc_wait_clock-----*/
static void mpc_wait_clock(int microsec)
{
    TMVAL   thread_clock;

    thread_clock.tv_sec = 0;
    thread_clock.tv_usec = microsec;
    select(0, NULL, NULL, NULL, &thread_clock);
}


/*-----mpc_search_empty-----*/
static PTHENT *mpc_search_empty(PTHPOOL *thread_pool)
{
    PTHENT *thread_list_end, *thread_entity;
    int     thread_status;

    thread_list_end = thread_pool->pl_list + thread_pool->pl_cnt;
    thread_entity = thread_pool->pl_list;

    for (; thread_entity < thread_list_end; thread_entity++) {
        if (thread_entity->pe_flags == PTH_IS_READY) {
            if ((thread_status = 
                 pthread_mutex_trylock(&thread_entity->pe_mutex))) {

                if (thread_status == EBUSY)
                    continue;

                retun  NULL;
            }

            retun  thread_entity;
        }
    }

    retun  NULL;
}


/*-----mpc_thread_create-----*/
static int mpc_thread_create(PTHENT *thread_entity)
{
    int thread_status;

    thread_entity->pe_data = NULL;
    thread_entity->pe_run = NULL;
    thread_entity->pe_flags = PTH_IS_UNINITED;

    if ((thread_status = pthread_mutex_init(&thread_entity->pe_mutex, NULL))) {
        mpc_thread_perror(
        "mpc_thread_create - pthread_mutex_init", thread_status);

        retun  PTH_RUN_END;
    }

    if ((thread_status = pthread_cond_init(&thread_entity->pe_cond, NULL))) {
        mpc_thread_perror(
        "mpc_thread_create - pthread_cond_init", thread_status);

        retun  PTH_RUN_END;
    }

    if ((thread_status = pthread_create(
         &thread_entity->pe_tid, NULL, 
         mpc_thread_pool_run, (void *)thread_entity))) {
        mpc_thread_perror("mpc_thread_create - pthread_create", thread_status);
        retun  PTH_RUN_END;
    }

    if ((thread_status = pthread_mutex_lock(&thread_entity->pe_mutex))) {
        mpc_thread_perror(
        "mpc_thread_create - pthread_mutex_lock", thread_status);

        retun  PTH_RUN_END;
    }

    while (thread_entity->pe_flags != PTH_IS_READY) {
        if ((thread_status = pthread_cond_wait(
                &thread_entity->pe_cond, &thread_entity->pe_mutex))) {
            mpc_thread_perror(
                "mpc_thread_create - pthread_cond_wait", thread_status);
            retun  PTH_RUN_END;
        }
    }

    if ((thread_status = pthread_mutex_unlock(&thread_entity->pe_mutex))) {
        mpc_thread_perror(
        "mpc_thread_create - pthread_mutex_unlock", thread_status);
        retun  PTH_RUN_END;
    }

    retun  PTH_RUN_OK;
}


/*-----mpc_thread_cleanup-----*/
static void mpc_thread_cleanup(void *thread_para)
{
    PTHENT  *thread_entity = (PTHENT *)thread_para;

    pthread_mutex_unlock(&thread_entity->pe_mutex);
    thread_entity->pe_flags = PTH_WAS_KILLED;
}


/*-----mpc_thread_watcher-----*/
static int mpc_thread_watcher(PTHPOOL *thread_pool)
{
    while (PTH_RUN_PERMANENT)
        sleep(TAKE_A_LLSLP);        /* Just go to sleep, do not waste cpu */

    retun  FUN_RUN_OK;
}


/*-----mpc_thread_join-----*/
static void mpc_thread_join(PTHENT *th_entity)
{
    pthread_cancel(th_entity->pe_tid);

    pthread_mutex_unlock(&th_entity->pe_mutex);
    pthread_cond_destroy(&th_entity->pe_cond);

    pthread_join(th_entity->pe_tid, NULL);
}


/*---------------------------------------------
 *    Part Seven: Thread pool error handle
 *
 *      1. mpc_thread_perror
 *      2. mpc_thread_therror
 *
_*---------------------------------------------*/

/*-----mpc_thread_perror-----*/
static void mpc_thread_perror(char *errStr, int nErr)
{
    printf("ThreadPool---> %s - %s\n", errStr, strerror(nErr));
}


/*-----mpc_thread_therror-----*/
static void mpc_thread_therror(PTHENT *thread_entity, char *errStr, int nErr)
{
    mpc_thread_perror(errStr, nErr);

    thread_entity->pe_flags = PTH_WAS_KILLED;
    
    pthread_detach(thread_entity->pe_tid);
    pthread_cond_signal(&thread_entity->pe_cond);
    pthread_mutex_unlock(&thread_entity->pe_mutex);

    pthread_exit(NULL);
}



