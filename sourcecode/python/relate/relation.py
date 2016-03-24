#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.03.23 07:50"
__modification_time__ = "2016.03.24 09:35"
__intro__ = "keyword relater"


#----------------------------------------------
#                  Import
#----------------------------------------------

import time


#----------------------------------------------
#             class KeywordRelater
#----------------------------------------------

class KeywordRelater:
    #------------------------------------------
    #              Class data 
    #------------------------------------------

    # keyword relation map
    __relation_map = {}

    # relate block index
    TIMES = 0
    LATEST = 0


    #------------------------------------------
    #              Constructor
    #------------------------------------------

    def __init__(self):
        pass

    #------------------------------------------
    #               Destructor
    #------------------------------------------

    def __del__(self):
        pass

    #------------------------------------------
    #             start to relate
    #------------------------------------------

    def relating(self, target, relate):
        if self.__relation_map.has_key(target) == False:
            self.__relation_map[target] = {}

        if self.__relation_map[target].has_key(relate) == False:
            self.__relation_map[target][relate] = [0, 0]

        relate_block = self.__relation_map[target][relate]

        relate_block[self.TIMES] += 1
        relate_block[self.LATEST] = time.time()



