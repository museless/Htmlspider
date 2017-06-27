# -*- coding:utf8 -*-

"""
author: Muse
creation: 2016.01.25 23:45
modification: 2016.05.19 14:25
intro: The mysql client
"""


import MySQLdb
import thread

from datacontrolconfig import * 


class DataControl(object):
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

    def __init__(self, database, charseting = "utf8", max_packet = 0):
        self.controler = \
            MySQLdb.connect(user = UserName, passwd = UserPassword,
                db = database, charset = charseting, 
                unix_socket = "/tmp/mysql.sock")
            
        self.cursor = self.controler.cursor()

        self.exec_lock = thread.allocate_lock()
        self.insert_lock = thread.allocate_lock()

        self._init_max_packet(max_packet)

    def __del__(self):
        self.insert_lock.acquire()
        self.final_insert()
        self.insert_lock.release()

    def _init_max_packet(self, max_packet):
        self.max_packet = max_packet

        if max_packet == 0:
            self._execute("show variables like '%max_allowed_packet%'")
            result = self.cursor.fetchall()

            self.max_packet = int(result[0][1])

    def select(self, operate_id, tabname_and_order, limit = -1, *parameters):
        if SelectSql.has_key(operate_id) == False:
            return  None

        select = "select %s from %s " % \
                 (SelectSql[operate_id][self.FIELD_INDEX], tabname_and_order)

        if SelectSql[operate_id][self.CONDITION_INDEX] != "":
            select += "where %s " % SelectSql[operate_id][self.CONDITION_INDEX]
        
            if len(parameters) != 0:
                select = select % parameters 

        if limit != -1:
            select += "limit %d" % limit

        return  self._execute(select)

    def create(self, operate_id, table_name):
        if CreateSql.has_key(operate_id) == False:
            return  False 

        if CreateSql[operate_id][self.LIKE_INDEX] != "":
            create = "create table if not exists %s like %s" % (
                     table_name, CreateSql[operate_id][self.LIKE_INDEX])

        elif CreateSql[operate_id][self.CREATE_FIELD_INDEX] != "":
            create = "create table if not exists %s(%s)" % (
                     table_name, CreateSql[operate_id][self.CREATE_FIELD_INDEX])
        else:
            return  False
    
        self._execute(create)
        return  True

    def insert_ready(self, operate_id, table_name):
        if InsertSql.has_key(operate_id) == False:
            return  None

        if not isinstance(InsertSql[operate_id], list) or \
                len(InsertSql[operate_id]) < 2:
            return  None

        field_str = InsertSql[operate_id][self.INSERT_FIELD_INDEX]

        self.insert_head = "insert ignore %s" % table_name
        self.insert_head += (field_str == "") and " values" or \
            "(%s) values" % field_str 

        self.insert_field = u"(%s)" % \
            InsertSql[operate_id][self.INSERT_FORMAT_INDEX]

        self.insert_total_len = 0
        self.insert_buff = []

    def pre_insert(self, *parameters):
        """
        use it insert many data, per data store in insert buffer
        but if data bigger than max_packet, insert

        """
        if self.insert_head == "" or self.insert_field == "":
            return  None

        data = self.insert_field % parameters
        length = len(data) * 3

        self.insert_lock.acquire()

        if self.insert_total_len + length > self.max_packet - 256:
            self.final_insert()
            self.insert_total_len = 0

        self.insert_buff.append(data)
        self.insert_total_len += length
        self.insert_lock.release()
 
    def final_insert(self):
        """
        insert data store in insert buff

        """
        if self.insert_buff == [] or "" or self.insert_field == "":
            return  None

        self._execute(u"%s%s" % (self.insert_head, ",".join(self.insert_buff)))
        self.insert_buff = []

    def update(self, operate_id, table_name, *parameters):
        if UpdateSql.has_key(operate_id) == False:
            return  None

        update_string = "update %s set %s " % \
            (table_name, UpdateSql[operate_id][self.UPDATE_SET_INDEX])

        if UpdateSql[operate_id][self.UPDATE_LIMIT_INDEX] != "":
            update_string += \
            "where %s" % UpdateSql[operate_id][self.UPDATE_LIMIT_INDEX]

        self._execute(update_string % parameters)

    def delete(self, operate_id, table_name, *parameters):
        if DeleteSql.has_key(operate_id) == False or \
           not isinstance(DeleteSql[operate_id], (str, unicode)) or \
           DeleteSql[operate_id] == "":
            return  False

        delete_string = "delete from %s where %s" % \
            (table_name, DeleteSql[operate_id])

        self._execute(delete_string % parameters)

    def _execute(self, sql_string):
        if len(sql_string) == 0:
            return  None

        self.exec_lock.acquire()
        result = self.cursor.execute(sql_string.encode("utf8"))
        self.exec_lock.release()

        return  result 

    def escaping(self, sql_string):
        sql_string = sql_string.replace('"', '\\"')
        sql_string = sql_string.replace("'", "\\'")

        return  sql_string

