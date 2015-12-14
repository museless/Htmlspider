/*----------------------------------------------
    modification time: 2015.11.19 21:20
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

/* Part Five */
static  int     sp_html_is_tag(const char *tag_start, int tag_len);
static  int     sp_html_is_tag_end(const char *tag_string);


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
    char    tag_buff[TAG_MAX_LEN];
    int     tag_len = len;

    if (!(html_string = 
          sp_html_get_tag_string((char *)html_string, &tag_len)))
        return  NULL;

    snprintf(tag_buff, TAG_MAX_LEN, "%.*s", tag_len, html_string);

    char   *tag_end;
    int     ret, counter = 1;

    do {
        html_string += tag_len;
        len -= tag_len;

        if (!(tag_end = sp_html_code_location(html_string, len, tag_buff)))
            return  NULL;

        if ((ret = sp_html_is_tag_end(tag_end)) == FUN_RUN_FAIL)
            return  NULL;

        counter += ((ret) ? -1 : 1);

        len -= (tag_end - html_string);
        html_string = tag_end;

    } while (counter);

    return  tag_end - 2; 
}


/*-----sp_html_get_tag_string-----*/
char *sp_html_get_tag_string(char *tag_start, int *tag_len)
{
    int     string_len = *tag_len;

    if (!(sp_html_is_tag(tag_start, string_len)))
        return  NULL;

    tag_start += (tag_start[1] == '/') ? 2 : 1;
    string_len = (string_len > TAG_MAX_LEN) ? TAG_MAX_LEN : string_len;

    int     count;

    for (count = 0; count < string_len; count++) {
        if (!isalnum(tag_start[count]))
            break;
    }

    if (count == string_len || (tag_start[count] != '>' && tag_start[count] != ' ')) {
        errno = EINVAL;
        return  NULL;
    }

    *tag_len = count;

    return  tag_start;
}


/*----------------------------------------------
           Part Five: Html check

           1. sp_html_is_tag
           2. sp_html_is_tag_end

------------------------------------------------*/

/*-----sp_html_is_tag-----*/
static int sp_html_is_tag(const char *tag_start, int tag_len)
{
    if (tag_len < TAG_MIN_LEN || *tag_start != '<') {
        errno = EINVAL;
        return  FUN_RUN_END;
    }

    return  FUN_RUN_OK;
}


/*-----sp_html_is_tag_end-----*/
static int sp_html_is_tag_end(const char *tag_string)
{
    if (*(tag_string - 1) == '<')
        return  FUN_RUN_END;

    if (!strncmp(tag_string - 2, "</", 2))
        return  FUN_RUN_OK;  

    return  FUN_RUN_FAIL;
}

