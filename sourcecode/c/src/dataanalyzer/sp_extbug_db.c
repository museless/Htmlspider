/*---------------------------------------------
 *     modification time: 2016-02-23 22:30:59
 *     mender: Muse
 *---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
 *---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Eight part
 *
 *      Part Zero:  Include
 *      Part One:   Local data
 *      Part Two:   Local function
 *      Part Three: Define
 *
 *      Part Four:  Database operate
 *      Part Five:  Database module
 *      Part Six:   Database clean
 *      Part Seven: Database error dispose
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

#include "spmpool.h"
#include "spextb.h"
#include "speglobal.h"


/*---------------------------------------------
 *          Part Two: Local function
-*---------------------------------------------*/

/* Part Six */
static  void    exbug_newsdb_unlink(void);
static  void    exbug_dicdb_unlink(void);
static  void    exbug_keysdb_unlink(void);


/*---------------------------------------------
 *         Part Four: Database operate
 *
 *         1. exbug_database_init
 *         2. exbug_content_download
 *         3. exbug_rewind_exmark
 *
-*---------------------------------------------*/

/*-----exbug_database_init-----*/
int exbug_database_init(void)
{
    mysql_library_init(nrOpt, sqlOpt, NULL);

    if (!mysql_init(&dbNewsHandler)) {
        elog_write("exbug_database_init - mysql_init", "dbNewsHandler", "failed");
        return  FUN_RUN_END;
    }

    if (mc_conf_read("news_database_name", CONF_STR, 
            dbNewsName, SQL_DBNAME_LEN) == FUN_RUN_FAIL) {
        mc_conf_print_err("news_database_name");
        return  FUN_RUN_END;
    }

    if (!mysql_real_connect(&dbNewsHandler, NULL, 
            DBUSRNAME, DBUSRKEY, dbNewsName, 0, NULL, 0)) {
        elog_write("exbug_database_init - mysql_real_connect", 
            "newsDataBase", "failed");
        return  FUN_RUN_END;
    }

    mysql_query(&dbNewsHandler, "set names utf8");

    if (mgc_add(exbGarCol, NULL_POINT, (gcfun)exbug_newsdb_unlink) == MGC_FAILED)
        elog_write("exbug_database_init - mgc_add", FUNCTION_STR, ERROR_STR);

    /* Mysql Dic handler init */
    if (!mysql_init(&dbDicHandler)) {
        elog_write("exbug_module_database_init - mysql_init", 
            "dbDicHandler", "failed");
        return  FUN_RUN_FAIL;
    }

    if (mc_conf_read("dic_database_name", CONF_STR, 
            dbDicName, SQL_DBNAME_LEN) == FUN_RUN_FAIL) {
        mc_conf_print_err("dic_database_name");
        return  FUN_RUN_FAIL;
    }

    if (!mysql_real_connect(&dbDicHandler, NULL, 
            DBUSRNAME, DBUSRKEY, dbDicName, 0, NULL, 0)) {
        elog_write("exbug_module_database_init - mysql_real_connect", 
            "dicDataBase", "failed");
        return  FUN_RUN_FAIL;
    }

    if (mgc_add(exbGarCol, NULL_POINT, (gcfun)exbug_dicdb_unlink) == MGC_FAILED)
        elog_write("exbug_module_database_init - mgc_add",
            FUNCTION_STR, ERROR_STR);

    /* read word table name */
    if (mc_conf_read("word_table_name", CONF_STR,
            tblWordName, SQL_TABNAME_LEN) == FUN_RUN_FAIL) {
        mc_conf_print_err("word_table_name");
        return  FUN_RUN_END;
    }

    return  FUN_RUN_OK;
}


/*-----exbug_content_download-----*/
void *exbug_content_download(void)
{
    MSLRES *result;
    
    if (mysql_query(&dbNewsHandler, sqlSeleCom) != FUN_RUN_END) {
        exbug_dberr_deal(&dbNewsHandler, dbNewsName,
            "exbug_content_download - mysql_query - sqlSeleCom");
        return  NULL;
    }

    if ((result = mysql_store_result(&dbNewsHandler)) == NULL) {
        if (mysql_errno(&dbNewsHandler))
            exbug_dberr_deal(&dbNewsHandler, dbNewsName,
                "exbug_content_download - mysql_store_result");

        return  NULL;
    }

    return  result;
}


