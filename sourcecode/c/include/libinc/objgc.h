/*---------------------------------------------
 *   modification time: 2016.07.26 16:40
 *   mender: Muse
 *   file: objgc.h
-*---------------------------------------------*/

#pragma once

/* define */
#define TRY_A_LIMIT 0x8
#define TRY_MORE    0x10
#define TRY_BIGGER  0x20

#define SLEEP_A_SEC 0x1

#define GC_DEFOBJ   ((char *)1)


/* typedef */
typedef void   (*gcfun)(void *);

typedef struct  musegc      Gc;
typedef struct  musegcobj   Gcobj;

typedef struct  musegco     MGCO;


/* struct */
struct musegc  {
    uint32_t    count;
    Gcobj      *list;
};

struct musegcobj {
    Gcobj  *next;

    void   *obj;
    gcfun   cleaner;
};

struct musegco {
    void   *mgco_obj;
    gcfun   mgco_cleaner;

    MATOS   mgco_lock;
    int32_t mgco_tlimit;
};


/* global function */
bool    mgc_init(Gc *gc);
bool    mgc_add(Gc *gc, void *obj, gcfun cleaner);
bool    mgc_all_clean(Gc *gc);

int32_t mgc_one_init(MGCO *pMgco, gcfun pCleaner, int nLimit);
int32_t mgc_one_add(MGCO *pMgco, void *pObj);
void    mgc_one_clean(MGCO *pMgco);
