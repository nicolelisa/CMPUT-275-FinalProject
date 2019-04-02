CC= g++
CFLAGS= -c -Wall -O2 -std=c++11 
LFLAGS=
PROGS= airport
OBJS= mdigraph.o airport.o

# executable targets
airport: $(OBJS)
  $(CC) airport.o -o airport

airport.o: data_and_graphs.cpp digraph.o airport.h wdigraph.h
  $(CC) data_and_graphs.cpp -o airport.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
  $(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
  @rm -f $(OBJS) $(PROGS)