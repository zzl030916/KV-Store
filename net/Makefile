LIB_SRC = EventLoop.cc
BINARIES = test1 test2

all : $(BINARIES)

$(BINARIES) : 
	g++ -o $@ $(LIB_SRC) $(filter %.cc,$^)

test1 : test1.cc
test2 : test2.cc


clean:
	rm -f $(BINARIES)