compile:
	gcc -Wall sched.c -o sched
clean:
	rm sched
runFIFO: compile
	./sched -fifo input1.txt out1.txt
runSJF: compile
	./sched -sjf input1.txt out1.txt
runRR: compile
	./sched -rr input1.txt out1.txt
