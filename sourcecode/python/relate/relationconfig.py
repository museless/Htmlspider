#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.03.24 09:35"
__modification_time__ = "2016.04.29 12:50"
__intro__ = "keyword relater conf"


#----------------------------------------------
#             Config parameters
#----------------------------------------------

# max len for deque
MaxSave = 10

# in second
Export_time = 300

# data map template
RelationDataTemplate = """# -*- coding:utf8 -*-

from collections import deque

TimeStamp = %d
TimeString = "%s"

RelationMap = %s
"""
