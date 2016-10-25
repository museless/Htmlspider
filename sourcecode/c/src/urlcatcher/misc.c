/*---------------------------------------------
 *     modification time: 2016.05.16 18:15
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Six part
 *
 *       Part Zero:  Include
 *       Part One:   Local data
 *       Part Two:   Local function
 *       Part Three: Define
 *
 *       Part Four:  Help tip
 *       Part Five:  Cleaning function
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *           Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spurlb.h"
#include "spuglobal.h"


/*---------------------------------------------
 *          Part One: Local data
-*---------------------------------------------*/

static char helpTips[] =
"urlcatcher - charge in catching url\n"
"usage:\n\turlcatcher [options] parameters\n\n"
"options:\n"
"\t-c <configuration paths>    configuration's path\n"
"\t-h                          helping\n\n";



/*---------------------------------------------
 *          Part Four: Help tip
 *
 *          1. ubug_print_help
 *
-*---------------------------------------------*/

/*-----ubug_print_help-----*/
void ubug_print_help(void)
{
    printf(helpTips);
}


/*---------------------------------------------
 *       Part Five: Cleaning function
 *
 *       1. ubug_free_weblist
 *       2. ubug_db_clean
 *
-*---------------------------------------------*/

/*-----ubug_free_weblist-----*/
void ubug_free_weblist(void *pNull)
{
    WEBIN   *pList = urlSaveList;

    while (urlSaveList != NULL) {
        pList = urlSaveList->w_next;
        free(urlSaveList);
        urlSaveList = pList;
    }
}


/*-----ubug_db_clean-----*/
void ubug_db_clean(void *pNULL)
{
    mysql_close(&urlDataBase);
    mysql_library_end();
}

