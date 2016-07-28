/*---------------------------------------------
 *     modification time: 2016-07-10 13:15:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: muselog.h 
 *     creation time: 2016-07-08 16:01:13
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define
 *       Part Two:   Typedef
 *       Part Three: Struct 
 *       Part Four:  Function 
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#include <errno.h>
#include <syslog.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>

#include <unistd.h>
#include <fcntl.h>


/*---------------------------------------------
 *             Part One: Define
-*---------------------------------------------*/

#define DEF_LOGNAME "/var/log/Muselog"
#define LOGNAME_LEN 0xFC

#define LOG_OPTION  (LOG_CONS)

/* level for flog and vlog */
#define EMERG       LOG_EMERG
#define ALERT       LOG_ALERT
#define CRIT        LOG_CRIT
#define ERR         LOG_ERR
#define WARN        LOG_WARNING
#define NOTI        LOG_NOTICE
#define INFO        LOG_INFO
#define DEBUG       LOG_DEBUG


/*---------------------------------------------
 *             Part Two: Typedef
-*---------------------------------------------*/

typedef struct muselog  Muselog;


/*---------------------------------------------
 *             Part Three: Struct
-*---------------------------------------------*/

struct muselog {
    char    logname[LOGNAME_LEN];
    int32_t facility;
};


/*---------------------------------------------
 *             Part Four: Function
-*---------------------------------------------*/

bool    log_start(Muselog *log, const char *logname, int facility);
bool    flog(Muselog *log, int level, const char *fmt, ...);
bool    vlog(Muselog *log, int level, const char *fmt, va_list ap);
bool    log_move(Muselog *log, const char *save, int size);


