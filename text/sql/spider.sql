/*---------------------------------------------
 *           create all need database
-*---------------------------------------------*/

create database if not exists Url;
create database if not exists News;
create database if not exists Dictionary;
create database if not exists Keyword;


/*---------------------------------------------
 *          drop all example table
-*---------------------------------------------*/

drop table Url.Example;
drop table News.Example;
drop table Keyword.Example;


/*---------------------------------------------
 *          create url example table 
-*---------------------------------------------*/

create table Url.Example(
ID bigint(32) not null primary key auto_increment,
Url char(255) unique not null, Poff tinyint(4), 
Foff tinyint(4), State bool default 0, Errt tinyint default 0);


/*---------------------------------------------
 *          create news example table 
-*---------------------------------------------*/

create table News.Example(
Ind char(48) not null primary key,
Time char(5) not null, 
Source char(24) character set utf8 not null,
Title char(128) character set utf8 not null, 
Url char(128) not null, Extmark tinyint(1) default 0,
Content MEDIUMTEXT character set utf8 not null);


/*---------------------------------------------
 *         create dictionary table 
-*---------------------------------------------*/

create table if not exists Dictionary.t_word_noun(
Word varchar(48) not null primary key,
Len tinyint(2), State int(2) default 0);


/*---------------------------------------------
 *        create keyword example table
-*---------------------------------------------*/

create table Keyword.Example(
Ind char(48) not null primary key, 
Keyword varchar(2048) character set utf8 not null, 
Keynum int(32) not null, Keyflags tinyint(1) default 0) 

