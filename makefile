CXX = g++
CXXFLAGS = -std=c++11 -pthread -O3
FF_LOCAL = -I ../mc-fastflow-code

ser: src/jacobiser.cpp bin/helper.o
	$(CXX) src/jacobiser.cpp bin/helper.o -o bin/jacobiser -std=c++11

par: src/jacobipar.cpp bin/helper.o bin/bar.o
	$(CXX) src/jacobipar.cpp bin/bar.o bin/helper.o -o bin/jacobipar $(CXXFLAGS)

ff: src/jacobiFor.cpp bin/helper.o
	$(CXX) src/jacobiFor.cpp bin/helper.o -o bin/jacobiFor $(FF_LOCAL) $(CXXFLAGS)


bin/%.o: src/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm bin/jacobiser bin/jacobipar bin/jacobiFor
