include ../make.inc

HEADS = ../includes/type.h

exe = test1 test2

all: $(exe)	
test1: 	test1.cpp $(HEADS)
	$(CXX) $(CXXFLAGS) -o test1 test1.cpp
test2: 	test2.cpp $(HEADS)
	$(CXX) $(CXXFLAGS) -o test2 test2.cpp

clean:
	rm -rf *.o $(exe)
