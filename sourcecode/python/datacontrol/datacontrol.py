# -*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.25 23:45"
__modification_time__ = "2016.03.19 17:25"
__intro__ = "it was a mysql client"


#----------------------------------------------
#                  Import
#----------------------------------------------

import MySQLdb
import thread

from datacontrolconfig import * 


#----------------------------------------------
#             class DataCatcher
#----------------------------------------------

class DataControl:
    # insert data
    insert_buff = []
    insert_head = ""
    insert_field = ""

    # select data index
    FIELD_INDEX = 0
    CONDITION_INDEX = 1

    # create data index
    CREATE_FIELD_INDEX = 0
    LIKE_INDEX = 1

    # insert data index
    INSERT_FIELD_INDEX = 0
    INSERT_FORMAT_INDEX = 1

    # update data index
    UPDATE_SET_INDEX = 0
    UPDATE_LIMIT_INDEX = 1

    #------------------------------------------
    #               Constructor
    #------------------------------------------

    def __init__(self, database_name, charseting = "utf8"):
        self.controler = \
            MySQLdb.connect(
            user = UserName, passwd = UserPassword, 
            db = database_name, charset = charseting, 
            unix_socket="/tmp/mysql.sock")
            
        self.cursor = self.controler.cursor()

        self.exec_lock = thread.allocate_lock()
        self.insert_lock = thread.allocate_lock()

    #------------------------------------------
    #               Destructor 
    #------------------------------------------

    def __del__(self):
        pass

    #------------------------------------------
    #        joint to the select string 
    #------------------------------------------

    def select(self, operate_id, table_name, limit = -1):
        if SelectSql.has_key(operate_id) == False:
            return  None

        select = "select %s from %s " % \
                 (SelectSql[operate_id][self.FIELD_INDEX], table_name)

        if SelectSql[operate_id][self.CONDITION_INDEX] != "":
            select += "where %s " % SelectSql[operate_id][self.CONDITION_INDEX]

        if limit != -1:
            select += "limit %d" % limit

        return  self.__execute(select)

    #------------------------------------------
    #        joint to the create string 
    #------------------------------------------

    def create(self, operate_id, table_name):
        if CreateSql.has_key(operate_id) == False:
            return  None

        if CreateSql[operate_id][self.LIKE_INDEX] != "":
            create = "create table if not exists %s like %s" % (
                     table_name, CreateSql[operate_id][self.LIKE_INDEX])

        elif CreateSql[operate_id][self.CREATE_FIELD_INDEX] != "":
            create = "create table if not exists %s(%s)" % (
                     table_name, CreateSql[operate_id][self.CREATE_FIELD_INDEX])
        else:
            return  None
    
        self.__execute(create)

    #------------------------------------------
    #       joint to the insert sql head 
    #------------------------------------------

    def insert_ready(self, operate_id, table_name):
        if InsertSql.has_key(operate_id) == False:
            return  None

        self.insert_head = \
        "insert ignore %s(%s) values" % \
        (table_name, InsertSql[operate_id][self.INSERT_FIELD_INDEX])

        self.insert_field = \
        u"(%s)" % InsertSql[operate_id][self.INSERT_FORMAT_INDEX]
 
    #------------------------------------------
    #          insert to insert list 
    #------------------------------------------

    def pre_insert(self, *parameters):
        if self.insert_head == "" or self.insert_field == "":
            return  None

        self.insert_lock.acquire()
        self.insert_buff.append(self.insert_field % parameters)
        self.insert_lock.release()
 
    #------------------------------------------
    #        final insert to the mysql 
    #------------------------------------------
 
    def final_insert(self):
        if self.insert_buff == [] or \
           self.insert_head == "" or self.insert_field == "":
            return  None

        self.__execute(
        u"%s%s" % (self.insert_head, ",".join(self.insert_buff)))

        self.insert_buff = []

    #------------------------------------------
    #        joint to the update string 
    #------------------------------------------

    def update(self, operate_id, table_name, *parameters):
        if UpdateSql.has_key(operate_id) == False:
            return  None

        update_string = \
        "update %s set %s " % \
        (table_name, UpdateSql[operate_id][self.UPDATE_SET_INDEX])

        if UpdateSql[operate_id][self.UPDATE_LIMIT_INDEX] != "":
            update_string += \
            "where %s" % UpdateSql[operate_id][self.UPDATE_LIMIT_INDEX]

        self.__execute(update_string % parameters)

    #------------------------------------------
    #    execute the sql command by string
    #------------------------------------------

    def __execute(self, sql_string):
        if sql_string == "":
            return  None

        self.exec_lock.acquire()
        result = self.cursor.execute(sql_string.encode("utf8"))
        self.exec_lock.release()

        return  result 

    #------------------------------------------
    #             string escaping
    #------------------------------------------

    def escaping(self, sql_string):
        sql_string = sql_string.replace('"', '\\"')
        sql_string = sql_string.replace("'", "\\'")

        return  sql_string

