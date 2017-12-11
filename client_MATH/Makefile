# module load gcc/4.7.3

CPP = g++
CPP_FLAGS = -m64 -O3

LINK = $(CPP)
LINK_FLAGS = -m64

EXE = client
OBJS = $(EXE).o

all: $(EXE)

$(EXE).o: $(EXE).cpp
	$(CPP) $(CPP_FLAGS) -o $@ -c $<

$(EXE): $(OBJS)
	$(LINK) $(LINK_FLAGS) -o $@ $+

clean:
	rm -f *.o $(EXE) 
