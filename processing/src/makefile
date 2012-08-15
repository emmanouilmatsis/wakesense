CXX = g++
CFLAGS = -Wall -g -c
LDFLAGS = -Wall -g
LIBS = -L /usr/local/lib -l sqlite3

EXECUTABLE = exe
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = Parser/Parser.h MLPTID/Entry.h MLPTID/Index.h Database/Database.h
SOURCES = main.cpp Parser/Parser.cpp MLPTID/Entry.cpp MLPTID/Index.cpp Database/Database.cpp

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).* ../data/database.db
