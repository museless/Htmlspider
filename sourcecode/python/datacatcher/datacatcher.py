#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.13 01:10"
__modification_time__ = "2016.01.13 01:10"
__intro__ = "a news content catch class"


#----------------------------------------------
#                  Import
#----------------------------------------------

from bs4 import BeautifulSoup


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

        if len(tag_object.contents) == 0:
            tree_node[1] = self.count_chinese_word(tag_object)
            return  tree_node

        for child in tag_object.contents:
            if isinstance(child, element.NavigableString) == False \
               and child.name != 'script':
                node = text_tree_build(child)

                text_tree[2].add(node)
                chinese_words += node[1] 

        tree_node[1] = chinese_words

        return  tree_node

    #------------------------------------------
    #         counting chinese word
    #------------------------------------------

    def count_chinese_word(self, tag_object):
        pass

