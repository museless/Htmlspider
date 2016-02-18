/*---------------------------------------------
 *      Source file content Five part
 *
 *      Part Zero:  Include 
 *      Part One:   Define
 *      Part Two:   Local data
 *      Part Three: Local function
 *
 *      Part Four:  Extbug message frame
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part One: Include
-*---------------------------------------------*/

#include "sp.h"
#include "spextb.h"


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/

static  void    exbug_msg_ctime(void);
static  void    exbug_msg_kwork(void *kpPara);
static  int     exbug_msg_send(int sendFd);


/*---------------------------------------------
 *      Part Four: Extbug message frame
 *
 *      1. exbug_msg_init
 *      2. exbug_msg_ctime
 *      3. exbug_msg_kwork
 *      4. exbug_msg_send
 *
-*---------------------------------------------*/

/*-----exbug_msg_init-----*/
void *exbug_msg_init(int message_fd)
{
    return  sp_msg_frame_init(ebNameBuf, PART_EXTBUG,
                message_fd, TAKE_A_SHNAP, exbug_msg_ctime, 
                exbug_msg_kwork, exbug_msg_send);
}


/*-----exbug_msg_ctime-----*/
void exbug_msg_ctime(void)
{
    /* nothing */
}


/*-----exbug_msg_kwork-----*/
void exbug_msg_kwork(void *pPara)
{
    /* nothing */
}


/*-----exbug_msg_send-----*/
int exbug_msg_send(int sendFd)
{
    return  FUN_RUN_OK;
}


/*----------------------------------------------
 *      Part Two: Extbug message assist
 *
 *      1. exbug_wait_arouse
 *
-*---------------------------------------------*/

/*-----exbug_wait_arouse-----*/
void exbug_wait_arouse(int waitFd, int wTime)
{
    PMSG    waitMsg;
    int     nRet;

    while(FUN_RUN_OK) {
        if((nRet = sp_msg_select_read(waitFd, &waitMsg, wTime)) > FUN_RUN_END) {
            if(sp_msg_exam_recver(&waitMsg, PART_EXTBUG, tbNameBuf) == FUN_RUN_OK) {
                if(sp_msg_exam_command(&waitMsg, KEEP_WORKING))
                    break;
            }
        }
    }
}
