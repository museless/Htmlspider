/*---------------------------------------------
 *     modification time: 2016-07-03 11:45:45
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-09-01 
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Five part
 *
 *      Part Zero:  Include
 *      Part One:   Local data
 *      Part Two:   Local function
 *      Part Three: Define
 *
 *      Part Four:  Mysql operate
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spuglobal.h"
#include "spurlb.h"


/*---------------------------------------------
 *          Part Four: Mysql operate
 *
 *          1. ubug_init_database
 *          2. ubug_create_dbtable
 *          3. ubug_tran_db
 *          4. ubug_tran_db_whole
 *          5. ubug_tran_db_force
 *
-*---------------------------------------------*/

/*-----ubug_init_database-----*/
void ubug_init_database(void)
{
    char    database_name[SQL_DBNAME_LEN];

    if (mc_conf_read("urls_database_name", CONF_STR,
            database_name, SQL_DBNAME_LEN) == FUN_RUN_FAIL) {
        elog_write("urlbug", "config setting wrong",  "urls_database_name");
        ubug_sig_error();
    }

    if (!mysql_simple_connect(&urlDataBase, database_name, NULL, 0)) {
        elog_write("ubug_init_database - mysql_connect", "urlDataBase", "Failed");
        ubug_sig_error();
    }

    if (!mgc_add(&urlGarCol, GC_DEFOBJ, ubug_db_clean))
        ubug_perror("ubug_init_database - mgc_add", errno);

    ubug_create_dbtable();
}


/*-----ubug_create_dbtable-----*/
void ubug_create_dbtable(void)
{
    if (mysql_creat_table(
        &urlDataBase, CREAT_URL_TAB, urlTabName, urlMaxLen) != FRET_P) {
        if (mysql_error_log(&urlDataBase, 
                urlTabName, "ubug_create_dbtable - creatTab") != FRET_P)
            ubug_sig_error();
    }
}


/*-----ubug_tran_db-----*/
void ubug_tran_db(void *pInfo, void *uData, char *pUrl, int uLen)
{
    WEBIN   *web_info = (WEBIN *)pInfo;
    UDATA   *pData = (UDATA *)uData;
        
    if (!buff_size_enough(web_info->w_buff, SQL_PERCOM_MLEN))
        ubug_tran_db_force(web_info->w_buff);

    buff_size_add(web_info->w_buff, ((buff_stru_empty(web_info->w_buff)) ? 
        sprintf(buff_place_start(web_info->w_buff), 
            TRAN_URL_BEG, urlTabName, uLen, pUrl, pData->ud_poff, pData->ud_foff) : 

        sprintf(buff_place_end(web_info->w_buff), 
            TRAN_URL, uLen, pUrl, pData->ud_poff, pData->ud_foff)));
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
    mato_lock(writeStoreLock);

    if (!buff_stru_empty(pBuff)) {
        if (mysql_real_query(&urlDataBase, 
                buff_place_start(pBuff), buff_now_size(pBuff)) != FRET_Z) {
            if (mysql_error_log(&urlDataBase, urlTabName, 
                    "ubug_tran_db_force - mysql_query") != FRET_P)
                ubug_sig_error();
        }

        buff_stru_make_empty(pBuff);
    }

    mato_unlock(writeStoreLock);
}
