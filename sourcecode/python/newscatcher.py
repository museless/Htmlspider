#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.28 16:34"
__modification_time__ = "2016.02.03 10:00"
__intro__ = "news catcher interface"


#----------------------------------------------
#                  Import
#----------------------------------------------

import urllib
import time
import chardet

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
    time_str = "20160201"#time.strftime("%Y%m%d")

    return  "U" + time_str, "N" + time_str


#----------------------------------------------
#             catch url's data
#----------------------------------------------

def url_data_get(url_receiver, url_tabname, url, url_id):
    url_data = None

    try:
        url_data = urllib.urlopen(url)
    except:
        print("Url: %s cannot get" % url)
        url_receiver.update(1, url_tabname, 2, url_id)

    if url_data != None:
        ret_code = url_data.getcode()

        if ret_code != 200:
            print("Url: %s - ret: %d" % (url, ret_code))
            url_receiver.update(1, url_tabname, 2, url_id)
            url_data = None 

    return  url_data


#----------------------------------------------
#             catch html content
#----------------------------------------------

def html_data_get(url_receiver, url_tabname, url_data, url_id):
    html = url_data.read()

    if len(html) < 256:
        url_receiver.update(1, url_tabname, 3, url_id)
        return  None

    return  html 


#----------------------------------------------
#                html extract 
#----------------------------------------------

def html_extract(data_catcher, url, html):
    if "gmw" in url:
        data_catcher.reading(html, decode = "utf8")
        return

    if "qq.com" not in url:
        data_catcher.reading(html)
        return

    data_catcher.reading(html, use_parser = "html.parser", decode = "gb2312")


#----------------------------------------------
#           upload news content
#----------------------------------------------

def upload_news(data_catcher, news_uploader, url_id, url):
    title, source = data_catcher.title_and_data_source()

    news_uploader.pre_insert(
        str(url_id), time.strftime("%H:%M"), \
        source, news_uploader.escaping(title), url, \
        news_uploader.escaping(data_catcher.news_content()))


#----------------------------------------------
#             handle url result
#----------------------------------------------

def handle_url_result(
    data_row, data_catcher, url_receiver, 
    news_uploader, url_tabname, news_tabname):

    url = data_row[URL_INDEX]
    url_id = data_row[URL_ID_INDEX]

    url_receiver.update(1, url_tabname, 9, url_id)
    url_data = url_data_get(url_receiver, url_tabname, url, url_id)

    if url_data != None:
        html = html_data_get(url_receiver, url_tabname, url_data, url_id)

        if html == None:
            return

        html_extract(data_catcher, url, html)
        upload_news(data_catcher, news_uploader, url_id, url)
        url_receiver.update(1, url_tabname, 1, url_id)


#----------------------------------------------
#          datacatcher mainly job 
#----------------------------------------------

def catcher_work(
    data_catcher, url_receiver, news_uploader, 
    url_tabname, news_tabname, url_limit = 32):

    news_uploader.create(1, news_tabname) 
    news_uploader.insert_ready(1, news_tabname)

    while True:
        url_receiver.select(1, url_tabname, url_limit)
        results = url_receiver.cursor.fetchall()

        for data_row in results:
            handle_url_result(data_row, data_catcher, url_receiver, 
                news_uploader, url_tabname, news_tabname)

        news_uploader.final_insert()

        time.sleep(2)

