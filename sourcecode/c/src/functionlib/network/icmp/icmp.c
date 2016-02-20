/*---------------------------------------------
        Source code contain Seven part

        Part Zero:  Include
        Part One:   Define
        Part Two:   Local data
        Part Three: Local function

        Part Four:  ICMP create
        Part Five:  ICMP type's method
        Part Six:   ICMP checksum calc
        Part Seven: ICMP resolve check

-----------------------------------------------*/

/*---------------------------------------------
        Creation time: 2015/10/29 11:30
        Creator: William

        Modification time: 2015/11/03 08:09
        Mender: William
    
-----------------------------------------------*/

/*---------------------------------------------
              Part Zero: Include
-----------------------------------------------*/

#include "icmp.h"


/*---------------------------------------------
               Part One: Define
-----------------------------------------------*/


/*---------------------------------------------
            Part Two: Local data
-----------------------------------------------*/


/*---------------------------------------------
          Part Three: Local function
-----------------------------------------------*/

/* Part Five */
static  int     icmp_echo_create(
                char *write_buf, int buf_len, uShort type, 
                uShort code, const char *data, int data_len);

static  void    icmp_type_err_assign_errno(uShort code);

/* Part Six */
static  uShort icmp_checksum(uShort *addr, int byte_num);

/* Part Seven */
static  int     icmp_resolve_check(
                ICMP *icmp_data, int icmp_size, uShort check_type, uShort check_code);

static  int     icmp_resolve_type_check(ICMP *icmp, uShort code);
static  int     icmp_resolve_type_echo(ICMP *icmp);


/*---------------------------------------------
            Part Four: ICMP create

            1. icmp_create
            2. icmp_resolve

-----------------------------------------------*/

/*-----icmp_echo_create-----*/  
int icmp_create(char *write_buf, int buf_len, uShort type, 
                uShort code, const char *data, int data_len)
{
    if (buf_len < DG_ICMP_MINLEN) {
        errno = EILSEQ;
        return  FRET_Z;
    }

    switch (type) {
        case  TYPE_ECHO:  
        case  TYPE_ECHO_REPLY:
                return  icmp_echo_create(
                        write_buf, buf_len, type, code, data, data_len);
    }

    errno = EINVAL;
    return  FRET_Z;
}


/*-----icmp_resolve-----*/
int icmp_resolve(uChar *data, int data_len, int type, int code)
{
    ICMP   *icmp_data = (ICMP *)data;

    if (data_len < DG_ICMP_MINLEN) {
        errno = ENODATA;
        return  FRET_Z;
    }

    if (icmp_data->icmp_type == TYPE_ERROR) {
        icmp_type_err_assign_errno(icmp_data->icmp_code);
        return  FRET_Z;
    }

    if (icmp_resolve_check(icmp_data, data_len, type, code))
        return  FRET_P;

    return  FRET_Z;
}


/*---------------------------------------------
           Part Five: ICMP type's method

           1. icmp_echo_create
           2. icmp_type_err_assign_errno

-----------------------------------------------*/

/*-----icmp_echo_create-----*/
static int icmp_echo_create(
           char *write_buf, int buf_len, uShort type, 
           uShort code, const char *data, int data_len)
{
    ICMPECHO    icmp_echo;
    int         total_size = data_len + TYPE_ECHO_MIN_LEN;

    if (total_size > buf_len)
        return  FRET_Z;
    
    icmp_echo.icmp_entity.icmp_type = type;
    icmp_echo.icmp_entity.icmp_code = code;

    icmp_echo.icmp_entity.icmp_checksum = 0;
    icmp_echo.icmp_echo_id = getpid();
    icmp_echo.icmp_echo_seqnum = 0;

    memset(&icmp_echo.icmp_pad, 0, 18);

    icmp_echo.icmp_entity.icmp_checksum =
    icmp_checksum((uShort *)&icmp_echo, total_size);

    memcpy(write_buf, &icmp_echo, TYPE_ECHO_MIN_LEN);
    memcpy(write_buf + TYPE_ECHO_MIN_LEN, data, data_len);

    return  total_size;
}


/*-----icmp_type_err_assign_errno-----*/
static void icmp_type_err_assign_errno(uShort code)
{
    switch (code) {
        case  CODE_NETWORK_UNREACH:
        case  CODE_HOST_UNREACH:
                errno = EHOSTUNREACH;
                break;

        case  CODE_PROTO_UNREACH:
        case  CODE_PORT_UNREACH:
                errno = ECONNREFUSED;
                break;

        case  CODE_NEED_BURST:
                errno = EMSGSIZE;
                break;

        case  CODE_ROUTE_FAIL:
        case  CODE_NETWORK_UNKNOWN:
        case  CODE_HOST_UNKNOWN:
                errno = EHOSTUNREACH;
                break;

        case  CODE_NETWORK_CLOSED:
        case  CODE_TOS_NETWORK_UNREACH:
                errno = ENETUNREACH;
                break;

        case  CODE_HOST_ABANDON:
        case  CODE_HOST_CLOSED:
        case  CODE_TOS_HOST_UNREACH:
                errno = EHOSTDOWN;
                break;

        case  CODE_SHUT_BY_FILTER:
        case  CODE_EXCEED_HOST_AUTHORITY:
        case  CODE_NO_POWER:
                errno = EPERM;
                break;
    }
}


/*---------------------------------------------
           Part Six: ICMP checksum calc

           1. icmp_checksum

-----------------------------------------------*/

/*-----icmp_checksum-----*/
static uShort icmp_checksum(uShort *addr, int byte_num)
{
    uLong  checksum = 0;

    while (byte_num > 1) {
        checksum += *addr;
        addr++;
        byte_num -= 2;
    }

    if (byte_num)
        checksum += *(uChar *)addr;

    while (checksum >> 16)
        checksum = (checksum & 0xFFFF) + (checksum >> 16);
    
    return  (uShort)(~checksum);
}


/*---------------------------------------------
           Part Seven: ICMP resolve

           1. icmp_resolve_check
           2. icmp_resolve_type_check

-----------------------------------------------*/

/*-----icmp_resolve_check-----*/
static int icmp_resolve_check(
           ICMP *icmp, int icmp_size, uShort check_type, uShort check_code)
{
    uShort recv_checksum;
    
    if (icmp->icmp_type == check_type && icmp->icmp_code == check_code) {
        recv_checksum = icmp->icmp_checksum;
        icmp->icmp_checksum = 0;

        icmp->icmp_checksum = icmp_checksum((uShort *)icmp, icmp_size);

        if (icmp->icmp_checksum != recv_checksum) {
            errno = EBADMSG;
            return  FRET_Z;
        }

        if (!icmp_resolve_type_check(icmp, check_code))
            return  FRET_Z;
        
        return  FRET_P;
    }
    
    errno = EINVAL;
    return  FRET_Z;
}


/*-----icmp_resolve_type_check-----*/
static int icmp_resolve_type_check(ICMP *icmp, uShort code)
{
    switch (code) {
        case  TYPE_ECHO_REPLY:
        case  TYPE_ECHO:
            return  icmp_resolve_type_echo(icmp);
    }
    
    return  FRET_P;
}


/*-----icmp_resolve_type_echo-----*/
static int icmp_resolve_type_echo(ICMP *icmp)
{
    ICMPECHO   *echo = (ICMPECHO *)icmp;
    
    if (echo->icmp_echo_id != getpid()) {
        errno = ENODATA;
        return  FRET_Z;
    }

    return  FRET_P;
}
