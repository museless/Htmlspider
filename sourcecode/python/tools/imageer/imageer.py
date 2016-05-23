# -*- coding:utf8 -*-

#==============================================
#                Code header
#==============================================

_author = "Muse"
_create = "2016.05.22 01:50"
_modified = "2016.05.24 01:10"


#==============================================
#                  import
#==============================================

from math import radians, sin, cos
from PIL import Image, ImageFont, ImageDraw
from imageerconfig import *


#==============================================
#               Class Imageer
#==============================================

class Imageer:
    #==========================================
    #                  Data 
    #==========================================

    # constant
    TREMS_IDX = 0
    RELATE_IDX = 1

    #==========================================
    #               Constructor
    #==========================================

    def __init__(self, xy, inner_size, color = "black"):
        self.xy = xy
        self.image = Image.new("RGB", xy, color)
        self.draw = ImageDraw.Draw(self.image)
        self.inner = inner_size

    #==========================================
    #            make the picture
    #==========================================

    def make(self, center_str, datas, radius, size):
        self._make((self.xy[0] / 2, self.xy[1] / 2), 
            center_str, datas, radius, size)

    def _make(self, midpoint, center_str, datas, radius, size):
        points = self._get_points(midpoint, radius, len(datas))
        font = ImageFont.truetype(FontPath + FontFile, size, encoding = "utf-8")

        if points: 
            for index, point in enumerate(points):
                self.draw.line((point, midpoint))
                terms = datas[index]

                slave = (len(terms) == 2) and terms[self.RELATE_IDX] or []

                self._make(point, terms[self.TREMS_IDX],
                    slave, radius - 24, size - 2)

        arcp = (midpoint[0] - self.inner, midpoint[1] - self.inner, midpoint[0] + self.inner, midpoint[1] + self.inner)

        self.draw.chord(arcp, 0, 360, fill = "black")
        fitxy = self._get_fitxy(list(midpoint), center_str, size)
        self.draw.text(fitxy, center_str, font = font, fill = "white")

    #==========================================
    #             save the picture
    #==========================================

    def save(self, file_path, file_type):
        self.image.save(file_path, file_type)

    #==========================================
    #      get fit output zone for string
    #==========================================

    def _get_fitxy(self, points, string, font_size):
        points[1] -= font_size
        length = 0

        for char in string:
            length += (char > u"\u0100") and font_size or font_size >> 1

        points[0] -= length >> 1

        return  points

    #==========================================
    #               get points
    #==========================================

    def _get_points(self, coordinate, radius, number):
        if number == 0:
            return  None 

        points = []
        per_angle = float(360) / number
        total_angle = 0
    
        for index in range(0, number):
            angle = radians(total_angle)
            symbol = (total_angle <= 90 and total_angle >= 270) and 1 or -1
    
            a = radius * sin(angle) * symbol
            b = radius * cos(angle) * symbol
    
            points.append((coordinate[0] + a, coordinate[1] + b))
            total_angle += per_angle
    
        return  points


