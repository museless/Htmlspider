#-*- utf8 -*-

import codecs
import chardet 

from datacatcher import DataCatcher

def open_html(path):
    with open(path) as file_desc:
        return  file_desc.read()

if __name__ == "__main__":
    catcher = DataCatcher()
    catcher.reading(open_html("qq.htm"), use_parser = "html.parser", decode = "gb2312")

    print("%s\n%s" % catcher.title_and_data_source())
    print(catcher.news_content())
