CXX = g++
CXXFLAGS = -Wall -Wextra -ggdb
ALGORITHMS = Kruskal Prim
ALGOS = $(foreach algo, $(ALGORITHMS), $(BUILD_DIR)/$(algo).o)
UTILS = Utilities
TEST = MSTTest
MST_FACTORY = MSTFactory
GRAPHS_DIR = ../graphs
BUILD_DIR = ../build

all: $(ALGOS) $(BUILD_DIR)/$(UTILS).o $(BUILD_DIR)/$(MST_FACTORY).o $(TEST)

$(BUILD_DIR):
	-mkdir -p $@

$(BUILD_DIR)/$(UTILS).o: $(UTILS).cpp $(UTILS).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MST_FACTORY).o: $(MST_FACTORY).cpp $(MST_FACTORY).hpp $(GRAPHS_DIR)/Subgraph.hpp $(GRAPHS_DIR)/Graph.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $< $@

$(BUILD_DIR)/%.o: %.cpp %.hpp $(GRAPHS_DIR)/Graph.hpp $(GRAPHS_DIR)/Subgraph.hpp $(MST_FACTORY).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST): $(TEST).o $(ALGOS) Subgraph.o AdjacencyGraph.o | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

Subgraph.o: $(GRAPHS_DIR)/Subgraph.cpp $(GRAPHS_DIR)/Subgraph.hpp $(GRAPHS_DIR)/Graph.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

AdjacencyGraph.o: $(GRAPHS_DIR)/AdjacencyGraph.cpp $(GRAPHS_DIR)/AdjacencyGraph.hpp $(GRAPHS_DIR)/Graph.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST).o: $(TEST).cpp $(UTILS).hpp $(MST_FACTORY).hpp doctest.h $(GRAPHS_DIR)/Graph.hpp $(GRAPHS_DIR)/Subgraph.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -rf $(BUILD_DIR)/$(UTILS).o $(BUILD_DIR)/$(MST_FACTORY).o $(ALGOS) $(TEST)

.PHONY: all clean