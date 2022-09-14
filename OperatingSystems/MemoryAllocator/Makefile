
CC=gcc

BINS=libmyalloc.so 
	
libmyalloc.so:  allocator.c
	$(CC) -O2 -DNDEBUG -Wall -fPIC -shared allocator.c -o libmyalloc.so

clean:
	rm $(BINS) *.o

