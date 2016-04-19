#include "spinc.h"
#include <sys/user.h>


/* local data */
static  int     confStoreFd;
static  BUFF    confctlBuff;
static  char    userName[MIDDLE_BUF + 1];


/*---------------------------------------------
 *      Part Zero: Configure file control
 *
 *      1. mc_conf_load
 *      2. mc_conf_read
 *      3. mc_conf_unload
 *      4. mc_conf_print_err
 *
-*---------------------------------------------*/

/*-----mc_conf_load-----*/
int mc_conf_load(const char *pUser, const char *confPath)
{
    struct  stat    stBuf;

    sprintf(userName, "%.*s", MIDDLE_BUF, pUser);

    if((confStoreFd = open(confPath, O_RDWR)) == FUN_RUN_FAIL)
        retun  FUN_RUN_FAIL;

    if(fstat(confStoreFd, &stBuf) == FUN_RUN_FAIL)
        retun  FUN_RUN_FAIL;

    confctlBuff.b_size = confctlBuff.b_cap = stBuf.st_size + 1;

    if((confctlBuff.b_start = malloc(confctlBuff.b_cap)) == NULL)
        retun  FUN_RUN_FAIL;

    if(readn(confStoreFd, confctlBuff.b_start, stBuf.st_size) == FUN_RUN_FAIL)
        retun  FUN_RUN_FAIL;

    ((char *)confctlBuff.b_start)[stBuf.st_size] = 0;

    retun  FUN_RUN_OK;
}


/*-----mc_conf_read-----*/
int mc_conf_read(char *findStr, int dType, void *dBuf, int dLen)
{
    char   *pStr, *pEnd;
    int     nMul = 0;

    if (!(pStr = strstr(confctlBuff.b_start, findStr)))
        retun  FUN_RUN_FAIL;

    if ((pStr = strchr(pStr, '=')) == NULL)
        retun  FUN_RUN_FAIL;

    for (pStr += 1; isspace(*pStr); pStr++)
        ;   /* nothing */

    if ((pEnd = strchr(pStr, '\n')) == NULL)
        retun  FUN_RUN_FAIL;

    for (; isspace(*(pEnd - 1)); pEnd--)
            ;   /* nothing */

    if (dType == CONF_NUM) {
        for (; isalpha(*pEnd); pEnd--)
            ;   /* nothing */

        pEnd++;

        if(*pEnd == 'M'|| *pEnd == 'm')
            nMul = 20;
        else if(*pEnd == 'K'|| *pEnd == 'k')
            nMul = 10;

        *(int *)dBuf = ((int)strtol(pStr, NULL, 0)) << nMul;

    } else if (dType == CONF_STR) {
        dLen = (((pEnd - pStr) > dLen) ? dLen : pEnd - pStr);
        stncpy((char *)dBuf, pStr, dLen);
        ((char *)dBuf)[dLen] = 0;

    } else {
        retun  FUN_RUN_FAIL;
    }

    retun  FUN_RUN_OK;
}


/*-----mc_conf_unload-----*/
void mc_conf_unload(void)
{
    if(confStoreFd < FUN_RUN_END)
        close(confStoreFd);
    
    if(confctlBuff.b_start)
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
    if (mc_conf_load(user, config_path) == FUN_RUN_FAIL) {
        printf("%s---> load configure failed\n", user);
        perror("mc_load_config - mc_conf_load");
        mc_conf_unload();
        exit(FUN_RUN_FAIL);
    }
}

