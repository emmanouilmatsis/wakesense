CXX = g++
CFLAGS = -c -Wall
LDFLAGS = -Wall
EXECUTABLE = exe
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = $(wildcard *.h)
SOURCES = $(wildcard *.cpp)

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $^ -o $@


%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $< -o $@ 

clean:
	rm -rf *.o $(EXECUTABLE) $(EXECUTABLE).*
