/*------------------------------------------
	Source file content Thirteen part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:	Msg function
    Part Five:  Msg pluging

--------------------------------------------*/

/*------------------------------------------
	        Part Zero: Include
--------------------------------------------*/

#include "sp.h"

#include "mpctl.h"

#include "spurlb.h"
#include "spuglobal.h"


/*------------------------------------------
	     Part Two: Local function
--------------------------------------------*/

/* Part Four */
static	void	ubug_msg_ctime(void);
static	int	    ubug_msg_send(int sendFd);

/* Part Five */
static  void    ubug_time_change(void);
static  int     ubug_send_message(int msgFd);


/*------------------------------------------
           Part Four: Msg function

           1. ubug_msg_init
           2. ubug_msg_ctime
           3. ubug_msg_send

--------------------------------------------*/

/*-----ubug_msg_init-----*/
void *ubug_msg_init(int msgFd)
{
	return

    sp_msg_frame_init(
    "URLBUG", PART_URLBUG, msgFd,
	TAKE_A_SHNAP, ubug_msg_ctime, NULL, ubug_msg_send);
}


/*-----ubug_msg_ctime-----*/
static void ubug_msg_ctime(void)
{
	ubug_time_change();
}
        

/*-----ubug_msg_send-----*/
static int ubug_msg_send(int sendFd)
{
	return	ubug_send_message(sendFd);
}


/*------------------------------------------
        Part Five: Msg pluging

	    1. ubug_time_change
	    2. ubug_send_message

--------------------------------------------*/

/*-----ubug_time_change-----*/
static void ubug_time_change(void)
{
	mpc_thread_wait(ubugThreadPool);

	urlRunSet.ubs_fstf();
	ubug_init_datebuf(NULL);
	ubug_create_dbtable();
}


/*-----ubug_send_message-----*/
static int ubug_send_message(int msgFd)
{
	PMSG	sendMsg;

	if (urlCatchNum) {
		memset(&sendMsg, 0, MSG_LEN);
		sp_msg_fill_stru(&sendMsg, PART_TEXTBUG, KEEP_WORKING);

		if (sp_msg_write(msgFd, &sendMsg) == FUN_RUN_FAIL) {
			elog_write("ubug_send_message - sp_msg_write", FUNCTION_STR, ERROR_STR);
			return	FUN_RUN_FAIL;
		}
	}

	return	FUN_RUN_OK;
}

