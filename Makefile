SRC 	:= src
BUILD 	:= build

.PHONY all: setup lox


lox:
	g++ $(SRC)/lox.cpp -o $(BUILD)/lox
	cp $(BUILD)/lox lox


setup:
	mkdir -p $(BUILD)


run:
	$(BUILD)/lox


.PHONY clean:
	-rm -rf $(BUILD)
	-rm -rf lox
