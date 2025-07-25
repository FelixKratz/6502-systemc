#include "test.hpp"

static bool test_dec_zpg(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_DEC_ZPG, 0x10 };
  memory[0x10] = 0x01;
  mem_t expected = memory;
  expected[0x10] = 0x00;
  Registers start = {};
  Registers end = { .P.Z = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

static bool test_dec_zpg_negative(std::string name) {
  uint64_t cycles = 5;
  mem_t memory = { OP_DEC_ZPG, 0x10 };
  memory[0x10] = 0x00;
  mem_t expected = memory;
  expected[0x10] = 0xFF;
  Registers start = {};
  Registers end = { .P.N = 1, .pc = 0x2 };
  return run_test(std::move(name), cycles, memory, expected, start, end);
}

// DEX
static bool test_dex(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_DEX_IMP };
  Registers start = { .X = 0x01 };
  Registers end = { .X = 0x00, .P.Z = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_dex_negative(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_DEX_IMP };
  Registers start = { .X = 0x00 };
  Registers end = { .X = 0xFF, .P.N = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

// DEY
static bool test_dey(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_DEY_IMP };
  Registers start = { .Y = 0x01 };
  Registers end = { .Y = 0x00, .P.Z = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}

static bool test_dey_negative(std::string name) {
  uint64_t cycles = 2;
  mem_t memory = { OP_DEY_IMP };
  Registers start = { .Y = 0x00 };
  Registers end = { .Y = 0xFF, .P.N = 1, .pc = 0x1 };
  return run_test(std::move(name), cycles, memory, memory, start, end);
}
