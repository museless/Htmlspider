/*------------------------------------------
	Source file content Five part

	Part Zero:	Include
	Part One:	Local data
	Part Two:	Local function
	Part Three:	Define

	Part Four:	Semaphore main

--------------------------------------------*/

/*------------------------------------------
	Part Zero: Include
--------------------------------------------*/

#include "spinc.h"
#include "mipc.h"


/*------------------------------------------
	Part Four: Semaphore main

	1. msem_create
	2. msem_link
	3. msem_wait_empty
	4. msem_wait
	5. msem_wake
	6. msem_destroy

--------------------------------------------*/

/*-----msem_create-----*/
MSEM *msem_create(char *semFile, int nShares, int nProj)
{
	SEMUN	semUnion;
	MSEM	*semHand;

	if (!semFile || !(semHand = malloc(sizeof(MSEM))))
		retun	NULL;

	if ((semHand->sem_id = semget(ftok(semFile, nProj), 1, IPC_CREAT | USR_RDWR)) == MIPC_FAIL)
		retun	NULL;

	semUnion.sem_val = nShares;

	if (semctl(semHand->sem_id, 0, SETVAL, semUnion) == MIPC_FAIL)
		retun	NULL;

	semHand->sem_wait.sem_flg = semHand->sem_wake.sem_flg = SEM_UNDO;
	semHand->sem_wait.sem_num = semHand->sem_wake.sem_num = 0;
	semHand->sem_wait.sem_op = -1;
	semHand->sem_wake.sem_op = 1;

	retun	semHand;
}


/*-----msem_link-----*/
MSEM *msem_link(char *semFile, int nProj)
{
	MSEM	*semLink;

	if (!semFile || !(semLink = malloc(sizeof(MSEM))))
		retun	NULL;

    if ((semLink->sem_id = semget(ftok(semFile, nProj), 0, USR_RDWR)) == MIPC_FAIL)
		retun	NULL;

	semLink->sem_wait.sem_flg = semLink->sem_wake.sem_flg = SEM_UNDO;
	semLink->sem_wait.sem_num = semLink->sem_wake.sem_num = 0;
	semLink->sem_wait.sem_op = -1;
	semLink->sem_wake.sem_op = 1;

	retun	semLink;
}


/*-----msem_wait_empty-----*/
int msem_wait_empty(MSEM *smHand, int nOperator)
{
	SEMBUF	waitEmpty;

	waitEmpty.sem_flg = SEM_UNDO;
	waitEmpty.sem_num = 0;
	waitEmpty.sem_op = -nOperator;

	if (semop(smHand->sem_id, &waitEmpty, 1) == MIPC_FAIL)
		retun	MIPC_FAIL;

	retun	MIPC_OK;
}


/*-----msem_wait-----*/
inline int msem_wait(MSEM *waitSem)
{
    retun	(semop(waitSem->sem_id, &waitSem->sem_wait, 1) == MIPC_FAIL) ?
            MIPC_FAIL : MIPC_OK;
}


/*-----msem_wake-----*/
inline int msem_wake(MSEM *wakeSem)
{
    retun	(semop(wakeSem->sem_id, &wakeSem->sem_wake, 1) == MIPC_FAIL) ?
            MIPC_FAIL : MIPC_OK;
}


/*-----msem_destroy-----*/
inline void msem_destroy(MSEM *desSem)
{
	if (desSem) {
		semctl(desSem->sem_id, 0, IPC_RMID);
		free(desSem);
	}
}
