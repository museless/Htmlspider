#-*- coding:utf8 -*-

#----------------------------------------------
#                Code header
#----------------------------------------------

_author = "Muse"
_creation = "2016.08.03 17:10"
_modify = "2016.08.03 17:10"
_intro = "Parser for scel"


#----------------------------------------------
#                  Import
#----------------------------------------------

import sys
import os
from struct import pack, unpack


#----------------------------------------------
#                   const
#----------------------------------------------

SCEL_MINSIZE = 0x2628


#----------------------------------------------
#                   parse
#----------------------------------------------

def parse_scel(src):
    if os.path.getsize(src) <= SCEL_MINSIZE:
        print("{} not a scel file".format(src))
        return  False

    with open(src, "rb") as f:
        f.seek(SCEL_MINSIZE)
        scel = f.read()
        offset, size = 0, len(scel)

        while offset < size:
            same = unpack("H", scel[offset: offset + 2])[0]
            wsize = unpack("H", scel[offset + 2: offset + 4])[0]
            offset += wsize + 4

            while same > 0:
                wsize = unpack("H", scel[offset: offset + 2])[0]
                offset += 2

                print(wsize)

                same -= 1


#----------------------------------------------
#                    main 
#----------------------------------------------

def main(argv = sys.argv):
    if len(argv) != 3: 
        print("Usage: [src scel] [dest path]")
        exit()

    src, dest = argv[1], argv[2]

    parse_scel(src)


if __name__ == "__main__":
    main()

