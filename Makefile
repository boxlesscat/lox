SRC_DIR 	:= src
INC_DIR 	:= include
BUILD_DIR 	:= build

SRC 		:= $(wildcard $(SRC_DIR)/*.cpp)
OBJ			:= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP			:= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.d)

CXX			:= g++
CPPFLAGS	:= -I $(INC_DIR) -MMD -MP
CXXFLAGS	:= -std=c++20


lox::
	mkdir -p $(BUILD_DIR)

lox:: $(OBJ)
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $< -o $@ $(CPPFLAGS) $(CXXFLAGS)

.PHONY clean:
	-rm -rf build
	-rm -rf lox

-include $(DEP)
