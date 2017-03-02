CC		=clang++-3.5
##CC    =g++
EXE		=main
OBJ		=main.o engine.o common.o
FLAG	=-Wall -std=c++11 -O2
BUILDDIR=build
SRCDIR	=source
TESTDIR	=test
TC		=testcancel
TP		=testprint
TM		=testmodify
TB		=testbuy
TS		=testsell
TL		=testlarge
TLV		=40

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

test:$(TC) $(TP) $(TM) $(TB) $(TS)

# test of CANCEL..
$(TC):$(BUILDDIR)/$(EXE) $(TESTDIR)/$(TC).dat $(TESTDIR)/$(TC).cor 
	@echo "Unit testing for functionality of CANCEL:"
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/$(TC).dat > $(BUILDDIR)/$(TC).tmp
	diff $(TESTDIR)/$(TC).cor $(BUILDDIR)/$(TC).tmp
	@echo "expected result: no difference between these 2 compared files" 
	@echo 

# test of PRINT..
$(TP):$(BUILDDIR)/$(EXE) $(TESTDIR)/$(TP).dat $(TESTDIR)/$(TP).cor 
	@echo "Unit testing for functionality of PRINT:"
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/$(TP).dat > $(BUILDDIR)/$(TP).tmp
	diff $(TESTDIR)/$(TP).cor $(BUILDDIR)/$(TP).tmp
	@echo "expected result: no difference between these 2 compared files" 
	@echo 

# test of MODIFY..
$(TM):$(BUILDDIR)/$(EXE) $(TESTDIR)/$(TM).dat $(TESTDIR)/$(TM).cor 
	@echo "Unit testing for functionality of MODIFY:"
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/$(TM).dat > $(BUILDDIR)/$(TM).tmp
	diff $(TESTDIR)/$(TM).cor $(BUILDDIR)/$(TM).tmp
	@echo "expected result: no difference between these 2 compared files" 
	@echo 

# test of BUY..
$(TB):$(BUILDDIR)/$(EXE) $(TESTDIR)/$(TB).dat $(TESTDIR)/$(TB).cor 
	@echo "Unit testing for functionality of BUY:"
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/$(TB).dat > $(BUILDDIR)/$(TB).tmp
	diff $(TESTDIR)/$(TB).cor $(BUILDDIR)/$(TB).tmp
	@echo "expected result: no difference between these 2 compared files" 
	@echo 

# test of SELL..
$(TS):$(BUILDDIR)/$(EXE) $(TESTDIR)/$(TS).dat $(TESTDIR)/$(TS).cor 
	@echo "Unit testing for functionality of SELL:"
	$(BUILDDIR)/$(EXE) < $(TESTDIR)/$(TS).dat > $(BUILDDIR)/$(TS).tmp
	diff $(TESTDIR)/$(TS).cor $(BUILDDIR)/$(TS).tmp
	@echo "expected result: no difference between these 2 compared files" 
	@echo 

# large scale test..
$(TL):$(BUILDDIR)/$(EXE) $(SRCDIR)/largetest/largetest.cpp
	@echo "Compile $(SRCDIR)/largetest/largetest.cpp"
	$(CC) -o largetest $(FLAG) $(SRCDIR)/largetest/largetest.cpp
	mv largetest $(BUILDDIR)
	$(BUILDDIR)/largetest ${TLV} > $(BUILDDIR)/$(TL).dat
	@echo "There are ${TLV} orders sent into this large scale test"
	$(BUILDDIR)/$(EXE) < $(BUILDDIR)/$(TL).dat > $(BUILDDIR)/$(TL).tmp
	cat $(BUILDDIR)/$(TL).tmp
	@echo 
