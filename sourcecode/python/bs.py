#-*- utf8 -*-

import codecs
import chardet
import urllib

from datacatcher import DataCatcher

def open_html(path):
    with open(path) as file_desc:
        return  file_desc.read()

if __name__ == "__main__":
    url_data = urllib.urlopen("http://money.163.com/16/0201/08/BENPL1PV00253B0H.html")
    html = url_data.read()

    catcher = DataCatcher()
    catcher.reading(html)

    print("%s\n%s" % catcher.title_and_data_source())
    print(catcher.news_content())
