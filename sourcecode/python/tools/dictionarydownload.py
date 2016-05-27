#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.05.27 16:00"
__modification_time__ = "2016.05.27 16:00"
__intro__ = "download dicall's data"


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys
import re

from modules import load_module
from urllib import urlopen
from time import sleep


#----------------------------------------------
#                 catch data
#----------------------------------------------

def catch_data(url_data, terms_list, re_compile):
    if "Error report" in url_data:
        return  False

    index, text = 0, url_data

    while True:
        result = re_compile.search(text)

        if not result:
            break

        data = result.group(1).split("：")
        terms_list.append(data)

        index = result.end()
        text = text[index: ]

    return  True

#----------------------------------------------
#                  download 
#----------------------------------------------

def download(data_map):
    re_compile = re.compile(r"href=\"/indu[^;]*;([^<]*)") 

    for keys, website in data_map.iteritems():
        print(keys)
        page, url, terms = 1, website, []
        
        while True:
            try:
                url_entity = urlopen(url)
            except:
                sleep(2)
                continue

            if not catch_data(url_entity.read(), terms, re_compile):
                break

            page += 1
            print("Url: %s - %d" % (url, page))
            url = website.replace("w1", "w%d" % page)
            sleep(1)

        print(terms)
        break
    
#----------------------------------------------
#                main function
#----------------------------------------------

def main(argv = sys.argv):
    if len(argv) < 2:
        print("Usage: [path]")
        return

    download(load_module(argv[1]).WebsiteMap)

#----------------------------------------------
#                    main 
#----------------------------------------------

if __name__ == "__main__":
    main()

