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


/*----------------------------------------------
 *             Part Two: Local data
-*----------------------------------------------*/


/*----------------------------------------------
           Part Three: Local function
------------------------------------------------*/

static  void   *data_extract(MSLROW data_row);


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
    
    MSLRES *result;

    if (!(result = mysql_return_result(&sql_handler, "select * from data"))) {
        printf("Orm---> %s - mysql_return_result failed\n", __func__);
        printf("error: %s\n", mysql_error(&sql_handler));
        exit(1);
    }

    MSLROW  data_row;
    char   *data_chunk;

    while ((data_row = mysql_fetch_row(result))) {
        if (!(data_chunk = data_extract(data_row))) {

        }
    }

    mysql_free_result(result);
    mysql_close(&sql_handler);

    exit(-1);
}



/*-----data_extract-----*/
void *data_extract(MSLROW data_row)
{
    printf("%s - %s\n", data_row[0], data_row[1]);

    return  NULL;
}
