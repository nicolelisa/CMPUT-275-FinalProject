CC= g++
CFLAGS= -c -Wall -O2 -std=c++11 
LFLAGS=
PROGS= airport
OBJS= digraph.o wdigraph.o airport.o

# executable targets
airport: $(OBJS)
	$(CC) airport.o -o airport

airport.o: data_and_graphs.cpp wdigraph.h digraph.h
	$(CC) data_and_graphs.cpp -o airport.o $(CFLAGS)

wdigraph.o: digraph.h wdigraph.h
	$(CC) wdigraph.h -o wdigraph.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm -f $(OBJS) $(PROGS)