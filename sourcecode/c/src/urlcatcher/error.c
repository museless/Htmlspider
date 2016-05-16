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
 *       Part Four:  Urlbug error dispose
 *
 *---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"


/*---------------------------------------------
 *      Part Four: Urlbug error dispose
 *
 *      1. ubug_sig_error
 *      2. ubug_perror
 *
-*---------------------------------------------*/

/*-----ubug_sig_error-----*/
void ubug_sig_error(void)
{
    kill(getpid(), SIGINT);
}


/*-----ubug_perror-----*/
void ubug_perror(char *errStr, int nErr)
{
    printf("Urlbug---> %s: %s\n", errStr, strerror(nErr));
}
