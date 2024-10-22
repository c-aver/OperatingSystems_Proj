CXX = g++
CXXFLAGS = -Wall -Wextra -ggdb -std=c++17 -ggdb
ALGORITHMS = Kruskal Prim
ALGOS = $(foreach algo, $(ALGORITHMS), $(BUILD_DIR)/$(algo).o)
UTILS = Utilities
MST_TEST = MSTTest
UTIL_TEST = UtilitiesTest
MST_FACTORY = MSTFactory
GRAPHS_DIR = ../graphs
BUILD_DIR = ../build
SUBGRAPH = Subgraph
ADJ_GRAPH = AdjacencyGraph
GRAPH = Graph

all: $(ALGOS) $(BUILD_DIR)/$(UTILS).o $(BUILD_DIR)/$(MST_FACTORY).o $(MST_TEST) $(UTIL_TEST)

$(BUILD_DIR):
	-mkdir -p $@

$(BUILD_DIR)/$(UTILS).o: $(UTILS).cpp $(UTILS).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MST_FACTORY).o: $(MST_FACTORY).cpp $(MST_FACTORY).hpp $(GRAPHS_DIR)/Subgraph.hpp $(GRAPHS_DIR)/Graph.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $< $@

$(BUILD_DIR)/%.o: %.cpp %.hpp $(GRAPHS_DIR)/Graph.hpp $(GRAPHS_DIR)/Subgraph.hpp $(MST_FACTORY).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MST_TEST): $(MST_TEST).o $(ALGOS) $(BUILD_DIR)/$(SUBGRAPH).o $(BUILD_DIR)/$(ADJ_GRAPH).o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/$(SUBGRAPH).o: $(GRAPHS_DIR)/$(SUBGRAPH).cpp $(GRAPHS_DIR)/$(SUBGRAPH).hpp $(GRAPHS_DIR)/$(GRAPH).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(ADJ_GRAPH).o: $(GRAPHS_DIR)/$(ADJ_GRAPH).cpp $(GRAPHS_DIR)/$(ADJ_GRAPH).hpp $(GRAPHS_DIR)/$(GRAPH).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MST_TEST).o: $(MST_TEST).cpp $(UTILS).hpp $(MST_FACTORY).hpp doctest.h $(GRAPHS_DIR)/$(GRAPH).hpp $(GRAPHS_DIR)/$(SUBGRAPH).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(UTIL_TEST): $(UTIL_TEST).o $(BUILD_DIR)/$(UTILS).o $(BUILD_DIR)/$(SUBGRAPH).o $(BUILD_DIR)/$(ADJ_GRAPH).o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(UTIL_TEST).o: $(UTIL_TEST).cpp $(UTILS).hpp doctest.h $(GRAPHS_DIR)/$(GRAPH).hpp $(GRAPHS_DIR)/$(SUBGRAPH).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -rf $(BUILD_DIR)/$(UTILS).o $(BUILD_DIR)/$(MST_FACTORY).o $(ALGOS) $(UTIL_TEST) $(MST_TEST)

.PHONY: all clean