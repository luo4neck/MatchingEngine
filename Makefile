CC		=clang++-3.5
EXE		=main
OBJ		=main.o engine.o common.o
FLAG	=-Wall -std=c++11 -O2
BUILDDIR=build
SRCDIR	=source

$(EXE):$(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(FLAG)
	@echo 
	@echo "All .o and executable file generated.." 
	@echo 
	mkdir $(BUILDDIR)
	mv ./$(OBJ) ./$(BUILDDIR)
	mv ./$(EXE) ./$(BUILDDIR)
	@echo
	@echo "Moved to directory /build for running and testing.."

main.o:$(SRCDIR)/main.cpp
	$(CC) -c $(SRCDIR)/main.cpp $(FLAG)

engine.o:$(SRCDIR)/engine.cpp $(SRCDIR)/engine.h 
	$(CC) -c $(SRCDIR)/engine.cpp $(FLAG)

common.o:$(SRCDIR)/common.cpp $(SRCDIR)/common.h
	$(CC) -c $(SRCDIR)/common.cpp $(FLAG)


.PHONY:clean
clean:
	rm -rf ./$(BUILDDIR)
