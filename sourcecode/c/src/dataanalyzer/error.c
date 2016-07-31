/*---------------------------------------------
 *     modification time: 2016-07-31 11:05:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

#include "sp.h"

#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *          Part One: Error dealing
 *
 *          1. exbug_sig_quit
 *
-*---------------------------------------------*/

/*-----exbug_sig_quit-----*/
void exbug_sig_quit(int type)
{
    if (type == PTHREAD_ERROR)
        mato_dec(pthreadCtlLock);

    setmsg(LM20);
    kill(getpid(), SIGINT);
}

