/*---------------------------------------------
 *     modification time: 2015-12-07 14:24:29
 *     mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *     file: mpool_web.c 
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
 *                  Include
-*---------------------------------------------*/

#include "spinc.h"
#include "spmpool.h"


/*---------------------------------------------
 *          Part Four: Mempool API
 *
 *	        1. wmpool_create
 *	        2. wmpool_malloc
 *  	    3. wmpool_free
 *	        4. wmpool_destroy
 *
 *	        macro function (spmpool.h)
 *
 *	        5. wmpool_bit_is_block
 *
-*----------------------------------------------*/

/*-----wmpool_create-----*/
WPOOL *wmpool_create(int clip_num, int per_clip_size)
{
	WPOOL  *handle;
	int	    bitmap_num;

	if (clip_num < 1 || per_clip_size <= 0) {
        errno = EINVAL;
		return	NULL;
	}

	if (!(handle = malloc(sizeof(WPOOL))))
		return	NULL;

    mato_init(handle->wmp_atomic, 1); 

	handle->wmp_psize = per_clip_size;
	handle->wmp_clip = clip_num;

    if (!(handle->wmp_start = malloc(handle->wmp_psize * handle->wmp_clip)))
		return	NULL;

    bitmap_num = clip_num / WMP_BITS_PER_BITMAP;
    bitmap_num += ((clip_num % WMP_BITS_PER_BITMAP) ? 1 : 0);

    if (!(handle->wmp_bitmap = calloc(bitmap_num, sizeof(int)))) {
        free(handle->wmp_start);
        return	NULL;
    }

	return	handle;
}


/*-----wmpool_malloc-----*/
void *wmpool_malloc(WPOOL *handler)
{
	int	   *bitmap_pointer = handler->wmp_bitmap;
	int	    count, bit_offset = 0;

    mato_lock(handler->wmp_atomic);

    for (count = 0; bit_offset < handler->wmp_clip; count++, bit_offset++) {
		if (count == WMP_BITS_PER_BITMAP) {
			count = 0;
			bitmap_pointer++;
		}

		if (!wmpool_bit_is_block(*bitmap_pointer, count)) {
			*bitmap_pointer |= (WMP_BIT_BLOCK << count);
			break;
		}
	}

    mato_unlock(handler->wmp_atomic);

    char    *return_addr = NULL;

    if (bit_offset != handler->wmp_clip) {
        return_addr = (bit_offset * handler->wmp_psize) + handler->wmp_start;

    } else {
        errno = ESPIPE;
    }

	return  return_addr;
}


/*-----wmpool_calloc-----*/
void *wmpool_calloc(WPOOL *handler)
{
    void	*calloc_addr;

	if (!(calloc_addr = wmpool_malloc(handler)))
        return  NULL;

	memset(calloc_addr, 0, handler->wmp_psize);

	return  calloc_addr;
}


/*-----wmpool_free-----*/
void wmpool_free(WPOOL *handler, void *free_addr)
{
    char   *addr = (char *)free_addr;
	int	    num_bits, *bitmap_pointer;

    if (addr > (handler->wmp_start + (handler->wmp_psize * (handler->wmp_clip - 1))))
		return;

    num_bits = (uInt)((addr - handler->wmp_start) / handler->wmp_psize);

    bitmap_pointer = handler->wmp_bitmap + (num_bits >> WMP_BITS_PER_BITMAP_SHIFT);
	*bitmap_pointer &= (~(WMP_BIT_BLOCK << (num_bits % WMP_BITS_PER_BITMAP)));
}


/*-----wmpool_destroy-----*/
void wmpool_destroy(void *handler)
{
	WPOOL	*free_handler = (WPOOL *)handler;

	if (free_handler) {
		if (free_handler->wmp_start)
			free(free_handler->wmp_start);

		if (free_handler->wmp_bitmap)
			free(free_handler->wmp_bitmap);

		free(free_handler);
	}
}

