#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.25 23:45"
__modification_time__ = "2016.01.25 23:45"
__intro__ = "it was a mysql client"


#----------------------------------------------
#            Data control config 
#----------------------------------------------

__db_user_name__ = "root"
__db_user_password__ = "WdSr0922"


#----------------------------------------------
#            database sql define 
#----------------------------------------------

CATCH_URL = 1

DatabaseSql = {
    CATCH_URL : "select * from %s where State = 0 limit %d",
}




