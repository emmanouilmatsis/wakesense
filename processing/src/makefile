CXX = g++
CFLAGS = -c -Wall
LDFLAGS = -Wall
EXECUTABLE = exe
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = Parser/Parser.h Cluster/Cluster.h PearsonCorrelation/PearsonCorrelation.h HierarchicalCluster/HierarchicalCluster.h 
SOURCES = main.cpp Parser/Parser.cpp HierarchicalCluster/HierarchicalCluster.cpp

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $< -o $@ 

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).*
