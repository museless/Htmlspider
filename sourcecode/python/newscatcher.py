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
#                  Data
#----------------------------------------------

URL_ID_INDEX = 0
URL_INDEX = 1


#----------------------------------------------
#            catcher initialize
#----------------------------------------------

def catcher_initialize():
    url_receiver = DataControl("Url");
    news_uploader = DataControl("News")
    data_catcher = DataCatcher()

    return  url_receiver, news_uploader, data_catcher


#----------------------------------------------
#           get url, new table name  
#----------------------------------------------

def table_name_get():
    time_str = time.strftime("%Y%m%d")

    return  "U" + time_str, "N" + time_str


#----------------------------------------------
#             catch url's data
#----------------------------------------------

def url_data_get(url_receiver, url, url_id):
    url_data = None

    try:
        url_data = urllb.urlopen(url)
    except:
        url_receiver.update(1, 2, url_id)

    if url_data != None:
        ret_code = url_data.getcode()

        if ret_code != 200:
            url_receiver.update(1, 2, url_id)
            url_data = None 

    return  url_data


#----------------------------------------------
#          datacatcher mainly job 
#----------------------------------------------

def catcher_work(
    url_receiver, news_uploader, url_tabname, news_tabname, url_limit = 32):

    while True:
        url_receiver.select(1, url_tabname, url_limit)
        results = url_receiver.fetchall()

        for row in results:
            url = results[URL_INDEX]
            url_id = results[URL_ID_INDEX]
            url_data = url_data_get(url_receiver, url, url_id)

            if url_data == None:
                continue

            html = url_data.read()
