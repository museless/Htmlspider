import MySQLdb
import urllib

from datacatcher import DataCatcher

# connect to mysql
sql_handler = MySQLdb.connect(user = 'root', passwd = 'WdSr0922', db = "Url")
cursor = sql_handler.cursor()

# get data catcher
data_catcher = DataCatcher()

while True:
    cursor.execute("select * from U20160119 limit 1")

    result = cursor.fetchone()

    if result == None:
        print("all data was catch")
        while True:
            pass

    url = result[1]

    # start to get url
    url_data = urllib.urlopen(url)
    ret_code = url_data.getcode()

    if ret_code != 200:
        print("cannot get: %s - code: %d" % (url, ret_code))
        continue

    data_catcher.reading(url_data.read())
    print("title: %s" % data_catcher.title())
    print("data source: %s" % data_catcher.data_source())

    content = data_catcher.news_content()
    content = (content == "") and data_catcher().title or content

    print("Content:\n%s\n\n\n" % content)

cursor.close()
sql_handler.close()
