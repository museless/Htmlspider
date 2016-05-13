#-*- coding: utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.03.16 00:50"
__modification_time__ = "2016.05.12 23:55"
__intro__ = "Keyword relator"


#----------------------------------------------
#                  Import
#----------------------------------------------

import time
import sys

from datacontrol import DataControl
from relate import KeywordRelater
from collections import namedtuple


#----------------------------------------------
#             relator initialize
#----------------------------------------------

def relator_initialize():
    keyword_supporter = DataControl("Keyword", "")
    newser = DataControl("News")

    return  KeywordRelater("../../text/relation/"), keyword_supporter, newser


#----------------------------------------------
#               get table name  
#----------------------------------------------

def table_name_get():
    time_str = time.strftime("%Y%m%d")

    return  "K" + time_str, "N" + time_str


#----------------------------------------------
#           handle keyword list
#----------------------------------------------

def handle_keyword_list(relator, keyword_data, source):
    keywords = str(keyword_data.Keyword)
    words = keywords.rsplit(";")

    for target_word in words:
        if target_word != "":
            relator.count(target_word, source)

            for relate_word in words:
                if relate_word != target_word and relate_word != "":
                    relator.relating(target_word, relate_word)


#----------------------------------------------
#            relator mainly job 
#----------------------------------------------

def relator_work(relator, keyword_supporter, keyword_table, 
        newser, news_table, keylist_num = 32):

    Results = namedtuple("KeywordResult", ["Index", "Keyword", "Keynum", "Flags"])

    while True:
        keyword_supporter.select(2, keyword_table, keylist_num)
        results = keyword_supporter.cursor.fetchall()

        for data_row in results:
            keyword_data = Results(*data_row)
            
            newser.select(6, news_table, -1, keyword_data.Index)
            news_title = newser.cursor.fetchall()[0][0]

            handle_keyword_list(relator, keyword_data, news_title)
            keyword_supporter.update(2, keyword_table, 1, keyword_data.Index)

        time.sleep(8)


#==============================================
#                   Main
#==============================================

if __name__ == "__main__":
    relator, keyword_supporter, newser = relator_initialize()
    keyword_table, news_table = table_name_get()

    try:
        relator_work(relator, keyword_supporter, 
            keyword_table, newser, news_table)

    except KeyboardInterrupt:
        relator.shut()
        print("Relater shutdown...")


