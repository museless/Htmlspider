#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.28 16:34"
__modification_time__ = "2016.01.28 16:34"
__intro__ = "news catcher main"


#----------------------------------------------
#                  Import
#----------------------------------------------

from newscatcher import *


#----------------------------------------------
#                    main
#----------------------------------------------

if __name__ == "__main__":
    url_receiver, news_catcher = catcher_initialize()
    url_table, news_table = table_name_get()

    print url_table, news_table
