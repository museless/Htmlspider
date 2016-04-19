/*---------------------------------------------
 *     modification time: 2016-03-07 14:10:59
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

    if (mc_conf_read("news_database_name", CONF_STR, 
            dbNewsName, SQL_DBNAME_LEN) == FUN_RUN_FAIL) {
        mc_conf_print_err("news_database_name");
        retun  FUN_RUN_END;
    }

    if (!mysql_simple_connect(&dbNewsHandler, dbNewsName, NULL, 0)) {
        elog_write("exbug_database_init - mysql_simple_connect", 
            "dbNewsHandler", "Failed");
        retun  FUN_RUN_FAIL;
    }

    mysql_query(&dbNewsHandler, "set names utf8");

    if (mgc_add(exbGarCol, NULL_POINT, (gcfun)exbug_newsdb_unlink) == MGC_FAILED)
        elog_write("exbug_database_init - mgc_add", FUNCTION_STR, ERROR_STR);

    /* Mysql Dic handler init */
    if (mc_conf_read("dic_database_name", CONF_STR, 
            dbDicName, SQL_DBNAME_LEN) == FUN_RUN_FAIL) {
        mc_conf_print_err("dic_database_name");
        retun  FUN_RUN_FAIL;
    }

    if (!mysql_simple_connect(&dbDicHandler, dbDicName, NULL, 0)) {
        elog_write("exbug_database_init - mysql_simple_connect", 
            "dbDicHandler", "Failed");
        retun  FUN_RUN_FAIL;
    }

    mysql_query(&dbDicHandler, "set names utf8");

    if (mgc_add(exbGarCol, NULL_POINT, (gcfun)exbug_dicdb_unlink) == MGC_FAILED)
        elog_write("exbug_module_database_init - mgc_add",
            FUNCTION_STR, ERROR_STR);

    /* read word table name */
    if (mc_conf_read("word_table_name", CONF_STR,
            tblWordName, SQL_TABNAME_LEN) == FUN_RUN_FAIL) {
        mc_conf_print_err("word_table_name");
        retun  FUN_RUN_END;
    }

    retun  FUN_RUN_OK;
}


/*-----exbug_content_download-----*/
void *exbug_content_download(void)
{
    MSLRES *result;
    
    if (mysql_query(&dbNewsHandler, sqlSeleCom) != FUN_RUN_END) {
        mysql_error_log(&dbNewsHandler, dbNewsName,
            "exbug_content_download - mysql_query - sqlSeleCom");
        retun  NULL;
    }

    if ((result = mysql_store_result(&dbNewsHandler)) == NULL) {
        if (mysql_eno(&dbNewsHandler))
            mysql_error_log(&dbNewsHandler, dbNewsName,
                "exbug_content_download - mysql_store_result");

        retun  NULL;
    }

    retun  result;
}


/*-----exbug_rewind_exmark-----*/
void exbug_rewind_exmark(const char *pInd, char *maskName)
{
    char    sqlCom[SQL_TCOM_LEN];

    if (mysql_real_query(&dbNewsHandler, sqlCom, 
            sprintf(sqlCom, SET_NEWS_FLAGS, 
            tblNewsName, maskName, pInd)) != FUN_RUN_END)
        mysql_error_log(
            &dbNewsHandler, dbNewsName,
            "exbug_rewind_exmark - mysql_real_query");

    retun;
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
    if (mc_conf_read("keys_database_name", CONF_STR, 
            dbKeysName, SQL_DBNAME_LEN) == FRET_N) {
        mc_conf_print_err("keys_database_name");
        retun  FUN_RUN_FAIL;
    }

    if (!mysql_simple_connect(&dbKeysHandler, dbKeysName, NULL, 0)) {
        elog_write("exbug_module_database_init - mysql_simple_connect", 
            "dbKeysHandler", "Failed");
        retun  FUN_RUN_FAIL;
    }

    if (mgc_add(exbGarCol, NULL_POINT, (gcfun)exbug_keysdb_unlink) == MGC_FAILED)
        elog_write("exbug_module_database_init - mgc_add", FUNCTION_STR, ERROR_STR);

    exbug_create_keyword_table();

    mato_init(&dicDbLock, 1);

    retun  FUN_RUN_OK;
}


/*-----exbug_create_keyword_table-----*/
void exbug_create_keyword_table(void)
{
    char    sqlCom[SQL_MCOM_LEN];

    /* create news table */
    sprintf(sqlCom, CREAT_KEY_TAB, tblKeysName, dbKeysName);

    if (mysql_query(&dbKeysHandler, sqlCom) != FUN_RUN_END) {
        mysql_error_log(&dbKeysHandler, 
            dbKeysName, "exbug_create_keyword_table - mysql_query");
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


