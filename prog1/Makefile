CXXFLAGS=-g -Wall -Wextra -pedantic -std=c++11 -O4
SRCEXT=cpp
HEADEREXT=hpp
LDFLAGS=-ldl
SOURCES=$(shell find . -type f -name "*.$(SRCEXT)")
OBJECTS=$(patsubst %.$(SRCEXT),%.o,$(SOURCES))
HEADERS=$(shell find . -type f -name "*.$(HEADEREXT)")
EXECUTABLE=deque
RUN=./$(EXECUTABLE)
RUNV=valgrind --leak-check=full --track-origins=yes $(RUN)
RUNS=perf stat $(RUN)

ccred:=$(shell echo "\033[0;31m")
ccgreen:=$(shell echo "\033[0;92m")
ccyellow:=$(shell echo "\033[0;33m")
ccend:=$(shell echo "\033[0m")


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o:%.$(SRCEXT)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm $(EXECUTABLE) *.o

test: all
	$(RUN)

valgrind: all
	$(RUNV)

bench: all
	$(RUNS)

lint: $(SOURCES) $(HEADERS)
	cpplint $^

.PHONY: clean
