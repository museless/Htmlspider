/*---------------------------------------------
 *     modification time: 2016-07-24 08:00:45
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: threads.h 
 *     creation time: 2016-07-11 12:57:45
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Typedef
 *       Part Three: Struct
 *
 *       Part Four:  Function
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#pragma once


/*---------------------------------------------
 *            Part Two: Typedef
-*---------------------------------------------*/

typedef pthread_mutex_t     mutex_t;
typedef pthread_cond_t      cond_t;
typedef pthread_barrier_t   barrier_t;
typedef pthread_t           thread_t;

typedef void                (*throutine)(void *);

typedef struct threadpool   Threads;
typedef struct threadentity Pthent;


/*---------------------------------------------
 *            Part Three: Struct
-*---------------------------------------------*/

struct threadpool {
    Pthent     *threads;
    Pthent     *freelist;

    mutex_t     free_lock;
    cond_t      free_cond;

    int32_t     cnt;
    barrier_t   barrier;
};

struct threadentity {
    thread_t    tid;

    Threads    *pool;
    Pthent     *next;

    throutine   routine;
    void       *params;

    barrier_t  *barrier;
    mutex_t     mutex;
    cond_t      cond;

    int32_t     flags;
};


/*---------------------------------------------
 *            Part Four: Function
-*---------------------------------------------*/

bool    mpc_create(Threads *pool, int numbers);
bool    mpc_thread_wake(Threads *pool, throutine func, void *params);
bool    mpc_thread_trywake(Threads *pool, throutine func, void *params);
bool    mpc_thread_wait(Threads *pool);
bool    mpc_destroy(Threads *pool);


