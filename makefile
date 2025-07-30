export SYSTEMC_DISABLE_COPYRIGHT_MESSAGE=1

COMPILE_FLAGS = -arch arm64 -std=c++17
SYSTEM_C_PATH = /opt/homebrew/Cellar/systemc/3.0.1/

SRC_DIR = src
TEST_DIR = test
BIN_DIR = bin

SIM_BIN = ./$(BIN_DIR)/sim
TEST_BIN = ./$(BIN_DIR)/test
KD_TEST_BIN = ./$(BIN_DIR)/kd_test

run: $(SIM_BIN)
	$(SIM_BIN)

test: $(TEST_BIN)
	./test/test.sh $(TEST_BIN)

$(SIM_BIN): $(SRC_DIR)/*.hpp $(SRC_DIR)/*.cpp | $(BIN_DIR)
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o $(SIM_BIN) $(SRC_DIR)/simulation.cpp

$(TEST_BIN): $(SRC_DIR)/*.hpp $(SRC_DIR)/*.cpp $(TEST_DIR)/*.hpp $(TEST_DIR)/*.cpp | $(BIN_DIR)
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -I$(SRC_DIR)/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o $(TEST_BIN) $(TEST_DIR)/test.cpp

bin:
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

# Klaus Dormanns test suite for the 6502
kd_test: $(KD_TEST_BIN) $(BIN_DIR)/6502_functional_test.bin
	$(KD_TEST_BIN)

$(BIN_DIR)/6502_functional_test.bin: | $(BIN_DIR)
	echo "Downloading 6502_functional_test.bin from Klaus2m5/6502_65C02_functional_tests..."
	@curl -L -o $(BIN_DIR)/6502_functional_test.bin https://github.com/Klaus2m5/6502_65C02_functional_tests/raw/refs/heads/master/bin_files/6502_functional_test.bin


$(KD_TEST_BIN): $(SRC_DIR)/*.hpp $(SRC_DIR)/*.cpp $(TEST_DIR)/*.hpp $(TEST_DIR)/*.cpp | bin
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -I$(SRC_DIR)/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o $(KD_TEST_BIN) $(TEST_DIR)/kd_test.cpp
	
