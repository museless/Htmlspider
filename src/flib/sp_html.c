/*----------------------------------------------
    modification time: 2015.11.19 17:00
    mender: Muse
------------------------------------------------*/
/*----------------------------------------------
    file: sp_html.c
    author: Muse
    creation time: 2015.11.19 17:00
------------------------------------------------*/

/*----------------------------------------------
           Source file contain Five part

           Part Zero:  Include
           Part One:   Define
           Part Two:   Local data
           Part Three: Local function

           Part Four:  Html extract
            
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


/*----------------------------------------------
            Part Three: Local function
------------------------------------------------*/
static char *sp_html_is_tag(const char *tag_start, int tag_len);

/*----------------------------------------------
           Part Four: Html extract

           1. sp_html_code_location
           2. sp_html_tag_range_locate

------------------------------------------------*/

/*-----sp_html_code_location-----*/
char *sp_html_code_location(
      const char *html_text, int text_len, const char *html_code)
{
    return  strnstr((char *)html_text, html_code, text_len);
}


/*-----sp_html_tag_range_locate-----*/
char *sp_html_tag_range_locate(const char *html_string, int len)
{   
    int     tag_len = len;

    if (!(html_string = sp_html_get_tag_string(html_string, &tag_len)))
        return  NULL;
}


/*-----sp_html_get_tag_string-----*/
char *sp_html_get_tag_string(const char *tag_start, int *tag_len)
{
    char   *tag_end;
    int     string_len = *tag_len

    if (!(tag_end = sp_html_is_tag(tag_start, string_len)))
        return  NULL;

    tag_start += (tag_start[1] == '/') ? 2 : 1;
    string_len = (string_len > TAG_MAX_LEN) ? TAG_MAX_LEN : string_len;

    int     count;

    for (count = 0; count < string_len; count++) {
        if (!isalnum(tag_start[count]))
            break;
    }

    if (count == string_len || (tag_start[count] != '>' || tag_start[count] != ' '))
        return  NULL;

    *tag_len = string_len;

    return  tag_start;
}


/*----------------------------------------------
           Part Five: Html check

           1. sp_html_is_tag

------------------------------------------------*/

/*-----sp_html_is_tag-----*/
static char *sp_html_is_tag(const char *tag_start, int tag_len)
{
    if (tag_len < TAG_MIN_LEN || *tag_start != '<') {
        errno = EINVAL;
        return  FUN_RUN_END;
    }

    return  FUN_RUN_OK;
}

