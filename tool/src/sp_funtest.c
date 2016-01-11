/*----------------------------------------------
 *   modification time: 2016.01.10 16:00
 *   mender: Muse
-*----------------------------------------------*/
/*----------------------------------------------
 *   file: sp_funtest.c
 *   author: Muse
 *   creation time: 2015.11.19 21:45
-*----------------------------------------------*/

/*----------------------------------------------
 *         Source file contain Five part
 *
 *          Part Zero:  Include
 *          Part One:   Define
 *          Part Two:   Local data
 *          Part Three: Local function
 *
 *          Part Four:  Test
 *           
-*-----------------------------------------------*/

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


typedef struct format_type  FORMAT;

typedef struct string_index {
    KTBL   *pointer;

    FORMAT *format_element; 

} SIND;


struct format_type {
    FORMAT *next;
    char    type[4];
    int     len;
};


/*----------------------------------------------
 *             Part Two: Local data
-*----------------------------------------------*/

SIND    strIndexTable[256];

KTBL    sqlKeyTable[] = {
    {0, "select %s from %s limit 0", "*", "data"},
};


/*----------------------------------------------
 *          Part Three: Local function
-*-----------------------------------------------*/

static  void    create_index_table(MYSQL *sql_handler);
static  FORMAT *make_format_list(MSLRES *result);

static  void    exceling(
                MYSQL *sql_handle, 
                const char *command, int operate_id, int n_arg, ...);

static  char   *result_to_flow(SIND *format, MSLROW data_row);


/*----------------------------------------------
 *               Part Four: Test
 *
 *               1. main
 *               2. para_analysis
 *               3. test_part
 *
-*-----------------------------------------------*/

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

        strIndexTable[count].pointer = kt_point;
        strIndexTable[count].format_element = make_format_list(result);
        mysql_free_result(result);
    }
}


/*-----make_format_list-----*/
FORMAT *make_format_list(MSLRES *result)
{
    MYSQL_FIELD *data_field;
    FORMAT      *formating, **next;

    next = &formating;

    while ((data_field = mysql_fetch_field(result))) {
        printf("Type: %d, %lu\n", data_field->type, data_field->length);

        *next = malloc(sizeof(FORMAT));

        switch(data_field->type) {
          case MYSQL_TYPE_LONG:
            sprintf((*next)->type, "%s", "%l");
            (*next)->len = sizeof(long);
            break;

          case MYSQL_TYPE_TINY:
            sprintf((*next)->type, "%s", "%c");
            (*next)->len = sizeof(char);
            break;

          case MYSQL_TYPE_SHORT:
            sprintf((*next)->type, "%s", "%u");
            (*next)->len = sizeof(short);
            break;

          case MYSQL_TYPE_DECIMAL:
            sprintf((*next)->type, "%s", "%d");
            (*next)->len = sizeof(int);
            break;

          case MYSQL_TYPE_DOUBLE:
            sprintf((*next)->type, "%s", "%lf");
            (*next)->len = sizeof(double);
            break;

          default:
            sprintf((*next)->type, "%s", "%s");
            (*next)->len = data_field->length;
            break;
        }

        printf("Type: %s - len: %d\n\n", (*next)->type, (*next)->len);

        next = &(*next)->next;
    }

    (*next) = NULL;

    return  formating;
}


/*-----exceling-----*/
void exceling(
     MYSQL *sql_handle, const char *command, int operate_id, int n_arg, ...)
{
    MSLRES     *result;
    va_list     ap_list;
    char        excel_string[256];
    char       *data_flow;

    va_start(ap_list, n_arg);
    
    if (!strcmp(command, "select")) {
        SIND   *in_point = &strIndexTable[operate_id];

        sprintf(excel_string, "select %s from %s", 
        in_point->pointer->data_string, in_point->pointer->table_name);

        if (!(result = mysql_return_result(sql_handle, excel_string))) {
            printf("error: %s\n", mysql_error(sql_handle));
            exit(1);
        }

        MSLROW  data_row;

        while ((data_row = mysql_fetch_row(result)))
            data_flow = result_to_flow(in_point, data_row); 

        mysql_free_result(result);
    }
}


/*-----result_to_flow-----*/
char *result_to_flow(SIND *sind, MSLROW data_row)
{
    FORMAT *format = sind->format_element;
    int     count = 0;

    for (; format; format = format->next, count++) {
        if (!strcmp(format->type, "%d"))
            printf("Int: %d\n", atoi(data_row[count]));

        else if (!strcmp(format->type, "%l"))
            printf("Long: %ld\n", atol(data_row[count]));

        else if (!strcmp(format->type, "%s"))
            printf("String: %s\n", data_row[count]);

        else if (!strcmp(format->type, "%c"))
            printf("Tiny or Bool: %d\n", atoi(data_row[count]));

        else if (!strcmp(format->type, "%lf"))
            printf("Double: %lf\n", atof(data_row[count]));
        
        else
            printf("Don't know: %s\n", data_row[count]);
    }

    printf("\n");

    return  NULL;    
}

