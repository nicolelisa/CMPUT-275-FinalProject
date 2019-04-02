CC= g++
CFLAGS= -c -Wall -O2 -std=c++11 
LFLAGS=
PROGS= flightpath
OBJS= digraph.o  flightpath.o

# executable targets

all: flightpath

flightpath: $(OBJS)
	$(CC) $(OBJS) -o flightpath

# object targets

flightpath.o: data_and_graphs.cpp bruteforce.h nn.h digraph.h airport.h wdigraph.h
	$(CC) data_and_graphs.cpp -o flightpath.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm -f $(OBJS) $(PROGS)