#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.03.23 07:50"
__modification_time__ = "2016.05.19 07:50"
__intro__ = "keyword relater"


#----------------------------------------------
#                  Import
#----------------------------------------------

import os
import time

from relationconfig import *
from collections import deque


#----------------------------------------------
#             class KeywordRelater
#----------------------------------------------

class KeywordRelater:
    #------------------------------------------
    #              Class data 
    #------------------------------------------

    # keyword relation map
    __relation_map = {}
    
    # keyword map start timestamp
    __start_timestamp = 0
    __write_timestamp = 0

    # relate block index
    TIMES = 0
    SOURCES = 1

    #------------------------------------------
    #              Constructor
    #------------------------------------------

    def __init__(self, data_path):
        if data_path[-1] == "/":
            data_path = data_path[: -1]

        self.data_path = data_path

        self.__start_timestamp = int(time.time())

    #------------------------------------------
    #                shutdown
    #------------------------------------------

    def shut(self):
        unfinish_data_path = "%s/unfinish.py" % self.data_path

        file_desc = open(unfinish_data_path, "w")
        self.__write_map(file_desc)

    #------------------------------------------
    #          reload unfinish map
    #------------------------------------------

    def reload(self):
        unfinish_data_path = "%s/unfinish.py" % self.data_path

        try:    module = __import__(unfinish_data_path)
        except: return  False

        if hasattr(module, "RelationMap"):
            self.__relation_map = module.RelationMap.copy()
            self.__start_timestamp = module.TimeStamp

            os.remove(unfinish_data_path)

    #------------------------------------------
    #             start to relate
    #------------------------------------------

    def relating(self, target, relate):
        if target not in self.__relation_map:
            self.__relation_map[target] = {}

        if relate not in self.__relation_map[target]:
            self.__relation_map[target][relate] = [0]

        relate_block = self.__relation_map[target][relate]

        relate_block[self.TIMES] += 1

        self.__write_timestamp = int(time.time())

        self.__relation_map_save()

    #------------------------------------------
    #           count appear times
    #------------------------------------------

    def count(self, target, source):
        if target not in self.__relation_map:
            self.__relation_map[target] = {}

        if "appears" not in self.__relation_map[target]:
            self.__relation_map[target]["appears"] = [0, deque(maxlen = MaxSave)]

        appears = self.__relation_map[target]["appears"]

        appears[self.TIMES] += 1
        source = source.encode("utf8")

        if source not in appears[self.SOURCES]:
            appears[self.SOURCES].append(source)

    #------------------------------------------
    #          save the relation map
    #------------------------------------------

    def __relation_map_save(self):
        if self.__start_timestamp + Export_time < self.__write_timestamp:
            data_file = "%s/%s.py" % \
                (self.data_path, time.strftime("%Y%m%d%H%M%S"))

            with open(data_file, "w") as file_desc:
                self.__write_map(file_desc)
                self.__start_timestamp = self.__write_timestamp

                return  True

        return  False

    #------------------------------------------
    #         write map data to file 
    #------------------------------------------

    def __write_map(self, file_desc):
        time_str = time.strftime("%Y%m%d%H%M%S")
        
        file_desc.write(RelationDataTemplate % \
            (self.__write_timestamp, time_str, \
            str(self.__relation_map).decode("string_escape")))
