#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.28 16:34"
__modification_time__ = "2016.01.28 16:34"
__intro__ = "news catcher interface"


#----------------------------------------------
#                  Import
#----------------------------------------------

import urllib
import time

from datacatcher import DataCatcher
from datacontrol import DataControl


#----------------------------------------------
#            catcher initialize
#----------------------------------------------

def catcher_initialize():
    url_receiver = DataControl("Url");
    news_catcher = DataControl("News")

    return  url_receiver, news_catcher


#----------------------------------------------
#           get url, new table name  
#----------------------------------------------

def table_name_get():
    time_str = time.strftime("%Y%m%d")

    return  "U" + time_str, "N" + time_str


