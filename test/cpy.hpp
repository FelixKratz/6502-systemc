#include "test.hpp"

static bool test_cpy_abs_zero(std::string name) {
  uint64_t cycles = 4;
  mem_t memory = { OP_CPY_ABS, 0x10, 0x80 };
  memory[0x8010] = 0x22;
  Registers start = { .Y = 0x22 };
  Registers end = { .Y = 0x22, .P.Z = 1, .P.C = 1, .pc = 0x3 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_cpy_zpg_negative(std::string name) {
  uint64_t cycles = 3;
  mem_t memory = { OP_CPY_ZPG, 0x20 };
  memory[0x0020] = 0x80;
  Registers start = { .Y = 0x40 };
  Registers end = { .Y = 0x40, .P.N = 1, .P.C = 0, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
