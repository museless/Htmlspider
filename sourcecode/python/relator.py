#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.03.16 00:50"
__modification_time__ = "2016.03.16 00:50"
__intro__ = "Keyword relator"


#----------------------------------------------
#                  Import
#----------------------------------------------

import time

from datacontrol import DataControl


#----------------------------------------------
#             relator initialize
#----------------------------------------------

def relator_initialize():
    keyword_supporter = DataControl("Keyword");

    return  None, keyword_supporter


#----------------------------------------------
#               get table name  
#----------------------------------------------

def table_name_get():
    time_str = time.strftime("%Y%m%d")

    return  "K" + time_str


#----------------------------------------------
#           upload news content
#----------------------------------------------

def upload_news(data_catcher, news_uploader, url_id, url, charset):
    title, source = data_catcher.title_and_data_source(charset)

    news_uploader.pre_insert(
        str(url_id), time.strftime("%H:%M"), \
        source, news_uploader.escaping(title), url, \
        news_uploader.escaping(data_catcher.news_content(charset)))


#----------------------------------------------
#           handle keyword list
#----------------------------------------------

def handle_keyword_list(relator, data_row):
    pass


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

