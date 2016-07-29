/*---------------------------------------------
 *     modification time: 2016-07-10 21:10:00
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: muselog.c
 *     creation time: 2016-07-08 16:03:45
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *        Source file content Five part
 *
 *       Part Zero:  Include
 *       Part One:   Define 
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Log control
 *
-*---------------------------------------------*/

/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"


/*---------------------------------------------
 *          Part Four: Log control
 *
 *          1. log_start
 *          2. flog
 *          3. vlog
 *          4. log_move
 *
-*---------------------------------------------*/

/*-----log_start-----*/
bool log_start(Muselog *log, const char *logname, int facility)
{
    if (!log || !logname) {
        errno = EINVAL;
        return  false;
    }

    log->facility = facility;
    snprintf(log->logname, LOGNAME_LEN, logname);

    openlog(NULL, LOG_OPTION, facility);

    return  true;
}


/*-----flog-----*/
bool flog(Muselog *log, int level, const char *fmt, ...)
{
    if (!log || !fmt) {
        errno = EINVAL;
        return  false;
    }

    va_list ap;

    va_start(ap, fmt);
    vsyslog(log->facility | level, fmt, ap);

    return  true;
}


/*-----vlog-----*/
bool vlog(Muselog *log, int level, const char *fmt, va_list ap)
{
    if (!log || !fmt) {
        errno = EINVAL;
        return  false;
    }

    vsyslog(log->facility | level, fmt, ap);

    return  true;
}


/*-----log_move-----*/
bool log_move(Muselog *log, const char *save, int size)
{
    if (!log || !save) {
        errno = EINVAL;
        return  false;
    }

    int32_t out = open(save, O_WRONLY | O_CREAT);
    int32_t in = open(log->logname, O_RDWR); 

    if (out == -1 || in == -1)
        return  false;

    struct stat stats;

    if (fstat(in, &stats) == -1)
        return  false;

    if (sendfile(out, in, NULL, stats.st_size) == -1)
        return  false;

    if (size > -1) {
        if (ftruncate(in, size) == -1)
            return  false;
    }

    close(out);
    close(in); 

    return  true;
}   

