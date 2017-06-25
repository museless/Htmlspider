#-*- coding:utf8 -*-

"""
author: Muse
creation: 2016.02.03 10:00
modifiy: 2017.06.23 18:05
intro: news catcher, 6 ~ 8 threads may run the best
"""


import urllib
import time
import threading
import sys
import re

from datacatcher import DataCatcher
from datacontrol import DataControl

from multiprocessing.pool import ThreadPool


URL_ID_INDEX = 0
URL_INDEX = 1

Urlreceiver, Newsuploader = None, None
Urltable, Newstable = None, None


def catcher_initialize():
    global  Urlreceiver, Newsuploader

    Urlreceiver = DataControl("Url");
    Newsuploader = DataControl("News", max_allow_packet = 48 * 1024)

def table_name_get():
    global  Urltable, Newstable

    time_str = time.strftime("%Y%m%d")
    Urltable, Newstable = "U" + time_str, "N" + time_str

def url_data_get(url, url_id):
    url_data = None

    try:
        url_data = urllib.urlopen(url)

    except Exception:
        Urlreceiver.update(1, Urltable, 2, url_id)
        return  None

    if url_data != None:
        ret_code = url_data.getcode()

        if ret_code != 200:
            print("url: %s - return: %d" % (url, ret_code))
            Urlreceiver.update(1, Urltable, 2, url_id)
            url_data = None

    return  url_data

def html_data_get(url_data, url_id):
    html = url_data.read()

    if len(html) < 256:
        Urlreceiver.update(1, Urltable, 3, url_id)
        return  None

    return  html 

def catch_html_charset(html):
    result = re.search("charset=\"?([^\"]*)\"", html)

    if not result: 
        return  False

    return  result.groups(0)[0]

def html_extract(data_catcher, url, html, charset):
    if "qq.com" not in url:
        data_catcher.reading(html, decode = charset)
        return

    data_catcher.reading(html, use_parser = "html.parser", decode = "gb2312")

def upload_news(data_catcher, url_id, url, charset):
    title, source = data_catcher.title_and_data_source(charset)

    Newsuploader.pre_insert(str(url_id), time.strftime("%H:%M"), \
        source, Newsuploader.escaping(title), url, \
        Newsuploader.escaping(data_catcher.news_content(charset)))

def handle_url_result(data_row):
    data_catcher = DataCatcher()

    url = data_row[URL_INDEX]
    url_id = data_row[URL_ID_INDEX]

    Urlreceiver.update(1, Urltable, 9, url_id)
    url_data = url_data_get(url, url_id)

    if url_data != None:
        html = html_data_get(url_data, url_id)

        if html == None:
            return

        charset = catch_html_charset(html) 

        if charset == False:
            Urlreceiver.update(1, Urltable, 4, url_id)
            return

        html_extract(data_catcher, url, html, charset)

        upload_news(data_catcher, url_id, url, charset)
        Urlreceiver.update(1, Urltable, 1, url_id)

def catcher_work(url_limit = 8):
    Newsuploader.create(1, Newstable) 
    Newsuploader.insert_ready(1, Newstable)

    pool = ThreadPool()

    while True:
        Urlreceiver.select(1, Urltable, url_limit)
        pool.map(handle_url_result, Urlreceiver.cursor.fetchall())

        time.sleep(1)

def main(argv = sys.argv):
    catcher_initialize()
    table_name_get()
    catcher_work()

if __name__ == "__main__":
    main()

