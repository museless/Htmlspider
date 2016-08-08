/*---------------------------------------------
 *  modification time: 2016.08.07 20:10
 *  creator: Muse
 *  mender: Muse
 *  intro: pthread pool
-*---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Seven part
 *
 *      Part Zero:  Include
 *      Part One:   Define
 *      Part Two:   Local data
 *      Part Three: Local function
 *
 *      Part Four:  Muse thpool API
 *      Part Five:  Thread routine 
 *      Part Six:   Thread control 
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *           Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"


/*---------------------------------------------
 *              Part One: Define
-*---------------------------------------------*/

#define _join_list(pool, thread) { \
    thread->next = pool->freelist; \
    pool->freelist = thread; \
}

#define _quit_list(pool, thread) { \
    thread = pool->freelist; \
    pool->freelist = thread->next; \
    thread->next = NULL; \
}

#define _check_params(pool) { \
    if (!pool || !pool->threads) { \
        errno = EINVAL; \
        return  false; \
    } \
}


/*---------------------------------------------
 *            Part Two: Local data
-*---------------------------------------------*/

enum PTHREAD_STATE {
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
static  void   *_thread_routine(void *thread_para);
static  bool    _thread_prepare(Pthent *entity);

/* Part Six */
static  void    _time_wait(int sec, int nanosec);
static  Pthent *_empty_slot(Threads *thread_pool);
static  bool    _thread_create(Threads *pool, Pthent *thread);
static  void    _thread_cleanup(void *thread_para);

static  void    _thread_join(Pthent *th_entity);
static  bool    _thread_wait(Threads *pool, const struct timespec *abstime);


/*---------------------------------------------
 *      Part Four: Muse thpool API
 *
 *          1. mpc_create
 *          2. mpc_thread_wake
 *          3. mpc_thread_trywake
 *          4. mpc_thread_wait
 *          5. mpc_destroy
 *
-*---------------------------------------------*/

/*-----mpc_create------*/
bool mpc_create(Threads *pool, int numbers)
{
    if (!pool || numbers < 1) {
        errno = EINVAL;
        return  false;
    }

    if ((errno = pthread_barrier_init(&pool->barrier, NULL, numbers + 1)))
        return  false;

    pool->cnt = numbers;
    pool->freelist = NULL;

    if (!(pool->threads = malloc(pool->cnt * sizeof(Pthent)))) {
        mpc_destroy(pool);
        return  false;
    }

    if ((errno = pthread_mutex_init(&pool->free_lock, NULL)))
        return  false;

    if ((errno = pthread_cond_init(&pool->free_cond, NULL)))
        return  false;

    Pthent *thread = pool->threads;

    for (; thread < pool->threads + pool->cnt; thread++) {
        if (!_thread_create(pool, thread)) {
            mpc_destroy(pool);
            return  false;
        }

        _join_list(pool, thread);
    }

    pthread_barrier_wait(&pool->barrier);

    return  true;
}


/*-----mpc_thread_wake-----*/
bool mpc_thread_wake(Threads *pool, throutine func, void *params)
{
    if (!pool || !func) {
        errno = EINVAL;
        return  false;
    }

    Pthent *thread;

    if (!(thread = _empty_slot(pool)))
        return  false;

    thread->flags = PTH_IS_BUSY;
    thread->routine = func;
    thread->params = params;

    pthread_cond_signal(&thread->cond);
    pthread_mutex_unlock(&thread->mutex);

    return  true;
}


/*-----mpc_thread_trywake-----*/
bool mpc_thread_trywake(Threads *pool, throutine func, void *params)
{
    if (!pool || !func) {
        errno = EINVAL;
        return  false;
    }

    if (!pool->freelist) {
        errno = EBUSY;
        return  false;
    }

    return  mpc_thread_wake(pool, func, params);
}


/*-----mpc_thread_wait-----*/
bool mpc_thread_wait(Threads *pool)
{
    _check_params(pool);

    return  _thread_wait(pool, NULL);
}


/*-----mpc_thread_trywait-----*/
bool mpc_thread_trywait(Threads *pool, struct timespec *abstime)
{
    _check_params(pool);

    if (!abstime) {
        errno = EINVAL;
        return  false;
    }

    return  _thread_wait(pool, abstime);
}


/*-----mpc_destroy-----*/
bool mpc_destroy(Threads *pool)
{
    if (!pool) {
        errno = EINVAL;
        return  false;
    }

    if (pool->threads) {
        Pthent *th_entity = pool->threads;

        for (; th_entity < pool->threads + pool->cnt; th_entity++) {
            if (th_entity->flags != PTH_WAS_KILLED ||
                th_entity->flags != PTH_IS_UNINITED) {
                errno = pthread_mutex_trylock(&th_entity->mutex);

                if (errno == EBUSY && th_entity->flags == PTH_IS_BUSY) {
                    _time_wait(0, 1000000);
                    _thread_join(th_entity);
                    continue;
                }

                th_entity->flags = PTH_ALREADY_KILLED;
                pthread_cond_signal(&th_entity->cond);
            }
        }

        free(pool->threads);
    }

    pool->cnt = 0;

    pthread_mutex_destroy(&pool->free_lock);
    pthread_cond_destroy(&pool->free_cond);
    pthread_barrier_destroy(&pool->barrier);

    return  true;
}


/*----------------------------------------------
 *          Part Five: Thread routine
 *
 *          1. _thread_routine
 *          2. _thread_prepare
 *
**----------------------------------------------*/

/*-----_thread_routine-----*/
void *_thread_routine(void *thread_para)
{
    Pthent  *th_entity = (Pthent *)thread_para;
    Threads *pool = th_entity->pool; 

    _thread_prepare(th_entity);
    pthread_cleanup_push(_thread_cleanup, th_entity);

    while (true) {
        pthread_mutex_lock(&th_entity->mutex);

        while (th_entity->flags != PTH_IS_BUSY) {
            if (th_entity->flags == PTH_ALREADY_KILLED) 
                pthread_exit(NULL);

            pthread_cond_wait(&th_entity->cond, &th_entity->mutex);
        }

        th_entity->routine(th_entity->params);
        th_entity->flags = PTH_IS_READY;

        pthread_cond_signal(&th_entity->cond);
        pthread_mutex_unlock(&th_entity->mutex);

        pthread_mutex_lock(&pool->free_lock);
        _join_list(pool, th_entity); 
        pthread_cond_signal(&pool->free_cond);
        pthread_mutex_unlock(&pool->free_lock);
    }

    pthread_cleanup_pop(0);

    return  NULL;
}


/*-----_thread_prepare-----*/
bool _thread_prepare(Pthent *entity)
{
    if (entity->flags == PTH_IS_UNINITED) {
        if ((errno = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) ||
            (errno = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)))
            return  false;

        entity->flags = PTH_IS_READY;
        pthread_barrier_wait(entity->barrier);
    }

