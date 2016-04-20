#-*- utf8 -*-

import codecs
import urllib

from datacatcher import DataCatcher

def open_html(path):
    with open(path) as file_desc:
        return  file_desc.read()

if __name__ == "__main__":
    url_data = urllib.urlopen("http://www.p5w.net/news/gncj/201604/t20160418_1419096.htm")
    html = url_data.read()

    catcher = DataCatcher()
    catcher.reading(html, decode = "gb2312")

    print("%s\n%s" % catcher.title_and_data_source("utf8"))
    print("Content:\n%s" % catcher.news_content())
