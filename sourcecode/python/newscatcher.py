#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

_author = "Muse"
_creation = "2016.02.03 10:00"
_modifiy = "2016.07.04 16:30"
_intro = "news catcher, 6 ~ 8 threads may run the best"


#----------------------------------------------
#                  Import
#----------------------------------------------

import urllib
import time
import threading
import sys
import re

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
    news_uploader = DataControl("News", max_allow_packet = 48 * 1024)

    return  url_receiver, news_uploader


#----------------------------------------------
#           get url, new table name  
#----------------------------------------------

def table_name_get():
    time_str = time.strftime("%Y%m%d")

    return  "U" + time_str, "N" + time_str


#----------------------------------------------
#             catch url's data
#----------------------------------------------

def url_data_get(url_receiver, url_tabname, url, url_id):
    url_data = None

    try:
        url_data = urllib.urlopen(url)

    except Exception, error_str:
        url_receiver.update(1, url_tabname, 2, url_id)
        return  None

    if url_data != None:
        ret_code = url_data.getcode()

        if ret_code != 200:
            print("url: %s - return: %d" % (url, ret_code))
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
#            catch html's charset 
#----------------------------------------------

def catch_html_charset(html):
    result = re.search("charset=\"?([^\"]*)\"", html)

    if not result: 
        return  False

    return  result.groups(0)[0]


#----------------------------------------------
#                html extract 
#----------------------------------------------

def html_extract(data_catcher, url, html, charset):
    if "qq.com" not in url:
        data_catcher.reading(html, decode = charset)
        return

    data_catcher.reading(html, use_parser = "html.parser", decode = "gb2312")


#----------------------------------------------
#           upload news content
#----------------------------------------------

def upload_news(data_catcher, news_uploader, url_id, url, charset):
    title, source = data_catcher.title_and_data_source(charset)

    news_uploader.pre_insert(str(url_id), time.strftime("%H:%M"), \
        source, news_uploader.escaping(title), url, \
        news_uploader.escaping(data_catcher.news_content(charset)))


#----------------------------------------------
#             handle url result
#----------------------------------------------

def handle_url_result(data_row, url_receiver, news_uploader, url_tabname):
    data_catcher = DataCatcher()

    url = data_row[URL_INDEX]
    url_id = data_row[URL_ID_INDEX]

    url_receiver.update(1, url_tabname, 9, url_id)
    url_data = url_data_get(url_receiver, url_tabname, url, url_id)

    if url_data != None:
        html = html_data_get(url_receiver, url_tabname, url_data, url_id)

        if html == None:
            return

        charset = catch_html_charset(html) 

        if charset == False:
            url_receiver.update(1, url_tabname, 4, url_id)
            return

        html_extract(data_catcher, url, html, charset)

        upload_news(data_catcher, news_uploader, url_id, url, charset)
        url_receiver.update(1, url_tabname, 1, url_id)


#----------------------------------------------
#          datacatcher mainly job 
#----------------------------------------------

def catcher_work(url_receiver, news_uploader, 
        url_tabname, news_tabname, url_limit = 8):

    news_uploader.create(1, news_tabname) 
    news_uploader.insert_ready(1, news_tabname)

    while True:
        url_receiver.select(1, url_tabname, url_limit)
        results = url_receiver.cursor.fetchall()

        for data_row in results:
            thread_entity = threading.Thread(
                target = handle_url_result,
                args = (data_row, url_receiver, news_uploader, url_tabname))

            thread_entity.start()

        time.sleep(2)

#==============================================
#                   Main
#==============================================

def main(argv = sys.argv):
    url_receiver, news_uploader = catcher_initialize()
    url_table, news_table = table_name_get()

    catcher_work(url_receiver, news_uploader, url_table, news_table)


if __name__ == "__main__":
    main()


