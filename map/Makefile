CXXFLAGS := -g -O3 -Wall -Wextra -pedantic -std=c++11 -I./include
SRCEXT = cpp
HEADEREXT = hpp
LDFLAGS=
BIN_DIR = ./bin
OBJ_DIR = ./obj
INCLUDE_DIR = ./include
TEST_DIR = ./tests
TESTS := $(wildcard $(TEST_DIR)/*.$(SRCEXT))
TEST_OBJECTS := $(TESTS:$(TEST_DIR)/%.$(SRCEXT)=$(OBJ_DIR)/%.o)
TEST_BINS := $(TEST_OBJECTS:$(OBJ_DIR)/%.o=$(BIN_DIR)/%.out)
HEADERS := $(wildcard $(INCLUDE_DIR)/*.$(HEADEREXT))

ccred:=$(shell echo -e "\033[0;31m")
ccgreen:=$(shell echo -e "\033[0;92m")
ccyellow:=$(shell echo -e "\033[0;33m")
ccend:=$(shell echo -e "\033[0m")


all: $(TEST_OBJECTS) $(TEST_BINS)


$(BIN_DIR)/%.out : $(OBJ_DIR)/%.o
	@echo Building $@
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o : $(TEST_DIR)/%.$(SRCEXT)
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	-rm -f $(TEST_OBJECTS) $(TEST_BINS) vgcore.*

lint: $(SOURCES) $(HEADERS)
	cpplint $^

test-sl: ./bin/test-skiplist.out
	valgrind ./bin/test-skiplist.out

print:
	@echo $(HEADERS)
	@echo $(TESTS)
	@echo $(TEST_OBJECTS)
	@echo $(TEST_BINS)

.PHONY: clean
