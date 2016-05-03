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

from datacontrol import DataControl
from modules import load_module


#----------------------------------------------
#                Global data
#----------------------------------------------

OperateTable = "t_word_noun"


#----------------------------------------------
#           delete word from mysql
#----------------------------------------------

def delete(sql_handle, modules):
    global  OperateTable

    for terms in modules.CharDelete:
        sql_handle.delete(0, OperateTable, terms)


#----------------------------------------------
#              add word to mysql
#----------------------------------------------

def update(sql_handle, modules):
    global  OperateTable

    sql_handle.insert_ready(0, OperateTable) 

    for terms in modules.CharLeft:
        sql_handle.pre_insert(terms, len(terms), 0)

    sql_handle.final_insert()


#----------------------------------------------
#               Function map 
#----------------------------------------------

FunctionMap = {
    "update": update,
    "delete": delete,
}


#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    operate = sys.argv[1]
    file_path = sys.argv[2]

    if not FunctionMap.has_key(operate):
        print("Dictionary tools:")
        print("Usage: [update | delete] [file]")
        exit()

    modules = load_module(file_path)

    if modules:
        FunctionMap[operate](DataControl("Dictionary", "latin1"), modules)

