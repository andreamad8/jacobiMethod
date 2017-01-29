main: src/jacobiser.cpp
	g++ src/jacobiser.cpp -o bin/jacobiser -std=c++11

par: src/jacobipar.cpp
	g++ src/jacobipar.cpp -o bin/jacobipar -std=c++11 -pthread

ff: src/jacobiFor.cpp
	g++ src/jacobiFor.cpp -o bin/jacobiFor -I ../mc-fastflow-code -std=c++11 -pthread

phi: jacobiser.cpp
	icc jacobiser.cpp -o bin/jacobiserICC -mmic -std=c++11 -pthread -O3
	scp jacobiserICC mic0:
	scp test.sh mic0:


clean:
	rm jacobiser jacobipar jacobiFor
