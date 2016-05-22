# -*- coding:utf8 -*-

#==============================================
#                Code header
#==============================================

_author = "Muse"
_create = "2016.05.22 01:50"
_modified = "2016.05.23 20:20"


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
    #               Constructor
    #==========================================

    def __init__(self, xy, inner_size, color):
        self.xy = xy
        self.image = Image.new("RGB", xy, color)
        self.draw = ImageDraw.Draw(self.image)
        self.inner = inner_size

    #==========================================
    #            make the picture
    #==========================================

    def make(self, center_str, datas, radius, size):
        center_x, center_y = self.xy[0] / 2, self.xy[1] / 2
        points = self._get_points(center_x, center_y, radius, len(datas))
        font = ImageFont.truetype(FontPath + FontFile, size, encoding = "utf-8")

        for index, point in enumerate(points):
            self.draw.line((point, (center_x, center_y)))
            self.draw.text(point, datas[index], font = font, fill = "white")

        center_x -= self.inner
        center_y += self.inner

        self.draw.chord((center_x, center_x, center_y, center_y), 0, 360, fill = "black")
        self.draw.text((230, 235), center_str, font = font, fill = "white")

    #==========================================
    #             save the picture
    #==========================================

    def save(self, file_path):
        self.image.save(file_path, "jpeg")

    #==========================================
    #               get points
    #==========================================

    def _get_points(self, x, y, radius, number):
        points = []
        per_angle = float(360) / number
        total_angle = 0
    
        for index in range(0, number):
            angle = radians(total_angle)
            symbol = (total_angle <= 90 and total_angle >= 270) and 1 or -1
    
            a = radius * sin(angle) * symbol
            b = radius * cos(angle) * symbol
    
            points.append((x + a, y + b))
            total_angle += per_angle
    
        return  points


