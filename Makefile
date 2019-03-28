CC= g++
CFLAGS= -c -Wall -O2 -std=c++11 
LFLAGS=
PROGS= airport
OBJS= digraph.o wdigraph.o airport.o

# executable targets
airport: wdigraph.o digraph.o airport.o
	$(CC) airport.o -o airport $(LFLAGS)

airport.o: data_and_graphs.cpp wdigraph.o
	$(CC) data_and_graphs.cpp -o airport.o $(CFLAGS)

wdigraph.o: digraph.o wdigraph.h
	$(CC) wdigraph.h -o wdigraph.o $(CFLAGS)

digraph.o: digraph.h
	$(CC) digraph.h -o digraph.o $(CFLAGS)

clean:
	@rm -f $(OBJS) $(PROGS)