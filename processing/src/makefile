CXX = g++
CFLAGS = -Wall -g -c
LDFLAGS = -Wall -g
LIBS = -L/usr/local/lib -lsqlite3 -lboost_system -lboost_filesystem -L/usr/X11R6/lib -lglut -lGLU -lGL -lXi -lXmu -lXt -lXext -lX11 -lSM -lICE -lm
INC = -I/user/local/include -H/usr/X11R6/include -I.

EXECUTABLE = exe
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = Score/Score.h TrickDatabase/TrickDatabase.h TrickContainer/Trick.h TrickGenerator/TrickGenerator.h SupervisedTrickIdentifier/SupervisedTrickIdentifier.h UnsupervisedTrickIdentifier/UnsupervisedTrickIdentifier.h Parser/Parser.h Index/Index.h Index/Entry.h Database/Database.h Receiver/Receiver.h XBee/XBee.h SerialTimeout/SerialTimeout.h PearsonCorrelationCoefficient/PearsonCorrelationCoefficient.h Converter/Converter.h #TrickGenerator/TrickGeneratorFile.h
SOURCES = main.cpp Score/Score.cpp TrickDatabase/TrickDatabase.cpp TrickDatabase/Trick.cpp TrickGenerator/TrickGenerator.cpp SupervisedTrickIdentifier/SupervisedTrickIdentifier.cpp UnsupervisedTrickIdentifier/UnsupervisedTrickIdentifier.cpp Parser/Parser.cpp Index/Index.cpp Index/Entry.cpp Database/Database.cpp Receiver/Receiver.cpp XBee/XBee.cpp SerialTimeout/SerialTimeout.cpp #TrickGenerator/TrickGeneratorFile.cpp

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CFLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).* ./*.db
