#include "test.hpp"

static bool test_sec_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_SEC_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .P.C = 1, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sed_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_SED_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .P.D = 1, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_sei_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_SEI_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .P.I = 1, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
