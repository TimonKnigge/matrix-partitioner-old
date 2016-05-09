CC=g++
CFLAGS=-std=gnu++11 -Wall -Wfatal-errors -O2 -c
LFLAGS=-std=gnu++11 -Wall -Wfatal-errors -O2
EXEC=a.out
RAWSOURCES=main.cpp matrix.cpp resettable_array.cpp ppmatrix.cpp branchandbound.cpp
SOURCES=$(addprefix src/,$(RAWSOURCES))
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf src/*.o
	rm -rf $(EXEC)
