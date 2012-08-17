CXX = g++
CFLAGS = -Wall -g -c
LDFLAGS = -Wall -g
LIBS = -L /usr/local/lib -l sqlite3 -lboost_system
INC = -I /user/local/include

EXECUTABLE = exe
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = Receiver/Receiver.h XBee/XBee.h SerialTimeout/SerialTimeout.h
SOURCES = main.cpp Receiver/Receiver.cpp XBee/XBee.cpp SerialTimeout/SerialTimeout.cpp

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).*
