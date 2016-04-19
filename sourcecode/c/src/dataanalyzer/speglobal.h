#ifndef	_SPEGLOBAL_H
#define	_SPEGLOBAL_H

/*-------------------------
	exten data
---------------------------*/

exten	MGCH	*exbGarCol;
exten	BUFF	*extSaveBuf;

exten	void	*extbugMsgSet;			/* MSGSET */
exten	void	*threadMemPool, *procMemPool;	/* DMPH */
exten	void	*ebSemControl;			/* MSEM */

exten	EXBSET	exbRunSet;
exten	MYSQL	dbNewsHandler, dbDicHandler, dbKeysHandler;

exten	CLISTS	charTermList, charHeadSave;

exten	MGCO	extResCol;

exten	char	sqlSeleCom[];
exten	char	tblNewsName[], dbNewsName[];
exten	char	tblWordName[], dbDicName[];
exten	char	tblKeysName[], dbKeysName[];

exten	MATOS	pthreadCtlLock, freeCtlLock, nPaperLock, dicDbLock;

exten	uLong	tPaperNum;
exten	int	exbugIpcFd, nExbugPthead, upMaxTerms, keywordListSize;

#endif
