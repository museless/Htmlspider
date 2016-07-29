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
 *          Part Four: Help tip
 *
 *          1. ubug_print_help
 *
-*---------------------------------------------*/

/*-----ubug_print_help-----*/
void ubug_print_help(void)
{
    STAT    stat_buf;
    int     help_fd;

    help_fd = open("../sourcecode/c/src/urlcatcher/urlbug.hlp", O_RDWR);

    if (help_fd == FUN_RUN_FAIL) {
        ubug_perror("ubug_print_help - open", errno);
        exit(FUN_RUN_FAIL);
    }

    if (fstat(help_fd, &stat_buf) == FUN_RUN_FAIL) {
        ubug_perror("ubug_print_help - fstat", errno);
        exit(FUN_RUN_FAIL); 
    }

    char    help_content[stat_buf.st_size + 1];

    if (readn(help_fd, help_content, stat_buf.st_size) == FUN_RUN_FAIL) {
        ubug_perror("ubug_print_help - readn", errno);
        exit(FUN_RUN_FAIL);
    }

    help_content[stat_buf.st_size] = 0;

    printf("%s\n", help_content);
    
    close(help_fd);
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

