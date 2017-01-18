all: engine.cpp
	clang++-3.5 engine.cpp -Wall -o test_ -std=c++11

test: test_
	./test_

clean:
	rm test_
