#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

_author = "Muse"
_creation = "2016.07.29 14:30"
_modifiy = "2016.07.29 14:30"
_intro = "Log word's output"


#----------------------------------------------
#                   Import 
#----------------------------------------------

import logwordconf
import time
import sys


#----------------------------------------------
#                   data
#----------------------------------------------

Head = 45 * '-'


#----------------------------------------------
#                 open file
#----------------------------------------------

def openfile(suffix):
    f = open("logmsgdef." + suffix, "w")
    f.write("/*%s\n *   create time: %s\n-*%s*/\n\n" % 
        (Head, time.strftime("%Y-%m-%d %H:%M"), Head))

    return  f


#----------------------------------------------
#                 make file
#----------------------------------------------

def make_file():
    messages = logwordconf.Logwords
    idname_list, msg_list = [], []

    header, csrc = openfile("h"), openfile("c")

    for idx, msg in enumerate(messages):
        idname = "LM" + str(idx)

        idname_list.append("    %s," % idname)
        msg_list.append("    {%s, %s, %s, \"%s\", %s}," % (
            idname, msg[0], (msg[1] == 1) and "true" or "false", msg[2], 
            (isinstance(msg[3], str) and len(msg[3]) > 0) 
             and "\"%s\"" % msg[3] or "NULL"))
    
    header.write("#pragma once\n\nenum messageEnum = {\n%s\n\n    LMMax,\n}" % 
        ("\n".join(idname_list)))

    csrc.write("Logmsg logMessage[] = {\n%s\n}\n\n" % ("\n".join(msg_list)))

    header.close()
    csrc.close()


#----------------------------------------------
#                   Main 
#----------------------------------------------

def main(argv = sys.argv):
    make_file()


if __name__ == "__main__":
    main()
