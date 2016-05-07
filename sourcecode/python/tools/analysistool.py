#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.05.03 22:10"
__modification_time__ = "2016.05.08 20:50"
__intro__ = "Help to analysis the output data."


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys
import modules
import xlsxcter


#----------------------------------------------
#                sort relate
#----------------------------------------------

def __sort_relate(data_map, keys):
    the_key = data_map[keys]
    save = the_key["appears"]
    the_key["appears"] = [0]

    relate = sorted(the_key,
            lambda kone, ktwo: the_key[ktwo][0] - the_key[kone][0])

    the_key["appears"] = save

    return  relate

#----------------------------------------------
#             tracing one word
#----------------------------------------------

def keyword_trace(data_map, ways):
    word = sys.argv[3]
    max_rank = 20

    if word not in data_map:
        print("%s not existed" % word)
        return

    has_appear_list = [word]

    for times in range(0, 20):
        slave = __sort_relate(data_map, word)

        for index in range(0, max_rank):
            if slave[index] not in has_appear_list:
                break

        if index == max_rank:
            break

        string = "%s/%s" % (word, slave[index])

        occupy_rate = data_map[word][slave[index]][0] / \
            float(data_map[word]["appears"])

        word = slave[index]
        has_appear_list.append(word)

        print("\t%s(%f)" % (string, occupy_rate))

#----------------------------------------------
#                get ranking 
#----------------------------------------------

def keyword_rank(data_map, ways):
    saver = xlsxcter.Exwriter()
    number = int(sys.argv[3])
    rever = (ways == "min") and True or False
    terms = {keys: data["appears"] for keys, data in data_map.iteritems()}

    rand_list = sorted(terms, 
        lambda kone, ktwo: terms[ktwo] - terms[kone], reverse = rever)

    saver.write(["排名", "关键字", "出现次数", "最大关联关键词", "关联值"])

    for index, keys in enumerate(rand_list):
        relate = __sort_relate(data_map, keys)

        saver.write([index + 1, keys, terms[keys],
            relate[0], data_map[keys][relate[0]][0]])

        if index + 1 == number:
            break
        
    saver.save("Data")

#----------------------------------------------
#                Function map
#----------------------------------------------

FunctionMap = {
    "max" : keyword_rank,
    "min" : keyword_rank,
    "trace" : keyword_trace,
}


#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    file_path = sys.argv[1]
    operate = sys.argv[2]

    if operate not in FunctionMap:
        print("Analysis tools:")
        print("Usage: [file] [max | min] [num] | [trace [word]]")
        exit()

    modules = modules.load_module(file_path)

    if modules:
        FunctionMap[operate](modules.RelationMap, operate)

