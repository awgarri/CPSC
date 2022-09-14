all:
	gcc -Wall -g -c -o mythreads.o mythreads.c
	ar -cvr libmythreads.a mythreads.o
	gcc -Wall -g -o ./a.out preemptive_test.c libmythreads.a
clean:
	rm -f libmythreads.a mythreads.o a.out