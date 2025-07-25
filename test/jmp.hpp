#include "test.hpp"

static bool test_jmp_ind(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_JMP_IND, 0xFF, 0x30 };

  // Simulate page boundary bug: reads from $30FF and $3000, not $3100
  start_memory[0x30FF] = 0x12;
  start_memory[0x3000] = 0x40;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .pc = 0x4012 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_jmp_abs(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_JMP_ABS, 0xff, 0x3e };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .pc = 0x3eff };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_jmp_ind_no_bug(std::string name) {
  uint64_t cycles = 5;

  mem_t start_memory = { OP_JMP_IND, 0x10, 0x40 };
  start_memory[0x4010] = 0x34;
  start_memory[0x4011] = 0x12;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .pc = 0x1234 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_jmp_abs_low_address(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_JMP_ABS, 0x00, 0x10 }; // JMP $1000
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .pc = 0x1000 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
