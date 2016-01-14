import MySQLdb
import urllib
from bs4 import BeautifulSoup

sql_handler = MySQLdb.connect(user = 'root', passwd = 'WdSr0922', db = "Url")

# connect to mysql
cursor = sql_handler.cursor()
cursor.execute("select * from U20151207 limit 1")

result = cursor.fetchone()
url = result[1]

cursor.close()
sql_handler.close()

# start to get url
url_data = urllib.urlopen(url)
ret_code = url_data.getcode()

print "url: %s return code: %d" % (url, ret_code)

if ret_code != 200:
    print "cannot get"
    exit()

html_content = url_data.read()

# use beautiful soup to get html_content
soup = BeautifulSoup(html_content)

print soup.title
