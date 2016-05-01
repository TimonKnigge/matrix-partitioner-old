CC=g++
CFLAGS=-std=gnu++11 -Wall -O2 -c
LFLAGS=-std=gnu++11 -Wall -O2
EXEC=a.out
SOURCES=main.cpp matrix.cpp resettable_array.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(EXEC)
