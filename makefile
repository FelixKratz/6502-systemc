COMPILE_FLAGS = -arch arm64 -std=c++17
SYSTEM_C_PATH = /opt/homebrew/Cellar/systemc/3.0.1/

all: *.hpp *.cpp | bin
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o bin/sim simulation.cpp

test: *.hpp *.cpp | bin
	clang++ $(COMPILE_FLAGS) -I$(SYSTEM_C_PATH)/include/ -L$(SYSTEM_C_PATH)/lib/ -lsystemc -o bin/test test.cpp
	./test.sh ./bin/test

bin:
	mkdir -p bin
