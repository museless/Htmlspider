#ifndef _CONTAINER_H
#define _CONTAINER_H

/*-----------------------------
            include
-------------------------------*/


/*=============================
            Stack
===============================*/

/* define */
#define STACK_FRAME_DEF_SIZE    0x4
#define STACK_DEF_SIZE          0x1000

/* typedef */
typedef unsigned char   Byte;

typedef struct  stack   STACK;

/* struct */
struct  stack {
    Byte   *stack_top;
    Byte   *stack_current;

    int     stack_frame_size;
    int     stack_num_frame;
    int     stack_bytes;
};

/* global function */
STACK  *stack_init(int frame_size);
int     stack_push(STACK *stack_stu, void *data);
int     stack_pop(STACK *stack_stu, void *buff, int buff_size);
void    stack_destroy(STACK *stack_stu);

#endif

