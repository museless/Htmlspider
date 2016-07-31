/*---------------------------------------------
 *  modification time: 2016.02.16 08:31
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

static  pmut_t  sigIntLock = PTHREAD_MUTEX_INITIALIZER;
static  pmut_t  sigSegLock = PTHREAD_MUTEX_INITIALIZER;


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

    sigStru.sa_handler = exbug_signal_handler;
    sigStru.sa_mask = sigMask;
    sigStru.sa_flags = 0;

    if (sigaction(SIGINT, &sigStru, NULL) == FUN_RUN_FAIL) {
        perror("Extbug---> exbug_signal_init - sigaction - SIGINT");
        return  false;
    }

    /* for signal segv */
    sigdelset(&sigMask, SIGINT);
    sigStru.sa_mask = sigMask;

    if (sigaction(SIGSEGV, &sigStru, NULL) == FUN_RUN_FAIL) {
        perror("otbug_init_signal - sigaction - SIGSEGV");
        return  false;
    }

    return  true;
}


/*-----exbug_signal_handler-----*/
static void exbug_signal_handler(int nSignal)
{
    int nTimes = 0;

    if (nSignal == SIGINT) {
        pthread_mutex_lock(&sigIntLock);

        setmsg(LM26, "SIGINT");
        
        while (nTimes++ < nExbugPthead && !mato_sub_and_test(pthreadCtlLock, 0))
            sleep(TAKE_A_SEC);

        exbug_data_sync();
        mgc_all_clean(&objGc);

        printf("Extbug---> quitting...\n");
        exit(FUN_RUN_FAIL);
    }

    if (nSignal == SIGSEGV) {
        pthread_mutex_lock(&sigSegLock);

        setmsg(LM26, "SIGSEGV");
        exbug_sig_quit(PTHREAD_ERROR);
    }
}
