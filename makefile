main: jacobiser.cpp
	g++ jacobiser.cpp -o jacobiser -std=c++11

par: jacobipar.cpp
	g++ jacobipar.cpp -o jacobipar -std=c++11 -pthread

ff: jacobiFor.cpp
	g++ jacobiFor.cpp -o jacobiFor -I ../mc-fastflow-code -std=c++11 -pthread

phi: jacobiser.cpp
	icc jacobiser.cpp -o jacobiserICC -mmic -std=c++11 -pthread -O3
	scp jacobiserICC mic0:
	scp test.sh mic0:


clean:
	rm jacobiser jacobipar jacobiFor
