#pragma once


/*---------------------------------------------
 *                  define
-*---------------------------------------------*/

#define frame(owner) sp_normal_init(owner, &objGc)


/*---------------------------------------------
 *              global function
-*---------------------------------------------*/

/* sp_frame.c */
int32_t sp_normal_init(const char *owner, Gc *gc);

