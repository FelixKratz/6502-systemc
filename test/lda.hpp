#include "test.hpp"

static bool test_lda_zpg(std::string name) {
  uint64_t cycles = 3;

  mem_t start_memory = { OP_LDA_ZPG, 0xff };
  start_memory[0xff] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0x1a, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_zpx(std::string name) {
  uint64_t cycles = 4;

  mem_t start_memory = { OP_LDA_ZPX, 0xf1 };
  start_memory[0xdb] = 0x1a;

  mem_t end_memory = start_memory;

  Registers start_registers = { .X = 0xea};
  Registers end_registers = { .A = 0x1a, .X = 0xea, .pc = 0x2 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}

static bool test_lda_imm(std::string name) {
  uint64_t cycles = 2;

  mem_t start_memory = { OP_LDA_IMM, 0xff };
  mem_t end_memory = start_memory;

  Registers start_registers = {};
  Registers end_registers = { .A = 0xff, .pc = 0x2, .P.N = 1 };

  return run_test(std::move(name), cycles, start_memory, end_memory, start_registers, end_registers);
}
