/*---------------------------------------------
 *     modification time: 2016.07.28 13:15
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: logmsg.c
 *     creation time: 2016.07.28 13:15
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Log message 
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/


/*---------------------------------------------
 *          Part Four: Log message
 *
 *          1. _setlog
 *          2. _setfmt
 *
-*---------------------------------------------*/

/*-----_setlog-----*/
bool _setlog(Muselog *log, int32_t msg, 
        const char *file, const char *func, int32_t line, ...)
{
    if (!log || msg >= LMMax) {
        errno = EINVAL;
        return  false;
    }

    Logmsg *current = &logMessage[msg];

    if (current->id != msg) {
        errno = EINVAL;
        flog(log, ERR, "[logmessage: %d id is %d]", current->id, msg);
        return  false;
    }

    const char  *cause = (current->cause[0]) ? current->cause : DEF_CAUSE;
    const char  *errs = (current->need_error) ? strerror(errno) : DEF_ERROR;
    static char  fmts[256] = "No format";
    va_list      ap;

    if (current->fmt && strcmp(current->fmt, "") != 0) {
        va_start(ap, line);
        vsnprintf(fmts, 256, current->fmt, ap);
    }

    flog(log, current->level, LFMT, file, func, line, cause, errs, fmts);

    return  true;  
}


