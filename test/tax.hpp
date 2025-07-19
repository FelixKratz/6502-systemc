#include "test.hpp"

static bool test_tax_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_TAX_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .A = 0x2a };
  Registers end_registers = { .A = 0x2a, .X = 0x2a, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
