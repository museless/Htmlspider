#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author = "Muse"
__creation = "2016.07.27 00:00"
__modifiy = "2016.07.27 17:00"
__intro = "Log word's config"


#----------------------------------------------
#                    Macro
#----------------------------------------------

NoErrno = 0
NeedErrno = 1

#----------------------------------------------
#                Config data 
#----------------------------------------------

Logwords = [
    [ERROR, NoErrno, "ubug_print_help - open", "%d%d"],
    [WARN, NeedErrno, "ubug_print_help - open", "%d%d"],
    [CRIT, NeedErrno, "ubug_print_help - open", "%d%d"],
]
