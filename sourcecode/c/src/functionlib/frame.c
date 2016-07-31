/*--------------------------------------------
 *      Source file content Five part
 *
 *      Part Zero:  Include
 *      Part One:   Define
 *      Part Two:   Local data
 *      Part Three: Local function
 *
 *      Part Four:  Frame access 
 *
-*--------------------------------------------*/

/*--------------------------------------------
 *            Part Zero: Include
-*--------------------------------------------*/

#include "spinc.h"


/*--------------------------------------------
 *           Part Two: Local data
-*--------------------------------------------*/

static  char    ownNameSave[OWN_NAME_LEN];


/*--------------------------------------------
 *         Part Four: Frame access
 *
 *         1. sp_normal_init
 *
-*--------------------------------------------*/

/*-----sp_normal_init-----*/
int32_t sp_normal_init(const char *owner, Gc *gc)
{
    snprintf(ownNameSave, OWN_NAME_LEN - 1, "%s", owner);

    if (!mgc_init(gc)) {
        perror("sp_normal_init - mgc_init");
        exit(FUN_RUN_END);
    }

    if (!mgc_add(gc, GC_DEFOBJ, (gcfun)mc_conf_unload))
        perror("sp_normal_init - mgc_add - mc_conf_unload");

    return  FUN_RUN_OK;
}


