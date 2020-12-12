CXX = g++
CXXFLAGS = -Wall

OBJECTS = avl.o wordrange.o

wordrange: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o wordrange avl.o wordrange.o

avl.o: avl.cpp avl.h
	$(CXX) -g $(CXXFLAGS) -c avl.cpp
	
wordrange.o: avl.cpp avl.h wordrange.cpp
	$(CXX) -g $(CXXFLAGS) -c wordrange.cpp

clean: 
	rm -f *.o
	rm wordrange

