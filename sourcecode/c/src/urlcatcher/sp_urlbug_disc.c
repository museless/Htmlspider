/*---------------------------------------------
 *     modification time: 2016-01-17 22:10:59
 *     mender: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
 *---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Local data
 *       Part Two:   Local function
 *       Part Three: Define
 *
 *       Part Four:  Print Help tip
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *           Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spurlb.h"
#include "spuglobal.h"


/*---------------------------------------------
 *          Part Four: Print Help tip
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
        ubug_perror("ubug_print_help - open", eno);
        exit(FUN_RUN_FAIL);
    }

    if (fstat(help_fd, &stat_buf) == FUN_RUN_FAIL) {
        ubug_perror("ubug_print_help - fstat", eno);
        exit(FUN_RUN_FAIL); 
    }

    char    help_content[stat_buf.st_size + 1];

    if (readn(help_fd, help_content, stat_buf.st_size) == FUN_RUN_FAIL) {
        ubug_perror("ubug_print_help - readn", eno);
        exit(FUN_RUN_FAIL);
    }

    help_content[stat_buf.st_size] = 0;

    printf("%s\n", help_content);
    
    close(help_fd);
}

