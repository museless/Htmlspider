/*------------------------------------------
    Source file content Seven part

    Part Zero:  Include
    Part One:   Define
    Part Two:   Local data
    Part Three: Local function

    Part Four:  Mysql operate

--------------------------------------------*/


/*------------------------------------------
            Part Zero: Include
--------------------------------------------*/

#include "spinc.h"
#include "spdb.h"


/*------------------------------------------
	     Part Four: Mysql operate

	     1. mysql_string_exist_check
         2. mysql_return_result
         3. mysql_simple_connect

--------------------------------------------*/

/*-----mysql_simple_connect-----*/
int mysql_simple_connect(
    void *sql_handle, const char *database_name, const char *host, int port)
{
	mysql_library_init(nrOpt, sqlOpt, NULL);

	if (!mysql_init(sql_handle))
        return  FUN_RUN_END;

	if (!mysql_real_connect(
        sql_handle, host, DBUSRNAME, DBUSRKEY, database_name, port, NULL, 0))
        return  FUN_RUN_END;

    return  FUN_RUN_OK;
}


/*-----mysql_string_exist_check-----*/
int mysql_string_exist_check(void *chkSql, char *chkCom)
{
	MSLRES *sqlRes;
	int		fRet;
		
	if(mysql_query((MYSQL *)chkSql, chkCom) != FUN_RUN_END)
		return	FUN_RUN_FAIL;

	if((sqlRes = mysql_store_result((MYSQL *)chkSql)) == NULL)
		return	FUN_RUN_FAIL;

	fRet = mysql_num_rows(sqlRes);

	mysql_free_result(sqlRes);

	return	(fRet) ? 1 : 0;
}


/*-----mysql_return_result-----*/
void *mysql_return_result(void *sql_handle, const char *sql_string)
{
    MSLRES *result;
    char    string_buff[SQL_MCOM_LEN];
    int     str_len = snprintf(string_buff, SQL_MCOM_LEN, "%s", sql_string);

    if (mysql_real_query(sql_handle, string_buff, str_len))
        return  NULL;

    return  (result = mysql_store_result(sql_handle));
}
