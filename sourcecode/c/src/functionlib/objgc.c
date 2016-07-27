/*---------------------------------------------
 *   modification time: 2016.07.26 16:40
 *   mender: Muse
-*---------------------------------------------*/

/*---------------------------------------------
 *   file: objgc.c
 *   author: Muse
 *   creation time: 2015.11.19 17:00
-*---------------------------------------------*/

/*---------------------------------------------
 *          Source file contain Six part
 *
 *          Part Zero:  Include
 *          Part One:   Define
 *          Part Two:   Local data
 *          Part Three: Local function
 *
 *          Part Four:  ListGc
 *          Part Five:  Helper function
 *           
-*---------------------------------------------*/

/*---------------------------------------------
 *            Part Zero: Include
-*---------------------------------------------*/

#include "spinc.h"


/*---------------------------------------------
 *         Part Three: Local function
-*---------------------------------------------*/

static bool _search_obj(Gc *gc, void *obj);


/*---------------------------------------------
 *          Part Four: Mgc list control
 *
 *          1. mgc_init
 *          2. mgc_add
 *          3. mgc_all_clean
 *
-*---------------------------------------------*/

/*-----mgc_init-----*/
bool mgc_init(Gc *gc)
{
    if (!gc) {
        errno = EINVAL;
        return  false;
    }

    gc->count = 0;
    gc->list = NULL;

    return  true;
}


/*-----mgc_add-----*/
bool mgc_add(Gc *gc, void *obj, gcfun cleaner)
{
    if (!gc || !obj || !cleaner || _search_obj(gc, obj)) {
        errno = EINVAL;
        return  false;
    }

    Gcobj    *gcobj;

    if (!(gcobj = (Gcobj *)sbrk((intptr_t)sizeof(Gcobj))))
        return  false;

    gcobj->obj = obj;
    gcobj->cleaner = cleaner;
    gcobj->next = gc->list;

    gc->list = gcobj;
    gc->count++;

    return  true;
}


/*-----mgc_all_clean-----*/
bool mgc_all_clean(Gc *gc)
{
    if (!gc) {
        errno = EINVAL;
        return  false;
    }

    Gcobj  *list = gc->list;
    int32_t count;

    for (; list && count < gc->count; list = list->next, count++)
        list->cleaner(list->obj);

    gc->list = NULL;
    gc->count = 0;

    return  true;
}


/*---------------------------------------------
 *        Part Five: Helper function
 *
 *        1. _search_obj
 *
-*---------------------------------------------*/

/*-----_search_obj-----*/
bool _search_obj(Gc *gc, void *obj)
{
    if (obj == GC_DEFOBJ)
        return  false;

    Gcobj  *objs = gc->list;

    while (objs) {
        if (obj == objs->obj)
            return  true;

        objs = objs->next;
    }

    return  false;
}

