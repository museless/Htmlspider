/*---------------------------------------------
 *     modification time: 2016-08-02 00:15:30
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: chunkmpool.c 
 *     creation time: 2014-02-01
 *     author: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *       Source file content Four part
 *
 *       Part Zero:  Include
 *       Part One:   Define
 *       Part Two:   Local data
 *       Part Three: Local function
 *
 *       Part Four:  Mempool API
 *
-*---------------------------------------------*/


/*---------------------------------------------
 *             Part Zero: Include
-*---------------------------------------------*/

#include "sp.h"


/*---------------------------------------------
 *              Part One: Define
-*---------------------------------------------*/

#define PER_BITMAP_BITS     0x20
#define PER_BITMAP_SHIFT    0x5

#define WMP_BIT_BLOCK       0x1
#define WMP_BIT_UNBLOCK     0x0

#define is_bit_block(nByte, nBit) ((nByte >> nBit) & WMP_BIT_BLOCK)


/*---------------------------------------------
 *          Part Four: Mempool API
 *
 *          1. wmpool_create
 *          2. wmpool_malloc
 *          3. wmpool_free
 *          4. wmpool_destroy
 *
-*---------------------------------------------*/

/*-----wmpool_create-----*/
WPOOL *wmpool_create(int clip_num, int per_clip_size)
{
    WPOOL  *handle;
    int     bitmap_num;

    if (clip_num < 1 || per_clip_size < 1) {
        errno = EINVAL;
        return  NULL;
    }

    if (!(handle = malloc(sizeof(WPOOL))))
        return  NULL;

    mato_one(handle->wmp_atomic); 

    handle->wmp_psize = per_clip_size;
    handle->wmp_clip = clip_num;

    if (!(handle->wmp_start = malloc(handle->wmp_psize * handle->wmp_clip))) {
        free(handle);
        return  NULL;
    }

    bitmap_num = clip_num / PER_BITMAP_BITS;
    bitmap_num += ((clip_num % PER_BITMAP_BITS) ? 1 : 0);

    if (!(handle->wmp_bitmap = calloc(bitmap_num, sizeof(int)))) {
        free(handle->wmp_start);
        free(handle);
        return  NULL;
    }

    return  handle;
}


/*-----wmpool_malloc-----*/
void *wmpool_malloc(WPOOL *handler)
{
    int32_t *bitmap = handler->wmp_bitmap;
    int32_t  bit_offset = 0;

    mato_lock(handler->wmp_atomic);

    for (int count = 0; bit_offset < handler->wmp_clip; count++, bit_offset++) {
        if (count == PER_BITMAP_BITS) {
            count = 0;
            bitmap++;
        }

        if (!is_bit_block(*bitmap, count)) {
            *bitmap |= (WMP_BIT_BLOCK << count);
            break;
        }
    }

    mato_unlock(handler->wmp_atomic);

    uint8_t *addr = NULL;

    if (bit_offset != handler->wmp_clip) {
        addr = (bit_offset * handler->wmp_psize) + handler->wmp_start;

    } else {
        errno = ENOMEM;
    }

    return  addr;
}


/*-----wmpool_calloc-----*/
void *wmpool_calloc(WPOOL *handler)
{
    void   *addr = wmpool_malloc(handler);

    if (addr)
        memset(addr, 0, handler->wmp_psize);

    return  addr;
}


/*-----wmpool_free-----*/
void wmpool_free(WPOOL *handler, void *free_addr)
{
    uint8_t *addr = (uint8_t *)free_addr;
    int32_t  num_bits, *bitmap;

    if (addr < handler->wmp_start ||
        addr > (handler->wmp_start + (handler->wmp_psize * (handler->wmp_clip - 1))))
        return;

    num_bits = (addr - handler->wmp_start) / handler->wmp_psize;

    bitmap = handler->wmp_bitmap + (num_bits >> PER_BITMAP_SHIFT);
    *bitmap ^= WMP_BIT_BLOCK << (num_bits % PER_BITMAP_BITS);
}


/*-----wmpool_destroy-----*/
void wmpool_destroy(void *handler)
{
    WPOOL   *free_handler = (WPOOL *)handler;

    if (free_handler) {
        if (free_handler->wmp_start)
            free(free_handler->wmp_start);

        if (free_handler->wmp_bitmap)
            free(free_handler->wmp_bitmap);

        free(free_handler);
    }
}

