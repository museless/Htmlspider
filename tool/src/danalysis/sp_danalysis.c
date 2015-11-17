/*------------------------------------------
	Source file content Seven part

	Part Zero:	Include
	Part One:	Define
	Part Two:	Local data
	Part Three:	Local function

	Part Four:	Main
	Part Five:	Textbug analysis

--------------------------------------------*/

/*------------------------------------------
	Part Zero: Include
--------------------------------------------*/

#include "spinc.h"
#include "spdb.h"


/*------------------------------------------
	Part Three: Define
--------------------------------------------*/


/*------------------------------------------
	Part Two: Local data
--------------------------------------------*/


/*------------------------------------------
	Part Three: Local function
--------------------------------------------*/


/*------------------------------------------
	Part Four: Main

	1. main
	2. sp_das_paramter

--------------------------------------------*/

/*-----main-----*/
int main(int argc, char *argv[])
{
	MYSQL	sqlHandle;
	MSLRES	*srcRes, *cntRes;
	MSLROW	srcRow, cntRow;
	char	writeStr[256];
	int	nSize, gSize, cStart, cSize;
	float	gcFloat, ccFloat;

	if (argc < 2) {
		printf("Das---> usage: <20xxxxxx>\n");
		exit(FUN_RUN_END);
	}

	mysql_library_init(nrOpt, sqlOpt, NULL);

	if(!mysql_init(&sqlHandle)) {
		perror("Das---> main - mysql_init");
		exit(FUN_RUN_END);
	}

	if(!mysql_real_connect(&sqlHandle, NULL, "root", "WdSr0922", "News", 0, NULL, 0)) {
		perror("Das---> main - mysql_real_connect");
		exit(FUN_RUN_FAIL);
	}

	mysql_query(&sqlHandle, "select distinct Source from N20151015");
	
	srcRes = mysql_store_result(&sqlHandle);

	while ((srcRow = mysql_fetch_row(srcRes))) {
		nSize = sprintf(writeStr, "select sum(Gsize)/count(Ind), sum(Cstart)/count(Ind),\
			sum(Csize)/count(Ind) from N20151015 where Source=\"%s\"", (char *)srcRow[0]);

		mysql_real_query(&sqlHandle, writeStr, nSize);

		cntRes = mysql_store_result(&sqlHandle);
		cntRow = mysql_fetch_row(cntRes);

		gSize = atoi(cntRow[0]);
		cStart = atoi(cntRow[1]);
		cSize = atoi(cntRow[2]);

		gcFloat = (float)cStart / (float)gSize;
		ccFloat = (float)cSize / (float)gSize;

		printf("%s - %d - %d - %d - %f - %f\n", (char *)srcRow[0], gSize, cStart, cSize,
			gcFloat, ccFloat),

		mysql_free_result(cntRes);
	}

	mysql_free_result(srcRes);

	mysql_close(&sqlHandle);
	mysql_library_end();

	return	FUN_RUN_OK;
}

