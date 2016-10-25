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
 *          6. ubug_db_seterr
 *
-*---------------------------------------------*/

/*-----ubug_init_database-----*/
void ubug_init_database(void)
{
    char    dbname[SQL_DBNAME_LEN];

    if (!mc_conf_read("urls_database_name", CONF_STR, dbname, SQL_DBNAME_LEN)) {
        setmsg(LM19, "urls_database_name");
        ubug_sig_error();
    }

    if (!mysql_simple_connect(&urlDataBase, dbname, NULL, 0)) {
        setmsg(LM22, "urls connect failed");
        ubug_sig_error();
    }

    if (!mgc_add(&objGc, GC_DEFOBJ, ubug_db_clean))
        setmsg(LM5, "database clean");

    ubug_create_dbtable();
}


/*-----ubug_create_dbtable-----*/
void ubug_create_dbtable(void)
{
    if (mysql_creat_table(&urlDataBase,
            CREAT_URL_TAB, urlTabName, urlMaxLen) != FRET_P)
        ubug_db_seterror();
}


/*-----ubug_tran_db-----*/
void ubug_tran_db(void *pInfo, void *uData, char *url, int len)
{
    WEBIN   *web_info = (WEBIN *)pInfo;
    UDATA   *data = (UDATA *)uData;
        
    if (!buff_size_enough(web_info->w_buff, SQL_PERCOM_MLEN))
        ubug_tran_db_force(web_info->w_buff);

    buff_size_add(web_info->w_buff, ((buff_stru_empty(web_info->w_buff)) ? 
        sprintf(buff_place_start(web_info->w_buff), 
            TRAN_URL_BEG, urlTabName, len, url, data->ud_poff, data->ud_foff) : 

        sprintf(buff_place_end(web_info->w_buff), 
            TRAN_URL, len, url, data->ud_poff, data->ud_foff)));
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
                buff_place_start(pBuff), buff_now_size(pBuff)) != FRET_Z)
            ubug_db_seterror();

        buff_stru_make_empty(pBuff);
    }

    mato_unlock(writeStoreLock);
}


/*-----ubug_db_seterror-----*/
void ubug_db_seterror(void)
{
    setmsg(LM22, MYERR_STR(&urlDataBase));

    if (!mysql_error_log(&urlDataBase, urlTabName)) {
        setmsg(LM22, "restart failed");
        ubug_sig_error();
    }
}

