/*-----------------------------
        url all create
-------------------------------*/

create table if not exists t_url_news(
    ID          bigint(32) not null primary key auto_increment,
    Latest      char(64) DEFAULT NULL,
    Blockmask   tinyint(1) DEFAULT '1'
);

create table if not exists t_url_csto like t_url_news;


/*-----------------------------
   insert csto table's data
-------------------------------*/

insert t_url_csto values
("http://www.csto.com/project/list?page=1", "NULL", 1),
("http://www.csto.com/project/list?page=2", "NULL", 1),
("http://www.csto.com/project/list?page=3", "NULL", 1),
("http://www.csto.com/project/list?page=4", "NULL", 1),
("http://www.csto.com/project/list?page=5", "NULL", 1),
("http://www.csto.com/project/list?page=6", "NULL", 1),
("http://www.csto.com/project/list?page=7", "NULL", 1),
("http://www.csto.com/project/list?page=8", "NULL", 1),
("http://www.csto.com/project/list?page=9", "NULL", 1),
("http://www.csto.com/project/list?page=10", "NULL", 1),
("http://www.csto.com/project/list?page=11", "NULL", 1),
("http://www.csto.com/project/list?page=12", "NULL", 1),
("http://www.csto.com/project/list?page=13", "NULL", 1),
("http://www.csto.com/project/list?page=14", "NULL", 1),
("http://www.csto.com/project/list?page=15", "NULL", 1);
