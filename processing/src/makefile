CXX = g++
CFLAGS = -Wall -g -c
LDFLAGS = -Wall -g
LIBS = -L /usr/local/lib -l sqlite3 -lboost_system
INC = -I /user/local/include

EXECUTABLE = exe
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = MLPTID/MLPTID.h PCCTID/PCCTID.h Parser/Parser.h Index/Index.h Index/Entry.h Database/Database.h Receiver/Receiver.h XBee/XBee.h SerialTimeout/SerialTimeout.h PearsonCorrelationCoefficient/PearsonCorrelationCoefficient.h BTOI/BTOI.h
SOURCES = main.cpp MLPTID/MLPTID.cpp PCCTID/PCCTID.cpp Parser/Parser.cpp Index/Index.cpp Index/Entry.cpp Database/Database.cpp Receiver/Receiver.cpp XBee/XBee.cpp SerialTimeout/SerialTimeout.cpp

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).* ../data/database.db
