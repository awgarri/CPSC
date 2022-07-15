compile:
	gcc -Wall sched.c -o sched
clean:
	rm sched
runFifo: compile
	./sched -fifo input1.txt output.txt
runSjf: compile
	./sched -sjf input1.txt output.txt
runRR: compile
	./sched -rr input1.txt output.txt
