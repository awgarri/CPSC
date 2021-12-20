CC = g++ -std=c++11
BINS = doublyLinkedList

all:
	$(CC) -o doublyLinkedList doublyLinkedList.cpp

run:
	./doublyLinkedList

clean:
	rm -f $(BINS)