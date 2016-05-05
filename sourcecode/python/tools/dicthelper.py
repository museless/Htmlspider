#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.04.24 23:20"
__modification_time__ = "2016.05.03 22:05"
__intro__ = "Dictionary's adjust tool"


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys

sys.path.append("../")

import imp
import helperconf

from datacontrol import DataControl
from modules import load_module
from collections import namedtuple


#----------------------------------------------
#       rearrange the outside dictionary
#----------------------------------------------

def arrange(sql_handle, modules):
    chars_file = helperconf.Terms
    max_len = helperconf.TermsMaxLen + 1
    keyword = namedtuple("Keyword", ["Terms", "Length", "Flags"])

    for length in range(2, max_len):
        sql_handle.select(3, helperconf.OperateTable, -1, length) 
        results = sql_handle.cursor.fetchall()

        for data_row in results:
            per_row = keyword(*data_row)     
            print(str(per_row).decode("string_escape"))

#----------------------------------------------
#           delete word from mysql
#----------------------------------------------

def delete(sql_handle, modules):
    for terms in modules.CharDelete:
        sql_handle.delete(0, helperconf.OperateTable, terms)


#----------------------------------------------
#              add word to mysql
#----------------------------------------------

def update(sql_handle, modules):
    sql_handle.insert_ready(0, helperconf.OperateTable) 

    for terms in modules.CharLeft:
        sql_handle.pre_insert(terms, len(terms), 0)

    sql_handle.final_insert()


#----------------------------------------------
#               Function map 
#----------------------------------------------

FunctionMap = {
    "update": update,
    "delete": delete,
    "arrange" : arrange
}


#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    operate = sys.argv[1]

    if not FunctionMap.has_key(operate):
        print("Dictionary tools:")
        print("Usage: [[update | delete] [file]] [arrange]")
        exit()

    modules = None

    if operate in ["update", "delete"]:
        file_path = sys.argv[2]
        modules = load_module(file_path)

    FunctionMap[operate](DataControl("Dictionary", "latin1"), modules)

