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

sys.path.append("../")

import xlsxcter
from datacontrol import DataControl
from collections import namedtuple


#----------------------------------------------
#              output the last
#----------------------------------------------

def output_last(time):
    news_tab, keys_tab = "N%s" % time, "K%s" % time
    saver = xlsxcter.Exwriter()

    keyword = DataControl("Keyword", "latin1")
    news = DataControl("News", "utf8")

    news.select(8, "%s order by Time desc " % news_tab)
    results = news.cursor.fetchall()
    named = namedtuple("News", ["Index", "Time", "Title",])
    loops = 200

    saver.write(["时间", "新闻标题", "关键词"])

    for data in results:
        name_res =  named(*data)

        if "Invaild" in name_res.Title:
            continue

        keyword.select(7, keys_tab, -1, name_res.Index)
        ret_value = keyword.cursor.fetchall()

        if len(ret_value) != 0:
            key_list = (ret_value[0][1].rsplit(";"))[: 10]
            key_list = ", ".join(key_list)

            saver.write([name_res.Time, 
                name_res.Title.encode("utf8"),
                key_list.encode("latin1")])

            loops -= 1

        if loops == 0:
            break
    
    saver.save("Data")


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

