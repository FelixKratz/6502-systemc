export SYSTEMC_DISABLE_COPYRIGHT_MESSAGE=1

COMPILE_FLAGS = -arch arm64 -std=c++17
SYSTEM_C_PATH = /opt/homebrew/Cellar/systemc/3.0.1/

SRC_DIR = src

SIM_BIN = ./bin/sim
TEST_BIN = ./bin/test

run: $(SIM_BIN)
	$(SIM_BIN)

test: $(TEST_BIN)
	./test.sh $(TEST_BIN)

$(SIM_BIN): $(SRC_DIR)/*.hpp $(SRC_DIR)/*.cpp | bin
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o $(SIM_BIN) $(SRC_DIR)/simulation.cpp

$(TEST_BIN): $(SRC_DIR)/*.hpp $(SRC_DIR)/*.cpp | bin
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o $(TEST_BIN) $(SRC_DIR)/test.cpp

bin:
	mkdir -p bin
