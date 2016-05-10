#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.04.24 23:20"
__modification_time__ = "2016.05.08 20:30"
__intro__ = "Dictionary's adjust tool"


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys

sys.path.append("../")

from datacontrol import DataControl
from collections import namedtuple


#----------------------------------------------
#         search words's news by index
#----------------------------------------------


#----------------------------------------------
#                   search
#----------------------------------------------

def search(news_sql, terms_sql, word, news_table, keyword_table):
    terms_sql.select(5, keyword_table, -1, word.decode("utf8"))
    results = terms_sql.cursor.fetchall() 
    keyword_named = namedtuple("Keywords", ["Index", "Keylist"])
    news_named = namedtuple("News", ["Title", "Url", "Content"])    

    for row in results:
        keys_res = keyword_named(*row)

        news_sql.select(6, news_table, -1, keys_res.Index)
        data =  news_sql.cursor.fetchall()

        for data_row in data:
            news_res = news_named(*data_row)

            print("Key: %s\nTitle: %s\nUrl: %s\nContent: %s\n" %\
                (keys_res.Keylist.encode("latin1").decode("utf8"), \
                 news_res.Title, news_res.Url, news_res.Content))

#----------------------------------------------
#               Function map 
#----------------------------------------------

FunctionMap = {
}


#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    date = str(sys.argv[1])
    word = sys.argv[2]

    if not word or not date:
        print("Newshelp tools:")
        print("Usage: [date] [word]")
        exit()

    news_sql = DataControl("News", "utf8")
    terms_sql = DataControl("Keyword", "latin1")

    search(news_sql, terms_sql, word, "N" + date, "K" + date)
