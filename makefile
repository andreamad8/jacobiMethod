CXX = g++
CXXFLAGS = -std=c++11 -pthread -O3
FF_LOCAL = -I ../mc-fastflow-code

all: ser par ff

ser: src/jacobiser.cpp
	$(CXX) src/jacobiser.cpp -o bin/jacobiser -std=c++11 -O3

par: src/jacobiPar.cpp
	$(CXX) src/jacobiPar.cpp  -o bin/jacobiPar $(CXXFLAGS)

ff: src/jacobiFor.cpp
	$(CXX) src/jacobiFor.cpp -o bin/jacobiFor $(FF_LOCAL) $(CXXFLAGS)

clean:
	rm bin/jacobiser bin/jacobipar bin/jacobiFor
