#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.05.03 22:10"
__modification_time__ = "2016.05.03 22:10"
__intro__ = "Used to import module"


#----------------------------------------------
#                  Import
#----------------------------------------------

import imp
import sys


#----------------------------------------------
#                load module
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

