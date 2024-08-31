SRC_DIR 		:= src
INC_DIR 		:= include
BUILD_DIR 		:= build

SRC 			:= $(wildcard $(SRC_DIR)/*.cpp)
OBJ				:= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP				:= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

CXX				:= g++
CPPFLAGS		:= -I $(INC_DIR) -MMD -MP -O2
CXXFLAGS		:= -std=c++20

PROFILEFLAGS 	?= # use -g -pg -no-pie -fno-builtin for profiling


lox::
	mkdir -p $(BUILD_DIR)

lox:: $(OBJ)
	$(CXX) $^ -o $@ $(PROFILEFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $< -o $@ $(CPPFLAGS) $(CXXFLAGS) $(PROFILEFLAGS)

.PHONY clean:
	-rm -rf build
	-rm -rf lox

-include $(DEP)
