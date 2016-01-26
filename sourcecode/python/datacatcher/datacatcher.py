#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.13 01:10"
__modification_time__ = "2016.01.25 23:30"
__intro__ = "a news content catcher"


#----------------------------------------------
#                  Import
#----------------------------------------------

from bs4 import BeautifulSoup
from bs4 import element

from catcherconfig import *


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
        'br',
    ]

    # member data
    charset = "utf-8"

    #------------------------------------------
    #              Constructor
    #------------------------------------------

    def __init__(self):
        pass

    #------------------------------------------
    #            reading the html
    #------------------------------------------

    def reading(self, html_content, use_parser = "lxml", decode = None):
        self.news_title = "Invaild title"
        self.news_source = "Invaild source"

        self.alloc_parser(html_content, use_parser, decode)

        self.final_content = ""
        self.br_number, self.p_number = 0, 0

        self.fit_node_list = {}
        self.text_tree_build(self.html_parser.body)

        self.select_tag()

    #------------------------------------------
    #         allocate a html parser
    #------------------------------------------
   
    def alloc_parser(self, html_content, use_parser, decode):
        if decode:
            html_content = html_content.decode(decode, "ignore")

        self.html_parser = BeautifulSoup(html_content, use_parser)

    #------------------------------------------
    #         get title and news source
    #------------------------------------------

    def title_and_data_source(self):
        if self.html_parser.findChild("title"):
            self.splite_news_title(self.html_parser.title.stripped_strings)

        return  self.news_title, self.news_source

    #------------------------------------------
    # splite title string to title and source
    #------------------------------------------

    def splite_news_title(self, news_titles):
        for string in news_titles:
            for strange_title in Strange_news_title:
                if strange_title in string:
                    return

            for offset in range(len(string)):
                if string[offset] == "_" or \
                   string[offset] == "|" or \
                   (string[offset] == "-" and \
                   string[offset + 1].isdigit() == False):
                    break

        self.news_title = string[0: offset]
        self.news_source = \
        (string[offset:].rsplit(string[offset])[-1]).rsplit(" ")[-1]

    #------------------------------------------
    #            build text tree
    #------------------------------------------

    def text_tree_build(self, tag_object):
        tag_count_list = [0, 0]
        chinese_words, tag_len = 0, 0
        select, child_data = {}, []

        if tag_object == None:
            return  None

        for child in tag_object.contents:
            if isinstance(child, element.NavigableString):
                chinese_words += self.count_chinese_word(child)
                tag_len += len(child)
                continue

            self.relate_tag_count(child.name, tag_count_list)

            if self.is_forbid_tag(child):
                continue

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
    #           check forbidden tag
    #------------------------------------------

    def is_forbid_tag(self, tag):
        if tag.name in self.Forbid_tag:
            tag.clear()
            return  True

        return  False

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

        self.content_tag = fit_key

    #==========================================
    #     the next function used to output 
    #               new content
    #==========================================

    #------------------------------------------
    #          get news content access
    #------------------------------------------

    def news_content(self):
        if self.content_tag == None:
            return  "Invaild content" 

        for string in self.content_tag.stripped_strings:
            if self.has_start_string(string):
                self.final_content = ""
                continue
            
            if self.has_ending_string(string):
                break

            self.final_content += string

        return  self.final_content

    #------------------------------------------
    #         has start string or not
    #------------------------------------------

    def has_start_string(self, check_string):
        for start_string in Start_terms:
            if start_string in check_string:
                return  True

        return  False

    #------------------------------------------
    #        has ending string or not
    #------------------------------------------

    def has_ending_string(self, check_string):
        for end_string in Ending_terms:
            if end_string in check_string:
                return  True

        return  False

