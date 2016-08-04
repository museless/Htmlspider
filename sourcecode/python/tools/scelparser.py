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
import chardet
from struct import pack, unpack


#----------------------------------------------
#                   const
#----------------------------------------------

SCEL_MINSIZE = 0x2628


#----------------------------------------------
#                   parse
#----------------------------------------------

def make_utf8(word):
    result = bytes() 

    for idx in range(0, len(word), 2):
        result += pack("BBB", 
            0xE0 | (word[idx + 1] >> 4), 
            0x80 | ((word[idx + 1] & 0xF) << 2) | (word[idx] >> 6),
            (0x80 | (word[idx] & 0x3F)))

    return  result 


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
            offset += 2
            wsize = unpack("H", scel[offset: offset + 2])[0]
            offset += (wsize + 2)

            for count in range(same):
                wsize = unpack("H", scel[offset: offset + 2])[0]
                offset += 2

                word = make_utf8(unpack("%ds" % wsize, scel[offset: offset + wsize])[0])
                print(word.decode("utf8"))
                offset += wsize + 0xc


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

