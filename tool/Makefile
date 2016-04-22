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
SRCTOOL	= src/sp_tool.c \
	 	  ../sourcecode/c/src/functionlib/sp_global.c \
		  ../sourcecode/c/src/functionlib/sp_pfun.c \
		  ../sourcecode/c/src/functionlib/memorypool/chunkmpool/mpool_web.c \
		  ../sourcecode/c/src/functionlib/dboper/dboper.c \
		  ../sourcecode/c/src/functionlib/mconfctl/museconfctl.c \
		  ../sourcecode/c/src/functionlib/memorybarrier/museato.c \
		  ../sourcecode/c/src/functionlib/garbagecolloter/musegc.c \
	      ../sourcecode/c/src/functionlib/container/sp_bufoper.c

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
.PHONY : tool test

all		: tool test
tool	: $(EXTOOL)
test	: $(EXTEST)

$(EXTOOL) : $(SRCTOOL)
	$(CC) -o $(EXTOOL) $(SRCTOOL) $(OTFLAGS) $(NMFLAGS) $(MSFLAGS) $(PTFLAGS)

$(EXTEST) : $(SRCTEST)
	$(CC) -o $(EXTEST) $(SRCTEST) $(OTFLAGS) $(NMFLAGS) $(MSFLAGS) $(PTFLAGS)

