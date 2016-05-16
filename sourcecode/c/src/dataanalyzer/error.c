#include "sp.h"

#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *          Part One: Error dealing
 *
 *          1. exbug_sig_error
 *          2. exbug_perror
 *
-*---------------------------------------------*/

/*-----exbug_sig_error-----*/
void exbug_sig_error(int nObj)
{
    if (nObj == PTHREAD_ERROR)
        mato_dec(&pthreadCtlLock);
    
    kill(getpid(), SIGINT);
}


/*-----exbug_perror-----*/
void exbug_perror(const char *string_head, int error_num)
{
    printf("Extbug---> %s - %s\n", string_head, strerror(error_num));
}

