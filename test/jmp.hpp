#include "test.hpp"

static bool test_jmp_abs(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_JMP_ABS, 0xff, 0x3e };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .pc = 0x3eff };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
