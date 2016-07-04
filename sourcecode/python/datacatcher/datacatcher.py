#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.01.13 01:10"
__modification_time__ = "2016.04.29 09:50"
__intro__ = "a news content catcher"


#----------------------------------------------
#                  Import
#----------------------------------------------

import time

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
    ChineseCmp = chr(127)

    # match tag
    Brtag = "br"
    Ptag = "p"

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

        self._text_tree_search(self.html_parser.body)

        self._select_tag()

    #------------------------------------------
    #         allocate a html parser
    #------------------------------------------
   
    def alloc_parser(self, html_content, use_parser, decode):
        html_content = html_content.decode(decode, "ignore")
        self.html_parser = BeautifulSoup(html_content, use_parser)

    #------------------------------------------
    #         get title and news source
    #------------------------------------------

    def title_and_data_source(self, charset):
        if self.html_parser.findChild("title"):
            self._splite_title(self.html_parser.title.stripped_strings)

        return  self.news_title, self.news_source

    #------------------------------------------
    # splite title string to title and source
    #------------------------------------------

    def _splite_title(self, news_titles):
        # for title
        for string in news_titles:
            for strange_title in Strange_news_title:
                if strange_title in string:
                    return

            for offset, char in enumerate(string):
                if char in ["_", "|"] or (char == "-" and \
                   string[offset + 1].isdigit() == False):
                    self.news_title = string[0: offset]
                    break

        if offset + 1 == len(string):
            self.news_title = string 
            self.news_source = "unknown source"
            return

        # for source
        self.news_source = string[offset: ]

        for index, char in enumerate(reversed(string)):
            if char in ["_", "|", "-", " "]:
                self.news_source = string[-index: ]
                break

    #------------------------------------------
    #            build text tree
    #------------------------------------------

    def _text_tree_search(self, tag_object):
        tag_count_list, select = [0, 0], {}
        chinese_words, tag_len = 0, 0

        for child in tag_object.contents:
            if isinstance(child, element.NavigableString):
                chinese_words += self._count_chinese(child)
                tag_len += len(child)
                continue

            if self._check_forbid_tag(child):
                continue

            self._relate_tag_count(child.name, tag_count_list)

            can_pass, child_data = self._text_tree_search(child)
            chinese_words += child_data[self.WORD_COUNT_INDEX]
            tag_len += child_data[self.TAG_LEN_INDEX]

            if can_pass and self._can_select(child_data): 
                select[child] = child_data 

        self._add_node(chinese_words, select)

        return  self._tag_can_pass(chinese_words, tag_len), \
                [chinese_words, tag_len, tag_count_list]

    #------------------------------------------
    #           check forbidden tag
    #------------------------------------------

    def _check_forbid_tag(self, tag):
        if tag.name in self.Forbid_tag:
            tag.clear()
            return  True

        return  False

    #------------------------------------------
    #          counting chinese word
    #------------------------------------------

    def _count_chinese(self, string):
        utf8_word_number = 0

        for char in unicode(string):
            if char > self.ChineseCmp:
                utf8_word_number += 1

        return  utf8_word_number

    #------------------------------------------
    #   counting relate tag's appear times
    #------------------------------------------

    def _relate_tag_count(self, child_name, tag_count_list):
        if child_name == self.Ptag:
            self.br_number += 1
            tag_count_list[self.P_NUMBER_INDEX] += 1 

        elif child_name == self.Brtag:
            self.p_number += 1
            tag_count_list[self.BR_NUMBER_INDEX] += 1

    #------------------------------------------
    #         can select this child
    #------------------------------------------

    def _can_select(self, child_data):
        return  (child_data[self.WORD_COUNT_INDEX] > Chinese_minumum and \
                 sum(child_data[self.TAG_COUNT_INDEX]) > 0) and \
                    True or False

    #------------------------------------------
    #        compare the compare rate
    #------------------------------------------

    def _add_node(self, total_word, child_dict):
        for key, values in child_dict.iteritems():
            if float(values[self.WORD_COUNT_INDEX]) / total_word >= Compare_rate:
                self.fit_node_list[key] = values

    #------------------------------------------
    #       check the tag is pass or not
    #------------------------------------------

    def _tag_can_pass(self, word_count, total_len):
        if total_len == 0:
            return  False

        return  (float(word_count) / total_len > Chinese_has_rate)

    #------------------------------------------
    # start to select the perfect content tag
    #------------------------------------------

    def _select_tag(self):
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

    def news_content(self, charset = None):
        if not self.content_tag:
            return  "Invaild content" 

        contents = []

        for string in self.content_tag.stripped_strings:
            if self._has_string(string, Start_terms):
                contents = [] 
                continue
            
            if self._has_string(string, Ending_terms):
                break

            contents.append(string)

        self.final_content = "".join(contents)

        return  self.final_content

    #------------------------------------------
    #      has start or end string or not
    #------------------------------------------

    def _has_string(self, check_string, terms):
        for per_check in terms:
            if per_check in check_string:
                return  True

        return  False

