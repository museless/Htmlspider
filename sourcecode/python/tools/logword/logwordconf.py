#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

_author = "Muse"
_creation = "2016.07.27 00:00"
_modifiy = "2016.07.29 13:25"
_intro = "Log word's config"


#----------------------------------------------
#                    Macro
#----------------------------------------------

EM = "EMERG"
AL = "ALERT"
CR = "CRIT"
ER = "ERR"
WA = "WARN"
NO = "NOTI"
IN = "INFO"
DE = "DEBUG"

#----------------------------------------------
#                Config data 
#----------------------------------------------

Logwords = [
    [ER, 0, "url spider config read error", "%s"],
    [ER, 0, "mysql_connect", "%s"],
    [ER, 0, "sp_net_html_download", "%s: host: %s"],
    [ER, 1, "ubug_init_dbuf", ""],
    [ER, 1, "mpc_thread_wake", ""],
    [ER, 1, "mgc_add", "%s"],
    [ER, 1, "open", ""],
    [ER, 1, "fstat", ""],
    [ER, 1, "readn", ""],
    [ER, 1, "sigaction", "%s"],
    [ER, 1, "mpc_create", ""],
    [ER, 1, "wmpool_create", "%s"],
    [ER, 1, "buff_stru_init", ""],
    [ER, 1, "ubug_pthread_apply_for_resource", "%s"],

    [ER, 1, "pthread_create", ""],
    [ER, 1, "mmdp_malloc", ""],
    [ER, 0, "mysql_simple_connect", "%s"],
    [ER, 1, "mmdp_create", ""],
    [ER, 0, "strchr", "%s"],
]
