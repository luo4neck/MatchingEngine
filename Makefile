CC		=clang++-3.5
##CC		=g++
EXE		=main
OBJ		=main.o engine.o common.o
FLAG	=-Wall -std=c++11 -O2
BUILDDIR=build
SRCDIR	=source
TESTDIR	=test
TC		=testcancel
TP		=testprint

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

.PHONY:clean
clean:
	# below line make use of '-mv' to clean the folder in case compilation 
	# was not successfully done..
	-mv $(OBJ) $(EXE) $(BUILDDIR)
	rm -rf $(BUILDDIR)

############ above cmds are for building, below cmds are for testing ############

test:$(TC) $(TP)

test1:$(BUILDDIR)/$(EXE) $(TESTDIR)/test1.dat
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/test1.dat

$(TC):$(BUILDDIR)/$(EXE) $(TESTDIR)/$(TC).dat $(TESTDIR)/$(TC).cor 
	@echo "Unit testing for functionality of CANCEL:"
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/$(TC).dat > $(BUILDDIR)/$(TC).tmp
	diff $(TESTDIR)/$(TC).cor $(BUILDDIR)/$(TC).tmp
	@echo "expected result: no difference between these 2 compared files" 
	@echo 

$(TP):$(BUILDDIR)/$(EXE) $(TESTDIR)/$(TP).dat $(TESTDIR)/$(TP).cor 
	@echo "Unit testing for functionality of PRINT:"
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/$(TP).dat > $(BUILDDIR)/$(TP).tmp
	diff $(TESTDIR)/$(TP).cor $(BUILDDIR)/$(TP).tmp
	@echo "expected result: no difference between these 2 compared files" 
	@echo 

#testmodify:
