/*---------------------------------------------
 * author: Muse
 * create: 2016.05.15 23:00
 * modify: 2016.05.15 23:00
 * intro:  config parser's header file
-*---------------------------------------------*/

#pragma once


/*---------------------------------------------
 *                  define 
-*---------------------------------------------*/ 

/* configure read setting */
#define CONF_STR    0x1
#define CONF_NUM    0x2


/*---------------------------------------------
 *                 typedef 
-*---------------------------------------------*/ 

typedef struct perconfdata  PerConfData;


/*---------------------------------------------
 *                  struct
-*---------------------------------------------*/ 

struct perconfdata {
    char   *find_str;
    int     data_type;
    void   *data_buff;
    int     data_max_len;
    char   *default_str;
};


/*---------------------------------------------
 *                interface
-*---------------------------------------------*/ 

int     mc_conf_load(const char *pUser, const char *confPath);
void	mc_conf_unload(void);
bool    mc_conf_read(char *findStr, int dType, void *dBuf, int dLen);
void	mc_conf_print_err(const char *pFind);

void    mc_load_config(const char *user, const char *config_path);
void    mc_conf_read_list(PerConfData *list, int len);

