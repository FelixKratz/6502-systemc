#include "test.hpp"

static bool test_tay_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_TAY_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x2a };
  Registers end_registers = { .A = 0x2a, .Y = 0x2a, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_tay_zero(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_TAY_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x00 };
  Registers end_registers = { .A = 0x00, .Y = 0x00, .P.Z = 1, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_tay_negative(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_TAY_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x80 };
  Registers end_registers = { .A = 0x80, .Y = 0x80, .P.N = 1, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
