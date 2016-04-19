/*---------------------------------------------
 *     modification time: 2016-04-04 21:50:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *        Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Signal operate
 *
 *---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "mpctl.h"

#include "spuglobal.h"
#include "spurlb.h"


/* local data */
static  pmut_t  sigIntLock = PTHREAD_MUTEX_INITIALIZER;


/*---------------------------------------------
 *         Part Four: Signal operate
 *
 *         1. ubug_init_signal
 *         2. ubug_signal_handler
 *
-*---------------------------------------------*/

/*-----ubug_init_signal-----*/
void ubug_init_signal(void)
{
    struct sigaction    sigStru;
    sigset_t            sigMask;

    /* for signal sigint */
    sigemptyset(&sigMask);
    sigaddset(&sigMask, SIGINT);
    sigaddset(&sigMask, SIGSEGV);

    sigStru.sa_mask = sigMask;
    sigStru.sa_handler = ubug_signal_handler;
    sigStru.sa_flags = 0;

    if(sigaction(SIGINT, &sigStru, NULL) == FUN_RUN_FAIL) {
        ubug_perror("ubug_init_signal - sigaction - SIGINT", eno);
        exit(FUN_RUN_FAIL);
    }

    /* for signal segv */
    sigdelset(&sigMask, SIGINT);
    sigStru.sa_mask = sigMask;

    if(sigaction(SIGSEGV, &sigStru, NULL) == FUN_RUN_FAIL) {
        ubug_perror("ubug_init_signal - sigaction - SIGSEGV", eno);
        exit(FUN_RUN_FAIL);
    }
}


/*-----ubug_signal_handler-----*/
void ubug_signal_handler(int nSign)
{
    if (nSign == SIGINT) {
        pthread_mutex_lock(&sigIntLock);

        printf("UrlBug---> caught SIGINT...\n");

        mpc_destroy(ubugThreadPool);

        if (urlRunSet.ubs_fstf)
            urlRunSet.ubs_fstf();

        mgc_all_clean(urlGarCol);
        
        printf("UrlBug---> quitting...\n");
        exit(FUN_RUN_FAIL);
    }

    if (nSign == SIGSEGV) {
        printf("Urlbug---> caught SIGSEGV\n");
        ubug_sig_error();
    }
}
