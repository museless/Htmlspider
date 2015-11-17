#ifndef	_MPCTL_H
#define	_MPCTL_H

/*---------------------
	include
-----------------------*/

#include <stdio.h>
#include <stdlib.h>

#define __USE_GNU
#include <pthread.h>

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


/*---------------------
	define
-----------------------*/

/* normal use */
#define	PTH_RUN_OK              1
#define	PTH_RUN_END		        0
#define	PTH_RUN_FAIL		    -1

#define	PTH_RUN_PERMANENT	    0x1


/*---------------------
	typedef
-----------------------*/

typedef	pthread_mutex_t	        pmut_t;
typedef	pthread_cond_t	        pcd_t;

typedef	void	                (*pthrun)(void *);

typedef	struct pthread_pool     PTHPOOL;
typedef	struct pthread_entity	PTHENT;


/*---------------------
	struct
-----------------------*/

struct	pthread_pool {
	PTHENT *pl_list;

	TMVAL   pl_tim;
	int     pl_cnt;
};

struct	pthread_entity {
	void   *pe_data;
	pthrun	pe_run;

	pth_t	pe_tid;

	pmut_t	pe_mutex;
	pcd_t	pe_cond;

    int     pe_flags;
};


/*---------------------
	glo fun
-----------------------*/

PTHPOOL    *mpc_create(int nPthread);
int 	    mpc_thread_wake(PTHPOOL *threadPool, pthrun runFun, void *pPara);
void	    mpc_thread_wait(PTHPOOL *thPool);
void	    mpc_destroy(PTHPOOL *thPool);


#endif
