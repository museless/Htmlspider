#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.25 23:45"
__modification_time__ = "2016.03.16 00:50"
__intro__ = "It just a mysql client"


#----------------------------------------------
#            Data control config 
#----------------------------------------------

UserName = "root"
UserPassword = "WdSr0922"


#----------------------------------------------
#            database sql define 
#----------------------------------------------

# create field, like  
CreateSql = {
    1: ["", "News.Example"],
}

# field, tablename
SelectSql = {
    0: ["*", ""],
    1: ["*", "State = 0"],

    2: ["*", "Keyflags = 0"],
}

# field, insert format
InsertSql = {
    1: [u"Ind, Time, Source, Title, Url, Content",\
        "\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", \"%s\""]
}

# update data, condition
UpdateSql = {
    1: [u"State = %d", "ID = %d"],

    # relator
    2: [u"Ind = \"%s\"", "Keyflags = %d"],
}

