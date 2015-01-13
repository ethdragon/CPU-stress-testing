CC=g++
CFLAGS= -pthread -std=c++0x -c -Wall 
LDFLAGS= -pthread
SOURCES= cpu_test.cpp KFilter.cpp simple_controller.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cpu

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o cpu

clobber:
	rm -f *.o