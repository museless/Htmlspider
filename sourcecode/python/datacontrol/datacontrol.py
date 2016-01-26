#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.25 23:45"
__modification_time__ = "2016.01.25 23:45"
__intro__ = "it was a mysql client"


#----------------------------------------------
#                  Import
#----------------------------------------------

import MySQLdb

from datacontrolconfig import *


#----------------------------------------------
#             class DataCatcher
#----------------------------------------------

class DataControl:
    #------------------------------------------
    #               Constructor
    #------------------------------------------

    def __init__(self, database_name):
        self.controler = \
        MySQLdb.connect(
        user = __db_user_name__,
        passwd = __db_user_password__, db = database_name)
            
        self.cursor = self.controler.cursor()

    #------------------------------------------
    #  execute the sql command by operate id
    #------------------------------------------

    def execute(self, operate_id): 
        if DatabaseSql.has_key(operate_id) is False:
            return  None

    #------------------------------------------
    #    execute the sql command by string
    #------------------------------------------

    def execute(self, sql_string):
        if sql_string == "":
            return  None

        cursor.execute(sql_string)

