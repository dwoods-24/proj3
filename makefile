EXECUTABLES = bin/sb-read bin/sb-analyze bin/sb-play
CPP = g++
CFLAGS = -Wall -Wextra

all: $(EXECUTABLES)

clean:
	rm -f bin/*



# Rule for object file
obj/disjoint-rank.o: src/disjoint-rank.cpp
	$(CPP) $(CFLAGS) -c obj/disjoint-rank.o src/disjoint-rank.cpp

# Compilation rules
bin/sb-read: src/sb-read.cpp
	$(CPP) -o bin/sb-read src/sb-read.cpp

bin/sb-analyze: src/sb-analyze.cpp obj/disjoint-rank.o
	$(CPP) $(CFLAGS) -o bin/sb-analyze src/sb-analyze.cpp obj/disjoint-rank.o

bin/sb-play: src/sb-play.cpp obj/disjoint-rank.o
	$(CPP) $(CFLAGS) -o bin/sb-play src/sb-play.cpp obj/disjoint-rank.o

