# flags #
CC		= gcc
STD  	= -std=c99
NMFLAGS	= -rdynamic -g -Wall -I include/libinc/ -I include/ -fstack-protector-all
PTFLAGS	= -pthread -lcrypt -lnsl -lm -lc 
MYFLAGS = -L/usr/local/lib/mysql -lmysqlclient -I /usr/local/include/mysql -lz

UBUGINC = -I src/urlcatcher
EBUGINC = -I src/dataanalyzer

# elf #
EXURLB	= ../../bin/urlcatcher
EXEXTB 	= ../../bin/dataanalyzer
EXTEST  = ../../bin/tester

# obj #
OURLB	= src/urlcatcher/urlcatcher.c src/urlcatcher/databasectl.c \
		  src/urlcatcher/misc.c src/urlcatcher/signal.c \
		  src/urlcatcher/uglobal.c src/urlcatcher/error.c \
	      src/urlcatcher/network.c src/urlcatcher/rule.c \
		  src/functionlib/sp_global.c \
		  src/functionlib/container/sp_bufoper.c \
		  src/functionlib/sp_misc.c src/functionlib/sp_pfun.c \
		  src/functionlib/log/sp_elog.c \
		  src/functionlib/sp_html.c src/functionlib/sp_frame.c \
		  src/functionlib/dboper/dboper.c \
		  src/functionlib/confparser/museconfparser.c \
		  src/functionlib/objgc.c \
		  src/functionlib/ipc/muse_sem.c \
		  src/functionlib/threadpool/musepthctl.c \
		  src/functionlib/network/sp_network.c \
		  src/functionlib/network/ping/ping.c \
		  src/functionlib/network/icmp/icmp.c \
		  src/functionlib/memorypool/chunkmpool/mpool_web.c 

OEXTB	= src/dataanalyzer/dataanalyzer.c src/dataanalyzer/database.c \
		  src/dataanalyzer/error.c src/dataanalyzer/global.c \
		  src/dataanalyzer/misc.c \
		  src/dataanalyzer/mmseg.c src/dataanalyzer/signal.c \
		  src/dataanalyzer/termsctl.c \
		  src/functionlib/sp_global.c \
		  src/functionlib/container/sp_bufoper.c \
		  src/functionlib/sp_misc.c src/functionlib/sp_pfun.c \
		  src/functionlib/log/sp_elog.c \
		  src/functionlib/sp_html.c src/functionlib/sp_frame.c \
		  src/functionlib/dboper/dboper.c \
		  src/functionlib/confparser/museconfparser.c \
		  src/functionlib/objgc.c \
		  src/functionlib/ipc/muse_sem.c \
		  src/functionlib/threadpool/musepthctl.c \
		  src/functionlib/memorypool/mmdpool/musemdpool.c

OTEST   = src/tester/tester.c \
	 	  src/functionlib/sp_global.c \
	      src/functionlib/container/sp_bufoper.c \
		  src/functionlib/sp_misc.c \
		  src/functionlib/sp_pfun.c \
		  src/functionlib/ipc/sp_message.c \
		  src/functionlib/log/sp_elog.c \
		  src/functionlib/network/sp_network.c \
		  src/functionlib/sp_html.c \
		  src/functionlib/sp_frame.c \
		  src/functionlib/memorypool/chunkmpool/mpool_web.c \
		  src/functionlib/dboper/dboper.c \
		  src/functionlib/confparser/museconfparser.c \
		  src/functionlib/objgc.c \
		  src/functionlib/ipc/muse_sem.c \
		  src/functionlib/threadpool/musepthctl.c \
		  src/functionlib/network/ping/ping.c \
		  src/functionlib/network/icmp/icmp.c \
		  src/urlcatcher/rule.c src/urlcatcher/uglobal.c

# phony
.PHONY : urlcatcher dataanalyzer tester

all:			urlcatcher dataanalyzer tester
urlcatcher:		$(EXURLB)
dataanalyzer:	$(EXEXTB)
tester:			$(EXTEST)

$(EXURLB) : $(OURLB)
	$(CC) -o $(EXURLB) $(OURLB) $(NMFLAGS) $(PTFLAGS) $(MYFLAGS) $(UBUGINC) $(STD)

$(EXEXTB) : $(OEXTB)
	$(CC) -o $(EXEXTB) $(OEXTB) $(NMFLAGS) $(PTFLAGS) $(MYFLAGS) $(EBUGINC) $(STD)

$(EXTEST) : $(OTEST)
	$(CC) -o $(EXTEST) $(OTEST) $(NMFLAGS) $(PTFLAGS) $(MYFLAGS) $(UBUGINC) $(STD)


