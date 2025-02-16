LIB_SRC = EventLoop.cc Channel.cc Poller.cc
BINARIES = test1 test2 test3

all : $(BINARIES)

$(BINARIES) : 
	g++ -o $@ $(LIB_SRC) $(filter %.cc,$^)

test1 : test1.cc
test2 : test2.cc
test3 : test3.cc

.PHONY : clean
clean:
	rm -f $(BINARIES)