/*---------------------------------------------
 *     modification time: 2016-07-31 17:55:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

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
 *      Part Seven: Database error
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"

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
bool exbug_database_init(void)
{
    mysql_library_init(nrOpt, sqlOpt, NULL);

    if (!mc_conf_read("news_database_name", CONF_STR, dbNewsName, SQL_DBNAME_LEN)) {
        mc_conf_print_err("news_database_name");
        return  false;
    }

    if (!mysql_simple_connect(&dbNewsHandler, dbNewsName, NULL, 0)) {
        setmsg(LM16, "news database handler");
        return  false;
    }

    mysql_query(&dbNewsHandler, "set names utf8");

    if (!mgc_add(&objGc, GC_DEFOBJ, (gcfun)exbug_newsdb_unlink))
        setmsg(LM5, "news db unlink");

    /* Mysql Dic handler init */
    if (!mc_conf_read("dic_database_name", CONF_STR, dbDicName, SQL_DBNAME_LEN)) {
        setmsg(LM0, "dic_database_name");
        return  false;
    }

    if (!mysql_simple_connect(&dbDicHandler, dbDicName, NULL, 0)) {
        setmsg(LM16, "dictionary database handler");
        return  false;
    }

    mysql_query(&dbDicHandler, "set names utf8");

    if (!mgc_add(&objGc, GC_DEFOBJ, (gcfun)exbug_dicdb_unlink))
        setmsg(LM5, "dictionary db unlink");

    if (!mc_conf_read("word_table_name", CONF_STR, tblWordName, SQL_TABNAME_LEN)) {
        setmsg(LM0, "word_table_name");
        return  false;
    }

    return  true;
}


/*-----exbug_content_download-----*/
void *exbug_content_download(void)
{
    MSLRES *result;
    
    if (mysql_query(&dbNewsHandler, sqlSeleCom) != FUN_RUN_END) {
        exbug_db_seterror(&dbNewsHandler, dbNewsName);
        return  NULL;
    }

    if (!(result = mysql_store_result(&dbNewsHandler))) {
        exbug_db_seterror(&dbNewsHandler, dbNewsName);
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
        exbug_db_seterror(&dbNewsHandler, dbNewsName);
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
    if (!mc_conf_read("keys_database_name", CONF_STR, dbKeysName, SQL_DBNAME_LEN)) {
        setmsg(LM0, "keys_database_name");
        return  FUN_RUN_FAIL;
    }

    if (!mysql_simple_connect(&dbKeysHandler, dbKeysName, NULL, 0)) {
        setmsg(LM16, "db keys handler");
        return  FUN_RUN_FAIL;
    }

    if (!mgc_add(&objGc, GC_DEFOBJ, (gcfun)exbug_keysdb_unlink))
        setmsg(LM5, "keys database");

    exbug_create_keyword_table();

    mato_init(dicDbLock, 1);

    return  FUN_RUN_OK;
}


/*-----exbug_create_keyword_table-----*/
void exbug_create_keyword_table(void)
{
    char    sqlCom[SQL_MCOM_LEN];

    /* create news table */
    sprintf(sqlCom, CREAT_KEY_TAB, tblKeysName, dbKeysName);

    if (mysql_query(&dbKeysHandler, sqlCom) != FUN_RUN_END)
        exbug_db_seterror(&dbKeysHandler, dbKeysName);
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
void exbug_newsdb_unlink(void)
{
    mysql_close(&dbNewsHandler);
}


/*-----exbug_dicdb_unlink-----*/
void exbug_dicdb_unlink(void)
{
    mysql_close(&dbDicHandler);
}


/*-----exbug_keysdb_unlink-----*/
void exbug_keysdb_unlink(void)
{
    mysql_close(&dbKeysHandler);
}


/*-----exbug_database_close-----*/
void exbug_database_close(void)
{
    mysql_library_end();
}


/*---------------------------------------------
 *          Part Six: Database error 
 *
 *          1. exbug_db_seterror
 *
-*---------------------------------------------*/

/*-----exbug_db_seterror-----*/
void exbug_db_seterror(void *db, const char *tabname)
{
    setmsg(LM22, MYERR_STR(db));

    if (!mysql_error_log(db, tabname)) {
        setmsg(LM22, "restart failed");
        exbug_sig_quit();
    }
}
