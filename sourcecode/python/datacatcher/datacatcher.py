#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.13 01:10"
__modification_time__ = "2016.01.17 22:00"
__intro__ = "a news content catch class"


#----------------------------------------------
#                  Import
#----------------------------------------------

from bs4 import BeautifulSoup
from bs4 import element

from catcherconfig import Chinese_minumum
from catcherconfig import Compare_rate
from catcherconfig import Chinese_has_rate


#----------------------------------------------
#             class DataCatcher
#----------------------------------------------

class DataCatcher:
    #------------------------------------------
    #              Class data 
    #------------------------------------------

    # Index
    WORD_COUNT_INDEX = 0
    TAG_LEN_INDEX = 1
    TAG_COUNT_INDEX = 2
    PASS_FLAGS_INDEX = 3

    P_NUMBER_INDEX = 0
    BR_NUMBER_INDEX = 1

    # forbidden tag
    Forbid_tag = [
        'script',
        'style',
    ]

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

        self.br_number = 0
        self.p_number = 0

        self.fit_node_list = {}
        self.text_tree_build(self.html_parser.body)

        self.select_tag()

    #------------------------------------------
    #             build text tree
    #------------------------------------------

    def text_tree_build(self, tag_object):
        tag_count_list = [0, 0]
        chinese_words, tag_len = 0, 0
        select, child_data = {}, []

        for child in tag_object.contents:
            if isinstance(child, element.NavigableString):
                chinese_words += self.count_chinese_word(child)
                tag_len += len(child)
                continue

            if child.name not in self.Forbid_tag:
                self.relate_tag_count(child.name, tag_count_list)

                child_data = self.text_tree_build(child)
                chinese_words += child_data[self.WORD_COUNT_INDEX]
                tag_len += child_data[self.TAG_LEN_INDEX]

                if child_data[self.PASS_FLAGS_INDEX] and \
                  (child_data[self.WORD_COUNT_INDEX] > Chinese_minumum and \
                  (child_data[self.TAG_COUNT_INDEX][self.P_NUMBER_INDEX] or \
                   child_data[self.TAG_COUNT_INDEX][self.BR_NUMBER_INDEX])):
                    select[child] = child_data 

        self.rate_compare(chinese_words, select)

        return  [chinese_words, tag_len, \
                 tag_count_list, self.tag_can_pass(chinese_words, tag_len)]

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
    #   counting relate tag's appear times
    #------------------------------------------

    def relate_tag_count(self, child_name, tag_count_list):
        br_add, p_add = 0, 0

        if child_name == 'p':
            p_add = 1

        elif child_name == 'br':
            br_add = 1

        self.br_number += br_add
        self.p_number += p_add

        tag_count_list[self.P_NUMBER_INDEX] += p_add
        tag_count_list[self.BR_NUMBER_INDEX] += br_add

    #------------------------------------------
    #        compare the compare rate
    #------------------------------------------

    def rate_compare(self, total_rate, child_dict):
        for key, values in child_dict.iteritems():
            if float(values[self.WORD_COUNT_INDEX]) / total_rate >= Compare_rate:
                self.fit_node_list[key] = values

    #------------------------------------------
    #       check the tag is pass or not
    #------------------------------------------

    def tag_can_pass(self, word_count, total_len):
        if total_len == 0:
            return  False

        return  (float(word_count) / total_len > Chinese_has_rate)

    #------------------------------------------
    # start to select the perfect content tag
    #------------------------------------------

    def select_tag(self):
        max_words, fit_key = 0, None

        for key, values in self.fit_node_list.iteritems():
            if values[self.WORD_COUNT_INDEX] > max_words:
                max_words = values[self.WORD_COUNT_INDEX]
                fit_key = key

        print fit_key
