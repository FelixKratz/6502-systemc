#include "test.hpp"

static bool test_tsx_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_TSX_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .S = 0x42 };
  Registers end_registers = { .S = 0x42, .X = 0x42, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_tsx_zero(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_TSX_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .S = 0x00 };
  Registers end_registers = { .S = 0x00, .X = 0x00, .P.Z = 1, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_tsx_negative(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_TSX_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .S = 0xFF };
  Registers end_registers = { .S = 0xFF, .X = 0xFF, .P.N = 1, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
