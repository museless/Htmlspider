#-*- utf8 -*-

import bs4
import codecs


def open_html(path):
    with open(path) as file_desc:
        return  file_desc.read()

if __name__ == "__main__":
    content = open_html("cfi.html")

    html_parser = bs4.BeautifulSoup(content, "lxml")
    tag_p = html_parser.p

    print html_parser.prettify()

"""
    with codecs.open("See", "w", "utf-8") as file_desc:
        file_desc.truncate()

        print tag_p.getText()
"""
