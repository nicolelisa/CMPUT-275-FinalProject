CC= g++
CFLAGS= -c -Wall -O2 -std=c++11 
LFLAGS=
PROGS= flightpath
OBJS= digraph.o  flightpath.o dijkstra.o

# executable targets

all: flightpath

flightpath: $(OBJS)
	$(CC) $(OBJS) -o flightpath

# object targets

flightpath.o: flightpath.cpp bruteforce.h nn.h digraph.h airport.h wdigraph.h
	$(CC) flightpath.cpp -o flightpath.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

dijkstra.o: dijkstra.cpp dijkstra.h
	$(CC) dijkstra.cpp -o dijkstra.o $(CFLAGS)

clean:
	@rm -f $(OBJS) $(PROGS)