#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.05.03 22:10"
__modification_time__ = "2016.05.12 01:30"
__intro__ = "Help to analysis the output data."


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys
import modules
import xlsxcter
import analysisconf

from modules import load_module


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

def _trace(data_map, word, io_write):
    max_rank, has_appear_list = 20, [word]

    io_write("股票: %s\n" % word)

    for times in range(0, 10):
        slave = __sort_relate(data_map, word)

        for index in range(0, max_rank):
            if slave[index] not in has_appear_list:
                break

        string = "%s / %s" % (word, slave[index])

        occupy_rate = data_map[word][slave[index]][0] / \
            float(data_map[word]["appears"])

        word = slave[index]
        has_appear_list.append(word)

        io_write("\t%s (%f)\n" % (string, occupy_rate))


def keyword_trace(data_map, ways):
    word = sys.argv[3]

    if word not in data_map:
        print("%s not existed" % word)
        return

    _trace(data_map, word, sys.stdout.write)

#----------------------------------------------
#              save some stock
#----------------------------------------------

def save_some_stock(data_map, sort_list, limit = 1000):
    stocks = load_module(analysisconf.StockNameSave)
    data_write = open("Stock", "w")

    for index, terms in enumerate(sort_list):
        if index == limit:
            break

        if terms.decode("utf8") in stocks.StockName:
            _trace(data_map, terms, data_write.write)
            data_write.write("\n\n")

#----------------------------------------------
#                get ranking 
#----------------------------------------------

def sort_terms(terms, way):
    return  sorted(terms, lambda fi, se: terms[se] - terms[fi], reverse = way)

def keyword_rank(data_map, ways):
    saver = xlsxcter.Exwriter()
    number = int(sys.argv[3])
    last_module = load_module(sys.argv[4])
    last_map = last_module.RelationMap

    rever = (ways == "min") and True or False
    terms = {keys: data["appears"] for keys, data in data_map.iteritems()}
    lasts = {keys: data["appears"] for keys, data in last_map.iteritems()}

    last_sorted = sort_terms(lasts, False)
    rand_list = sort_terms(terms, rever)

    saver.write(["排名", "关键字", "出现次数",
        "昨日排名对比", "最大关联关键词", "关联值"])

    for index, keys in enumerate(rand_list):
        relate = __sort_relate(data_map, keys)

        diff = (keys in last_sorted) and \
            last_sorted.index(keys) - index or "新词"

        saver.write([index + 1, keys, terms[keys],
            diff, relate[0], data_map[keys][relate[0]][0]])

        if index + 1 == number:
            break
        
    saver.save("Data")
    save_some_stock(data_map, rand_list)

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

