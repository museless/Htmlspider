/*------------------------------------------
	Source file content Five part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:  Print Help tip

--------------------------------------------*/

/*------------------------------------------
	        Part Zero: Include
--------------------------------------------*/

#include "spinc.h"
#include "sphtml.h"
#include "spnet.h"
#include "spdb.h"
#include "spmpool.h"
#include "spmsg.h"

#include "spurlb.h"
#include "spuglobal.h"


/*------------------------------------------
	    Part Four: Print Help tip

	    1. ubug_print_help

--------------------------------------------*/

/*-----ubug_print_help-----*/
void ubug_print_help(void)
{
	STAT	stBuf;
	char   *hpCont;
	int	    hpFd;

	if ((hpFd = 
         open("../src/urlcatcher/urlbug.hlp", O_RDWR)) == FUN_RUN_FAIL) {
		ubug_perror("ubug_print_help - open", errno);
		exit(FUN_RUN_FAIL);
	}

	if (fstat(hpFd, &stBuf) == FUN_RUN_FAIL) {
		ubug_perror("ubug_print_help - fstat", errno);
		exit(FUN_RUN_FAIL);	
	}

	if ((hpCont = malloc(stBuf.st_size + 1)) == NULL) {
		ubug_perror("ubug_print_help - malloc", errno);
		exit(FUN_RUN_FAIL);
	}

	hpCont[stBuf.st_size] = 0;
	if (readn(hpFd, hpCont, stBuf.st_size) == FUN_RUN_FAIL) {
		ubug_perror("ubug_print_help - readn", errno);
		exit(FUN_RUN_FAIL);
	}

	printf("%s\n", hpCont);
	
	free(hpCont);
	close(hpFd);
}

