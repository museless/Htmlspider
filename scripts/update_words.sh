#! /bin/bash

cd ../sourcecode/python/tools

python dicthelper.py update ../../../text/dictionary/char_left.py
python dicthelper.py delete ../../../text/dictionary/char_delete.py
python dicthelper.py arrange
