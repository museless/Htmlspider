/*---------------------------------------------
 *          delete all existed sql 
-*---------------------------------------------*/

drop table Url.Example;
drop table News.Example;


/*---------------------------------------------
 *           create all need database
-*---------------------------------------------*/

create database if not exists Url;
create database if not exists News;


/*---------------------------------------------
 *          create url example table 
-*---------------------------------------------*/

create table if not exists Url.Example(
ID bigint(32) not null primary key auto_increment,
Url char(255) unique not null, Poff tinyint(4), 
Foff tinyint(4), State bool default 0, Errt tinyint default 0);


/*---------------------------------------------
 *          create news example table 
-*---------------------------------------------*/

create table if not exists News.Example(
Ind char(48) not null primary key,
Time char(5) not null, 
Source char(24) character set utf8 not null,
Title char(128) character set utf8 not null, 
Url char(128) not null, Extmark tinyint(1) default 0,
Content MEDIUMTEXT character set utf8 not null);

