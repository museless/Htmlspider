#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.28 16:34"
__modification_time__ = "2016.01.31 20:34"
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
#             catch html content
#----------------------------------------------

def html_data_get(url_receiver, url_data, url_id):
    html = url_data.read()

    if len(html) < 256:
        url_receiver.update(1, 3, url_id)
        return  None

    return  html 


#----------------------------------------------
#                html extract 
#----------------------------------------------

def html_extract(data_catcher, url, html):
    if "qq.com" not in url:
        data_catcher.reading(html)
        return

    data_catcher.reading(html, use_parser = "html.parser", decode = "gb2312")


#----------------------------------------------
#          datacatcher mainly job 
#----------------------------------------------

def catcher_work(
    data_catcher, url_receiver, news_uploader, 
    url_tabname, news_tabname, url_limit = 32):

    news_uploader.insert_ready(0, news_tabname)

    while True:
        url_receiver.select(1, url_tabname, url_limit)
        results = url_receiver.fetchall()

        for data_row in results:
            url_receiver.update(1, 9, url_id)

            url = data_row[URL_INDEX]
            url_id = data_row[URL_ID_INDEX]
            url_data = url_data_get(url_receiver, url, url_id)

            if url_data == None:
                continue

            html = html_data_get(url_receiver, url_data, url_id)

            if html == None:
                continue
    
            html_extract(data_catcher, url, html)

            title, source = data_catcher.title_and_data_source()

            news_uploader.pre_insert(
            (str(url_id), time.strftime("%H:%M"), 
            source, title, url, data_catcher.news_content))

            url_receiver.update(1, 1, url_id)

        news_uploader.final_insert()

        time.sleep(2)
