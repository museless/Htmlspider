/* global data */
/* spnet.h */
char	*rPac =	"Accept-Language: zh-CN\r\n"
		"User-Agent: Mozilla/5.0 (X11; Fedora; Linux x86_64; rv:45.0) Gecko/20100101 Firefox/45.0\r\n"
		"Connection: Keep-Alive\r\n\r\n";

/* spdb.h */
char	*sqlOpt[] = {"db", "--default-file = my.cnf"};
int	nrOpt = (sizeof(sqlOpt) / sizeof(char *));

/* spinc.h */
char	kerNameBuf[] = {"Kernel"};
char	ubNameBuf[] = {"UrlBug"};
char	tbNameBuf[] = {"TextBug"};
char	ebNameBuf[] = {"ExtBug"};
