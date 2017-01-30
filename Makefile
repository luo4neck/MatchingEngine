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


#test all:
#	ls

test1:$(BUILDDIR)/$(EXE) $(TESTDIR)/test1.dat
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/test1.dat

testcancel:$(BUILDDIR)/$(EXE) $(TESTDIR)/testcancel.dat
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/testcancel.dat
	@echo 
	@echo "file used:" $(TESTDIR) "/testcancel.dat"
	@echo "expected result: order with price of 1300 disappeared"
	@echo 

testprint:$(BUILDDIR)/$(EXE) $(TESTDIR)/testprint.dat
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/testprint.dat
	@echo 
	@echo "file used:" $(TESTDIR) "/testprint.dat"
	@echo "expected result: SELL order with 2 prices printed, 1500-234, 1400-205"
	@echo "                 BUY order with 3 prices printed, 1300-100, 1200-610, 1100-410"
	@echo 

#testmodify:

.PHONY:clean
clean:
	# below line make use of '-mv' to clean the folder in case compilation 
	# was not successfully done..
	-mv $(OBJ) $(EXE) $(BUILDDIR)
	rm -rf $(BUILDDIR)
#	ls
