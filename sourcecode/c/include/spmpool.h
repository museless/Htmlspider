#ifndef	_SPMPOOL_H
#define	_SPMPOOL_H

/*-----------------------------
 *          Define
-*-----------------------------*/

#define	WMP_BITS_PER_BITMAP	        0x20    /* 32 */
#define	WMP_BITS_PER_BITMAP_SHIFT	0x5     /* 2 ^ 5 */
#define	WMP_PSIZE_BOR	            0x4B000
#define	WMP_PAGESIZE	            0x50000

#define	WMP_BIT_BLOCK	            0x1
#define	WMP_BIT_UNBLOCK	            0x0


/*-----------------------------
 *          Typedef
-*-----------------------------*/

typedef struct  w_mempool   WPOOL;


/*-----------------------------
 *          Struct
-*-----------------------------*/

struct	w_mempool {
    char   *wmp_start;
    int    *wmp_bitmap;

    MATOS   wmp_atomic; /* mutipthread protect */

    int	    wmp_psize;	/* page size */
    int	    wmp_clip;	/* block num */
};


/*-----------------------------
 *      Global function
-*-----------------------------*/

WPOOL  *wmpool_create(int clip_num, int per_clip_size);
void   *wmpool_malloc(WPOOL *handler);
void   *wmpool_calloc(WPOOL *handler);
void	wmpool_free(WPOOL *pHandler, void *free_addr);
void	wmpool_destroy(void *handler);


/*-----------------------------
 *       Macro function
-*-----------------------------*/

#define	wmpool_bit_is_block(nByte, nBit)	((nByte >> nBit) & WMP_BIT_BLOCK)

#endif
