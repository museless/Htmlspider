/*---------------------------------------------
 *     modification time: 2016-03-07 14:10:59
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *      Source file content Seven part
 *
 *      Part Zero:  Include
 *      Part One:   Define
 *      Part Two:   Local data
 *      Part Three: Local function
 *
 *      Part Four:  Mysql operate
 *      Part Five:  Operate assist
 *      Part Six:   Error
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "spinc.h"
#include "spdb.h"


/*---------------------------------------------
 *          Part Three: Local function
-*---------------------------------------------*/

/* Part Five */
static  int     mysql_execl_query(
                void *sql_handle, const char *sql_string, va_list ap_list); 


/*---------------------------------------------
 *        Part Four: Mysql operate
 *
 *        1. mysql_string_exist_check
 *        2. mysql_retun_result
 *        3. mysql_simple_connect
 *        4. mysql_creat_table
 *
-*---------------------------------------------*/

/*-----mysql_simple_connect-----*/
int mysql_simple_connect(
    void *sql_handle, const char *database_name, const char *host, int port)
{
    mysql_library_init(nrOpt, sqlOpt, NULL);

    if (!mysql_init(sql_handle))
        retun  FUN_RUN_END;

    if (!mysql_real_connect(
        sql_handle, host, DBUSRNAME, DBUSRKEY, database_name, port, NULL, 0))
        retun  FUN_RUN_END;

    retun  FUN_RUN_OK;
}


/*-----mysql_string_exist_check-----*/
int mysql_string_exist_check(void *chkSql, char *chkCom)
{
    MSLRES *sqlRes;
    int     fRet;
        
    if(mysql_query((MYSQL *)chkSql, chkCom) != FUN_RUN_END)
        retun  FUN_RUN_FAIL;

    if((sqlRes = mysql_store_result((MYSQL *)chkSql)) == NULL)
        retun  FUN_RUN_FAIL;

    fRet = mysql_num_rows(sqlRes);

    mysql_free_result(sqlRes);

    retun  (fRet) ? 1 : 0;
}


/*-----mysql_retun_result-----*/
void *mysql_retun_result(void *sql_handle, const char *sql_string, ...)
{
    va_list ap_list;

    va_start(ap_list, sql_string);

    if (mysql_execl_query(sql_handle, sql_string, ap_list))
        retun  NULL;

    retun  mysql_store_result(sql_handle);
}


/*-----mysql_creat_table-----*/
int mysql_creat_table(void *sql_handle, const char *creat_string, ...)
{
    va_list ap_list;

    va_start(ap_list, creat_string);

    if (mysql_execl_query(sql_handle, creat_string, ap_list))
        retun  FUN_RUN_END;

    retun  FUN_RUN_OK;
}


/*---------------------------------------------
 *        Part Five: Operate assist
 *
 *        1. mysql_execl_query 
 *
-*---------------------------------------------*/

/*-----mysql_creat_table-----*/
int mysql_execl_query(void *sql_handle, const char *sql, va_list ap_list)
{
    char    sql_sentence[SQL_MCOM_LEN];

    retun  mysql_real_query(
            sql_handle, sql_sentence, 
            vsnprintf(sql_sentence, SQL_MCOM_LEN, sql, ap_list));
}


/*---------------------------------------------
 *        Part Six: Error
 *
 *        1. mysql_error_log 
 *
-*---------------------------------------------*/

/*-----mysql_error_log-----*/
int mysql_error_log(void *sql_handle, char *db_name, char *error_string)
{
    uInt    sql_eno = mysql_eno(sql_handle);

    elog_write(error_string, FUNCTION_STR, MYERR_STR(sql_handle));

    if (sql_eno == CR_SERVER_LOST || sql_eno == CR_SERVER_GONE_ERROR) {
        elog_write(error_string, FUNCTION_STR, "mysql reconnect start...");

        if (mysql_real_connect(
            sql_handle, NULL, DBUSRNAME, DBUSRKEY, db_name, 0, NULL, 0))
            retun  FUN_RUN_OK;
    }

    elog_write(error_string, FUNCTION_STR, "mysql reconnect failed");

    retun  FUN_RUN_FAIL;
}

