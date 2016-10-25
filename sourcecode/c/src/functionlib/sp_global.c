/*---------------------------------------------
 *   modification time: 2016.07.26 16:40
 *   mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *   file: sp_global.c
 *   author: Muse
 *   creation time: 2015.11.19 17:00
-*---------------------------------------------*/

/*---------------------------------------------
 *          Source file contain Four part
 *
 *          Part Zero:  Include
 *          Part One:   Define
 *          Part Two:   Local data
 *          Part Three: Local function
 *           
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "spinc.h"


/*---------------------------------------------
 *            Part Two: Local data 
-*---------------------------------------------*/

char   *rPac =	
"Accept-Language: zh-CN\r\n"
"User-Agent: Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0\r\n"
"Connection: Keep-Alive\r\n\r\n";

char    httpAgents[] = {
"Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0",
};

char   *sqlOpt[] = {"db", "--default-file = my.cnf"};
int     nrOpt = (sizeof(sqlOpt) / sizeof(char *));


