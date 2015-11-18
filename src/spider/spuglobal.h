#ifndef	_SPUBDATA_H
#define	_SPUBDATA_H


/*---------------------------
	      typedef
-----------------------------*/

/* typedef struct */
typedef struct  forbid_word     FBSTR;
typedef struct  ubug_setting    UBSET;

/* typedef function */
typedef void    (*fn_ent)(WEBIN *);
typedef int     (*fn_dway)(WEBIN *);
typedef void    (*fn_init)(void);			            /* module init function */
typedef void    (*fn_st)(void *, void *, char *, int);	/* str = string tran */
typedef void    (*fn_stf)(void);                	    /* str = string tran force */

typedef int     (*fn_catch)(char *, char **); 
typedef int     (*fn_locate)(WEBIN *, char **, char **);


/*---------------------------
	       struct
-----------------------------*/

struct forbid_word {
    char   *fb_str;
    int     fb_len;
};

/* urlbug setting */
struct ubug_setting {
	fn_ent	    ubs_fent;
	fn_dway	    ubs_dway;

	fn_init	    ubs_init;
	fn_st	    ubs_fst;
	fn_stf	    ubs_fstf;

    fn_catch    ubs_catch;
    fn_locate   ubs_locate;

	int         ubs_rtime;
};

/*---------------------------
	    global data
-----------------------------*/

extern	WEBIN  *urlSaveList;
extern	WPOOL  *contStorePool, *urlStorePool;
extern	MSGSET *urlMsgSet;
extern	MGCH   *urlGarCol;
extern	void   *ubugThreadPool;	/* PTHPOOL (mpctl.h) */
extern	UBSET	urlRunSet;
extern	MYSQL	urlDataBase;
extern  SPPING  ubugPingInfo;

extern	int	    procCommuFd, urlMaxLen, urlCatchNum;
extern	MATOS	writeStoreLock;

extern	FBSTR	forbitStrList[];
extern	char	perWebPatt[][PATTERN_LEN];
extern	char	confNameBuf[], urlTabName[];

#endif
