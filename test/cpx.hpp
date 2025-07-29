#include "test.hpp"

static bool test_cpx_imm_equal(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_CPX_IMM, 0x42 };
  Registers start = { .X = 0x42 };
  Registers end = { .X = 0x42, .P.Z = 1, .P.C = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cpx_imm_less_than(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_CPX_IMM, 0x50 };
  Registers start = { .X = 0x40 };
  Registers end = { .X = 0x40, .P.N = 1, .P.C = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cpx_imm_greater_than(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_CPX_IMM, 0x30 };
  Registers start = { .X = 0x50 };
  Registers end = { .X = 0x50, .P.C = 1, .P.Z = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
