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
               Part Zero: Include
------------------------------------------------*/

#include "sp.h"


/*----------------------------------------------
                Part One: Define
------------------------------------------------*/


/*----------------------------------------------
              Part Two: Local data
------------------------------------------------*/

char    textContent[0x100] = {
            "<html>\n<div class=\"fuck\" id=\"shit\">\n \
             \r<a>shit man</a>\n \
             \r<div class=\"haha\"> <br> </br> </div>\n \
             \r<div id=\"ohoh\" class=\"yuy\"> </div>\n \
             \r</div>\n</html>\n"
        };

char    httpContent[0x100] = {
            "HTTP/1.0 200 OK\r\n\
             \rDate: 2015.11.25 22:18\r\n\
             \rServer: NCSA/1.3\r\n\
             \rLocation: www.fucker.com\r\n\r\n"
        };


/*----------------------------------------------
           Part Three: Local function
------------------------------------------------*/

/* Part Four */
static  int     para_analysis(int argc, char **argv);
static  void    test_part(void);


/*----------------------------------------------
                Part Four: Test

                1. main
                2. para_analysis
                3. test_part

------------------------------------------------*/

/*-----main-----*/
int main(int argc, char **argv)
{
    WEBIN   web_stu;
    char    url[0x40] = "http://www.csto.com/project/list?page=1";

    if (sp_url_seperate(url, strlen(url), &web_stu.w_ubuf) != FRET_P) {
        perror("sp_url_seperate");
        return  -1;
    }

    web_stu.w_conbuf = malloc(4096);
    web_stu.w_conbufsize = 4096;

    printf("Return value: %d\n", sp_net_html_download(&web_stu));
    
    printf(
    "Host: %s\nPath: %s\nFile: %s\nPort: %d\n",
    web_stu.w_ubuf.web_host, web_stu.w_ubuf.web_path,
    web_stu.w_ubuf.web_file, web_stu.w_ubuf.web_port);

    printf("Ret code: %d\n", atoi(&web_stu.w_conbuf[9]));

    free(web_stu.w_conbuf);

    return  1;
}


/*-----para_analysis-----*/
static int para_analysis(int argc, char **argv)
{
    return  1;
}


/*-----test_part-----*/
static void test_part(void)
{

}


