CXXFLAGS := -g -Wall -Wextra -pedantic -O3 -std=c++11 -I./include
SRCEXT = cpp
HEADEREXT = hpp
LDFLAGS=
BIN_DIR = ./bin
OBJ_DIR = ./obj
TEST_DIR = ./tests
TESTS := $(wildcard $(TEST_DIR)/*.$(SRCEXT))
TEST_OBJECTS := $(TESTS:$(TEST_DIR)/%.$(SRCEXT)=$(OBJ_DIR)/%.o)
TEST_BINS := $(TEST_OBJECTS:$(OBJ_DIR)/%.o=$(BIN_DIR)/%.out)
HEADERS := $(shell find . -type f -name "*.$(HEADEREXT)")

ccred:=$(shell echo "\033[0;31m")
ccgreen:=$(shell echo "\033[0;92m")
ccyellow:=$(shell echo "\033[0;33m")
ccend:=$(shell echo "\033[0m")


all: compile link

compile: $(TEST_OBJECTS)

link: $(TEST_BINS)

$(BIN_DIR)/%.out : $(OBJ_DIR)/%.o
	@echo Building $@
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o : $(TEST_DIR)/%.$(SRCEXT)
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm -f $(TEST_OBJECTS) $(TEST_BINS)

valgrind: all
	$(RUNV)

bench: all
	$(RUNS)

lint: $(SOURCES) $(HEADERS)
	cpplint $^

print:
	@echo $(TESTS)
	@echo $(TEST_OBJECTS)
	@echo $(TEST_BINS)

.PHONY: clean