    return  true;
}


/*-----------------------------------------------
 *          Part Six: Thread control
 *
 *          1. _time_wait
 *          2. _empty_slot
 *          3. _thread_create
 *          4. _thread_cleanup
 *          5. _thread_join
 *          6. _thread_wait
 *
**-----------------------------------------------*/

/*-----_time_wait-----*/
void _time_wait(int sec, int nanosec)
{
    struct timespec clock;

    clock.tv_sec = sec;
    clock.tv_nsec = nanosec;
    pselect(0, NULL, NULL, NULL, &clock, NULL);
}


/*-----_empty_slot-----*/
Pthent *_empty_slot(Threads *pool)
{
    Pthent  *entity;

    pthread_mutex_lock(&pool->free_lock);

    while (!pool->freelist)
        pthread_cond_wait(&pool->free_cond, &pool->free_lock);

    _quit_list(pool, entity);
    pthread_mutex_lock(&entity->mutex);

    pthread_mutex_unlock(&pool->free_lock);

    return  entity;
}


/*-----_thread_create-----*/
bool _thread_create(Threads *pool, Pthent *thread)
{
    thread->routine = thread->params = NULL;
    thread->next = NULL; 
    thread->flags = PTH_IS_UNINITED;
    thread->barrier = &pool->barrier;
    thread->pool = pool;

    if ((errno = pthread_mutex_init(&thread->mutex, NULL)) ||
        (errno = pthread_cond_init(&thread->cond, NULL)))
        return  false;

    errno = pthread_create(&thread->tid, NULL, _thread_routine, thread);

    return  (errno) ? false : true;
}


/*-----_thread_cleanup-----*/
void _thread_cleanup(void *thread_para)
{
    Pthent  *entity = (Pthent *)thread_para;

    pthread_mutex_unlock(&entity->mutex);

    pthread_mutex_destroy(&entity->mutex);
    pthread_cond_destroy(&entity->cond);

    entity->flags = PTH_WAS_KILLED;
}


/*-----_thread_join-----*/
void _thread_join(Pthent *th_entity)
{
    pthread_cancel(th_entity->tid);
    pthread_join(th_entity->tid, NULL);
}


/*-----_thread_wait-----*/
bool _thread_wait(Threads *pool, const struct timespec *abstime)
{
    Pthent *entity = pool->threads;

    for (; entity < pool->threads + pool->cnt; entity++) {
        while (entity->flags == PTH_IS_BUSY) {
            errno = (abstime) ? 
                pthread_cond_timedwait(&entity->cond, &entity->mutex, abstime) :
                pthread_cond_wait(&entity->cond, &entity->mutex);
        }

        if (errno == ETIMEDOUT || entity->flags == PTH_IS_BUSY) {
            _time_wait(abstime->tv_sec, abstime->tv_nsec);

            if (entity->flags == PTH_IS_BUSY)
                return  false;
        }

        if (errno)
            return  false;
    }

    return  true;
}

