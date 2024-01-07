SRC_DIR 	:= src
INC_DIR 	:= include
BUILD_DIR 	:= build

SRC 		:= $(wildcard $(SRC_DIR)/*.cpp)
OBJ			:= $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEP			:= $(OBJ:%.o=.d)

CXX			:= g++
CPPFLAGS	:= -I $(INC_DIR) -MMD -MP


lox::
	mkdir -p $(BUILD_DIR)

lox:: $(OBJ)
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $^ -o $@ $(CPPFLAGS)

.PHONY clean:
	-rm -rf build
	-rm -rf lox

-include $(DEP)
