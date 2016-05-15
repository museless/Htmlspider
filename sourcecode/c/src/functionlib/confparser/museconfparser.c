/*---------------------------------------------
 *   modification time: 2016.05.15 23:17
 *   mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *   file: museconfparser.c
 *   author: Muse
 *   creation time: 2015.11.19 17:00
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file contain Five part
 *
 *          Part Zero:  Include
 *          Part One:   Define
 *          Part Two:   Local data
 *          Part Three: Local function
 *
 *          Part Four:  Configure file control
 *           
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "spinc.h"
#include <sys/user.h>


/*---------------------------------------------
 *           Part Two: Local data
-*---------------------------------------------*/

static  int     confStoreFd;
static  BUFF    confctlBuff;
static  char    userName[MIDDLE_BUF + 1];


/*---------------------------------------------
 *      Part Four: Configure file control
 *
 *      1. mc_conf_load
 *      2. mc_conf_read
 *      3. mc_conf_unload
 *      4. mc_conf_print_err
 *      5. mc_load_config
 *      6. mc_conf_read_list
 *
-*---------------------------------------------*/

/*-----mc_conf_load-----*/
int mc_conf_load(const char *pUser, const char *confPath)
{
    struct  stat    stBuf;

    sprintf(userName, "%.*s", MIDDLE_BUF, pUser);

    if ((confStoreFd = open(confPath, O_RDWR)) == FRET_N)
        return  FRET_N;

    if (fstat(confStoreFd, &stBuf) == FRET_N)
        return  FRET_N;

    confctlBuff.b_size = confctlBuff.b_cap = stBuf.st_size + 1;

    if ((confctlBuff.b_start = malloc(confctlBuff.b_cap)) == NULL)
        return  FRET_N;

    if (readn(confStoreFd, confctlBuff.b_start, stBuf.st_size) == FRET_N)
        return  FRET_N;

    ((char *)confctlBuff.b_start)[stBuf.st_size] = 0;

    return  FUN_RUN_OK;
}


/*-----mc_conf_read-----*/
int mc_conf_read(char *findStr, int dType, void *dBuf, int dLen)
{
    char   *pStr, *pEnd;
    int     nMul = 0;

    if (!(pStr = strstr(confctlBuff.b_start, findStr)))
        return  FRET_N;

    if ((pStr = strchr(pStr, '=')) == NULL)
        return  FRET_N;

    for (pStr += 1; isspace(*pStr); pStr++)
        ;   /* nothing */

    if ((pEnd = strchr(pStr, '\n')) == NULL)
        return  FRET_N;

    for (; isspace(*(pEnd - 1)); pEnd--)
            ;   /* nothing */

    if (dType == CONF_NUM) {
        for (; isalpha(*pEnd); pEnd--)
            ;   /* nothing */

        pEnd++;

        if (*pEnd == 'M'|| *pEnd == 'm')
            nMul = 20;
        else if (*pEnd == 'K'|| *pEnd == 'k')
            nMul = 10;

        *(int *)dBuf = ((int)strtol(pStr, NULL, 0)) << nMul;

    } else if (dType == CONF_STR) {
        dLen = (((pEnd - pStr) > dLen) ? dLen : pEnd - pStr);
        strncpy((char *)dBuf, pStr, dLen);
        ((char *)dBuf)[dLen] = 0;

    } else {
        return  FRET_N;
    }

    return  FUN_RUN_OK;
}


/*-----mc_conf_unload-----*/
void mc_conf_unload(void)
{
    if (confStoreFd < FUN_RUN_END)
        close(confStoreFd);
    
    if (confctlBuff.b_start)
        free(confctlBuff.b_start);
}


/*-----mc_conf_print_err-----*/
void mc_conf_print_err(char *pFind)
{
    printf("%s - configure read [%s] failed\n", userName, pFind);
}



/*-----mc_load_config-----*/
void mc_load_config(const char *user, const char *config_path)
{
    if (mc_conf_load(user, config_path) == FRET_N) {
        printf("%s---> load configure failed\n", user);
        perror("mc_load_config - mc_conf_load");
        mc_conf_unload();
        exit(FRET_N);
    }
}


/*-----mc_conf_read_list-----*/
void mc_conf_read_list(PerConfData *list, int len)
{
    for (int index = 0; index < len; index++) {
        PerConfData *per = &list[index];

        if (mc_conf_read(per->find_str, 
                per->data_type, per->data_buff, per->data_max_len) == FRET_N) {
            mc_conf_print_err(per->find_str);

            if (per->data_max_len > 0) {
                memset(per->data_buff, 0, per->data_max_len);
                printf("set default to zero\n\n");
            }
        }
    }
}

