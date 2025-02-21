LIB_SRC = EventLoop.cc Channel.cc Poller.cc Timestamp.cc Timer.cc TimerQueue.cc
BINARIES = test1 test2 test3 test4

all : $(BINARIES)

$(BINARIES) : 
	g++ -g -o $@ $(LIB_SRC) $(filter %.cc,$^)

test1 : test1.cc
test2 : test2.cc
test3 : test3.cc
test4 : test4.cc

.PHONY : clean
clean:
	rm -f $(BINARIES)