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

while True:
    cursor.execute("select * from U20160119 where State = 0 limit 1")
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
        cursor.execute("update U20160119 set State = 3 where ID = %d" % result[0])
        continue

    ret_code = url_data.getcode()

    if ret_code != 200:
        file_desc.write("cannot get: %s - code: %d\n\n" % (url, ret_code))
        cursor.execute("update U20160119 set State = 2 where ID = %d" % result[0])
        continue

    html_data = url_data.read()

    if len(html_data) < 100:
        cursor.execute("update U20160119 set State = 4 where ID = %d" % result[0])
        continue

    data_catcher.reading(html_data)
    file_desc.write("url: %s\n" % url) 
    file_desc.write("title: %s\n" % data_catcher.title())
    file_desc.write("data source: %s\n" % data_catcher.data_source())

    content = data_catcher.news_content()
    content = (content == "") and data_catcher.title() or content

    file_desc.write("Content:\n%s\n\n\n" % content)
    cursor.execute("update U20160119 set State = 1 where ID = %d" % result[0])

cursor.close()
sql_handler.close()
