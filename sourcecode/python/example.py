import MySQLdb
import urllib
import codecs

from datacatcher import DataCatcher

# open file
file_desc = codecs.open("See", "w+", "utf-8")

# connect to mysql
sql_handler = MySQLdb.connect(user = 'root', passwd = 'WdSr0922', db = "Url")
cursor = sql_handler.cursor()

# get data catcher
data_catcher = DataCatcher()

table_name = "U20160122"

while True:
    cursor.execute("select * from %s where State = 0 limit 1" % table_name)
    result = cursor.fetchone()

    if result == None:
        print("all data was catch")
        while True:
            pass

    url = result[1]

    # start to get url
    try:
        url_data = urllib.urlopen(url)

    except:
        print("%s is wrong url" % url)
        cursor.execute(
        "update %s set State = 3 where ID = %d" % (table_name, result[0]))
        continue

    ret_code = url_data.getcode()

    if ret_code != 200:
        file_desc.write("cannot get: %s - code: %d\n\n" % (url, ret_code))
        cursor.execute(
        "update %s set State = 2 where ID = %d" % (table_name, result[0]))

        continue

    html_data = url_data.read()

    if len(html_data) < 100:
        cursor.execute(
        "update %s set State = 4 where ID = %d" % (table_name, result[0]))

        continue

    if "qq.com" in url:
        data_catcher.reading(
        html_data, use_parser = "html.parser", decode = "gb2312")
    else:
        data_catcher.reading(html_data)

    file_desc.write("Url: %s\n" % url)

    file_desc.write(
    "Title: %s\nSource: %s\n" % data_catcher.title_and_data_source())

    content = data_catcher.news_content()
    content = (content == "") and data_catcher.news_title or content

    file_desc.write("Content:\n%s\n\n\n" % content)
    cursor.execute("update %s set State = 1 where ID = %d" % (table_name, result[0]))

cursor.close()
sql_handler.close()