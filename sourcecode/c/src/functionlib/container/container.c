/*------------------------------------------
	Source file content Five part

	Part Zero:	Include
	Part One:	Define
	Part Two:	Local data
	Part Three:	Local function

	Part Four:	Stack

--------------------------------------------*/

/*------------------------------------------
	        Part Zero: Include
--------------------------------------------*/

#include "sp.h"


/*------------------------------------------
	        Part Four: Stack 
--------------------------------------------*/

/*-----stack_init-----*/
STACK *stack_init(int frame_size)
{
    STACK   *stack_stu;

    if (frame_size < 0) {
        errno = EINVAL;
        return  NULL;
    }

    if (!(stack_stu = malloc(sizeof(STACK))))
        return  NULL;

    stack_stu->stack_bytes = STACK_DEF_SIZE;

    if (!(stack_stu->stack_top = malloc(stack_stu->stack_bytes)))
        return  NULL;

    stack_stu->stack_current = stack_stu->stack_top;

    stack_stu->stack_frame_size = frame_size; 
    stack_stu->stack_num_frame = stack_stu->stack_bytes / frame_size;

    return  stack_stu;
}


/*-----stack_push-----*/
int stack_push(STACK *stack_stu, void *data)
{
    if (!stack_stu || !data) {
        errno = EINVAL;
        return  FUN_RUN_END;
    }

    if (!stack_stu->stack_num_frame)
        return  FUN_RUN_END;

    memcpy(stack_stu->stack_current, data, stack_stu->stack_frame_size);

    stack_stu->stack_current += stack_stu->stack_frame_size;
    stack_stu->stack_num_frame--;

    return  FUN_RUN_OK;
}


/*-----stack_pop-----*/
int stack_pop(STACK *stack_stu, void *buff, int buff_size)
{
    if (!stack_stu || !buff) {
        errno = EINVAL;
        return  FUN_RUN_END;
    }

    if (buff_size < stack_stu.stack_frame_size)
        return  FUN_RUN_END;

    stack_stu->stack_current -= stack_stu->stack_frame_size;
    memcpy(buff, stack_stu->stack_current, stack_stu->stack_frame_size);

    stack_stu->stack_num_frame++;

    return  FUN_RUN_OK;
}


/*-----stack_destroy-----*/
void stack_destroy(STACK *stack_stu)
{
    if (stack_stu) {
        if (stack_stu->stack_top)
            free(stack_stu->stack_top);

        free(stack_stu);
    }
}
