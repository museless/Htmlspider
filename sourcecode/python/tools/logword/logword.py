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
import sys


#----------------------------------------------
#                 make file
#----------------------------------------------

def make_file():
    messages = logwordconf.Logwords
    idname_list, msg_list = [], []

    for idx, msg in enumerate(messages):
        idname = "LM" + str(idx)

        idname_list.append("    %s," % idname)
        msg_list.append("    {%s, %s, %s, \"%s\", %s}," % (
            idname, msg[0], (msg[1] == 1) and "true" or "false", msg[2], 
            (isinstance(msg[3], str) and len(msg[3]) > 0) 
             and "\"%s\"" % msg[3] or "NULL"))
    
    print("Logmsg  logMessage[] = {\n%s\n}\n\n" % ("\n".join(msg_list)))
    print("enum messageEnum = {\n%s\n\n    LMMax,\n}" % ("\n".join(idname_list)))

#----------------------------------------------
#                   Main 
#----------------------------------------------

def main(argv = sys.argv):
    make_file()


if __name__ == "__main__":
    main()
