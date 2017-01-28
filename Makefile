CC		=clang++-3.5
EXE		=main
OBJ		=main.o engine.o common.o
FLAG	=-Wall -std=c++11 -O2
BUILDDIR=build
SRCDIR	=source
TESTDIR	=test

$(BUILDDIR)/$(EXE):$(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(FLAG)
	@echo "All .o and executable file generated.." 
	@echo 
	-mkdir $(BUILDDIR)
	mv $(OBJ) $(EXE) $(BUILDDIR)
	@echo "Moved to directory /build for running and testing.."
	@echo 


main.o:$(SRCDIR)/main.cpp
	$(CC) -c $(SRCDIR)/main.cpp $(FLAG)

engine.o:$(SRCDIR)/engine.cpp $(SRCDIR)/engine.h 
	$(CC) -c $(SRCDIR)/engine.cpp $(FLAG)

common.o:$(SRCDIR)/common.cpp $(SRCDIR)/common.h
	$(CC) -c $(SRCDIR)/common.cpp $(FLAG)


test1:$(BUILDDIR)/$(EXE) $(TESTDIR)/test1.dat
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/test1.dat


.PHONY:clean
clean:
	# below line make use of '-mv' to clean the folder in case compilation 
	# was not successfully done..
	-mv $(OBJ) $(EXE) $(BUILDDIR)
	rm -rf $(BUILDDIR)
	ls
