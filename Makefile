build : reverse.o
	gcc reverse.o -o reverse
	rm -rf *.o

clean : 
	rm -rf reverse
