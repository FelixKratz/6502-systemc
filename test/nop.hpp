#include "test.hpp"

static bool test_nop(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_NOP_IMM };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
