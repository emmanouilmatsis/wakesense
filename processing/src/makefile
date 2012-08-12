CXX = g++
CFLAGS = -Wall -g -c
LDFLAGS = -Wall -g
LIBS = -L /usr/local/lib -l sqlite3

EXECUTABLE = exe
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = MLPTID/MLP.h Database/Database.h
SOURCES = main.cpp MLPTID/MLP.cpp Database/Database.cpp

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).*
