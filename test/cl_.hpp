#include "test.hpp"

static bool test_clc_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_CLC_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .P.C = 1 };
  Registers end_registers = { .P.C = 0, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_cld_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_CLD_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .P.D = 1 };
  Registers end_registers = { .P.D = 0, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_cli_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_CLI_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .P.I = 1 };
  Registers end_registers = { .P.I = 0, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_clv_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_CLV_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .P.V = 1 };
  Registers end_registers = { .P.V = 0, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
