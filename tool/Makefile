# flags #
CC	= gcc
NMFLAGS = -g -Wall -I include/
OTFLAGS = -I ../include/ -I ../include/inclib -I ../include/inclib/musealgo \
		  -I../include/inclib/icmp -I../include/inclib/ping \
		  -I../include/inclib/museipc -I../include/inclib/container
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
SRCTOOL	= src/sp_tool.c ../src/flib/mconfctl/museconfctl.c ../src/flib/sp_global.c ../src/flib/sp_pfun.c \
	  ../src/flib/sp_bufoper.c

SRCRM	= src/sp_remove.c

SRCGA	= src/sp_gather.c ../src/flib/sp_global.c ../src/flib/malgo/sort_algo.c 

SRCFORM	= src/sp_format.c ../src/flib/sp_global.c ../src/flib/sp_pfun.c

SRCARR	= src/sp_word_arrange.c ../src/flib/sp_pfun.c
	
SRCTEST = src/sp_funtest.c \
	 	  ../src/flib/sp_global.c ../src/flib/sp_bufoper.c ../src/flib/sp_misc.c \
		  ../src/flib/sp_pfun.c ../src/flib/sp_message.c \
	      ../src/flib/sp_elog.c ../src/flib/sp_network.c ../src/flib/sp_html.c \
		  ../src/flib/sp_frame.c ../src/flib/chunkmpool/mpool_web.c \
		  ../src/flib/dboper/dboper.c ../src/flib/mconfctl/museconfctl.c \
		  ../src/flib/museato/museato.c ../src/flib/musegc/musegc.c \
		  ../src/flib/museipc/muse_sem.c ../src/flib/musetpool/musepthctl.c \
		  ../src/flib/ping/ping.c ../src/flib/icmp/icmp.c

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

