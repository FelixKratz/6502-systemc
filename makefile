export SYSTEMC_DISABLE_COPYRIGHT_MESSAGE=1

COMPILE_FLAGS = -arch arm64 -std=c++17
SYSTEM_C_PATH = /opt/homebrew/Cellar/systemc/3.0.1/

SIM_BIN = ./bin/sim
TEST_BIN = ./bin/test

run: $(SIM_BIN)
	$(SIM_BIN)

test: $(TEST_BIN)
	./test.sh $(TEST_BIN)

$(SIM_BIN): *.hpp *.cpp | bin
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o $(SIM_BIN) simulation.cpp

$(TEST_BIN): *.hpp *.cpp | bin
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o $(TEST_BIN) test.cpp

bin:
	mkdir -p bin
