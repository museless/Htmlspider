#-*- utf8 -*-

import codecs
import chardet 

from datacatcher import DataCatcher


#----------------------------------------------
# News content obtainning way from html:
#
# 1. Pushing html content into BeautifulSoup
#    and getting a bs entity.
# 2. Getting <body> tag.
# 3. Checking every body tag's children. If it
#    has children too, keep digging.
# 4. If a tag is childless, counting its chinese
#    words number, recording it at a list like
#    [tag handler, word number] and return it to
#    its father.
# 5. Finally, we make a list tree order by
#    html structure.
# 6. At this step, we will predefine a var, when
#    node in list tree that its word number bigger
#    than this var. We think its a valid node.
# 7. We sorting those valid node. And chose a fit one.
# 8. At last, printting out this tags scripped_string.
#----------------------------------------------


def open_html(path):
    with open(path) as file_desc:
        return  file_desc.read()

if __name__ == "__main__":
    catcher = DataCatcher()
    catcher.reading(open_html("cnfol.shtml"))
    print catcher.title()
    print catcher.data_source()
    print catcher.news_content()
