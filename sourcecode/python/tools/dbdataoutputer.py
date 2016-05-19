#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.05.19 14:30"
__modification_time__ = "2016.05.19 14:30"
__intro__ = "Help to output the database data."


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys
import xlsxcter


#----------------------------------------------
#                Function map
#----------------------------------------------

FunctionMap = {
    "last" : output_last,
}


#----------------------------------------------
#                 print help
#----------------------------------------------

def help_and_exit():
    print("Database output tools:")
    print("Usage: [time] [last]")
    exit()


#----------------------------------------------
#                main function
#----------------------------------------------

def main(argv = sys.argv):
    if len(argv) < 3:
        help_and_exit()

    time_str = argv[1]
    operate = argv[2]

    if operate not in FunctionMap:
        help_and_exit()

    FunctionMap[operate](time_str)


#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    main()

