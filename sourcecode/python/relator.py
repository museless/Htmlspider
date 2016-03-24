#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.03.16 00:50"
__modification_time__ = "2016.03.24 09:30"
__intro__ = "Keyword relator"


#----------------------------------------------
#                  Import
#----------------------------------------------

import time

from relate import KeywordRelater


#----------------------------------------------
#             relator initialize
#----------------------------------------------

def relator_initialize():
    keyword_supporter = DataControl("Keyword");

    return  KeywordRelater(), keyword_supporter


#----------------------------------------------
#               get table name  
#----------------------------------------------

def table_name_get():
    time_str = time.strftime("%Y%m%d")

    return  "K" + time_str


#----------------------------------------------
#           handle keyword list
#----------------------------------------------

def handle_keyword_list(relator, data_row):
    words = data_row.rsplit(data_row[2], ",")

    for target_word in words:
        for relate_word in words:
            if relate_word != target_word:
                relator.relating(target_word, relate_word)


#----------------------------------------------
#            relator mainly job 
#----------------------------------------------

def relator_work(relator, keyword_supporter, keyword_table, keylist_num = 32):
    last_timstamp = int(time.time())

    while True:
        keyword_supporter.select(2, keyword_table, keylist_num)
        results = keyword_supporter.cursor.fetchall()

        for data_row in results:
            handle_keyword_list(relator, data_row)


#==============================================
#                   Main
#==============================================

if __name__ == "__main__":
    relator, keyword_supporter = relator_initialize()
    keyword_table = table_name_get()

    relator_work(relator, keyword_supporter, keyword_table)

