SRC 	:= src
BUILD 	:= build

.PHONY all: setup lox


lox:
	g++ -o $(BUILD)/lox -I include $(SRC)/token.cpp $(SRC)/lox.cpp
	cp $(BUILD)/lox lox


setup:
	mkdir -p $(BUILD)


.PHONY clean:
	-rm -rf $(BUILD)
	-rm -rf lox
