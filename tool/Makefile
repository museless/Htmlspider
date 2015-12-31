# flags #
CC	= gcc
NMFLAGS = -g -Wall -I include/
OTFLAGS = -I ../sourcecode/c/include/ -I ../sourcecode/c/include/inclib \
		  -I ../sourcecode/c/include/inclib/musealgo \
		  -I../sourcecode/c/include/inclib/icmp \
		  -I../sourcecode/c/include/inclib/ping \
		  -I../sourcecode/c/include/inclib/museipc \
		  -I../sourcecode/c/include/inclib/container

MSFLAGS	= -I /usr/local/include/mysql -L /usr/local/lib/mysql -lmysqlclient
PTFLAGS = -rdynamic -pthread -lz -lcrypt -lnsl -lm

# Exf
EXTOOL	= bin/sptool
EXRM	= bin/sprm
EXGA	= bin/gatool
EXFORM	= bin/format
EXARR	= bin/arrage
EXTEST	= bin/test

# Obj
SRCTOOL	= src/sp_tool.c ../src/functionlib/mconfctl/museconfctl.c ../src/functionlib/sp_global.c ../src/functionlib/sp_pfun.c \
	  ../src/functionlib/sp_bufoper.c

SRCRM	= src/sp_remove.c

SRCGA	= src/sp_gather.c ../src/functionlib/sp_global.c ../src/functionlib/malgo/sort_algo.c 

SRCFORM	= src/sp_format.c ../src/functionlib/sp_global.c ../src/functionlib/sp_pfun.c

SRCARR	= src/sp_word_arrange.c ../src/functionlib/sp_pfun.c
	
SRCTEST = src/sp_funtest.c \
	 	  ../sourcecode/c/src/functionlib/sp_global.c \
	      ../sourcecode/c/src/functionlib/container/sp_bufoper.c \
		  ../sourcecode/c/src/functionlib/sp_misc.c \
		  ../sourcecode/c/src/functionlib/sp_pfun.c \
		  ../sourcecode/c/src/functionlib/ipc/sp_message.c \
		  ../sourcecode/c/src/functionlib/log/sp_elog.c \
		  ../sourcecode/c/src/functionlib/network/sp_network.c \
		  ../sourcecode/c/src/functionlib/sp_html.c \
		  ../sourcecode/c/src/functionlib/sp_frame.c \
		  ../sourcecode/c/src/functionlib/memorypool/chunkmpool/mpool_web.c \
		  ../sourcecode/c/src/functionlib/dboper/dboper.c \
		  ../sourcecode/c/src/functionlib/mconfctl/museconfctl.c \
		  ../sourcecode/c/src/functionlib/memorybarrier/museato.c \
		  ../sourcecode/c/src/functionlib/garbagecolloter/musegc.c \
		  ../sourcecode/c/src/functionlib/ipc/muse_sem.c \
		  ../sourcecode/c/src/functionlib/threadpool/musepthctl.c \
		  ../sourcecode/c/src/functionlib/network/ping/ping.c \
		  ../sourcecode/c/src/functionlib/network/icmp/icmp.c

# phony
.PHONY : tool gath form rm arrange test

all		: tool gath form rm arrange test
tool	: $(EXTOOL)
rm		: $(EXRM)
gath	: $(EXGA)
form	: $(EXFORM)
arrange : $(EXARR)
test	: $(EXTEST)

$(EXTOOL) : $(SRCTOOL)
	$(CC) -o $(EXTOOL) $(SRCTOOL) $(OTFLAGS) $(NMFLAGS) $(MSFLAGS) $(PTFLAGS)

$(EXRM) : $(SRCRM)
	$(CC) -o $(EXRM) $(SRCRM) $(OTFLAGS) $(NMFLAGS)

$(EXGA) : $(SRCGA)
	$(CC) -o $(EXGA) $(SRCGA) $(OTFLAGS) $(NMFLAGS) $(MSFLAGS) $(PTFLAGS)

$(EXFORM) : $(SRCFORM)
	$(CC) -o $(EXFORM) $(SRCFORM) $(OTFLAGS) $(NMFLAGS) $(MSFLAGS) $(PTFLAGS)

$(EXARR) : $(SRCARR)
	$(CC) -o $(EXARR) $(SRCARR) $(OTFLAGS) $(NMFLAGS) $(MSFLAGS) $(PTFLAGS)

$(EXTEST) : $(SRCTEST)
	$(CC) -o $(EXTEST) $(SRCTEST) $(OTFLAGS) $(NMFLAGS) $(MSFLAGS) $(PTFLAGS)

