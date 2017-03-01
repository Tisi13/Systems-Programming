CC = gcc

CFLAGS =  -Wall -Wextra -Werror

all: pingserver pingclient1 pingclient2 pingclient3

pingserver: pingserver.c
		$(CC) $(CFLAGS) pingserver.c -o pingserver

pingclient1: pingclient1.c
		$(CC) $(CFLAGS) pingclient1.c -o pingclient1

pingclient2: pingclient2.c
		$(CC) $(CFLAGS) pingclient2.c -o pingclient2

pingclient3: pingclient3.c
		$(CC) $(CFLAGS) pingclient3.c -o pingclient3

clean: 
		rm -rf pingserver pingclient1 pingclient2 pingclient3  *.o *~

