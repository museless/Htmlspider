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


#----------------------------------------------
#         search words's news by index
#----------------------------------------------


#----------------------------------------------
#                   search
#----------------------------------------------

def search(news_sql, terms_sql, word, news_table, keyword_table):
    terms_sql.select(5, keyword_table, -1, word.decode("utf8"))
    result = terms_sql.cursor.fetchall() 
   
    for row in result:
        print(row[0])

#----------------------------------------------
#               Function map 
#----------------------------------------------

FunctionMap = {
}


#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    word = sys.argv[1]
    date = str(sys.argv[2])

    if not word or not date:
        print("Newshelp tools:")
        print("Usage: [word] [date]")
        exit()

    news_sql = DataControl("News", "utf8")
    terms_sql = DataControl("Keyword", "latin1")

    search(news_sql, terms_sql, word, "N" + date, "K" + date)
