/*-----------------------------
        url all create
-------------------------------*/

create table if not exists t_url_news(
    ID          bigint(32) not null primary key auto_increment,
    Latest      char(64) DEFAULT NULL,
    Blockmask   tinyint(1) DEFAULT '1'
);

create table if not exists t_url_csto like t_url_news;
