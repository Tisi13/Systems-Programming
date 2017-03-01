CC=gcc
CFLAGS=-Wall

build: audioserver.o audioclient.o audio.o
	$(CC) $(CFLAGS) audioserver.o audio.o -o audioserver
	$(CC) $(CFLAGS) audioclient.o audio.o -o audioclient
	
clean:
		
	rm -rf audioserver
	rm -rf audioclient
	rm -rf *.o