/*-----exbug_rewind_exmark-----*/
void exbug_rewind_exmark(const char *pInd, char *maskName)
{
    char    sqlCom[SQL_TCOM_LEN];

    if (mysql_real_query(&dbNewsHandler, sqlCom, 
            sprintf(sqlCom, SET_NEWS_FLAGS, 
            tblNewsName, maskName, pInd)) != FUN_RUN_END)
        exbug_dberr_deal(
            &dbNewsHandler, dbNewsName,
            "exbug_rewind_exmark - mysql_real_query");

    return;
}


/*---------------------------------------------
 *        Part Five: Database module
 *
 *        1. exbug_module_database_init
 *        2. exbug_create_keyword_table
 *
-*---------------------------------------------*/

/*-----exbug_module_database_init-----*/
int exbug_module_database_init(void)
{
    /* Mysql Keys handler init */
    if (!mysql_init(&dbKeysHandler)) {
        elog_write("exbug_module_database_init - mysql_init", 
            "dbKeysHandler", "failed");
        return  FUN_RUN_FAIL;
    }

    if (mc_conf_read("keys_database_name", CONF_STR, 
            dbKeysName, SQL_DBNAME_LEN) == FRET_N) {
        mc_conf_print_err("keys_database_name");
        return  FUN_RUN_FAIL;
    }

    if (!mysql_real_connect(&dbKeysHandler, NULL, 
            DBUSRNAME, DBUSRKEY, dbKeysName, 0, NULL, 0)) {
        elog_write("exbug_module_database_init - mysql_real_connect", "keysDataBase", "failed");
        return  FUN_RUN_FAIL;
    }

    if (mgc_add(exbGarCol, NULL_POINT, (gcfun)exbug_keysdb_unlink) == MGC_FAILED)
        elog_write("exbug_module_database_init - mgc_add", FUNCTION_STR, ERROR_STR);

    exbug_create_keyword_table();

    mato_init(&dicDbLock, 1);

    return  FUN_RUN_OK;
}


/*-----exbug_create_keyword_table-----*/
void exbug_create_keyword_table(void)
{
    char    sqlCom[SQL_MCOM_LEN];

    /* create news table */
    sprintf(sqlCom, CREAT_KEY_TAB, tblKeysName, dbKeysName);

    if (mysql_query(&dbKeysHandler, sqlCom) != FUN_RUN_END) {
        exbug_dberr_deal(&dbKeysHandler, dbKeysName,
            "exbug_create_keyword_table - mysql_query");
        exbug_sig_error(PROC_ERROR);
    }
}


/*---------------------------------------------
 *          Part Six: Database clean
 *
 *          1. exbug_newsdb_unlink
 *          2. exbug_keysdb_unlink
 *          3. exbug_database_close
 *
-*---------------------------------------------*/

/*-----exbug_newsdb_unlink-----*/
static void exbug_newsdb_unlink(void)
{
    mysql_close(&dbNewsHandler);
}


/*-----exbug_dicdb_unlink-----*/
static void exbug_dicdb_unlink(void)
{
    mysql_close(&dbDicHandler);
}


/*-----exbug_keysdb_unlink-----*/
static void exbug_keysdb_unlink(void)
{
    mysql_close(&dbKeysHandler);
}


/*-----exbug_database_close-----*/
void exbug_database_close(void)
{
    mysql_library_end();
}


/*---------------------------------------------
 *     Part Seven: Database error dispose
 *
 *     1. exbug_dberr_deal
 *
-*---------------------------------------------*/

/*-----exbug_dberr_deal-----*/
int exbug_dberr_deal(void *sqlHand, char *dbName, char *pHead)
{
    MYSQL  *sql = (MYSQL *)sqlHand;
    uInt    my_errno = mysql_errno(sql);
    char    errno_str[16];

    sprintf(errno_str, "%d", mysql_errno(sql));
    elog_write(pHead, (char *)mysql_error(sql), errno_str);

    if (my_errno == CR_SERVER_LOST || my_errno == CR_SERVER_GONE_ERROR) {
        printf("Extbug---> mysql dropping - reconnect now\n");
        mysql_close(sql);
        mysql_init(sql);

        if (mysql_real_connect(sql, NULL, 
                DBUSRNAME, DBUSRKEY, dbName, 0, NULL, 0) == FRET_Z) {
            printf("Extbug---> mysql_server - reconnect ok\n");
            return  FUN_RUN_OK;
        }

        printf("Extbug---> reconnect failed\n");

    } else if (my_errno == ER_NO_SUCH_TABLE) {
        return  FUN_RUN_END;
    }

    return  FUN_RUN_FAIL;
}

