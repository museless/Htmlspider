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

UserName = "root"
UserPassword = "WdSr0922"


#----------------------------------------------
#            database sql define 
#----------------------------------------------

CreateSql = {
    1: ["", "News.Example"],
}

SelectSql = {
    0: ["*", ""],
    1: ["*", "State = 0"],
}

InsertSql = {
    1: ["Ind, Time, Source, Title, Url, Content",\
        "\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\""]
}

UpdateSql = {
    1: ["State = %d", "ID = %d"],
}

