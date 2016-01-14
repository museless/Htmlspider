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
        self.text_tree = self.text_tree_build(self.html_parser.body)

    #------------------------------------------
    #            build text tree
    #------------------------------------------

    def text_tree_build(self, tag_object):
        chinese_words = 0
        tree_node = [tag_object, 0, []]

        for child in tag_object.contents:
            if isinstance(child, element.NavigableString):
                chinese_words = self.count_chinese_word(child)
                continue

            if child.name != 'script':
                node = self.text_tree_build(child)

                tree_node[2].append(node)
                chinese_words += node[1] 

        tree_node[1] = chinese_words 

        return  tree_node

    #------------------------------------------
    #         counting chinese word
    #------------------------------------------

    def count_tag_word_number(self, tag_object):
        utf8_word_number = 0

        for string in tag_object.stripped_strings:
            utf8_word_number += self.count_chinese_word(string)

        return  utf8_word_number

    #------------------------------------------
    #         counting chinese word
    #------------------------------------------

    def count_chinese_word(self, string):
        print "string: ", string

        utf8_word_number = 0
        utf8_string = string.encode('utf-8')

        for index in range(len(utf8_string)):
            if utf8_string[index] > chr(127):
                utf8_word_number += 1

        return  utf8_word_number


