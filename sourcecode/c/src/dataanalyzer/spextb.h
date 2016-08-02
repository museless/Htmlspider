/*---------------------------------------------
 *     modification time: 2016.07.31 16:50
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     creation time: 2015-06-01 
 *     author: Muse 
-*---------------------------------------------*/

#pragma once


/*---------------------------------------------
 *                 define
-*---------------------------------------------*/

#define MAX_UP_TERMS    0xFF

#define WORD_LEN_LIMIT  0xF
#define UTF8_WORD_LEN   0x3
#define TWO_U8WORD_LEN  0x6

#define IDX_OFF         0x0
#define CONT_OFF        0x1

#define ETB_SEG_MAXTMS  0x10
#define WORD_CMP_MAX    ETB_SEG_MAXTMS

/* BYTE_CMP_MAX = UTF8_WORD_LEN * ETB_SEG_MAXTMS */
#define BYTE_CMP_MAX    0x30

#define UTF8_WHEAD      0xE0

#define SAVE_BUF_LEN    0x258
#define UPBUF_MAX       0x320

#define PRACTICE_UP_MAX 0x40
#define UPKEY_OTH_MAX   0x80

/* for mempool size */
#define PROC_MP_SIZE    0x4000
#define PER_WORD_MAX    0x40

#define PER_WD_MORE     0x2

/* for struct wordcnt */
#define WORDCNT_NOERR   0x0
#define WORDCNT_ERROR   0x1

#define TERMS_FORMAT    "%.*s;"
#define TFORMATS_LEN    3

#define MASK_EXT        "Extmark"
#define MASK_PRA        "Pramark"

#define PER_DOWN_MAX    0x40


/*---------------------------------------------
 *                  typedef
-*---------------------------------------------*/

typedef struct  clist   CLISTS;
typedef struct  whead   WHEAD;

typedef struct  newcont NCONT;
typedef struct  exbset  EXBSET;

typedef void    (*exbent)(void);

typedef bool    (*minit)(void);
typedef void    (*mwrt)(void *);
typedef void    (*mfwrt)(void);

typedef void    (*ctab)(void);

typedef struct  sepword SEPWORD;

/* word list */
typedef struct  wordst  WST;
typedef struct  wordcnt WDCT;
typedef struct  wdcomb  WDCB;

typedef void    (*rmode)(WDCT *);
typedef void    (*upmode)(WDCT *, const char *);
typedef int     (*exinit)(WDCB *, int);


/*---------------------------------------------
 *                   struct
-*---------------------------------------------*/

struct  clist {
    void    *c_n2;
    void    *c_n3;
    void    *c_n4;
    void    *c_n5;
    void    *c_n6;
    void    *c_n7;
    void    *c_n8;
    void    *c_n9;
    void    *c_n10;
    void    *c_n11;
    void    *c_n12;
    void    *c_n13;
    void    *c_n14;
    void    *c_n15;
    void    *c_n16;
};

struct  whead {
    char    dc_utf8[UTF8_WORD_LEN];
    int     dc_off;
    int     dc_cnt;
};

struct exbset {
    char   *emod_maname;
    exbent  emod_entry;

    minit   emod_init;
    exinit  emod_exinit;

    mwrt    emod_wrt;
    mfwrt   emod_fwrt;

    ctab    emod_tchange;
    rmode   emod_rmode;
    upmode  emod_up;
};

struct  newcont {
    const char *nc_cont;
    const char *nc_ind;
};

struct  sepword {
    char    sep_utf8[UTF8_WORD_LEN];
};

/* word list */
struct  wordst {
    char    *ws_buf;
    WST     *ws_next;

    int     ws_bytes;   /* all bytes */
    int     ws_cnt;     /* occurrence number */
};

struct  wordcnt {
    WST    *wc_list;

    int     wc_ndiff;   /* total different word */
    int     wc_total;   /* total word */
    uLong   wc_tbytes;  /* total size */
};

/* word combine */
struct  wdcomb {
    char    *wb_lterms;

    union {
        uLong   *wb_ltimes;
    };
};


/*---------------------------------------------
 *                global fun
-*---------------------------------------------*/

/* misc.c */
void    exbug_print_help(void);

/* signal.c */
bool    exbug_signal_init(void);

/* database.c */
bool    exbug_database_init(void);
void    exbug_database_close(void);
void    exbug_rewind_exmark(const char *pInd, char *maskName);
void   *exbug_content_download(void);  /* ret: MYSQL_RES */

int     exbug_module_database_init(void);
void    exbug_create_keyword_table(void);
void    exbug_db_seterror(void *db, const char *tabname, uint8_t type);

/* mmseg.c */
void    exbug_segment_entrance(WDCT *wcStru, const char *pNews);

void    exbug_word_add(WDCT *addCnt, const char *addStr, int addSize, int nTimes);
void    exbug_word_print(WDCT *printCnt);

/* termsctl.c */
void    exbug_extract_keyword(WDCT *extDic);

void    exbug_update_terms(WDCT *upList, const char *pInd);
void    exbug_data_sync(void);

int     exbug_dictionary_load(char *fConf, char *savConf, 
            char *fName, CLISTS *pTerm, CLISTS *charHead);

int     exbug_index_load(WHEAD **cStru, char *iName, int nTerms);
int     exbug_terms_load(WDCB **termStru, char *termFile, int nOff);

/* error.c */
void    exbug_sig_quit(int type);

