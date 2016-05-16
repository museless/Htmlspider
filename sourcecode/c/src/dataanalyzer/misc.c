/*---------------------------------------------
 *     modification time: 2016-02-17 21:30
 *     mender: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
 *---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Five part
 *
 *      Part Zero:  Include
 *      Part One:   Local data
 *      Part Two:   Local function
 *      Part Three: Define
 *
 *      Part Four:  Help print
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *           Part One: Local data
-*---------------------------------------------*/


/*---------------------------------------------
 *          Part Four: Help print
 *
 *          1. exbug_print_help
 *
-*---------------------------------------------*/

/*-----exbug_print_help-----*/
void exbug_print_help(void)
{
    char   *file_store;
    char    help_file_path[] = "extbug.hlp";
    int     file_size;

    file_size = read_all_file(&file_store, help_file_path, 0);

    if (file_size == FRET_N) {
        printf("Help file: %s\n", help_file_path);
        exbug_perror("exbug_print_help - read_all_file", errno);
        return;
    }

    printf("%.*s\n\n", file_size, file_store);

    free(file_store);
}

