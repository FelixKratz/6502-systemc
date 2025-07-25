#include "test.hpp"

static bool test_inc_zpg(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_INC_ZPG, 0x10 };
  memory[0x10] = 0x7F;
  mem_t expected = memory;
  expected[0x10] = 0x80;
  Registers start = {};
  Registers end = { .P.N = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_inc_zpg_zero(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_INC_ZPG, 0x10 };
  memory[0x10] = 0xFF;
  mem_t expected = memory;
  expected[0x10] = 0x00;
  Registers start = {};
  Registers end = { .P.Z = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

// INX
static bool test_inx(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_INX_IMP };
  Registers start = { .X = 0x7F };
  Registers end = { .X = 0x80, .P.N = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_inx_zero(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_INX_IMP };
  Registers start = { .X = 0xFF };
  Registers end = { .X = 0x00, .P.Z = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

// INY
static bool test_iny(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_INY_IMP };
  Registers start = { .Y = 0x7F };
  Registers end = { .Y = 0x80, .P.N = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_iny_zero(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_INY_IMP };
  Registers start = { .Y = 0xFF };
  Registers end = { .Y = 0x00, .P.Z = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
