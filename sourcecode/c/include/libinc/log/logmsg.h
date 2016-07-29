/*---------------------------------------------
 *     modification time: 2016.07.27 22:10
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: error.h 
 *     creation time: 2016.07.27 22:10
 *     author: Muse 
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Six part
 *
 *       Part Zero:  Include
 *       Part One:   Define
 *       Part Two:   Typedef
 *       Part Three: Struct 
 *       Part Four:  Data
 *       Part Five:  Function 
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#pragma once


/*---------------------------------------------
 *             Part One: Define
-*---------------------------------------------*/

#define writelog(log, msg, ...) \
        _setlog(&log, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define setmsg(msg, ...) \
        _setlog(&messageLog, msg, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define LFMT        "[%s - %s - %d] [%s: %s [%s]]"

#define DEF_CAUSE   "nocause"
#define DEF_ERROR   "noerror"


/*---------------------------------------------
 *             Part Two: Typedef
-*---------------------------------------------*/

typedef struct logmessage   Logmsg;


/*---------------------------------------------
 *             Part Three: Struct
-*---------------------------------------------*/

struct logmessage {
    int32_t     id;
    int8_t      level;

    bool        need_error;
    const char *cause;
    const char *fmt;
};


/*---------------------------------------------
 *             Part Five: Function
-*---------------------------------------------*/

bool    _setlog(Muselog *log, int32_t msg, 
            const char *file, const char *func, int32_t line, ...);

