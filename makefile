CXX = g++
CXXFLAGS = -std=c++11 -pthread -O3
FF_LOCAL = -I ../mc-fastflow-code

ser: src/jacobiser.cpp
	$(CXX) src/jacobiser.cpp -o bin/jacobiser -std=c++11

par: src/jacobipar.cpp
	$(CXX) src/jacobipar.cpp  -o bin/jacobipar $(CXXFLAGS)

ff: src/jacobiFor.cpp
	$(CXX) src/jacobiFor.cpp -o bin/jacobiFor $(FF_LOCAL) $(CXXFLAGS)

clean:
	rm bin/jacobiser bin/jacobipar bin/jacobiFor
