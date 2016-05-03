#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.05.03 22:10"
__modification_time__ = "2016.05.03 22:10"
__intro__ = "Help to analysis the output data."


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys
import modules


#----------------------------------------------
#                get ranking 
#----------------------------------------------

def keyword_rank(module, ways):
    number = int(sys.argv[3])
    rever = (ways == "min") and True or False
    terms = {keys: data["appears"] \
             for keys, data in module.RelationMap.iteritems()}

    rand_list = sorted(terms, 
        lambda kone, ktwo: terms[ktwo] - terms[kone], reverse = rever)

    for index, keys in enumerate(rand_list):
        print("%d. %s %d" % (index + 1, keys, terms[keys]))

        if index + 1 == number:
            break
        

#----------------------------------------------
#                Function map
#----------------------------------------------

FunctionMap = {
    "max" : keyword_rank,
    "min" : keyword_rank,
}


#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    file_path = sys.argv[1]
    operate = sys.argv[2]

    if operate not in FunctionMap:
        print("Analysis tools:")
        print("Usage: [file] [max | min] [num] | [relate [word]]")
        exit()

    modules = modules.load_module(file_path)

    if modules:
        FunctionMap[operate](modules, operate)

