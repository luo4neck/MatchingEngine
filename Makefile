cc=clang++-3.5
exe=main
obj=main.o engine.o common.o
flag=-Wall -std=c++11 -O2

$(exe):$(obj)
	$(cc) -o $(exe) $(obj) $(flag)

main.o:main.cpp
	$(cc) -c main.cpp $(flag)

engine.o:engine.cpp
	$(cc) -c engine.cpp $(flag)

common.o:common.cpp
	$(cc) -c common.cpp $(flag)

clean:
	rm *.o main
