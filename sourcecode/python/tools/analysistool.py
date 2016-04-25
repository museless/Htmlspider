#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.04.24 23:20"
__modification_time__ = "2016.04.24 23:20"
__intro__ = "Dataanalysis's tool"


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys

sys.path.append("../")

import imp

from datacontrol import DataControl


#----------------------------------------------
#               load module
#----------------------------------------------

def load_module(path):
    with open(path, "r") as file_desc:
        source = file_desc.read()
        module = sys.modules.setdefault(path, imp.new_module(path))

        code = compile(source, path, "exec")
        module.__file__ = path
        module.__package__ = ""

        exec(source, module.__dict__)

        return  module

    return  None

#----------------------------------------------
#           delete word from mysql
#----------------------------------------------

def delete_word(sql_handle, modules):
    for terms in modules.CharDelete:
        print(terms)

#----------------------------------------------
#              add word to mysql
#----------------------------------------------

def add_word(sql_handle, modules):
    pass


#----------------------------------------------
#                   Data
#----------------------------------------------

FunctionMap = {
    "add": add_word,
    "del": delete_word,
}


#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    operate = sys.argv[1]
    file_path = sys.argv[2]

    modules = load_module(file_path)
    sql_handle = None

    if modules:
        delete_word(sql_handle, modules)

