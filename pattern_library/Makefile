CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -ggdb
LEADER_FOLLOWER = leader_follower
REACTOR = reactor
BUILD_DIR = ../build

all: $(BUILD_DIR)/$(LEADER_FOLLOWER).o $(BUILD_DIR)/$(REACTOR).o

$(BUILD_DIR):
	-mkdir -p $@

$(BUILD_DIR)/$(LEADER_FOLLOWER).o: $(LEADER_FOLLOWER).cpp $(LEADER_FOLLOWER).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(REACTOR).o: $(REACTOR).cpp $(REACTOR).hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -rf $(BUILD_DIR)/$(LEADER_FOLLOWER).o

.PHONY: all clean