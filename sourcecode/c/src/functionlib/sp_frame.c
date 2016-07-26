/*------------------------------------------
	Source file content Five part

	Part Zero:	Include
	Part One:	Define
	Part Two:	Local data
	Part Three:	Local function

	Part Four:	Sp init frame
    Part Five:  Sp error print

--------------------------------------------*/

/*------------------------------------------
	Part Zero: Include
--------------------------------------------*/

#include "spinc.h"
#include "spmsg.h"
#include "spframe.h"


/*------------------------------------------
	        Part Two: Local data
--------------------------------------------*/

static	char	ownNameSave[OWN_NAME_LEN];


/*------------------------------------------
	     Part Three: Local function
--------------------------------------------*/

static  void    sp_normal_error(char *error_string);


/*------------------------------------------
	      Part Four: Sp init frame

	      1. sp_normal_init

--------------------------------------------*/

/*-----sp_normal_init-----*/
int sp_normal_init(
    char *owner_name, Gc *gc, MSGSET **msgSet, 
    msginit minitFun, char *errLoc, int msgFd)
{
	snprintf(ownNameSave, OWN_NAME_LEN - 1, "%s", owner_name);

	if (!mgc_init(gc)) {
        sp_normal_error("sp_normal_init - mgc_init");
		exit(FUN_RUN_END);
	}

	if (!mgc_add(gc, GC_DEFOBJ, (gcfun)mc_conf_unload))
        sp_normal_error("sp_normal_init - mgc_add - mc_conf_unload");

	if (msgFd != 0) {
		if (!(*msgSet = minitFun(msgFd))) {
            sp_normal_error("sp_normal_init - msg_init");
			return	FUN_RUN_END;
		}
	
		if (!mgc_add(gc, *msgSet, (gcfun)sp_msg_frame_destroy))
			sp_normal_error("sp_normal_init - mgc_add - msg_init");
	}

	/* elog init */
    if (elog_init(errLoc) == FUN_RUN_FAIL) {
		sp_normal_error("sp_normal_init - elog_init"); 
		return	FUN_RUN_END;
	}

    if (!mgc_add(gc, GC_DEFOBJ, (gcfun)elog_destroy))
        sp_normal_error("sp_normal_init - mgc_add - elog");

	return	FUN_RUN_OK;
}


/*------------------------------------------
	      Part Four: Sp error print

	      1. sp_normal_error

--------------------------------------------*/

/*-----sp_normal_error-----*/
/* should wait ownNameSave was inited */
static void sp_normal_error(char *error_string)
{
    printf("%s---> %s: %s\n", ownNameSave, error_string, strerror(errno));
}

