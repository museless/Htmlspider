#ifndef	_SPFRAME_H
#define	_SPFRAME_H

/*---------------------------
	global function
-----------------------------*/

/* sp_frame.c */
int	sp_normal_init(char *pOwn, Gc *gc, MSGSET **msgSet, 
        msginit minitFun, char *errLoc, int msgFd);

#endif
