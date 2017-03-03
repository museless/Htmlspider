# Htmlspider
Html spider. Working on news url and news content. And extracting data from news.

---
##Build database

1. enter Htmlspider/text/sql

2. mysql -uroot -pxxxx

3. source spider.sql;

4. use Url; source uall.sql;

5. use Dictionary; source dictionary.sql

---
##Build log

1. enter Htmlspider/syslogconf.d

2. cp dataanalyzer.conf /etc/syslog-ng/conf.d/

3. cp urlcatcher.conf /etc/syslog-ng/conf.d/

---
##Modify configuration

1. at Htmlspider/sourcecode/c/include/libinc/dbdef.h<br>
   make #define DBUSRKEY become your mysql root password
   
2. at Htmlspider/sourcecode/python/datacontrol/datacontrolconfig.py<br>
   UserPassword should be your mysql root password
   
---
##Install

1. enter Htmlspider/sourcecode/c/

2. make all

---
##Run

1. enter Htmlspider/bin

2. ./urlcatcher

3. ./newscatcher

4. ./dataanalyzer

