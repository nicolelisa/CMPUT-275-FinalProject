CC= g++
CFLAGS= -c -Wall -O2 -std=c++11 
LFLAGS=
PROGS= airport
OBJS= digraph.o wdigraph.o airport.o

# executable targets
airport: $(OBJS)
	$(CC) airport.o -o airport

<<<<<<< HEAD
airport.o: data_and_graphs.cpp wdigraph.h digraph.h
=======
airport.o: data_and_graphs.cpp wdigraph.o airport.h
>>>>>>> a70db87703faefc0b9ea7e10272abfce99e897e0
	$(CC) data_and_graphs.cpp -o airport.o $(CFLAGS)

wdigraph.o: digraph.h wdigraph.h
	$(CC) wdigraph.h -o wdigraph.o $(CFLAGS)

digraph.o: digraph.cpp digraph.h
	$(CC) digraph.cpp -o digraph.o $(CFLAGS)

clean:
	@rm -f $(OBJS) $(PROGS)