CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -ggdb
MAIN = main
PL_MAIN = pl_main
ALGORITHMS_DIR = ./algorithms
GRAPHS_DIR = ./graphs
PATTERN_LIB_DIR = ./pattern_library
BUILD_DIR = ./build
VALGRIND_DIR = ./valgrind_results

all: $(MAIN) $(PL_MAIN)

$(BUILD_DIR):
	-mkdir -p $@

$(MAIN): $(MAIN).o | $(BUILD_DIR)
	$(MAKE) -C $(ALGORITHMS_DIR)
	$(MAKE) -C $(GRAPHS_DIR)
	$(MAKE) -C $(PATTERN_LIB_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(BUILD_DIR)/*.o -lpthread

$(MAIN).o: $(MAIN).cpp $(ALGORITHMS_DIR)/*.hpp $(GRAPHS_DIR)/*.hpp $(PATTERN_LIB_DIR)/*.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PL_MAIN): $(PL_MAIN).o | $(BUILD_DIR)
	$(MAKE) -C $(ALGORITHMS_DIR)
	$(MAKE) -C $(GRAPHS_DIR)
	$(MAKE) -C $(PATTERN_LIB_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(BUILD_DIR)/*.o -lpthread

$(PL_MAIN).o: $(PL_MAIN).cpp $(ALGORITHMS_DIR)/*.hpp $(GRAPHS_DIR)/*.hpp $(PATTERN_LIB_DIR)/*.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

memcheck: $(MAIN)
	valgrind --leak-check=full -s --show-leak-kinds=all --track-origins=yes --log-file=$(VALGRIND_DIR)/memcheck.txt ./$(MAIN) 

helgrind: $(MAIN)
	valgrind --tool=helgrind --log-file=$(VALGRIND_DIR)/helgrind.txt ./$(MAIN)

callgrind: $(MAIN)
	valgrind --tool=callgrind --callgrind-out-file=$(VALGRIND_DIR)/callgrind.txt ./$(MAIN)
	callgrind_annotate $(VALGRIND_DIR)/callgrind.txt > $(VALGRIND_DIR)/callgrind_annotated.txt

clean:
	$(MAKE) -C $(ALGORITHMS_DIR) clean
	$(MAKE) -C $(GRAPHS_DIR) clean
	$(MAKE) -C $(PATTERN_LIB_DIR) clean
	rm -rf $(BUILD_DIR)
	rm -f *.o $(MAIN) $(PL_MAIN)

.PHONY: all clean