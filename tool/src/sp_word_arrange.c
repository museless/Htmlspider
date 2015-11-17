#include "spinc.h"


/* local function */
static  void    hcheck_start(char *content);


/*-----main-----*/
int main(void)
{
    char   *content_save;

    if (read_all_file(
        &content_save, "/MuseSp/dict/character", 0) == FUN_RUN_FAIL) {
        perror("HeadCheck---> main - read_all_file");
        return  -1;
    }

    hcheck_start(content_save);

    return  FUN_RUN_OK;
}


/*-----hcheck_start-----*/
static void hcheck_start(char *content)
{
    char    *ahead_word, *current_word;

    ahead_word = content;

    while (FUN_RUN_OK) {
        if (!(current_word = strchr(ahead_word, '\n')))
            break;

        current_word += 1;

        if (strncmp(ahead_word, current_word, 3) >= 0) {
            printf("%.3s >= %.3s\n", ahead_word, current_word);
            break;
        }

        ahead_word = current_word;
    }
}

