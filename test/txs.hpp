#include "test.hpp"

static bool test_txs_imp(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_TXS_IMP };
  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0x42 };
  Registers end_registers = { .X = 0x42, .S = 0x42, .pc = 0x1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
