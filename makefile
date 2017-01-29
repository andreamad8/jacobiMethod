CXX = g++
CXXFLAGS = -std=c++11 -pthread -O3
ser: src/jacobiser.cpp bin/helper.o
	$(CXX) src/jacobiser.cpp bin/helper.o -o bin/jacobiser -std=c++11

par: src/jacobipar.cpp bin/helper.o bin/bar.o
	$(CXX) src/jacobipar.cpp bin/bar.o bin/helper.o -o bin/jacobipar $(CXXFLAGS)

ff: src/jacobiFor.cpp bin/helper.o
	$(CXX) src/jacobiFor.cpp bin/helper.o -o bin/jacobiFor -I ../mc-fastflow-code $(CXXFLAGS)

phi:  src/jacobiser.cpp
	icc  src/jacobiser.cpp -o bin/jacobiserICC -mmic $(CXXFLAGS)
	scp bin/jacobiserICC mic0:
	scp test.sh mic0:

bin/%.o: src/%.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm bin/jacobiser bin/jacobipar bin/jacobiFor
