# -*- coding:utf8 -*-

#==============================================
#                Code header
#==============================================

_author = "Muse"
_create = "2016.05.22 01:50"
_modified = "2016.05.24 12:50"


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

    PER_ANGLE = 30

    #==========================================
    #               Constructor
    #==========================================

    def __init__(self, xy, radius, inner_size, color = "black"):
        self.xy = xy
        self.image = Image.new("RGB", xy, color)
        self.draw = ImageDraw.Draw(self.image)
        self.inner = inner_size
        self.radius = radius
        self.per_cut = int(radius * 0.3)
        self.mid = (self.xy[0] >> 1, self.xy[1] >> 1)

    #==========================================
    #            make the picture
    #==========================================

    def make(self, center_str, datas, size):
        self._make(self.mid, center_str, datas, self.radius, size)

    def _make(self, midpi, center_str, datas, 
            radius, size, start_angle = None, per_angle = None):

        points = self._get_points(midpi, radius, 
                    len(datas), start_angle, per_angle)

        font = ImageFont.truetype(FontPath + FontFile, size, encoding = "utf-8")

        if points: 
            for index, point_data in enumerate(points):
                self.draw.line((point_data[0], midpi))

                terms = datas[index]
                slave = (len(terms) == 2) and terms[self.RELATE_IDX] or []

                start = self._get_output_angle(point_data[1], len(slave))

                self._make(point_data[0], terms[self.TREMS_IDX],
                    slave, radius - self.per_cut, size - 2, start, self.PER_ANGLE)

        arcp = (midpi[0] - self.inner, 
                midpi[1] - self.inner, 
                midpi[0] + self.inner, 
                midpi[1] + self.inner)

        self.draw.chord(arcp, 0, 360, fill = "black")
        fitxy = self._get_fitxy(list(midpi), center_str, size)
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
    #           get output's angle
    #==========================================

    def _get_output_angle(self, angle, count):
        return  (count <= 1) and angle or angle - ((count - 1) * self.PER_ANGLE)

    #==========================================
    #               get points
    #==========================================

    def _get_points(self, coordinate, radius, 
            number, start_angle = None, angle = None):

        if number == 0:
            return  None 

        points = []
        per_angle = angle or (float(360) / number)
        total_angle = start_angle or 0
    
        for index in range(0, number):
            angle = radians(total_angle)
            symbol = (total_angle <= 90 and total_angle >= 270) and 1 or -1
    
            a = radius * sin(angle) * symbol
            b = radius * cos(angle) * symbol
    
            points.append([(coordinate[0] + a, coordinate[1] + b), total_angle])
            total_angle += per_angle
    
        return  points


