CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -ggdb
MAIN = main
PL_MAIN = pl_main
ALGORITHMS_DIR = ./algorithms
GRAPHS_DIR = ./graphs
PATTERN_LIB_DIR = ./pattern_library
BUILD_DIR = ./build

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

clean:
	$(MAKE) -C $(ALGORITHMS_DIR) clean
	$(MAKE) -C $(GRAPHS_DIR) clean
	$(MAKE) -C $(PATTERN_LIB_DIR) clean
	rm -rf $(BUILD_DIR)
	rm -f *.o $(MAIN)

.PHONY: all clean