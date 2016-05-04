#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

__author__ = "Muse"
__creation_time__ = "2016.05.04 22:00"
__modification_time__ = "2016.05.04 22:00"
__intro__ = "write xlsx"


#----------------------------------------------
#                  Import
#----------------------------------------------

import xlwt


#----------------------------------------------
#               class Exwriter
#----------------------------------------------

class Exwriter:
    #------------------------------------------
    #                   init
    #------------------------------------------

    def __init__(self):
        self.book = xlwt.Workbook(encoding = "utf8")
        self.sheet = self.book.add_sheet("Keyword")
        self.row = 0

    #------------------------------------------
    #         keep write to the sheet
    #------------------------------------------

    def write(self, data_list):
        for index, data in enumerate(data_list):
            self.sheet.write(self.row, index, str(data))

        self.row += 1

    #------------------------------------------
    #               save xlsx 
    #------------------------------------------

    def save(self, file_name):
        file_name += ".xlsx"

        self.book.save(file_name)
