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
import xlsxcter


#----------------------------------------------
#                get ranking 
#----------------------------------------------

def keyword_rank(module, ways):
    saver = xlsxcter.Exwriter()
    number = int(sys.argv[3])
    rever = (ways == "min") and True or False
    data_map = module.RelationMap
    terms = {keys: data["appears"] for keys, data in data_map.iteritems()}

    rand_list = sorted(terms, 
        lambda kone, ktwo: terms[ktwo] - terms[kone], reverse = rever)

    saver.write(["排名", "关键字", "出现次数", "最大关联关键词", "关联值"])

    for index, keys in enumerate(rand_list):
        the_key = data_map[keys]
        the_key["appears"] = [0]

        relate = sorted(the_key,
            lambda kone, ktwo: the_key[ktwo][0] - the_key[kone][0])

        saver.write([index + 1, keys, terms[keys],
            relate[0], the_key[relate[0]][0]])

        if index + 1 == number:
            break
        
    saver.save("Data")

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

