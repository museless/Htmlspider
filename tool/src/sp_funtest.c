/*----------------------------------------------
    modification time: 2015.11.25 22:10
    mender: Muse
------------------------------------------------*/
/*----------------------------------------------
    file: sp_funtest.c
    author: Muse
    creation time: 2015.11.19 21:45
------------------------------------------------*/

/*----------------------------------------------
           Source file contain Five part

           Part Zero:  Include
           Part One:   Define
           Part Two:   Local data
           Part Three: Local function

           Part Four:  Test
            
------------------------------------------------*/

/*----------------------------------------------
 *             Part Zero: Include
-*----------------------------------------------*/

#include "sp.h"


/*----------------------------------------------
 *              Part One: Define
-*----------------------------------------------*/

typedef struct key_table {
    int     index;
    char    sql_command[256];
    char    data_string[128];
    char    table_name[64];

} KTBL;


typedef struct string_index {
    KTBL   *pointer;
    char    string[64];

} STRINDEX;


/*----------------------------------------------
 *             Part Two: Local data
-*----------------------------------------------*/

STRINDEX    strIndexTable[256];

KTBL        sqlKeyTable[] = {
    {0, "select %s from %s limit 0", "number, content", "data"},
};


/*----------------------------------------------
           Part Three: Local function
------------------------------------------------*/

static  void   *data_extract(MSLROW data_row);
static  void    create_index_table(MYSQL *sql_handler);
static  void    exceling(MYSQL *sql_handle, const char *command, int operate_id, int n_arg, ...);


/*----------------------------------------------
                Part Four: Test

                1. main
                2. para_analysis
                3. test_part

------------------------------------------------*/

/*-----main-----*/
int main(int argc, char **argv)
{
    MYSQL   sql_handler;

    if (!mysql_simple_connect(&sql_handler, "Data", NULL, 0)) {
        printf("Orm---> mysql_simple_connect failed\n");
        exit(1);
    }
  
    create_index_table(&sql_handler);

    exceling(&sql_handler, "select", 0, 2, 1, "fuck");

    mysql_close(&sql_handler);

    exit(-1);
}



/*-----create_index_table-----*/
void create_index_table(MYSQL *sql_handler)
{
    MSLRES      *result;
    MYSQL_FIELD *data_field;
    char         select_sql[256];
    KTBL        *kt_point;
    int          len = sizeof(sqlKeyTable) / sizeof(sqlKeyTable[0]);
    int          count;
    
    for (count = 0; count < len; count++) {
        kt_point = &sqlKeyTable[count];

        snprintf(
        select_sql, 256, kt_point->sql_command, 
        kt_point->data_string, kt_point->table_name);

        if (!(result = mysql_return_result(sql_handler, select_sql))) {
            printf("error: %s\n", mysql_error(sql_handler));
            exit(1);
        }

        STRINDEX   *it_point = &strIndexTable[count];
        int         offset = 0;

        it_point->pointer = kt_point;

        while ((data_field = mysql_fetch_field(result))) {
            printf("Type: %d, %lu\n", data_field->type, data_field->length);

            switch(data_field->type) {
              case MYSQL_TYPE_LONG:
                if (data_field->length == 32)
                    offset += sprintf(it_point->string + offset, "%s,", "%d");

                else if (data_field->length == 64)
                    offset += sprintf(it_point->string + offset, "%s,", "%l");

                else if (data_field->length == 16)
                    offset += sprintf(it_point->string + offset, "%s,", "%d");

                else
                    offset += sprintf(it_point->string + offset, "%s,", "%c");

                break;

              case MYSQL_TYPE_TINY:
                offset += sprintf(it_point->string + offset, "%s,", "%c");
                break;

              case MYSQL_TYPE_SHORT:
                offset += sprintf(it_point->string + offset, "%s,", "%u");       
                break;

              case MYSQL_TYPE_DECIMAL:
                offset += sprintf(it_point->string + offset, "%s,", "%d");
                break;

              default:
                offset += sprintf(it_point->string + offset, "%s,", "%s");
                break;
            }
        }

        it_point->string[offset - 1] = 0;

        mysql_free_result(result);
    }
}


/*-----exceling-----*/
void exceling(MYSQL *sql_handle, const char *command, int operate_id, int n_arg, ...)
{
    MSLRES     *result;
    va_list     ap_list;
    char        excel_string[256];

    va_start(ap_list, n_arg);
    
    if (!strcmp(command, "select")) {
        STRINDEX   *in_point = &strIndexTable[operate_id];

        sprintf(excel_string, "select %s from %s", in_point->pointer->data_string, in_point->pointer->table_name);

        if (!(result = mysql_return_result(sql_handle, excel_string))) {
            printf("error: %s\n", mysql_error(sql_handle));
            exit(1);
        }

        MSLROW  data_row;

        while ((data_row = mysql_fetch_result(result))) {

        }

        mysql_free_result(result);
    }
}


/*-----data_extract-----*/
void *data_extract(MSLROW data_row)
{
    printf("%s - %s\n", data_row[0], data_row[1]);

    return  NULL;
}
