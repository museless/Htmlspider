/*---------------------------------------------
 *  modification time: 2016.08.08 22:30
 *  mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Five part
 *
 *      Part Zero:  Include
 *      Part One:   Local data
 *      Part Two:   Local function
 *      Part Three: Define
 *
 *      Part Four:  Signal handle
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *           Part One: Local data
-*---------------------------------------------*/

static mutex_t  sigIntLock = PTHREAD_MUTEX_INITIALIZER;
static mutex_t  sigSegLock = PTHREAD_MUTEX_INITIALIZER;


/*---------------------------------------------
 *          Part Two: Local function
-*---------------------------------------------*/

/* Part Four */
static  void    exbug_signal_handler(int nSignal);


/*---------------------------------------------
 *          Part Four: Signal handle
 *
 *          1. exbug_signal_init
 *          2. exbug_signal_handler
 *
-*----------------------------------------------*/

/*-----exbug_signal_init-----*/
bool exbug_signal_init(void)
{
    struct  sigaction   sigStru;
    sigset_t            sigMask;

    /* for signal int */
    sigemptyset(&sigMask);
    sigaddset(&sigMask, SIGINT);
    sigaddset(&sigMask, SIGSEGV);

    sigStru.sa_handler = exbug_signal_handler;
    sigStru.sa_mask = sigMask;
    sigStru.sa_flags = 0;

    if (sigaction(SIGINT, &sigStru, NULL) == FRET_N) {
        setmsg(LM9, "SIGINT");
        return  false;
    }

    /* for signal segv */
    sigdelset(&sigMask, SIGINT);
    sigStru.sa_mask = sigMask;

    if (sigaction(SIGSEGV, &sigStru, NULL) == FRET_N) {
        setmsg(LM9, "SIGSEGV");
        return  false;
    }

    return  true;
}


/*-----exbug_signal_handler-----*/
void exbug_signal_handler(int nSignal)
{
    if (nSignal == SIGINT) {
        pthread_mutex_lock(&sigIntLock);

        setmsg(LM26, "SIGINT");

        exbug_data_sync();
        mpc_destroy(&threadPool);
        mgc_all_clean(&objGc);

        printf("Extbug---> quitting...\n");
        exit(FRET_N);
    }

    if (nSignal == SIGSEGV) {
        pthread_mutex_lock(&sigSegLock);

        setmsg(LM26, "SIGSEGV");
        exbug_sig_quit();
    }
}

