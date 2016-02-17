/*------------------------------------------
	Source file content Seven part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:	Practise part
	Part Five:	Extract part

--------------------------------------------*/

/*------------------------------------------
	Part Zero: Include
--------------------------------------------*/

#include "mmdpool.h"

#include "spextb.h"
#include "speglobal.h"


/*------------------------------------------
	Part Two: Local function
--------------------------------------------*/


/*------------------------------------------
	Part Two: Local function
--------------------------------------------*/

/* Part Five */


/*------------------------------------------
	Part Three: Define
--------------------------------------------*/


/*------------------------------------------
	Part Five: Extract part

	1. exbug_extract_keyword
	2. exbug_update_terms

--------------------------------------------*/

/*-----exbug_extract_keyword-----*/
void exbug_extract_keyword(WDCT *extDic)
{
	//exbug_word_print(extDic);

	return;
}


/*-----exbug_update_terms-----*/
void exbug_update_terms(WDCT *upList, const char *pInd)
{
	WST	   *pList = upList->wc_list;
	char   *upString;
	int	    nOff, nCnt;

	if(!(upString = mmdp_malloc(threadMemPool, (upList->wc_tbytes + (upList->wc_ndiff * PER_WD_MORE)) + 1))) {
		elog_write("exbug_update_terms - mmdp_malloc", FUNCTION_STR, ERROR_STR);
		exbug_sig_error(PTHREAD_ERROR);
	}

	for(nOff = nCnt = 0; pList && nCnt < upMaxTerms; nCnt++, pList = pList->ws_next) {
		if(nOff + pList->ws_bytes > nKlistSize)
			break;	

		nOff += sprintf(&upString[nOff], TERMS_FORMAT, pList->ws_bytes, pList->ws_buf);
	}

	while(!mato_dec_and_test(&dicDbLock))
		mato_inc(&dicDbLock);

	if(!buff_size_enough(extSaveBuf, nOff + UPKEY_OTH_MAX)) {
		if(mysql_real_query(&dbKeysHandler, buff_place_start(extSaveBuf), buff_now_size(extSaveBuf))) {
			if(exbug_dberr_deal(&dbKeysHandler, dbKeysName, 
			"exbug_update_terms - mysql_real_query") != FUN_RUN_OK)
				exbug_sig_error(PTHREAD_ERROR);
		}

		buff_stru_make_empty(extSaveBuf);
	}

	buff_size_add(extSaveBuf, (buff_stru_empty(extSaveBuf) ? 
	sprintf(buff_place_start(extSaveBuf), INSERT_KEYWD, tblKeysName, pInd, nOff, upString, upList->wc_ndiff) :
	sprintf(buff_place_end(extSaveBuf), INSERT_KW_NEXT, pInd, nOff, upString, upList->wc_ndiff)));

	mato_inc(&dicDbLock);
}

