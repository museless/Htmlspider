# flags #
CC		= gcc
NMFLAGS	= -rdynamic -g -Wall -I include/inclib -I include/inclib/mmdpool -I include/ -fstack-protector-all
PTFLAGS	= -pthread -lcrypt -lnsl -lm -lc 
MYFLAGS = -L /usr/local/lib/mysql -lmysqlclient -I /usr/local/include/mysql -lz
PGFLAGS = -I include/inclib/ping -I include/inclib/icmp

UBUGINC = -I src/spider

IPCINC	= -I include/inclib/museipc
ALGOINC = -I include/inclib/musealgo

# elf #
EXURLB	= bin/spider

# obj #
OURLB	= src/spider/sp_urlbug.c src/spider/sp_urlbug_db.c \
		  src/spider/sp_urlbug_disc.c src/spider/sp_urlbug_signal.c \
		  src/spider/sp_urlbug_clean.c src/spider/sp_urlbug_msg.c \
		  src/spider/sp_urlbug_err.c src/spider/sp_urlbug_network.c \
		  src/spider/sp_urlbug_rule.c \
		  src/flib/sp_global.c src/flib/sp_bufoper.c src/flib/sp_misc.c \
		  src/flib/sp_pfun.c src/flib/sp_message.c src/flib/sp_elog.c \
		  src/flib/sp_network.c src/flib/sp_frame.c src/flib/chunkmpool/mpool_web.c \
		  src/flib/dboper/dboper.c src/flib/mconfctl/museconfctl.c \
		  src/flib/museato/museato.c src/flib/musegc/musegc.c \
		  src/flib/museipc/muse_sem.c src/flib/musetpool/musepthctl.c \
		  src/flib/ping/ping.c src/flib/icmp/icmp.c

# phony
.PHONY : spider

spider:	$(EXURLB)

$(EXURLB) : $(OURLB)
	$(CC) -o $(EXURLB) $(OURLB) $(NMFLAGS) $(PTFLAGS) $(MYFLAGS) $(UBUGINC) $(IPCINC) $(PGFLAGS)
