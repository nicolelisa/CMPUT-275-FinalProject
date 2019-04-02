CC= g++
CFLAGS= -c -Wall -O2 -std=c++11 
LFLAGS=
PROGS= flightpath
OBJS= digraph.o flightpath.o bruteforce.o nn.o

# executable targets
flightpath: $(OBJS)
	$(CC) flightpath.o -o flightpath

flightpath.o: data_and_graphs.cpp digraph.o airport.h wdigraph.h nn.o bruteforce.o 
	$(CC) data_and_graphs.cpp -o flightpath.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

bruteforce.o: bruteforce.cpp bruteforce.h
	$(CC) bruteforce.cpp -o bruteforce.o $(CFLAGS)

nn.o: nn.cpp nn.h
	$(CC) nn.cpp -o nn.o $(CFLAGS)

clean:
	@rm -f $(OBJS) $(PROGS)