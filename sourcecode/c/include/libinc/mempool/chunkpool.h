/*---------------------------------------------
 *     modification time: 2016-08-02 00:15:30
 *     mender: Muse
-*---------------------------------------------*/

#pragma once


/*---------------------------------------------
 *                   Define
-*---------------------------------------------*/

#define WMP_PSIZE_BOR   0x4B000
#define WMP_PAGESIZE    0x50000


/*---------------------------------------------
 *                  Typedef
-*---------------------------------------------*/

typedef struct chunkmpool   WPOOL;


/*---------------------------------------------
 *                  Struct
-*---------------------------------------------*/

struct chunkmpool {
    uint8_t *wmp_start;
    int32_t *wmp_bitmap;

    int32_t  wmp_psize; /* page size */
    int32_t  wmp_clip;  /* block num */

    MATOS    wmp_atomic; /* atomic lock */
};


/*---------------------------------------------
 *                 Function
-*---------------------------------------------*/

WPOOL  *wmpool_create(int clip_num, int per_clip_size);
void   *wmpool_malloc(WPOOL *handler);
void   *wmpool_calloc(WPOOL *handler);
void    wmpool_free(WPOOL *pHandler, void *free_addr);
void    wmpool_destroy(void *handler);


