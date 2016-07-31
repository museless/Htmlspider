/*---------------------------------------------
 *   create time: 2016-07-29 17:33
-*---------------------------------------------*/

#include "sp.h"

Logmsg logMessage[] = {
    {LM0, ERR, false, "config read error", "%s"},
    {LM1, ERR, false, "mysql_connect", "%s"},
    {LM2, WARN, false, "sp_net_html_download", "%s: host: %s"},
    {LM3, ERR, true, "ubug_init_dbuf", NULL},
    {LM4, ERR, true, "mpc_thread_wake", NULL},
    {LM5, ERR, true, "mgc_add", "%s"},
    {LM6, ERR, true, "open", NULL},
    {LM7, ERR, true, "fstat", NULL},
    {LM8, ERR, true, "readn", NULL},
    {LM9, ERR, true, "sigaction", "%s"},
    {LM10, ERR, true, "mpc_create", NULL},
    {LM11, ERR, true, "wmpool_create", "%s"},
    {LM12, ERR, true, "buff_stru_init", NULL},
    {LM13, ERR, true, "ubug_pthread_apply_for_resource", "%s"},
    {LM14, ERR, true, "pthread_create", NULL},
    {LM15, ERR, true, "mmdp_malloc", NULL},
    {LM16, ERR, false, "mysql_simple_connect", "%s"},
    {LM17, ERR, true, "mmdp_create", NULL},
    {LM18, ERR, false, "strchr", "%s"},
    {LM19, CRIT, false, "configure read failed", "%s"},
    {LM20, ALERT, false, "exit by sigint", NULL},
    {LM21, ERR, false, "pthread num out of limit", "%d"},
    {LM22, ERR, false, "mysql error", "%s"},
    {LM23, WARN, false, "strange url", "%s"},
    {LM24, ERR, true, "wmpool_malloc", "%s"},
    {LM25, INFO, false, "url", "%s%s%s - size: %d"},
    {LM26, ALERT, false, "caught signal", "%s"},
    {LM27, CRIT, false, "url len out of control", "%d"},
    {LM28, NOTI, false, "value set default", "%s"},
    {LM29, ERR, true, "msem_create", NULL},
    {LM30, ERR, true, "exbug_findex_load", NULL},
    {LM31, ERR, true, "read_all_file", NULL},
};

