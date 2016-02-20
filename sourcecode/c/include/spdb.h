#ifndef _SPDB_H
#define _SPDB_H

/*-----------------------------
            include
-------------------------------*/

#include <mysql.h>
#include <errmsg.h>
#include <mysqld_error.h>


/*-----------------------------
            define
-------------------------------*/

#define DBUSRNAME       "root"
#define DBUSRKEY        "WdSr0922"

#define SQL_TCOM_LEN    0x80    /* tiny */
#define SQL_LICOM_LEN   0xC0    /* little */
#define SQL_SCOM_LEN    0x100   /* small */
#define SQL_MCOM_LEN    0x200   /* middle */
#define SQL_BCOM_LEN    0x300   /* big */
#define SQL_GCOM_LEN    0x600   /* great */
#define SQL_LCOM_LEN    0x1000  /* large */
#define SQL_HCOM_LEN    0x4000  /* huge */
#define SQL_HHCOM_LEN   0x6000  /* huge huge */
#define SQL_TABNAME_LEN 0x20

#define SQL_DBNAME_LEN  0x80

#define SQL_DOMAIN_LEN  32
#define SQL_RELOC_LEN   64
#define SQL_STRCMP_LEN  64
#define SQL_SRCSTR_LEN  24
#define SQL_CHARSET_LEN 8

#define SQL_USTATE_LEN  0x30

#define SQL_PERCOM_MLEN 0x180


/*---------------------------------------------
 *               sql command
-*---------------------------------------------*/

#define DROP_VIEW   "drop view V%s"
#define DROP_TABLE  "drop table %s"


/*---------------------------------------------
 *                 for urlbug
-*---------------------------------------------*/

#define GET_DIRECTORY   "select Url, Latest from %s where Blockmask=1"
#define CREAT_URL_TAB   "create table if not exists %s\
(ID bigint(32) not null primary key auto_increment, \
Url char(%d) unique not null, Poff tinyint(4), Foff tinyint(4), \
State bool default 0, Errt tinyint default 0)"

#define TRAN_URL_BEG    "insert ignore %s(Url, POff, Foff) \
values(\"%.*s\",%d,%d)"

#define TRAN_URL        ",(\"%.*s\",%d,%d)"

#define REVIEW_URL      "select * from %s where Url=\"%.*s\""
#define UPDATE_LATEST   "update UALL set Latest=\"%s\" where Url=\"%s%s%s\""

/*-----------------------------
           for extbug
-------------------------------*/

#define REVIEW_WORD     "select Word from %s where Word=\"%.*s\""

#define TRAN_WD_BEG_ST  "insert %s(Word, Len, State) values(\"%.*s\",%d,%d)"
#define TRAN_WD_ST      ",(\"%.*s\",%d,%d)"

#define TRAN_WD_BEG     "insert %s(Word, Len) values(\"%.*s\",%d)"
#define TRAN_WD         ",(\"%.*s\",%d)"

#define DOWN_WD_ST      "select Word, State from %s"
#define DOWN_WD_NO_ST   "select Word from %s"

#define DELETE_KEYWORD  "delete from %s where Word=\"%.*s\""

#define GET_LIMIT_WORD  "select * from %s where Word like '%.3s%%'"

#define CREAT_KEY_TAB   "create table if not exists %s like %s.Example"

#define GET_NEWS_CONT   "select Ind, Content from %s where %s = 0 limit %d"
#define SET_NEWS_FLAGS  "update %s set %s=1 where Ind=\"%s\""

#define INSERT_KEYWD    "insert %s(Ind, Klist, Keynum) values(\"%s\", \"%.*s\", %d)"
#define INSERT_KW_NEXT  ",(\"%s\", \"%.*s\", %d)"

/*-----------------------------
           typedef
-------------------------------*/

typedef MYSQL_RES           MSLRES;
typedef MYSQL_ROW           MSLROW;

typedef struct  sstrlist    SSTRL;


/*-----------------------------
            struct
-------------------------------*/

struct  sstrlist {
    SSTRL  *s_next;
    char    s_str[SQL_STRCMP_LEN];
};


/*-----------------------------
          extern data
-------------------------------*/

extern  char   *sqlOpt[];
extern  int     nrOpt;


#endif
