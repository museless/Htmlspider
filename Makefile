# flags #
CC		= gcc
NMFLAGS	= -rdynamic -g -Wall -I include/inclib -I include/inclib/mmdpool -I include/ -fstack-protector-all
PTFLAGS	= -pthread -lcrypt -lnsl -lm -lc 
MYFLAGS = -L /usr/local/lib/mysql -lmysqlclient -I /usr/local/include/mysql -lz
PGFLAGS = -I include/inclib/ping -I include/inclib/icmp

UBUGINC = -I src/urlcatcher

IPCINC	= -I include/inclib/museipc
ALGOINC = -I include/inclib/musealgo

# elf #
EXURLB	= bin/urlcatcher

# obj #
OURLB	= src/urlcatcher/sp_urlbug.c src/urlcatcher/sp_urlbug_db.c \
		  src/urlcatcher/sp_urlbug_disc.c src/urlcatcher/sp_urlbug_signal.c \
		  src/urlcatcher/sp_urlbug_clean.c src/urlcatcher/sp_urlbug_msg.c \
		  src/urlcatcher/sp_urlbug_err.c src/urlcatcher/sp_urlbug_network.c \
		  src/urlcatcher/sp_urlbug_rule.c \
		  src/functionlib/sp_global.c \
		  src/functionlib/container/sp_bufoper.c \
		  src/functionlib/sp_misc.c src/functionlib/sp_pfun.c \
		  src/functionlib/log/sp_elog.c \
		  src/functionlib/sp_html.c src/functionlib/sp_frame.c \
		  src/functionlib/dboper/dboper.c \
		  src/functionlib/mconfctl/museconfctl.c \
		  src/functionlib/memorybarrier/museato.c \
		  src/functionlib/garbagecolloter/musegc.c \
		  src/functionlib/ipc/muse_sem.c src/functionlib/ipc/sp_message.c \
		  src/functionlib/threadpool/musepthctl.c \
		  src/functionlib/network/sp_network.c \
		  src/functionlib/network/ping/ping.c src/functionlib/network/icmp/icmp.c \
		  src/functionlib/memorypool/chunkmpool/mpool_web.c 

# phony
.PHONY : urlcatcher

urlcatcher:	$(EXURLB)

$(EXURLB) : $(OURLB)
	$(CC) -o $(EXURLB) $(OURLB) $(NMFLAGS) $(PTFLAGS) $(MYFLAGS) $(UBUGINC) $(IPCINC) $(PGFLAGS)
