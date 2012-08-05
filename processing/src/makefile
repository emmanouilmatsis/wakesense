CXX = g++
CFLAGS = -Wall -g -c
LDFLAGS = -Wall -g
EXECUTABLE = exe
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = SetIdentifier/SetIdentifier.h Parser/Parser.h PearsonCorrelation/PearsonCorrelation.h
SOURCES = main.cpp SetIdentifier/SetIdentifier.cpp Parser/Parser.cpp

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).*
