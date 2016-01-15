#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.13 01:10"
__modification_time__ = "2016.01.14 11:10"
__intro__ = "a news content catch class"


#----------------------------------------------
#                  Import
#----------------------------------------------

from bs4 import BeautifulSoup
from bs4 import element

from catcherconfig import Chinese_minumum
from catcherconfig import Compare_rate


#----------------------------------------------
#             class DataCatcher
#----------------------------------------------

class DataCatcher:
    #------------------------------------------
    #              Constructor
    #------------------------------------------

    def __init__(self, html_content):
        self.html = html_content
        self.reading()

    #------------------------------------------
    #            reading the html
    #------------------------------------------

    def reading(self):
        self.html_parser = BeautifulSoup(self.html, "lxml")
        self.charset = self.html_parser
        self.select_list = []

        self.text_tree_build(self.html_parser.body)

    #------------------------------------------
    #             build text tree
    #------------------------------------------

    def text_tree_build(self, tag_object):
        chinese_words = 0
        child_list = {}

        for child in tag_object.contents:
            if isinstance(child, element.NavigableString):
                chinese_words += self.count_chinese_word(child)
                continue

            if child.name != 'script':
                word_number = self.text_tree_build(child)

                if word_number > Chinese_minumum:
                    child_list[child] = word_number

                chinese_words += word_number 

        print "Total", chinese_words
        self.select_list_insert(child_list, chinese_words)

        return  chinese_words

    #------------------------------------------
    #     count this tag's words number 
    #------------------------------------------

    def count_tag_word_number(self, tag_object):
        utf8_word_number = 0

        for string in tag_object.stripped_strings:
            utf8_word_number += self.count_chinese_word(string)

        return  utf8_word_number

    #------------------------------------------
    #          counting chinese word
    #------------------------------------------

    def count_chinese_word(self, string):
        utf8_word_number = 0

        for index in range(len(string)):
            if string[index] > chr(127):
                utf8_word_number += 1

        return  utf8_word_number

    #------------------------------------------
    #  insert child into select_list if child
    #         meeting the condition
    #------------------------------------------

    def select_list_insert(self, child_list, total_words):
        for child, number in child_list.items():
            print number
            if number / total_words >= Compare_rate:
                self.select_list.append(child)
