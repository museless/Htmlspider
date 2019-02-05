# -*- coding: utf8 -*-

import sys
import os

from struct import pack, unpack


SCEL_MINSIZE = 0x2628


def word_format(word_list, dest):
    with open(dest, "w") as f:
        f.write("#-*- coding:utf8 -*-\n\nWord = [\n")

        for word in word_list:
            f.write("    \"%s\",\n" % word)

        f.write("]\n")


def make_utf8(word):
    result = bytes() 

    for idx in range(0, len(word), 2):
        result += pack(
            "BBB",
            0xE0 | (word[idx + 1] >> 4), 
            0x80 | ((word[idx + 1] & 0xF) << 2) | (word[idx] >> 6),
            (0x80 | (word[idx] & 0x3F))
        )

    return  result 


def parse_scel(src):
    if os.path.getsize(src) <= SCEL_MINSIZE:
        print("{} not a scel file".format(src))
        return  False

    word_list = []

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

                word = make_utf8(
                    unpack(
                        "%ds" % wsize, 
                        scel[offset: offset + wsize]
                    )[0]
                )

                word_list.append(word.decode("utf8"))
                offset += wsize + 0xc

    return  word_list


def main(argv = sys.argv):
    if len(argv) != 3: 
        print("Usage: [src scel] [dest path]")
        exit()

    src, dest = argv[1], argv[2]

    word_format(parse_scel(src), dest)


if __name__ == "__main__":
    main()

