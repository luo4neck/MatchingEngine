all: engine.cpp
	clang++-3.5 engine.cpp -Wall -O2 -o engine -std=c++11

1:
	clang++-3.5 main.cpp order.h engine.cpp common.cpp -std=c++11

test: engine
	./engine

clean:
	rm *.gch *.o
