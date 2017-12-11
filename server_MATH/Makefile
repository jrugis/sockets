CPP = g++
CPP_FLAGS = -march=native -m64 -O3

LINK = $(CPP)
LINK_FLAGS = -lpari

EXE = server
OBJS = server.o solver.o

all: server

solver.o: solver.cpp solver.h
	$(CPP) $(CPP_FLAGS) -c solver.cpp

server.o: server.cpp
	$(CPP) $(CPP_FLAGS) -c server.cpp

$(EXE): $(OBJS)
	$(LINK) -o $(EXE) $(OBJS) $(LINK_FLAGS)

clean:
	rm -f *.o server
