/*------------------------------------------
	Source file content Five part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:	Mysql operate

--------------------------------------------*/


/*------------------------------------------
	        Part Zero: Include
--------------------------------------------*/

#include "sp.h"

#include "spuglobal.h"
#include "spurlb.h"


/*------------------------------------------
        Part Four: Mysql operate

        1. ubug_init_database
        2. ubug_create_dbtable
        3. ubug_tran_db
        4. ubug_tran_db_whole
        5. ubug_tran_db_force
        6. ubug_tran_db_real
        7. ubug_dberr

--------------------------------------------*/

/*-----ubug_init_database-----*/
void ubug_init_database(void)
{
	char	database_name[SQL_DBNAME_LEN];

	if (mc_conf_read(
        "urls_database_name", CONF_STR,
        database_name, SQL_DBNAME_LEN) == FUN_RUN_FAIL) {

        elog_write("urlbug", "config setting wrong",  "urls_database_name");
		ubug_sig_error();
	}

    if (!mysql_simple_connect(&urlDataBase, database_name, NULL, 0)) {
        elog_write(
        "ubug_init_database - mysql_simple_connect", "urlDataBase", "Failed");

	    ubug_sig_error();
	}

	if (mgc_add(urlGarCol, NULL_POINT, ubug_db_clean) == MGC_FAILED)
		ubug_perror("ubug_init_database - mgc_add", errno);

	ubug_create_dbtable();
}


/*-----ubug_create_dbtable-----*/
void ubug_create_dbtable(void)
{
    if (mysql_creat_table(
        &urlDataBase, CREAT_URL_TAB, urlTabName, urlMaxLen) != FUN_RUN_OK) {
        if (ubug_dberr(
            &urlDataBase, "ubug_create_dbtable - creatTab") != FUN_RUN_OK)
			ubug_sig_error();
	}
}


/*-----ubug_tran_db-----*/
void ubug_tran_db(void *pInfo, void *uData, char *pUrl, int uLen)
{
    WEBIN   *web_info = (WEBIN *)pInfo;
	UDATA	*pData = (UDATA *)uData;
        
	if (!buff_size_enough(web_info->w_buff, SQL_PERCOM_MLEN))
		ubug_tran_db_force(web_info->w_buff);

	buff_size_add(web_info->w_buff, 
      
    ((buff_stru_empty(web_info->w_buff)) ? 
	
    sprintf(
    buff_place_start(web_info->w_buff), TRAN_URL_BEG, urlTabName, uLen, pUrl,
	pData->ud_poff, pData->ud_foff, web_info->w_pattern) : 

	sprintf(
    buff_place_end(web_info->w_buff), TRAN_URL, uLen, pUrl,
	pData->ud_poff, pData->ud_foff, web_info->w_pattern)));
}


/*-----ubug_tran_db_whole-----*/
void ubug_tran_db_whole(void)
{
    WEBIN   *pInfo;
        
    for (pInfo = urlSaveList; pInfo; pInfo = pInfo->w_next)
        ubug_tran_db_force(pInfo->w_buff);
}


/*-----ubug_tran_db_force-----*/
void ubug_tran_db_force(BUFF *pBuff)
{
	while (!mato_dec_and_test(&writeStoreLock))
		mato_inc(&writeStoreLock);

	if (!buff_stru_empty(pBuff)) {
		if (ubug_tran_db_real(pBuff) != FUN_RUN_END) {
			if (ubug_dberr(&urlDataBase, "ubug_tran_db_force - mysql_query") != FUN_RUN_OK)
				ubug_sig_error();
		}

		buff_stru_make_empty(pBuff);
	}

	mato_inc(&writeStoreLock);
}


/*-----ubug_tran_db_real-----*/
int ubug_tran_db_real(BUFF *pBuff)
{
	return
    (!buff_stru_empty(pBuff)) ? 
    mysql_real_query(
    &urlDataBase, buff_place_start(pBuff), buff_now_size(pBuff)) : FUN_RUN_FAIL;
}


/*-----ubug_dberr-----*/
int ubug_dberr(MYSQL *sHandler, char *withStr)
{
	uInt	sql_errno = mysql_errno(sHandler);

	elog_write(withStr, FUNCTION_STR, MYERR_STR(sHandler));

    if (sql_errno == CR_SERVER_LOST || sql_errno == CR_SERVER_GONE_ERROR) {
        if (mysql_real_connect(
            &urlDataBase, NULL, DBUSRNAME, DBUSRKEY, urlTabName, 0, NULL, 0))
			return	FUN_RUN_OK;
	}

	return	FUN_RUN_FAIL;
}
