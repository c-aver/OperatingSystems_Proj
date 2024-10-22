CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -ggdb
ADJ = AdjacencyGraph
SUBGRAPH = Subgraph
GRAPH_LIB = Graph.hpp
BUILD_DIR = ../build

all: $(BUILD_DIR)/$(ADJ).o $(BUILD_DIR)/$(SUBGRAPH).o

$(BUILD_DIR)/:
	-mkdir -p $@

$(BUILD_DIR)/%.o: %.cpp %.hpp $(GRAPH_LIB) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -rf $(BUILD_DIR)/$(ADJ).o $(BUILD_DIR)/$(SUBGRAPH).o

.PHONY: all clean