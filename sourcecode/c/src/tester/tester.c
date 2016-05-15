/*----------------------------------------------
 *   modification time: 2016.05.15 23:40
 *   mender: Muse
-*----------------------------------------------*/

/*----------------------------------------------
 *   file: tester.c
 *   author: Muse
 *   creation time: 2015.11.19 21:45
-*----------------------------------------------*/

/*----------------------------------------------
 *         Source file contain Five part
 *
 *          Part Zero:  Include
 *          Part One:   Define
 *          Part Two:   Local data
 *          Part Three: Local function
 *
 *          Part Four:  Test
 *           
-*-----------------------------------------------*/

/*----------------------------------------------
 *             Part Zero: Include
-*----------------------------------------------*/

#include "sp.h"

#include "ping.h"


/*----------------------------------------------
 *              Part One: Define
-*----------------------------------------------*/

/*----------------------------------------------
 *             Part Two: Local data
-*----------------------------------------------*/

static char         str_buff[64];
static int          number;
static int          test;
static char         saver[33];

static PerConfData  data[] = {
    {"str", CONF_STR, str_buff, 64},
    {"number", CONF_NUM, &number, 4},
    {"data", CONF_NUM, &test, 1},
    {"fucker", CONF_STR, saver, 32},
};


/*----------------------------------------------
 *          Part Three: Local function
-*-----------------------------------------------*/


/*----------------------------------------------
 *               Part Four: Test
 *
 *               1. main
 *
-*-----------------------------------------------*/

/*-----main-----*/
int main(int argc, char **argv)
{
    mc_load_config("Tester", "/mnt/hgfs/Muse/CsSpider/sourcecode/c/src/tester/test.conf");

    mc_conf_read_list(data, sizeof(data) / sizeof(data[0]));

    printf("%s - %d - %d\n", str_buff, number, test);

    return  -1;
}


