all: engine.cpp
	clang++-3.5 engine.cpp -Wall -o test -std=c++11

test: test
	./test

clean:
	rm test
